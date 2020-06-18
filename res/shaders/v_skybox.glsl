#version 330 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    TexCoords = pos;
    vec4 position = (P * V * vec4(pos, 1.0));
    gl_Position = position.xyww;
}  
