#version 330 core
uniform vec2 res;
// time, in seconds
uniform float time;
out vec4 FragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / res.xy;
    vec3 col = 0.5 + 0.5*cos(time + uv.xyx+vec3(0,2,4));
    FragColor = vec4(col, 1);
} 