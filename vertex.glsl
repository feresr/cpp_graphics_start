#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec3 oColor;
out vec2 texCoords;
out vec3 oNormal;
out vec3 oFragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    // send the fargment position and the normal positions in world space
    oFragPos = (model * position).xyz;

    // multiplying with the model matrix would do the job 99% of the times. however if the model matrix does a non uniform scale, or a translation then the normals would not be normals anymore
    // transpose(inverse(model)) = this calculates a model matrix that removes non uniform scales and translations
    // ideally we should calculate this on the CPU and send it as a uniform just like the model matrix
    oNormal = (transpose(inverse(model)) * vec4(aNormal, 0.0)).xyz;

    texCoords = aTexCoords;
    gl_Position = projection * view * model * position;
}