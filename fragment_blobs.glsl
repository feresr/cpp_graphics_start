#version 330 core
uniform vec2 res;
uniform float time;
out vec4 fragColor;

void main()
{
    struct Circle {
    	vec2 pos;
        float radius;
    };
    
    const int numCircles = 3;
    Circle circles[] = Circle[numCircles](
    	Circle(vec2(0.0f, 0.0f), .45f),
        Circle(vec2(-0.2f, 0.2f), .35f),
        Circle(vec2(0.2f, -0.0f), .5f)
    );
        
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = (2.0*gl_FragCoord.xy -res.xy)/res.x;

    float f = 0.0f;
    float threshold = 0.9;
    for(int i=0;i< numCircles;i++) {
    	Circle circle = circles[i];
        
        circle.pos.x += .1f * cos(float(i) * time/2.0f);// * cos(time);
        circle.pos.y += .1f * sin(float(i) * time/2.0f); 
        
        float dist = length(uv - circle.pos);
    	f += smoothstep(circle.radius, circle.radius - threshold, dist);
	}
   
    
    // Time varying pixel color
    //vec3 col = 0.5 + 0.5*cos(time+uv.xyx+vec3(0,2,4));
    if (f >= .05) {
        vec3 col = 0.5 + 0.5*cos(time+uv.xyx+vec3(0,2,4));
    	fragColor = vec4(col, 1.0);//texture(iChannel0, gl_FragCoord.xy/res.yx + vec2(.5f, .3f));
    } else {
    	fragColor = vec4(0.0, 0.0, 0.0,1.0);   
    }
} 