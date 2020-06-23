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

objl::Loader Loader;

Skybox *skybox;

Drawable *board = 0;
Animated *pieces[8][8] = {nullptr};
std::vector<Animated *> CaptureWhitePieces;
std::vector<Animated *> CaptureBlackPieces;
int firstPieceStatus = 1;
int secondPieceStatus = 1;

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

    camera = new Camera(glm::vec3(0, 0.3, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
    sp = new ShaderProgram("./res/shaders/v_shader.glsl", NULL, "./res/shaders/f_shader.glsl");


    if (!Loader.LoadFile("./res/models/PiezasAjedrez.obj")) {
        fprintf(stderr, "Nie można wczytać siatki obiektów.\n");
        exit(EXIT_FAILURE);
    }

    tex0 = new Texture("./res/textures/chess/Tableroambient.png", "textureMap0");
    tex1 = new Texture("./res/textures/chess/TableroDiffuse02.png", "textureMap1");
    processMesh(Loader.LoadedMeshes[0]);
    board = new Drawable(VAO, EBO, sp);
    board->PushTexture(tex0);
    board->PushTexture(tex1);

    tex0 = new Texture("./res/textures/chess/Ambient.png", "textureMap0");
    tex1 = new Texture("./res/textures/chess/PiezasAjedrezDiffuseMarmol.png", "textureMap1");
    tex2 = new Texture("./res/textures/chess/PiezasAjedrezDiffuseMarmolBlack.png", "textureMap1");

    // generate bishops
    processMesh(Loader.LoadedMeshes[1]);
    for (int i = 0; i < 2; i++) {
        pieces[2 + 3 * i][0] = new Animated(glm::vec3(-BOARD_CORNER + (2 + 3 * i) * FIELD_SIZE, 0, BOARD_CORNER),
                                            glm::vec3(0, 0, 0), WHITE, VAO, EBO, sp);
        pieces[2 + 3 * i][0]->PushTexture(tex0);
        pieces[2 + 3 * i][0]->PushTexture(tex1);
        pieces[2 + 3 * i][7] = new Animated(glm::vec3(-BOARD_CORNER + (2 + 3 * i) * FIELD_SIZE, 0, -BOARD_CORNER),
                                            glm::vec3(0, 0, 0), BLACK, VAO, EBO, sp);
        pieces[2 + 3 * i][7]->PushTexture(tex0);
        pieces[2 + 3 * i][7]->PushTexture(tex2);
    }

    // generate rooks
    processMesh(Loader.LoadedMeshes[2]);
    for (int i = 0; i < 2; i++) {
        pieces[7 * i][0] = new Animated(glm::vec3(-BOARD_CORNER + 7 * i * FIELD_SIZE, 0, BOARD_CORNER),
                                        glm::vec3(0, 0, 0), WHITE, VAO, EBO, sp);
        pieces[7 * i][0]->PushTexture(tex0);
        pieces[7 * i][0]->PushTexture(tex1);
        pieces[7 * i][7] = new Animated(glm::vec3(-BOARD_CORNER + 7 * i * FIELD_SIZE, 0, -BOARD_CORNER),
                                        glm::vec3(0, 0, 0), BLACK, VAO, EBO, sp);
        pieces[7 * i][7]->PushTexture(tex0);
        pieces[7 * i][7]->PushTexture(tex2);
    }

    // generate knights
    processMesh(Loader.LoadedMeshes[3]);
    for (int i = 0; i < 2; i++) {
        pieces[1 + 5 * i][0] = new Animated(glm::vec3(-BOARD_CORNER + (1 + 5 * i) * FIELD_SIZE, 0, BOARD_CORNER),
                                            glm::vec3(0, 0, 0), WHITE, VAO, EBO, sp);
        pieces[1 + 5 * i][0]->PushTexture(tex0);
        pieces[1 + 5 * i][0]->PushTexture(tex1);
        pieces[1 + 5 * i][7] = new Animated(glm::vec3(-BOARD_CORNER + (1 + 5 * i) * FIELD_SIZE, 0, -BOARD_CORNER),
                                            glm::vec3(0, 0, 0), BLACK, VAO, EBO, sp);
        pieces[1 + 5 * i][7]->PushTexture(tex0);
        pieces[1 + 5 * i][7]->PushTexture(tex2);
    }

    // generate queens
    processMesh(Loader.LoadedMeshes[4]);

    pieces[3][0] = new Animated(glm::vec3(-BOARD_CORNER + 3 * FIELD_SIZE, 0, BOARD_CORNER),
                                glm::vec3(0, 0, 0), WHITE, VAO, EBO, sp);
    pieces[3][0]->PushTexture(tex0);
    pieces[3][0]->PushTexture(tex1);
    pieces[3][7] = new Animated(glm::vec3(-BOARD_CORNER + 3 * FIELD_SIZE, 0, -BOARD_CORNER),
                                glm::vec3(0, 0, 0), BLACK, VAO, EBO, sp);
    pieces[3][7]->PushTexture(tex0);
    pieces[3][7]->PushTexture(tex2);

    //generate kings
    processMesh(Loader.LoadedMeshes[5]);

    pieces[4][0] = new Animated(glm::vec3(-BOARD_CORNER + 4 * FIELD_SIZE, 0, BOARD_CORNER),
                                glm::vec3(0, 0, 0), WHITE, VAO, EBO, sp);
    pieces[4][0]->PushTexture(tex0);
    pieces[4][0]->PushTexture(tex1);
    pieces[4][7] = new Animated(glm::vec3(-BOARD_CORNER + 4 * FIELD_SIZE, 0, -BOARD_CORNER),
                                glm::vec3(0, 0, 0), BLACK, VAO, EBO, sp);
    pieces[4][7]->PushTexture(tex0);
    pieces[4][7]->PushTexture(tex2);

    // generate pawns
    processMesh(Loader.LoadedMeshes[6]);
    for (int i = 0; i < 8; i++) {
        pieces[i][1] = new Animated(glm::vec3(-BOARD_CORNER + FIELD_SIZE * i, 0, BOARD_CORNER - FIELD_SIZE),
                                    glm::vec3(0, 0, 0), WHITE, VAO, EBO, sp);
        pieces[i][1]->PushTexture(tex0);
        pieces[i][1]->PushTexture(tex1);
        pieces[i][6] = new Animated(glm::vec3(-BOARD_CORNER + FIELD_SIZE * i, 0, -BOARD_CORNER + FIELD_SIZE),
                                    glm::vec3(0, 0, 0), BLACK, VAO, EBO, sp);
        pieces[i][6]->PushTexture(tex0);
        pieces[i][6]->PushTexture(tex2);
    }

    skybox = new Skybox();

    inputStream.open(INPUT_FILE);
}

void freeOpenGLProgram(GLFWwindow *window) {
    inputStream.close();
}

std::vector<int> nextAction(std::vector<int> action) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[i][j] != nullptr && pieces[i][j]->isAnimationInProgress()) {
                return action;
            }
        }
    }

    std::string data;
    getline(inputStream, data);
    action[0] = data[0];
    action[1] = data[2] - 'A';
    action[2] = data[3] - '1';
    if (action[0] == 'M') { // M(Move) Piece Destination
        action[3] = data[5] - 'A';
        action[4] = data[6] - '1';
    } else if (action[0] == 'P') { //P(Promotion) Piece Mesh
        action[3] = data[5] - '0';
    } else if (action[0] == 'C') { //C (Castling) King Destination Rook Destination
        action[3] = data[5] - 'A';
        action[4] = data[6] - '1';
        action[5] = data[8] - 'A';
        action[6] = data[9] - '1';
        action[7] = data[11] - 'A';
        action[8] = data[12] - '1';
    } else if (action[0] == 'E') { // E(En passant) Piece Destination Piece
        action[3] = data[5] - 'A';
        action[4] = data[6] - '1';
        action[5] = data[8] - 'A';
        action[6] = data[9] - '1';
    }
    return action;
}

