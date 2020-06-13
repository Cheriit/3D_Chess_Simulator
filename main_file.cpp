/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "src/libs/lodepng.h"
#include "src/libs/OBJ_Loader.h"
#include "shaders/shaderprogram.h"
#include "src/models/myCube.h"
#include "src/models/myTeapot.h"
#include "src/models/skybox.h"

float speed_x = 0;
float speed_y = 0;
float aspectRatio = 1;
float zoom_speed = 0;
ShaderProgram *sp;
ShaderProgram *skyboxShader;

GLuint tex0;
GLuint tex1;
GLuint CubeMap;

GLuint *object;
GLuint Cube;

//Odkomentuj, żeby rysować kostkę
float *vertices = myCubeVertices;
float *normals = myCubeNormals;
float *texCoords = myCubeTexCoords;
int vertexCount = myCubeVertexCount;
int vertexCountTmp = 0;

//Procedura obsługi błędów
void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_LEFT)
            speed_x = -PI / 2;
        if (key == GLFW_KEY_RIGHT)
            speed_x = PI / 2;
        if (key == GLFW_KEY_UP)
            speed_y = PI / 2;
        if (key == GLFW_KEY_DOWN)
            speed_y = -PI / 2;
        if (key == GLFW_KEY_Z)
            zoom_speed = PI / 2;
        if (key == GLFW_KEY_X)
            zoom_speed = -PI / 2;
    }
    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_LEFT)
            speed_x = 0;
        if (key == GLFW_KEY_RIGHT)
            speed_x = 0;
        if (key == GLFW_KEY_UP)
            speed_y = 0;
        if (key == GLFW_KEY_DOWN)
            speed_y = 0;
        if (key == GLFW_KEY_Z)
            zoom_speed = 0;
        if (key == GLFW_KEY_X)
            zoom_speed = 0;
    }
}

void windowResizeCallback(GLFWwindow *window, int width, int height)
{
    if (height == 0)
        return;
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

GLuint readTexture(const char *filename)
{
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    // wczytanie do pamieci komputera
    std::vector<unsigned char> image; // wektor do wczytania obrazu
    unsigned width, height;           //wymiary
    // wczytanie obrazu
    unsigned error = lodepng::decode(image, width, height, filename);

    //Import do pamięci karty graficznej
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex); //uaktywnij uchwyt
    //Wczytaj obraz do pamieci Karty Graficznej skojaroznej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return tex;
}

