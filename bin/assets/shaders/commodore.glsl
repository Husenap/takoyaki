// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 pixelSize = vec2(8.0, 4.0);

    vec2 uv = (floor(fragCoord/pixelSize)*pixelSize*2.0 - iResolution.xy) / iResolution.y;

    vec2 texel = fragCoord.xy / pixelSize;

    vec3 col = vec3(0.0);

    float d = length(uv);

    d = smoothstep(0.25+0.25*sin(iTime), 1.0-0.25*cos(iTime*1.132), d);

    col += mix( iColor1.rgb,
                iColor2.rgb,
                step(d, (iPattern[(int(texel.x))%4][(int(texel.y))%4]+0.99999) / 16.0));

    fragColor = vec4(col, 1.0);
}