glm::vec3 destination(int x, int y, float height) {
    return glm::vec3(-BOARD_CORNER + x * FIELD_SIZE, height, BOARD_CORNER - y * FIELD_SIZE);
}

glm::vec3 destinationCaptured(bool is_white) {
    if (is_white) {
        int x = CaptureWhitePieces.size() / 8;
        int y = CaptureWhitePieces.size() % 8;
        return glm::vec3(CAPTURED_WHITE + x * FIELD_SIZE, 2 * HEIGHT, -BOARD_CORNER + y * FIELD_SIZE);
    } else {
        int x = CaptureBlackPieces.size() / 8;
        int y = CaptureBlackPieces.size() % 8;
        return glm::vec3(CAPTURED_BLACK + x * FIELD_SIZE, 2 * HEIGHT, BOARD_CORNER - y * FIELD_SIZE);
    }
}

int move(std::vector<int> action, int status) {
    Animated *piece = pieces[action[1]][action[2]];
    piece->startAnimation();
    glm::vec3 dest = destination(action[3], action[4], HEIGHT);
    status = piece->Move(dest, status, HEIGHT);
    if (status == 0) {
        pieces[action[3]][action[4]] = piece;
        pieces[action[1]][action[2]] = nullptr;
        piece->endAnimation();
    }
    return status;
}

