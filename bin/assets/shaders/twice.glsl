// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

float saturate(in float f){
    return clamp(f, 0.0, 1.0);
}

vec2 pR(in vec2 p, float a){
    float c = cos(a);
    float s = sin(a);
    return p * mat2(c, s, -s, c);
}
#if 0
float sdBox(in vec2 p, in vec2 b){
    return max(abs(p.x)-b.x, abs(p.y)-b.y);
}
#else
float sdBox(in vec2 p, in vec2 b){
    vec2 d = abs(p) - b;
    return length(max(d, vec2(0.0))) + max(min(d.x,0.0),min(d.y,0.0));
}
#endif
float sdCircle(in vec2 p, float r){
    return length(p) - r;
}
float sdStick(in vec2 p, in vec2 a, in vec2 b, in float ra, in float rb){
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = saturate(dot(pa, ba) / dot(ba, ba));
    return length(pa-ba*h) - mix(ra, rb, h);
}
float SmoothMin(in float a, in float b, in float k){
    float h = max(k-abs(a-b), 0.0001);
    return min(a, b) - h*h/(k*4.);
}
float SmoothMax(in float a, in float b, in float k){
    float h = max(k-abs(a-b), 0.0);
    return max(a, b) + h*h/(k*4.);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 col = vec3(0.0);

    vec2 p = uv;

    float d = sdBox(pR(p-iBoxLeftTip.xy, iBoxLeftTipAngle), iBoxLeftTip.zw);
    float d1 = sdStick(p, iTopLeftStick.xy, iTopLeftStick.zw, iTopLeftStickWidth.x, iTopLeftStickWidth.y);

    d = SmoothMin(d, d1, 0.3);

    col += smoothstep(0.0, 0.001, d);
    col = mix(col, vec3(cos(d*20.)), col.r);

    fragColor = vec4(col, 1.0);
}