GLuint setupSkybox()
{
    std::vector<std::string> faces = {
        "./src/textures/skybox/right.png",
        "./src/textures/skybox/left.png",
        "./src/textures/skybox/top.png",
        "./src/textures/skybox/bottom.png",
        "./src/textures/skybox/front.png",
        "./src/textures/skybox/back.png",
    };
    GLuint cubeMap;
    std::vector<unsigned char> image;
    unsigned width, height;
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    for (int i = 0; i < 6; i++)
    {
        unsigned error = lodepng::decode(image, width, height, faces[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)image.data());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenBuffers(1, &Cube);
    glBindBuffer(GL_ARRAY_BUFFER, Cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(myCubeVertices), skyboxVertices, GL_STATIC_DRAW);

    return cubeMap;
}
/**
 * Moves model to VBO
 * @param vertices  array with vertexes, each 4 numbers describes 1 vertex
 * @param normals   array with normals, each 4 numbers describes 1 normal
 * @param texCoords array with texturing coordinates, each 2 numbers describes 1 texturing coordinate
 * @param vertexCount amount of vertexes in mesh
 * @return VBO buffer array, with indexes 0 - vertices, 1 - normals, 2 - texturing coordinates
 **/
GLuint *bufferModel(objl::Mesh Mesh)
{
    int vertexCount = Mesh.Indices.size();
    vertexCountTmp = vertexCount;
    float vertices[vertexCount * 3];
    float normals[vertexCount * 3];
    float texCoords[vertexCount * 2];

    int j = 0;
    objl::Vertex vertex;
    for (int i = 0; i < vertexCount; i++)
    {
        vertex = Mesh.Vertices[Mesh.Indices[i]];
        vertices[j] = vertex.Position.X;
        vertices[j + 1] = vertex.Position.Y;
        vertices[j + 2] = vertex.Position.Z;

        normals[j] = vertex.Normal.X;
        normals[j + 1] = vertex.Normal.Y;
        normals[j + 2] = vertex.Normal.Z;

        texCoords[j - i] = vertex.TextureCoordinate.X;
        texCoords[j - i + 1] = vertex.TextureCoordinate.Y;
        j += 3;
    }

    GLuint *bufVertices = (GLuint *)malloc(sizeof(GLuint) * 3);
    glGenBuffers(3, bufVertices);
    glBindBuffer(GL_ARRAY_BUFFER, bufVertices[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, bufVertices[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, bufVertices[2]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 2, texCoords, GL_STATIC_DRAW);

    return bufVertices;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow *window)
{
    objl::Loader Loader;
    bool results = Loader.LoadFile("./src/models/PiezasAjedrez.obj");
    if (!results)
    {
        fprintf(stderr, "Nie można wczytać siatki obiektów.\n");
        exit(EXIT_FAILURE);
    }
    //************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    sp = new ShaderProgram("shaders/v_simplest.glsl", NULL, "shaders/f_simplest.glsl");
    skyboxShader = new ShaderProgram("shaders/v_skybox.glsl", NULL, "shaders/f_skybox.glsl");
    tex0 = readTexture("src/textures/metal.png");
    tex1 = readTexture("src/textures/metal_spec.png");
    object = bufferModel(Loader.LoadedMeshes[0]);
    CubeMap = setupSkybox();
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow *window)
{
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
    glDeleteTextures(1, &tex0);
    glDeleteTextures(1, &tex1);
    glDeleteTextures(1, &CubeMap);
    glDeleteBuffers(3, object);
    glDeleteBuffers(1, &Cube);
    delete object;
    delete sp;
    delete skyboxShader;
}
/**
 * Draws object
 * @param vbo vbo buffer array
 * @param tex0 base texture
 * @param tex1 diffuse map
 * @param vertexCount amount of vertices 
 **/
void drawObject(GLuint *vbo, GLuint tex0, GLuint tex1, int vertexCount)
{
    glEnableVertexAttribArray(sp->a("vertex"));                       //Włącz przesyłanie danych do atrybutu vertex
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);                            //Przypisz odpowieni bufor danych ze współrzędnymi wierzchołków
    glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, 0); //Wskaż bufor dla atrybutu vertex

    glEnableVertexAttribArray(sp->a("normal"));                       //Włącz przesyłanie danych do atrybutu normal
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);                            //Przypisz odpowieni bufor danych ze współrzędnymi wektorów normalnych
    glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, 0); //Wskaż bufor dla atrybutu normal

    glEnableVertexAttribArray(sp->a("texCoord0"));                       //Włącz przesyłanie danych do atrybutu texCoord
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);                               //Przypisz odpowieni bufor danych ze współrzędnymi teksturowania
    glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, 0); //Wskaż bufor dla atrybutu texCoord

    glUniform1i(sp->u("textureMap0"), 0); //Przypisz do zmiennej jednorodnej nr. jednostki teksturującej
    glActiveTexture(GL_TEXTURE0);         //Aktywuj jednostkę teksturującą
    glBindTexture(GL_TEXTURE_2D, tex0);   //Przypisz teksturę do jednostki teksturującej

    glUniform1i(sp->u("textureMap1"), 1); //Przypisz do zmiennej jednorodnej nr. jednostki teksturującej
    glActiveTexture(GL_TEXTURE1);         //Aktywuj jednostkę teksturującą
    glBindTexture(GL_TEXTURE_2D, tex1);   //Przypisz teksturę do jednostki teksturującej

    glDrawArrays(GL_TRIANGLES, 0, vertexCountTmp); //Narysuj obiekt

    glDisableVertexAttribArray(sp->a("vertex"));    //Wyłącz przesyłanie danych do atrybutu vertex
    glDisableVertexAttribArray(sp->a("normal"));    //Wyłącz przesyłanie danych do atrybutu normal
    glDisableVertexAttribArray(sp->a("texCoord0")); //Wyłącz przesyłanie danych do atrybutu texCoord
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow *window, float angle_x, float angle_y, float zoom)
{
    //************Tutaj umieszczaj kod rysujący obraz******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = glm::lookAt(
        glm::vec3(0, 0, -2.5 * zoom), // Pozycja
        glm::vec3(0, 0, 0),           // Obserwowany punkt
        glm::vec3(0.0f, 1.0f, 0.0f)   // Wektor nosa
    );

    glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 M = glm::mat4(1.0f);
    M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
    M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu

    sp->use(); //Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

    // drawObject(object, tex0, tex1, vertexCount);

    glDepthMask(GL_FALSE);
    skyboxShader->use();
    glUniformMatrix4fv(skyboxShader->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(skyboxShader->u("V"), 1, false, glm::value_ptr(V));
    glBindVertexArray(Cube);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

int main(void)
{
    GLFWwindow *window; //Wskaźnik na obiekt reprezentujący okno

    glfwSetErrorCallback(error_callback); //Zarejestruj procedurę obsługi błędów

    if (!glfwInit())
    { //Zainicjuj bibliotekę GLFW
        fprintf(stderr, "Nie można zainicjować GLFW.\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(500, 500, "OpenGL", NULL,
                              NULL); //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

    if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
    {
        fprintf(stderr, "Nie można utworzyć okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(
        window);         //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
    glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

    if (glewInit() != GLEW_OK)
    { //Zainicjuj bibliotekę GLEW
        fprintf(stderr, "Nie można zainicjować GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window); //Operacje inicjujące

    float angle_x = 0; //Aktualny kąt obrotu obiektu
    float angle_y = 0; //Aktualny kąt obrotu obiektu
    float zoom = 1;    //Aktualne przybliżenie

    //Główna pętla
    glfwSetTime(0);                        //Zeruj timer
    while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
    {
        angle_x += speed_x *
                   glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        angle_y += speed_y *
                   glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        zoom += zoom_speed * glfwGetTime();
        glfwSetTime(0);                            //Zeruj timer
        drawScene(window, angle_x, angle_y, zoom); //Wykonaj procedurę rysującą
        glfwPollEvents();                          //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
    }

    freeOpenGLProgram(window);

    glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
    glfwTerminate();           //Zwolnij zasoby zajęte przez GLFW
    exit(EXIT_SUCCESS);
}
