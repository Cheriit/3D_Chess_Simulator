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
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "OBJ_Loader/OBJ_Loader.h"
#include <stdlib.h>
#include <stdio.h>

#include "../constants.h"
#include "ShaderProgram.h"
#include "Drawable.h"
#include "Texture.h"
#include "Skybox.h"

float speed_x = 0;
float speed_y = 0;
float aspectRatio = 1;
float zoom_speed = 0;
ShaderProgram *sp;

Texture *tex0;
Texture *tex1;

VertexArray* VAO;
ElementBuffer* EBO;

Skybox* skybox;

Drawable* object = 0;
Drawable* object2 = 0;

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

void processMesh(objl::Mesh Mesh)
{
    int vertexCount = Mesh.Vertices.size();
    std::vector<float> vertices;
    objl::Vertex vertex;
    for (int i = 0; i < vertexCount; i++)
    {
        vertex = Mesh.Vertices[i];
        vertices.push_back(vertex.Position.X);
        vertices.push_back(vertex.Position.Y);
        vertices.push_back(vertex.Position.Z);

        vertices.push_back(vertex.Normal.X);
        vertices.push_back(vertex.Normal.Y);
        vertices.push_back(vertex.Normal.Z);

        vertices.push_back(vertex.TextureCoordinate.X);
        vertices.push_back(vertex.TextureCoordinate.Y);
    }

    GLCall(VAO = new VertexArray());
    VAO->Bind();
    GLCall(VertexBuffer* VBO = new VertexBuffer(vertices.data(), vertexCount * 8));
    GLCall(VAO->AddLayout(*VBO, 0, 3, 8, 0));
    GLCall(VAO->AddLayout(*VBO, 1, 3, 8, 3));
    GLCall(VAO->AddLayout(*VBO, 2, 2, 8, 6));

    GLCall(EBO = new ElementBuffer(Mesh.Indices.data(), Mesh.Indices.size()));
}

void initOpenGLProgram(GLFWwindow *window)
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    glEnable(GL_MULTISAMPLE);

    sp = new ShaderProgram("./res/shaders/v_simplest.glsl", NULL, "./res/shaders/f_simplest.glsl");

    objl::Loader Loader;
    if (!Loader.LoadFile("./res/models/PiezasAjedrez.obj"))
    {
        fprintf(stderr, "Nie można wczytać siatki obiektów.\n");
        exit(EXIT_FAILURE);
    }

    tex0 = new Texture("./res/textures/chess/TableroDiffuse02.png", "textureMap0");
    //tex1 = new Texture("./res/textures/chess/Tableroambient.png", "textureMap1");
    tex1 = new Texture("./res/textures/chess/PiezasAjedrezDiffuseMarmol.png", "textureMap0");

    processMesh(Loader.LoadedMeshes[0]);
    object = new Drawable(VAO, EBO, sp);
    object->PushTexture(tex0);

    processMesh(Loader.LoadedMeshes[1]);
    object2 = new Drawable(VAO, EBO, sp);
    object2->PushTexture(tex1);
    //object->PushTexture(tex1);

    GLCall(skybox = new Skybox());
}

void freeOpenGLProgram(GLFWwindow *window)
{
}

void drawScene(GLFWwindow *window, float angle_x, float angle_y, float zoom)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = glm::lookAt(
        glm::vec3(0, 0, -0.5), // Pozycja
        glm::vec3(0, 0, 0),           // Obserwowany punkt
        glm::vec3(0.0f, 1.0f, 0.0f)   // Wektor nosa
    );

    glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f);

    glm::mat4 M = glm::mat4(1.0f);
    M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); 
    M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); 

    sp->use(); 

    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

    GLCall(object->Draw());
    GLCall(object2->Draw());
    GLCall(skybox->Draw(glm::mat4(glm::mat3(V)), P));
    glfwSwapBuffers(window);
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        fprintf(stderr, "Nie można zainicjować GLFW.\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);

    if (!window)
    {
        fprintf(stderr, "Nie można utworzyć okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);         
    glfwSwapInterval(1);
    {
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Nie można zainicjować GLEW.\n");
            exit(EXIT_FAILURE);
        }

        initOpenGLProgram(window);

        double angle_x = 0;
        double angle_y = 0;
        double zoom = 1;

        //Główna pętla
        glUseProgram(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glfwSetTime(0);
        while (!glfwWindowShouldClose(window))
        {
            angle_x += speed_x * glfwGetTime();
            angle_y += speed_y * glfwGetTime();
            zoom += zoom_speed * glfwGetTime();
            glfwSetTime(0);
            drawScene(window, angle_x, angle_y, zoom);
            glfwPollEvents();
        }

        freeOpenGLProgram(window);
        glfwDestroyWindow(window);
}
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
