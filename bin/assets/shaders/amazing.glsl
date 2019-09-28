// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;


    vec3 col = vec3(0.0);
    col += length(uv-vec2(sin(iTime*1.21+1.3), cos(iTime*0.83+1.6))) * vec3(0.1, 0.05, 0.1) * 0.2;
    col = col*0.99+0.01;
    for(int i = 0; i < 3; ++i){
        uv *= 1.05f;
    uv.x = abs(uv.x);
    uv.y = abs(uv.y);

    if(abs(length(uv-vec2(sin(iTime), 0.0f))-0.5)-0.015 < 0.0){
        uv.y *= -1.0;
        uv.x *= -1.0;
    }
    if(abs(length(uv)-2.)-0.05 < 0.0){
        uv.y *= -1.0;
        uv.x *= -1.0;
    }
    if(abs(length(uv)-0.8)-0.02 < 0.0){
        uv.y *= -1.0;
    }
    if(abs(length(uv)-0.9)-0.02 < 0.0){
        uv.y *= -1.0;
    }
        float d = smoothstep(0.41, 0.3, length(uv + vec2(sin(iTime), 0.0f)));
        d = max(d, smoothstep(0.31, 0.2, length(uv + vec2(sin(iTime+3.)*2.f, cos(iTime*1.7f)))));

        col[i] += d;
    }

    col = pow(col, vec3(0.4545));

    fragColor = vec4(col, 1.0);
}
