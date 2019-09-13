// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

const float FOV = 0.4;

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
	col = rd;

    fragColor = vec4(col, 1.0);
}
