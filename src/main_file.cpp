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
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/OBJ_Loader/OBJ_Loader.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include "../constants.h"
#include "ShaderProgram.h"
#include "Animated.h"
#include "Texture.h"
#include "Skybox.h"
#include "Drawable.h"
#include "Camera.h"

float aspectRatio = 1;
ShaderProgram *sp;

Texture *tex0;
Texture *tex1;
Texture *tex2;

VertexArray *VAO;
ElementBuffer *EBO;

Skybox *skybox;

Drawable *board = 0;
Animated *piece[8][8] = {nullptr};

Camera *camera;

std::ifstream inputStream;

void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {
    if (height == 0)
        return;
    aspectRatio = (float) width / (float) height;
    glViewport(0, 0, width, height);
}

void processMesh(objl::Mesh Mesh) {
    int vertexCount = Mesh.Vertices.size();
    std::vector<float> vertices;
    objl::Vertex vertex;
    for (int i = 0; i < vertexCount; i++) {
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

    VAO = new VertexArray();
    VAO->Bind();
    VertexBuffer *VBO = new VertexBuffer(vertices.data(), vertexCount * 8);
    VAO->AddLayout(*VBO, 0, 3, 8, 0);
    VAO->AddLayout(*VBO, 1, 3, 8, 3);
    VAO->AddLayout(*VBO, 2, 2, 8, 6);

    EBO = new ElementBuffer(Mesh.Indices.data(), Mesh.Indices.size());
}

void initOpenGLProgram(GLFWwindow *window) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glEnable(GL_MULTISAMPLE);

    camera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
    sp = new ShaderProgram("./res/shaders/v_simplest.glsl", NULL, "./res/shaders/f_simplest.glsl");

    objl::Loader Loader;
    if (!Loader.LoadFile("./res/models/PiezasAjedrez.obj")) {
        fprintf(stderr, "Nie można wczytać siatki obiektów.\n");
        exit(EXIT_FAILURE);
    }


    tex0 = new Texture("./res/textures/chess/TableroDiffuse02.png", "textureMap0");
    tex1 = new Texture("./res/textures/chess/Tableroambient.png", "textureMap1");
    //tex1 = new Texture("./res/textures/chess/PiezasAjedrezDiffuseMarmol.png", "textureMap0");
    tex2 = new Texture("./res/textures/chess/PiezasAjedrezDiffuseMarmolBlack.png", "textureMap0");

    processMesh(Loader.LoadedMeshes[0]);
    board = new Drawable(VAO, EBO, sp);
    board->PushTexture(tex0);

    // generate bishops
    processMesh(Loader.LoadedMeshes[1]);
    for (int i = 0; i < 2; i++) {
        piece[2 + 3 * i][0] = new Animated(glm::vec3(-BOARD_CORNER + (2 + 3 * i) * FIELD_SIZE, 0, BOARD_CORNER),
                                           glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[2 + 3 * i][0]->PushTexture(tex1);
        piece[2 + 3 * i][7] = new Animated(glm::vec3(-BOARD_CORNER + (2 + 3 * i) * FIELD_SIZE, 0, -BOARD_CORNER),
                                           glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[2 + 3 * i][7]->PushTexture(tex2);
    }

    // generate rooks
    processMesh(Loader.LoadedMeshes[2]);
    for (int i = 0; i < 2; i++) {
        piece[7 * i][0] = new Animated(glm::vec3(-BOARD_CORNER + 7 * i * FIELD_SIZE, 0, BOARD_CORNER),
                                       glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[7 * i][0]->PushTexture(tex1);
        piece[7 * i][7] = new Animated(glm::vec3(-BOARD_CORNER + 7 * i * FIELD_SIZE, 0, -BOARD_CORNER),
                                       glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[7 * i][7]->PushTexture(tex2);
    }

    // generate knights
    processMesh(Loader.LoadedMeshes[3]);
    for (int i = 0; i < 2; i++) {
        piece[1 + 5 * i][0] = new Animated(glm::vec3(-BOARD_CORNER + (1 + 5 * i) * FIELD_SIZE, 0, BOARD_CORNER),
                                           glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[1 + 5 * i][0]->PushTexture(tex1);
        piece[1 + 5 * i][7] = new Animated(glm::vec3(-BOARD_CORNER + (1 + 5 * i) * FIELD_SIZE, 0, -BOARD_CORNER),
                                           glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[1 + 5 * i][7]->PushTexture(tex2);
    }

    // generate queens
    processMesh(Loader.LoadedMeshes[4]);

    piece[3][0] = new Animated(glm::vec3(-BOARD_CORNER + 3 * FIELD_SIZE, 0, BOARD_CORNER),
                               glm::vec3(0, 0, 0), VAO, EBO, sp);
    piece[3][0]->PushTexture(tex1);
    piece[3][7] = new Animated(glm::vec3(-BOARD_CORNER + 3 * FIELD_SIZE, 0, -BOARD_CORNER),
                               glm::vec3(0, 0, 0), VAO, EBO, sp);
    piece[3][7]->PushTexture(tex2);

    //generate kings
    processMesh(Loader.LoadedMeshes[5]);

    piece[4][0] = new Animated(glm::vec3(-BOARD_CORNER + 4 * FIELD_SIZE, 0, BOARD_CORNER),
                               glm::vec3(0, 0, 0), VAO, EBO, sp);
    piece[4][0]->PushTexture(tex1);
    piece[4][7] = new Animated(glm::vec3(-BOARD_CORNER + 4 * FIELD_SIZE, 0, -BOARD_CORNER),
                               glm::vec3(0, 0, 0), VAO, EBO, sp);
    piece[4][7]->PushTexture(tex2);

    // generate pawns
    processMesh(Loader.LoadedMeshes[6]);
    for (int i = 0; i < 8; i++) {
        piece[i][1] = new Animated(glm::vec3(-BOARD_CORNER + FIELD_SIZE * i, 0, BOARD_CORNER - FIELD_SIZE),
                                   glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[i][1]->PushTexture(tex1);
        piece[i][6] = new Animated(glm::vec3(-BOARD_CORNER + FIELD_SIZE * i, 0, -BOARD_CORNER + FIELD_SIZE),
                                   glm::vec3(0, 0, 0), VAO, EBO, sp);
        piece[i][6]->PushTexture(tex2);
    }

    skybox = new Skybox();

    inputStream.open(INPUT_FILE);
}

void freeOpenGLProgram(GLFWwindow *window) {
}

std::vector<int> nextMove(std::vector<int> move) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (piece[i][j] != nullptr && piece[i][j]->isAnimationInProgress()) {
                return move;
            }
        }
    }

    std::string data;
    getline(inputStream, data);
    move[0] = data[0];
    if (move[0] == 'M' || move[0] == 'E') { // M(Move) / E(En passant) Piece Destination
        move[1] = data[2] - 'A';
        move[2] = data[3] - '0';
        move[3] = data[5] - 'A';
        move[4] = data[6] - '0';
    } else if (move[0] == 'P') { //P(Promotion) Piece Mesh
        move[1] = data[2] - 'A';
        move[2] = data[3] - '0';
        move[3] = data[5] - '0';
    } else if () { //C (Castling) King Destination Rook Destination
        move[1] = data[2] - 'A';
        move[2] = data[3] - '0';
        move[3] = data[5] - 'A';
        move[4] = data[6] - '0';
        move[5] = data[8] - 'A';
        move[6] = data[9] - '0';
        move[7] = data[11] - 'A';
        move[8] = data[12] - '0';
    }
    return move;
}

void drawScene(GLFWwindow *window, std::vector<int> move) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = camera->GetCameraMatrix();

    glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f);

    glm::mat4 M = glm::mat4(1.0f);

    sp->use();
    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

    board->Draw(M);
    M = glm::translate(M, glm::vec3(0, 0.025, 0));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (piece[i][j] != nullptr) {
                piece[i][j]->Draw(M);
            }
        }
    }

    skybox->Draw(glm::mat4(glm::mat3(V)), P);
    glfwSwapBuffers(window);
}

int main(void) {

    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Nie można zainicjować GLFW.\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!window) {
        fprintf(stderr, "Nie można utworzyć okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    {
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Nie można zainicjować GLEW.\n");
            exit(EXIT_FAILURE);
        }

        initOpenGLProgram(window);

        //Główna pętla
        glUseProgram(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glfwSetTime(0);
        std::vector<int> move(9, -1);
        while (!glfwWindowShouldClose(window)) {
            camera->CameraMouseCallback(window);
            camera->CameraKeyCallback(window);
            glfwSetTime(0);
            move = nextMove(move);
            drawScene(window, move);
            glfwPollEvents();
        }

        freeOpenGLProgram(window);
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
