#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lightSource1;
uniform vec4 lightSource2;

//Atrybuty
layout (location = 0) in vec3 vertex;//wspolrzedne wierzcholka w przestrzeni modelu
layout (location = 1) in vec3 normal;//wektor normalny w przestrzeni modelu
layout (location = 2) in vec2 texCoord0;

//Zmienne interpolowane
out vec4 lightVector;
out vec4 viewerVector;
out vec4 normalVector;

out vec2 iTexCoord0;
//out vec2 iTexCoord1;

void main(void) {
    vec4 vertex4 = vec4(vertex, 1.0);
    vec4 normal4 = vec4(normal, 1.0);
    vec4 lp = lightSource1;//pozcyja światła, przestrzeń świata
    lightVector = normalize(V * lp - V*M*vertex4);//wektor do światła w przestrzeni oka
    viewerVector = normalize(vec4(0, 0, 0, 1) - V * M * vertex4);//wektor do obserwatora w przestrzeni oka
    normalVector = normalize(V * M * normal4);//wektor normalny w przestrzeni oka

    iTexCoord0 = texCoord0;//interpolowane współrzędne teksturowania

    gl_Position=P*V*M*vertex4;//pozycja wierzchołka
}
