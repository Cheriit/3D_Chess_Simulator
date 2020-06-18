#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
layout (location = 0) in vec3 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location = 1) in vec3 normal; //wektor normalny w przestrzeni modelu
layout (location = 2) in vec2 texCoord0;

//Zmienne interpolowane
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;

void main(void) {
    vec4 vertex_new = vec4(vertex, 1.0);
    vec4 normal_new = vec4(normal, 1.0);
    vec4 lp = vec4(0, 0, -150, 1);                        //pozcyja światła, przestrzeń świata
    l = normalize(V * lp - V*M*vertex_new);                 //wektor do światła w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex_new);   //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal_new);                      //wektor normalny w przestrzeni oka

    iTexCoord0 = texCoord0;                             //interpolowane współrzędne teksturowania
    
    gl_Position=P*V*M*vertex_new;                           //pozycja wierzchołka
}
