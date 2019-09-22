// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#


// Thanks to iq for the noise routine, the vignette, and some repetition stuff.
// Also thanks to mu6k for some repetion/mirroring tricks used in ALLTHECUBES and
// Daedelus for finding a funny bottleneck in the code structure on some GPU's :D

//#define BAWLS
//#define TUNNEL
//#define ALLTHECUBES
//#define SLICES
//#define TWISTERS
#define OUTDOORZ

struct Material
{
    vec3 additiveColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float specularExponent;
    float backgroundAmount;
};
   
Material mixMaterials(Material a, Material b, float x)
{
    return Material(
        mix(a.additiveColor, b.additiveColor, x),
        mix(a.diffuseColor, b.diffuseColor, x),
        mix(a.specularColor, b.specularColor, x),
        mix(a.specularExponent, b.specularExponent, x),
        mix(a.backgroundAmount, b.backgroundAmount, x));
}

struct SceneResult
{
    float d;
    Material material;
};

vec3 rotateX(vec3 x, float an)
{
    float c = cos(an);
    float s = sin(an);
    return vec3(x.x, x.y * c - x.z * s, x.z * c + x.y * s);
}

vec3 rotateY(vec3 x, float an)
{
    float c = cos(an);
    float s = sin(an);
    return vec3(x.x * c - x.z * s, x.y, x.z * c + x.x * s);
}

vec3 rotateZ(vec3 x, float an)
{
    float c = cos(an);
    float s = sin(an);
    return vec3(x.x * c - x.y * s, x.y * c + x.x * s, x.z);
}

// Noise by iq
float hash( float n ) { return fract(sin(n)*753.5453123); }
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}

float fbm(vec3 x)
{
    float ret = noise(x);
    ret += noise(x * 2.0) / 2.0;
    ret += noise(x * 4.0) / 4.0;
    ret += noise(x * 8.0) / 8.0;
    ret += noise(x * 16.0) / 16.0;
    return ret;
}

float fbm2(vec3 x)
{
    float ret = noise(x);
    //ret += noise(x * 2.0) / 2.0;
    ret += noise(x * 4.0) / 4.0;
    //ret += noise(x * 8.0) / 8.0;
    ret += noise(x * 16.0) / 16.0;
    return ret;
}

float fbm3(vec3 x)
{
    float ret = noise(x); x = rotateY(x, .833);
    //ret += noise(x * 2.0) / 2.0;
    ret += noise(x * 4.0) / 4.0;
    //ret += noise(x * 8.0) / 8.0;
    //ret += noise(x * 16.0) / 16.0;
    return ret;
}

#define saturate(x) (clamp((x), 0.0, 1.0))

vec3 nightSky(vec3 eyeDir)
{
    float starsBrightness = pow(noise(eyeDir * 200.0), 86.0) * 2.2;
    vec3 stars = vec3(saturate(starsBrightness));
    
    float horizonBrightness = pow(1.0 - abs(eyeDir.y) + .01, 22.0) * .3;
    vec3 horizon = vec3(0.8, 0.9, 1.0) * horizonBrightness;
    
    vec3 moonPos = normalize(vec3(1, 2, 2));
    float moonBrightness = saturate(pow(max(dot(eyeDir, moonPos), 0.0) + .01, 3000.0));
    vec3 moonColor = mix(vec3(0.9), vec3(.8, .6, .5), fbm(eyeDir * 24.0));
    vec3 moon = moonColor * moonBrightness;
    
    vec3 clearSky = stars + horizon + moon;
    
    return clearSky;
}

float sphere(vec3 p, vec3 pos, float radius)
{
    return length(p - pos) - radius;
}

float cube(vec3 p, vec3 pos, vec3 size)
{
    vec3 d = abs(p - pos) - size;
    return max(max(d.x, d.y), d.z);
}

