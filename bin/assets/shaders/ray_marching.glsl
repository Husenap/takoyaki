// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

const float FOV = 0.4;

const int MAX_STEPS = 512;
const float MAX_DIST = 100.;
const float MIN_DIST = 1e-3;

#define ZERO (min(0, iFrame))

float saturate(float f){
    return clamp(f, 0., 1.);
}

float GetDist(vec3 p){
    float sphere = length(p - iSpherePosition) - 0.5;
    float plane = p.y + 0.5;
    return min(sphere, plane);
}

float RayMarch(vec3 ro, vec3 rd){
    vec3 p = ro;
    float t = 0.0;
    for(int i = ZERO; i < MAX_STEPS; ++i){
        p = rd*t + ro;

        float d = GetDist(p);

        t += d;

        if(d < MIN_DIST || t > MAX_DIST){
            break;
        }
    }
    return t;
}

vec3 GetNormal(vec3 pos){
 	vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*GetDist( pos + e.xyy ) + 
					  e.yyx*GetDist( pos + e.yyx ) + 
					  e.yxy*GetDist( pos + e.yxy ) + 
					  e.xxx*GetDist( pos + e.xxx ) );
}

vec3 GetColor(vec3 ro, vec3 rd){
    float d = RayMarch(ro, rd);

    vec3 res = vec3(0.0);

    if(d < MAX_DIST){
        vec3 p = rd * d + ro;
        vec3 n = GetNormal(p);
        vec3 h = reflect(rd, n);

        res = vec3(0.18);

        float fre = saturate(1.0+dot(n,rd));

        vec3 sun_dir = normalize(iSunDirection);
        vec3 sun_hal = normalize(sun_dir-rd);
        float sun_dif = saturate(dot(n, sun_dir));
        float sun_spe = pow(saturate(dot(n, sun_hal)), 8.0)*sun_dif*(0.04+0.96*pow(saturate(1.+dot(sun_hal,rd)),5.0));
        float sky_dif = sqrt(saturate(n.y*0.5+0.5));
        float sky_spe = smoothstep(0., 0.5, h.y)*(0.04+0.96*pow(fre,4.0));
        float bou_dif = sqrt(saturate(0.1-0.9*n.y)) * saturate(1.-0.1*p.y);

        vec3 light = vec3(0.0);
        light += sun_dif * iSunColor.rgb;
        light += sky_dif * iSkyColor.rgb;
        light += bou_dif * iBounceColor.rgb;
        res = res*light;
        res += sun_spe*iSunColor.rgb*1.1;
        res += sky_spe*iSkyColor.rgb*0.7;


        res = pow(res, vec3(0.4545));
    }else{
        vec3 skyColor = vec3(0.4, 0.75, 1.0) - 0.7*rd.y;
        skyColor = mix(skyColor, vec3(0.7, 0.75, 0.8), exp(-10.*rd.y));
        res = skyColor;
    }

    return res;
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

    vec3 col = vec3(0.0);
	col = GetColor(ro, rd);

    fragColor = vec4(col, 1.0);
}
