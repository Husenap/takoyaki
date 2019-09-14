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
	col.rg = uv.xy;

    fragColor = vec4(col, 1.0);
}