int capture(std::vector<int> action, int status, bool enPassant) {
    Animated *piece = pieces[action[1]][action[2]];
    piece->startAnimation();
    glm::vec3 pieceDest = destination(action[3], action[4], 0);

    Animated *capturedPiece;
    glm::vec3 capturedDest;
    if (enPassant) {
        capturedPiece = pieces[action[5]][action[6]];
        capturedPiece->startAnimation();
        capturedDest = destinationCaptured(capturedPiece->isWhite());
    } else {
        capturedPiece = pieces[action[3]][action[4]];
        capturedPiece->startAnimation();
        capturedDest = destinationCaptured(capturedPiece->isWhite());
    }

    if (firstPieceStatus) {
        firstPieceStatus = piece->Move(pieceDest, firstPieceStatus, 0);
    }
    if (secondPieceStatus) {
        secondPieceStatus = capturedPiece->Move(capturedDest, secondPieceStatus, HEIGHT);
    }
    if (firstPieceStatus == 0 && secondPieceStatus == 0) {
        if (capturedPiece->isWhite()) {
            CaptureWhitePieces.push_back(capturedPiece);
        } else {
            CaptureBlackPieces.push_back(capturedPiece);
        }

        if (enPassant) {
            pieces[action[5]][action[6]] = nullptr;
        } else {
            pieces[action[3]][action[4]] = nullptr;
        }

        pieces[action[3]][action[4]] = piece;
        pieces[action[1]][action[2]] = nullptr;
        firstPieceStatus = 1;
        secondPieceStatus = 1;
        piece->endAnimation();
        capturedPiece->endAnimation();
        return 0;
    }
    return 1;
}

int promotion(std::vector<int> action, int status) {
    Animated *piece = pieces[action[1]][action[2]];
    piece->startAnimation();

    if (status <= 1) {
        piece->MoveUp(1.0);
    } else if (piece->MoveDown()) {
        piece->endAnimation();
        return 0;
    }

    if (piece->GetPosition().y > 1.0) {
        processMesh(Loader.LoadedMeshes[action[3]]);
        pieces[action[1]][action[2]] = new Animated(piece->GetPosition(), piece->GetRotation(), piece->isWhite(),
                                                    VAO, EBO, sp);
        std::vector < Texture * > textures = piece->GetTextures();
        for (int i = 0; i < textures.size(); i++) {
            pieces[action[1]][action[2]]->PushTexture(textures[i]);
        }
        pieces[action[1]][action[2]]->startAnimation();
        return 2;
    } else if (status == 2) {
        return 2;
    } else {
        return 1;
    }
}

int castling(std::vector<int> action, int status) {
    Animated *king = pieces[action[1]][action[2]];
    king->startAnimation();
    Animated *rook = pieces[action[5]][action[6]];
    rook->startAnimation();
    glm::vec3 kingDest = destination(action[3], action[4], 0);
    glm::vec3 rookDest = destination(action[7], action[8], HEIGHT);
    if (firstPieceStatus) {
        firstPieceStatus = king->Move(kingDest, firstPieceStatus, 0);
    }
    if (secondPieceStatus) {
        secondPieceStatus = rook->Move(rookDest, secondPieceStatus, HEIGHT);
    }
    if (firstPieceStatus == 0 && secondPieceStatus == 0) {
        pieces[action[3]][action[4]] = king;
        pieces[action[1]][action[2]] = nullptr;
        pieces[action[7]][action[8]] = rook;
        pieces[action[5]][action[6]] = nullptr;
        firstPieceStatus = 1;
        secondPieceStatus = 1;
        king->endAnimation();
        rook->endAnimation();
        return 0;
    }
    return 1;
}

int makeAction(std::vector<int> action, int status) {
    int newStatus;
    if (action[0] == 'M') {
        if (pieces[action[3]][action[4]] == nullptr) {
            newStatus = move(action, status);
        } else {
            newStatus = capture(action, status, false);
        }
    } else if (action[0] == 'E') {
        newStatus = capture(action, status, true);
    } else if (action[0] == 'P') {
        newStatus = promotion(action, status);
    } else if (action[0] == 'C') {
        newStatus = castling(action, status);
    }
    return newStatus;
}

void drawScene(GLFWwindow *window, float angle) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = camera->GetCameraMatrix();

    glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f);

    glm::mat4 M = glm::mat4(1.0f);



    sp->use();
    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

    glm::mat4 lightM = glm::mat4(1.0f);
    lightM = glm::rotate(lightM, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4  lightPoint= lightM * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    glUniform4f(sp->u("lightSource"), lightPoint.x, lightPoint.y, lightPoint.z, 1);


    for (int i = 0; i < CaptureWhitePieces.size(); i++) {
        CaptureWhitePieces[i]->Draw(M);
    }

    for (int i = 0; i < CaptureBlackPieces.size(); i++) {
        CaptureBlackPieces[i]->Draw(M);
    }

    board->Draw(M);
    M = glm::translate(M, glm::vec3(0, 0.025, 0));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[i][j] != nullptr) {
                pieces[i][j]->Draw(M);
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
        std::vector<int> action(9, -1);
        float angle = 0;
        int status = 0;
        while (!glfwWindowShouldClose(window)) {
            camera->CameraMouseCallback(window);
            camera->CameraKeyCallback(window);
            glfwSetTime(0);
            action = nextAction(action);
            status = makeAction(action, status);
            angle += LIGHT_SPEED * glfwGetTime();
            drawScene(window, angle);
            glfwPollEvents();
        }

        freeOpenGLProgram(window);
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
