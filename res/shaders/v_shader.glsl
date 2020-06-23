#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lightSource;//pozcyja światła ruchomego, przestrzeń świata


//Atrybuty
layout (location = 0) in vec3 vertex;//wspolrzedne wierzcholka w przestrzeni modelu
layout (location = 1) in vec3 normal;//wektor normalny w przestrzeni modelu
layout (location = 2) in vec2 texCoord0;

//Zmienne interpolowane
out vec4 lightVector[5];
//out vec4 lightVector1;
//out vec4 lightVector2;
//out vec4 lightVector3;
//out vec4 lightVector4;
out vec4 viewerVector;
out vec4 normalVector;

out vec2 iTexCoord0;

void main(void) {
    vec4 vertex4 = vec4(vertex, 1.0);
    vec4 normal4 = vec4(normal, 1.0);
    lightVector[0] = normalize(V * lightSource - V*M*vertex4);//wektor do światła w przestrzeni oka
    lightVector[1] = normalize(V * vec4(1, 0.35, 0, 1) - V*M*vertex4);//wektor do światła w przestrzeni oka
    lightVector[2] = normalize(V * vec4(-1, 0.35, 0, 1) - V*M*vertex4);//wektor do światła w przestrzeni oka
    lightVector[3] = normalize(V * vec4(0, 0.35, 1, 1) - V*M*vertex4);//wektor do światła w przestrzeni oka
    lightVector[4] = normalize(V * vec4(0, 0.35, -1, 1) - V*M*vertex4);//wektor do światła w przestrzeni oka
    viewerVector = normalize(vec4(0, 0, 0, 1) - V * M * vertex4);//wektor do obserwatora w przestrzeni oka
    normalVector = normalize(V * M * normal4);//wektor normalny w przestrzeni oka

    iTexCoord0 = texCoord0;//interpolowane współrzędne teksturowania

    gl_Position=P*V*M*vertex4;//pozycja wierzchołka
}
