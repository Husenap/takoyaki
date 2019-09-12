vec2 N22(in vec2 p){
    vec3 a = fract(p.xyx*vec3(123.34, 234.34, 345.65));
    a += dot(a, a+34.45);
    return fract(vec2(a.x*a.y, a.y*a.z));
}

float voronoi(vec2 uv){
    vec2 gv = fract(uv)-0.5;
    vec2 id = floor(uv);
    
    float minDist = 99.;
    vec2 cid = vec2(0.0);
    
    for(float dx = -1.0; dx <= 1.0; ++dx){
        for(float dy = -1.0; dy <= 1.0; ++dy){
            vec2 offset = vec2(dx, dy);
            
            vec2 n = N22(vec2(id+offset));
            vec2 p = offset + sin(n*(iTime+10.)) * 0.5;
            
            float d = length(gv - p);
            
            if(d < minDist){
                minDist = d;
                cid = id+offset;
            }
        }
    }
    
    return minDist;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord ){ 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 col = vec3(0.0);
    
    col += voronoi(uv*5.+hejsan+halloj);

    fragColor = vec4(col, 1.0);
}