float bawlsNoise(vec3 p)
{
    vec3 normalNoisePos = p * 5.0;// + vec3(0.0, iTime, 0.0);
    return
        saturate(pow((fbm(normalNoisePos) - .6) * 4.0, 2.0))
        + pow(fbm(normalNoisePos * 3.0 + 40.0), 6.0) * .08
        ;
}

#ifdef BAWLS
#define ballsTime (iTime * .3)
#define ball1Pos (vec3(cos(ballsTime * .44), cos(ballsTime * .99), sin(ballsTime * 1.3)) * 1.4)
#define ball2Pos (vec3(cos(ballsTime * 1.3), sin(ballsTime * .78), cos(ballsTime)) * 1.2)
#define ball3Pos (vec3(sin(ballsTime * .77), sin(ballsTime * .12), sin(ballsTime * 2.0)) * 1.2)
#define ball1Rad (sin(ballsTime + 1.0) * .4 + .6)
#define ball2Rad (cos(ballsTime * 1.6 + 2.0) * .4 + .6)
#define ball3Rad (sin(ballsTime * .84 + 3.0) * .4 + .6)

float metaballs(vec3 p)
{
    float ball1 = sphere(p, ball1Pos, ball1Rad);
    float ball2 = sphere(p, ball2Pos, ball2Rad);
    float ball3 = sphere(p, ball3Pos, ball3Rad);
    
    float srf =
        1.0 / (ball1 + ball1Rad) +
        1.0 / (ball2 + ball2Rad) +
        1.0 / (ball3 + ball3Rad);
    
    return 1.0 / srf - .4;
}

SceneResult f(vec3 p)
{
    float diag = (p.x + p.y + p.z) * 2.0 + iTime * 1.2;
    float materialMix = saturate(pow(abs(sin(diag) * 1.1), 120.0));
    
    Material smoothMaterial = Material(
        vec3(0.0),
        vec3(.08),
        vec3(2.0),
        36.0,
        0.52);
    
    Material fuzzyMaterial = Material(
        vec3(0.0),
        mix(vec3(1.0, .2, .1), vec3(.9, .1, .1), saturate(pow(fbm(p * 7.0) * 1.1 - .1, 8.0))),
        vec3(.3),
        6.0,
        0.4);
    
    Material material = mixMaterials(smoothMaterial, fuzzyMaterial, materialMix);
    float d =
        metaballs(p)
        - (fbm3(p * 12.0 + iTime * .4) - .1) * materialMix * .01
        ;
    
    return SceneResult(d, material);
}

SceneResult tf(vec3 p)
{
    SceneResult bawlsResult = f(p);
    
    float normalNoise = bawlsNoise(p);
    
    return SceneResult(bawlsResult.d + normalNoise * .001, bawlsResult.material);
}
#endif

#ifdef TUNNEL
vec2 tunnelUv(vec3 p)
{
    vec3 tunnelP = p * .1;
    return vec2(atan(tunnelP.y, tunnelP.x) * 2.0, tunnelP.z + iTime * .125);
}

SceneResult tunnel(vec3 p)
{
    Material material = Material(
        vec3(0.0),
        vec3(.8, .9, 1.0) * .32,
        vec3(.2),
        36.0,
        0.2);
    
    vec2 uv = tunnelUv(p);
    float d = 6.0 - length(p.xy + vec2(sin(p.z * .4 + iTime * .21), cos(p.z * .4 + iTime * .34)) * .7) + fbm3(vec3(uv, iTime * .1)) * 4.0;
    
    float cutoutSize = 1.2;
    float cutout = -(min(abs(p.x) - cutoutSize, abs(p.y) - cutoutSize));
    
    return SceneResult(max(d, cutout), material);
}

SceneResult f(vec3 p)
{
    return tunnel(p);
}

SceneResult tf(vec3 p)
{
    SceneResult original = f(p);
    
    vec2 uv = tunnelUv(p);
    return SceneResult(
        original.d + fbm(vec3(uv * 2.1, iTime * .1) * 3.4) * .4,
        original.material);
}
#endif

