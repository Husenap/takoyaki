// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

#define ZERO (min(0, iFrame%4))
const float FOV = 0.4;
const float MAX_STEPS = 256;
const float MAX_DIST = 40.;
const float MIN_DIST = 0.001;

struct Material{
    vec3 diffuseColor;
    vec3 specularColor;
    float specularExponent;
    float backgroundAmount;
};

Material MixMaterials(in Material a, in Material b, in float t){
    return Material(mix(a.diffuseColor, b.diffuseColor, t),
                    mix(a.specularColor, b.specularColor, t),
                    mix(a.specularExponent, b.specularExponent, t),
                    mix(a.backgroundAmount, b.backgroundAmount, t));
}

struct SceneResult{
    float d;
    Material mat;
};

float saturate(in float f){
    return clamp(f, 0.0, 1.0);
}

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
float fbm3(vec3 x)
{
    float ret = noise(x);
    //ret += noise(x * 2.0) / 2.0;
    ret += noise(x * 4.0) / 4.0;
    //ret += noise(x * 8.0) / 8.0;
    //ret += noise(x * 16.0) / 16.0;
    return ret;
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
vec3 outdoorBackground(vec3 eyePos, vec3 eyeDir)
{
    vec3 sky =
        vec3(0.08)
        + mix(vec3(.8, .9, 1.0), vec3(1.0, .9, .8), pow(1.0 - abs(eyeDir.y), 2.1)) * .8
        ;
    
    float cloudsMix =
        saturate(pow(fbm(vec3(eyeDir.xz * 7.0, iTime * .1)) - .2, 6.0))
        * pow(max(eyeDir.y, 0.0), 1.4) * .8;
    
    vec3 clouds = mix(vec3(.8), vec3(1.0), fbm(eyeDir * 24.0));
    
    return mix(sky, clouds, cloudsMix);
}
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
vec3 BackgroundColor(in vec3 ro, in vec3 rd){
    //return nightSky(rd);
    return neonTunnelBackground(ro, rd);
    //return outdoorBackground(ro, rd);
    //return abs(rd);
}

SceneResult Scene(in vec3 p){
    Material mat = Material(
        vec3(0.2, 0.2, 0.2),
        vec3(0.3, 0.3, 0.3),
        50.0,
        0.4
    );
    //mat.backgroundAmount = 0.0;

    float diag = (p.x + p.y + p.z) * 2.0 + iTime * 1.2;
    float materialMix = saturate(pow(abs(sin(diag) * 1.1), 120.0));
    
    Material smoothMaterial = Material(
        vec3(.08),
        vec3(2.0),
        36.0,
        0.12);
    
    Material fuzzyMaterial = Material(
        mix(vec3(1.0, .2, .1), vec3(.9, .1, .1), saturate(pow(fbm(p * 7.0) * 1.1 - .1, 8.0))),
        vec3(.3),
        6.0,
        0.1);
    
    Material material = MixMaterials(smoothMaterial, fuzzyMaterial, materialMix);

    float d = length(p-vec3(sin(iTime), 0.0, 0.0)) - 1.0;
    //d = min(d, p.y+1.0);

    d += - (fbm3(p * 12.0 + iTime * .4) - .1) * materialMix * .01;
    d *= 0.9;

    return SceneResult(d, material);
}

SceneResult Raymarch(in vec3 ro, in vec3 rd, inout float t){
    SceneResult res;

    vec3 p = ro;
    t = 0.0;

    for(int i = ZERO; i < MAX_STEPS; ++i){
        p = rd*t + ro;       

        res = Scene(p);
        res.d = abs(res.d);

        if(res.d < MIN_DIST) break;

        t += res.d;

        if(t > MAX_DIST){
            t = -1.0;
            break;
        }
    }

    return res;
}

vec3 CalcLight( in vec3 p,
                in vec3 rd,
                in vec3 n,
                in vec3 lightPos,
                in vec3 lightColor,
                in Material material,
                in float ao){
    vec3 ld = normalize(lightPos - p);
    vec3 diffuse = max(material.diffuseColor * saturate(dot(n, ld)) - ao, vec3(0.0));
    vec3 h = reflect(ld, n);
    vec3 specular = material.specularColor * pow(saturate(dot(h,rd)), material.specularExponent);
    return (diffuse + specular) * lightColor;
}

vec3 CalcNormal(in vec3 p){
 	vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*Scene( p + e.xyy ).d + 
					  e.yyx*Scene( p + e.yyx ).d + 
					  e.yxy*Scene( p + e.yxy ).d + 
					  e.xxx*Scene( p + e.xxx ).d );
}

float CalcAO(in vec3 p, in vec3 n){
    float occlusionTerm = 0.0;
    for(float i = 1.0; i < 8.0; ++i){
        occlusionTerm += max(-Scene(p+n*i*0.1).d, 0.0) / pow(2.0, i);
    }
    return occlusionTerm;
}

vec3 GetColor(in vec3 ro, in vec3 rd){
    vec3 color = BackgroundColor(ro, rd);

    float t = 0.0;
    SceneResult hit = Raymarch(ro, rd, t);

    if(t >= 0.0){
        vec3 p = ro + rd * t;
        vec3 n = CalcNormal(p);

        float ao = CalcAO(p, n);
        float skyDiffuse = saturate(0.5+0.5*dot(n, vec3(0., 1., 0.)));

        vec3 surfaceColor = vec3(0.0);
        surfaceColor += CalcLight(p, rd, n, iLightPos1, iLightColor1.rgb, hit.mat, ao);
        surfaceColor += BackgroundColor(p, reflect(rd, n)) * hit.mat.backgroundAmount;

        //surfaceColor *= hit.mat.diffuseColor;
        surfaceColor *= 1.0-ao*2.0;

        color = surfaceColor;
    }

    color = pow(color, vec3(0.4545));
    return color;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 lookAt = iCameraTarget;
    vec3 cameraPosition = iCameraOrigin;

    vec3 forward = normalize(lookAt - cameraPosition);
    vec3 right = normalize(vec3(forward.z, 0.0, -forward.x));
    vec3 up = normalize(cross(forward, right));

    vec3 ro = cameraPosition;
    vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

    vec3 col = vec3(0.0);

    col = GetColor(ro, rd);

    col = pow(col*vec3(1.0, 0.99, 1.06), vec3(1.2));

    vec2 q = fragCoord.xy/iResolution.xy;
    col *= 0.4+0.6*pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1);

    col += pow(max(col - .2, 0.0), vec3(1.4)) * 0.4;

    fragColor = vec4(col, 1.0);
}
