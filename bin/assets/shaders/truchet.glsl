// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

float Hash21(vec2 p){
    p = fract(p*vec2(233.34, 851.74));
    p += dot(p, p+23.45);
    return fract(p.x*p.y);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 col = vec3(0.0);

    float rot = 0.785*sin(iTime*0.1);
    float c = cos(rot);
    float s = sin(rot);
    const mat2 m2 = mat2(c, s, -s, c);
    vec2 p = uv * m2;
    vec2 gv = fract(p*iZoom)-.5;
    vec2 id = floor(p*iZoom);

    float n = Hash21(id);
    gv.x *= step(0.5+0.2*sin(iTime), n)*2.-1;

    float width = iWidth;

    float side = sign(gv.x+gv.y+0.001)*0.5;
    vec2 cuv = gv-side;
    float d = length(cuv);
    float mask = smoothstep(0.01, -0.01, abs(d-.5)-width);
    float angle = atan(cuv.x, cuv.y);
    float checker = mod(id.x+id.y, 2)*2.-1.;
    float flow = sin(iTime*checker*angle*10.);

    float x = sin(checker*angle*2.-iTime)*0.5+0.5;
    float y = abs(length(gv-side)-.5)/width;

	col += mask*y*x;

    //if(gv.x > 0.48 || gv.y > 0.48) col = vec3(1.,0.,0.);

    fragColor = vec4(col, 1.0);
}