#ifdef ALLTHECUBES
// rep stolen from iq
float repCubes( vec3 p, vec3 c )
{
    vec3 q = mod(p,c)-0.5*c;
    return cube(q, vec3(0.0), vec3(0.34));
}

SceneResult f(vec3 p)
{
    Material material = Material(
        vec3(0.0),
        vec3(.8),
        vec3(.02),
        36.0,
        0.6);
    float t = iTime * .4;
    vec3 q = rotateY(rotateZ(rotateX(p, t * .22), t * .34), t * .44 + sin(p.y * .05 + t * .2));
    float e = .95;
    for (int i = 0; i < 4; i++)
    {
        q = rotateX(rotateZ(abs(q - e * float(i)), 40.0 + t * .12), t * .116);
        e = e * .5 + e * e * .3;
    }
    float d = max(repCubes(q, vec3(.8)), cube(q, vec3(0.0), vec3(6.0, 6.0, 6.0)));
    
    return SceneResult(d, material);
}

SceneResult tf(vec3 p)
{
    float diag = (p.x + p.y + p.z) * 2.2;
    float materialMix = saturate(pow((sin(diag) * .5 + .5) * 1.2, 40.0));
    
    Material material = Material(
        vec3(0.0),
        vec3(.08),//vec3(1.0, .2, .1),
        vec3(1.01),
        6.0,
        0.0);
    
    SceneResult original = f(p);
    return SceneResult(original.d + (fbm(p * 3.0) + 1.4) * materialMix * .03, mixMaterials(original.material, material, materialMix));
}
#endif

#ifdef SLICES
SceneResult f(vec3 p)
{
    Material material = Material(
        vec3(0.0),
        vec3(.88),
        vec3(2.0),
        36.0,
        0.52);
    
    float slashesAmt = .5;
    
    //float slashes = abs(fract((p.x + p.y + p.z) * 8.0) / 16.0 - 1.0 / 32.0) - Slashes / 32.0;
      float slashes = abs(fract((p.x + p.y + p.z) / 2.0) - 1.0 / 2.0) - slashesAmt / 2.0;
    float d = max(cube(p, vec3(0.0), vec3(4.0)), slashes);
    
    return SceneResult(d, material);
}

SceneResult tf(vec3 p)
{
    SceneResult original = f(p);
    return SceneResult(original.d + bawlsNoise(p) * .001, original.material);
}
#endif

#ifdef TWISTERS
SceneResult f(vec3 p)
{
    Material material = Material(
        vec3(0.0),
        vec3(.08),
        vec3(2.0),
        36.0,
        0.82);
    
    vec3 q = rotateY(p, sin(p.y * .03 + iTime * .73) * 6.2);
    float d = 1000.0;
    for (int i = 0; i < 3; i++)
    {
        float an = float(i) / 3.0 * 2.0 * 3.141592;
        d = min(d, length(q.xz + vec2(cos(an), sin(an)) * 2.2) - 1.0);
    }
    
    float cutoutPos = sin(iTime * 1.7) * 3.0;
    float cutoutSize = sin(iTime * 2.1) + 1.5;
    float cutout = -(abs(p.y - cutoutPos) - cutoutSize);
    d = max(d, cutout);
    
    return SceneResult(d, material);
}

SceneResult tf(vec3 p)
{
    SceneResult original = f(p);
    return SceneResult(original.d + bawlsNoise(p) * .003, original.material);
}
#endif

#ifdef OUTDOORZ
// rep stolen from iq
float repCylinders( vec3 p, vec2 c )
{
    vec2 derp = mod(p.xz, c);
    vec2 q = derp-0.5*c;
    return length(q) - .34;
}

SceneResult f(vec3 p)
{
    float diag = p.x + p.y + p.z;
    
    Material material = Material(
        vec3(0.0),
        vec3(.08) * vec3(1., .9, .8),
        vec3(2.0),
        36.0,
        0.42);
    
    float s = sphere(p, vec3(sin(iTime)), 5.0+3.0*cos(iTime)) + noise(p * .4 + iTime);
    float cylinders = repCylinders(p + noise(p * .1 + iTime) * .8, vec2(10.0));
    float ground = p.y + 12.0 + (sin(p.x * .4) + cos(p.z * .4)) * .4;
    
    return SceneResult(min(min(s, cylinders), ground), material);
}

