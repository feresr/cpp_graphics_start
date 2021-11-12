#version 330 core
uniform vec2 resolution;
// time, in seconds
uniform float time;
out vec4 FragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy * 2.0 - 1.0;
    float aspect = resolution.x / resolution.y;
    uv.x *= aspect;
    float d = length(uv);
    float radius = 0.5;
	float t = step(clamp(d, 0.0, 1.0), radius);
    vec3 col = 0.5 + 0.5*cos(time + uv.xyx+vec3(0,2,4));
    FragColor = vec4(col, t);
} 