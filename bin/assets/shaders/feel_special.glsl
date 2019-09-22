// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

float saturate(in float f){
    return clamp(f, 0., 1.);
}

float hash(vec3 p) {
    p  = fract( p*0.3183099+.1 );
	p *= 17.0;
    return fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}
float noise( in vec3 x ) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
    return mix(mix(mix( hash(p+vec3(0,0,0)), 
                        hash(p+vec3(1,0,0)),f.x),
                   mix( hash(p+vec3(0,1,0)), 
                        hash(p+vec3(1,1,0)),f.x),f.y),
               mix(mix( hash(p+vec3(0,0,1)), 
                        hash(p+vec3(1,0,1)),f.x),
                   mix( hash(p+vec3(0,1,1)), 
                        hash(p+vec3(1,1,1)),f.x),f.y),f.z);
}
float N21(vec2 p){
    p = fract(p*vec2(23.34, 81.74));
    p += dot(p, p+23.45);
    return fract(p.x*p.y);
}

const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 );
void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec2 p = uv*iGlitterResolution;

    vec3 col = vec3(0.0);

    float spotSize = 0.1;
    float r = noise(vec3(p, 0.0))*(1.+spotSize)-spotSize*0.5;
    float f = 0.0;
    vec3 q = vec3(p*0.1, 0.);
            f  = 0.5000*noise( q ); q = m*q*2.01;
            f += 0.2500*noise( q ); q = m*q*2.02;
            f += 0.1250*noise( q ); q = m*q*2.03;
            f += 0.0625*noise( q ); q = m*q*2.01;
            r += f*0.01;
    for(float i = 0.0; i < iFilterLevel; ++i){
        r = r+r-smoothstep(0., 1., r);
    }
    vec3 glitterColor = mix(iGlitterDark.rgb, iGlitterMiddle.rgb, saturate(r*2.));
    glitterColor = mix(glitterColor, iGlitterLight.rgb, saturate(r*2.-1.));
    col = glitterColor;
	//col = vec3(r);

    col = pow(col, vec3(0.4545));

    fragColor = vec4(col, 1.0);
}
