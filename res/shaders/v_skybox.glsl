#version 330 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    TexCoords = vec3(M*vec4(pos, 1.0));
    gl_Position = (P * V * M * vec4(pos, 1.0)).xyww;
}  