SceneResult tf(vec3 p)
{
    float diag = (p.x + p.y + p.z) * 2.2;
    float materialMix = saturate(pow((sin(diag) * .5 + .5) * 1.2, 40.0));
    
    Material material = Material(
        vec3(0.0),
        vec3(.08),//vec3(1.0, .2, .1),
        vec3(1.01),
        6.0,
        0.0);
    
    SceneResult original = f(p);
    return SceneResult(original.d + (fbm(p * 3.0) + 1.4) * materialMix * .03, mixMaterials(original.material, material, materialMix));
}
#endif

vec3 lightContribution(
    vec3 eyePos,
    vec3 eyeDir,
    vec3 normal,
    vec3 lightPos,
    vec3 lightColor,
    Material material,
    float occlusionTerm)
{
    vec3 l = normalize(lightPos - eyePos);
    vec3 diffuse = max(material.diffuseColor * (max(dot(normal, l), 0.0)) - occlusionTerm, vec3(0.0));
    vec3 refVec = reflect(l, normal);
    vec3 specular = material.specularColor * pow(max(dot(refVec, eyeDir), 0.0), material.specularExponent);
    return (diffuse + specular) * lightColor;
}

#if defined TUNNEL || defined OUTDOORZ
#define light1Pos (vec3(sin(iTime * 1.1), cos(iTime), 20.0))
#define light2Pos (vec3(cos(iTime * .84), cos(iTime * .45), 20.0))
#else
#define lightTime (iTime * .2)
#define light1Pos (vec3(sin(lightTime * 1.1), cos(lightTime) * .3 + .4, sin(lightTime * .76)) * 20.0)
#define light2Pos (vec3(cos(lightTime * .84), cos(lightTime * .45) * .3 + .4, sin(lightTime * 1.2)) * 20.0)
#endif
vec3 light1Color = vec3(0.7, .85, 1.0);
vec3 light2Color = vec3(1.0, .85, .7);

vec3 outdoorBackground(vec3 eyePos, vec3 eyeDir)
{
    vec3 sky =
        vec3(0.08)
        + pow(max(dot(eyeDir, normalize(light1Pos - eyePos)), 0.0), 27.0) * light1Color * .4
        + pow(max(dot(eyeDir, normalize(light2Pos - eyePos)), 0.0), 27.0) * light2Color * .4
        + mix(vec3(.8, .9, 1.0), vec3(1.0, .9, .8), pow(1.0 - abs(eyeDir.y), 2.1)) * .8
        ;
    
    float cloudsMix =
        saturate(pow(fbm(vec3(eyeDir.xz * 7.0, iTime * .1)) - .2, 6.0))
        * pow(max(eyeDir.y, 0.0), 1.4) * .8;
    
    vec3 clouds = mix(vec3(.8), vec3(1.0), fbm(eyeDir * 24.0));
    
    return mix(sky, clouds, cloudsMix);
}

vec3 getColor(float x)
{
    return
        mix(
            vec3(.2, .8, 1.0),
            mix(
                vec3(.7, .2, 1.0),
                mix(
                    vec3(1.0, .6, .2),
                    vec3(1.0),
                    step(.6, x)),
                step(.4, x)),
            step(.2, x));
}

vec3 neonTunnelBackground(vec3 eyePos, vec3 eyeDir)
{
    float t = iTime * .2;
    float an = atan(eyeDir.x, eyeDir.z) / (2.0 * 3.141592) * 26.0;
    float f = (1.0 - pow(abs(fract(an) * 2.0 - 1.0), 23.0)) * (1.0 - pow(abs(eyeDir.y), 3.0));
    f *= saturate(pow(fbm(vec3(floor(an), t * 1.4, 0.0)) - .2, 12.0) * 3.0 + .1);
    f *= (1.0 - pow(saturate(fbm(eyeDir * 33.0) - .8), 3.0)) * .6 + .4;
    float c = saturate(pow(fbm(vec3(floor(an), 0.0, t * .13)) - .1, 12.0) * 3.0 + .1);
    return getColor(c) * f * .7 * 2.0;
}

