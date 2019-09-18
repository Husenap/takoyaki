// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

const float FOV = 0.4;

const float ID_GROUND = 1.0;
const float ID_BODY = 2.0;
const float ID_EYES = 3.0;
const float ID_PUPILS = 4.0;

float saturate(float f){
    return clamp(f, 0., 1.);
}

float smoothMin(in float a, in float b, in float k){
    float h = max(k-abs(a-b), 0.0);
    return min(a, b) - h*h/(k*4.0);
}
float smoothMax(in float a, in float b, in float k){
    float h = max(k-abs(a-b), 0.0);
    return max(a, b) + h*h/(k*4.0);
}

float sdStick(in vec3 p, vec3 a, vec3 b, float ra, float rb){
    vec3 ba = b-a;
    vec3 pa = p-a;

    float h = saturate(dot(pa,ba)/dot(ba,ba));

    float r = mix(ra, rb, h);
    return length(pa-h*ba) - r;
}
float sdEllipsoid(in vec3 p, vec3 r){
    float k0 = length(p/r);
    float k1 = length(p/r/r);
    return k0*(k0-1.0)/k1;
}

float sdSphere(in vec3 p, float r){
    return length(p) - r;
}

vec2 sdGuy(in vec3 p){
    float time = iTime;
    float t = fract(time);
    float y = 4. * t * (1.0-t);
    float eart = fract(+time+0.2);
    float eary = 4. * eart * (1.0-eart);
    float dy = 4.0*(1.0-2.0*t);

    vec2 u = normalize(vec2(1.0, dy));
    vec2 v = normalize(vec2(-dy, 1.0));

    vec3 center = vec3(0., y-0.15, 0.);

    float sy = 0.5 + 0.5*y;
    float sz = 0.75/sy;
    vec3 radius = vec3(0.25*sz, 0.25*sy, 0.25*sz);

    vec3 q = p-center;

    //q.yz = vec2(dot(u,q.yz), dot(v,q.yz));

    float d = sdEllipsoid(q, radius);

    vec3 h = q;
    vec3 sh = vec3(abs(h.x), h.yz);

    // head
    float d2 = sdEllipsoid(h - vec3(0.0, 0.28, 0.0), vec3(0.15, 0.2, 0.23));
    float d3 = sdEllipsoid(h - vec3(0.0, 0.28, 0.1), vec3(0.23, 0.2, 0.2));

    d2 = smoothMin(d2, d3, 0.05);
    d = smoothMin(d, d2, 0.15);

    // eyebrows
    vec3 eyebrow = sh - iEyebrowOffset;
    eyebrow.xy = (mat2(24.,7.,-7.,24.)/25.)*eyebrow.xy;
    d2 = sdEllipsoid(eyebrow, iEyebrowSize);
    d = smoothMin(d, d2, 0.04);

    // mouth
    d2 = 0.8*sdEllipsoid(h-vec3(0.0, 0.15 + 3.0*h.x*h.x, -0.15), vec3(0.1, 0.04, 0.2));
    d = smoothMax(d, -d2, 0.02);

    //ears
    d2 = sdStick(sh, vec3(0.15, 0.4, 0.02), vec3(0.25, 0.55-eary*0.2, -0.02), 0.01, 0.04);
    d = smoothMin(d, d2, 0.03);

    vec2 res = vec2(d, ID_BODY);

    // eye
    float d4 = sdSphere(sh - vec3(0.08, 0.28, -0.16), 0.05);
    if(d4 < d) res = vec2(d4, ID_EYES);

    d4 = sdSphere(sh - vec3(0.09, 0.28, -0.18), 0.03);
    if(d4 < res.x) res = vec2(d4, ID_PUPILS);

    return res;
}

vec2 Map(in vec3 p){
    vec2 guy = sdGuy(p);
    float ground = p.y + 0.25;

    return (ground<guy.x) ? vec2(ground, ID_GROUND) : guy;
}

vec2 RayMarch(in vec3 ro, in vec3 rd){
    float m = -1.0;
    float t = 0.0;

    for(int i = 0; i < 100; ++i){
        vec3 p = rd*t + ro;

        vec2 h = Map(p);

        m = h.y;
        if(h.x < 0.001)break;

        t += h.x;
        if(t > 20.0)break;
    }

    if(t > 20.0) m = -1.0;

    return vec2(t, m-0.5);
}

float CalcShadow(in vec3 ro, in vec3 rd){
    float res = 1.0;

    float t = 0.01;
    for(int i = 0; i < 100; ++i){
        vec3 p = rd * t + ro;
        float h = Map(p).x;

        res = min(32.0*h.x/t, res);

        t += h;
        if(t > 20.0)break;
    }

    return res;
}

vec3 CalcNormal(in vec3 p){
    vec2 e = vec2(1.0, -1.0)*0.5773*0.0005;
    return normalize(e.xyy*Map(p+e.xyy).x +
                     e.yyx*Map(p+e.yyx).x +
                     e.yxy*Map(p+e.yxy).x +
                     e.xxx*Map(p+e.xxx).x );
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 lookAt = iCameraTarget;
    vec3 cameraPosition = iCameraOrigin;
    
    vec3 forward = normalize(lookAt-cameraPosition);
    vec3 right = normalize(vec3(forward.z, 0., -forward.x ));
    vec3 up = normalize(cross(forward,right));

    vec3 ro = cameraPosition; 
    vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

    vec3 col = vec3(0.4, 0.75, 1.0) - 0.7*rd.y;
    col = mix(col, vec3(0.7, 0.75, 0.8), saturate(exp(-10.*rd.y)));

    vec2 tm = RayMarch(ro, rd);

    if(tm.y > 0.){
        float t = tm.x;
        vec3 p = rd*t + ro;
        vec3 n = CalcNormal(p);

        vec3 albedo = vec3(0.2);

        if(tm.y < ID_GROUND){
            albedo = iGroundColor.rgb;
        }else if(tm.y < ID_BODY){
            albedo = iBodyColor.rgb;
        }else if(tm.y < ID_EYES){
            albedo = iEyesColor.rgb;
        }else if(tm.y < ID_PUPILS){
            albedo = iPupilsColor.rgb;
        }


        vec3 sunDirection = normalize(iSunDirection);
        float sunDiffuse = saturate(dot(n, sunDirection));
        float sunShadow = CalcShadow(p+n*0.01, sunDirection);

        float skyDiffuse = saturate(0.5+0.5*dot(n, vec3(0.,1.,0.)));

        float bounceDiffuse = saturate(dot(n, vec3(0.,-1.,0.)));

        col  = iSunLight.rgb * sunDiffuse * sunShadow;
        col += iSkyLight.rgb * skyDiffuse;
        col += iBounceLight.rgb * bounceDiffuse;

        col *= albedo;
    }

    col = pow(col, vec3(0.4545));

    fragColor = vec4(col, 1.0);
}