vec3 background(vec3 eyePos, vec3 eyeDir)
{
    return
#if defined TUNNEL || defined OUTDOORZ
        outdoorBackground(eyePos, eyeDir);
#else
        neonTunnelBackground(eyePos, eyeDir);
#endif
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 pixelPos = (fragCoord - iResolution.xy / 2.0) / iResolution.y;
    
    vec3 eyeDir = normalize(vec3(pixelPos, 1));
    
    vec2 normMouse = vec2(0.0);//iMouse.xy / iResolution.xy - .5;
    float camRotX = sin(iTime * .1) + normMouse.y * 3.141592;
    float camRotY = iTime * .5 + normMouse.x * 2.0 * 3.141592;
    float camRotZ = sin(iTime * .083) * .4;
    
    eyeDir = rotateZ(eyeDir, camRotZ);
    eyeDir = rotateX(eyeDir, camRotX);
    eyeDir = rotateY(eyeDir, camRotY);
    
    vec3 eyePos =
#ifdef BAWLS
        vec3(0.0, 0.0, -4.0);
#elif defined TUNNEL
        vec3(0.0, -7.0, -4.0);
#else
        vec3(0.0, -0.0, -14.0);
#endif
    eyePos = rotateX(eyePos, camRotX);
    eyePos = rotateY(eyePos, camRotY);
    
    vec3 color = background(eyePos, eyeDir);
    
    const float maxDisTravelled = 40.0;
    float disTravelled = 0.0;
    const int maxIterations = 160;
    for (int i = 0; i < maxIterations; i++)
    {
        float d = f(eyePos).d;
        if (d <= 0.0)
            break;
        
        d = max(d, .025);
        eyePos += eyeDir * d;
        disTravelled += d;
        if (disTravelled >= maxDisTravelled)
            break;
    }
    
    if (disTravelled < maxDisTravelled)
    {
        SceneResult td = tf(eyePos);
        vec3 normal = normalize(
            vec3(
                tf(eyePos + vec3(.003, 0, 0)).d - td.d,
                tf(eyePos + vec3(0, .003, 0)).d - td.d,
                tf(eyePos + vec3(0, 0, .003)).d - td.d));

        float occlusionTerm = 0.0;
        for (float i = 1.0; i < 4.0; i += 1.0)
            occlusionTerm += max(-f(eyePos + normal * i * .1).d, 0.0) / pow(2.0, i);
        occlusionTerm *= 2.0;

        vec3 surfaceColor =
            td.material.additiveColor
            + lightContribution(eyePos, eyeDir, normal, light1Pos, light1Color, td.material, occlusionTerm)
            + lightContribution(eyePos, eyeDir, normal, light2Pos, light2Color, td.material, occlusionTerm)
            + background(eyePos, reflect(eyeDir, normal)) * td.material.backgroundAmount;

        //vec3 iterationGlow = vec3(.9, 1.0, 1.1) * pow(float(i) / float(maxIterations), 2.0);

        color =
            mix(
                color,
                surfaceColor,
                1.0 - pow(disTravelled / maxDisTravelled, 2.0))
            //+ iterationGlow
            ;
    }
    
    vec2 q = fragCoord.xy / iResolution.xy;
    color = pow(color * vec3(1.0, .99, 1.06), vec3(1.2));
    
   	//color = pow(color, vec3(3.0));
    
    // Vignette (stolen from iq)
    color *= 0.4 + 0.6*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1 );
    
    // Cheap "bloom emulation" to better match the actual intro :)
    color += pow(max(color - .2, 0.0), vec3(1.4)) * .5;
    
    fragColor = vec4(color, 1);
}