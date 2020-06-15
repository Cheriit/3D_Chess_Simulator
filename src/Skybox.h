#pragma once
#include "Drawable.h"
//TODO Skybox class
class Skybox :
    public Drawable
{

};

//skyboxShader = new ShaderProgram("./res/shaders/v_skybox.glsl", NULL, "./res/shaders/f_skybox.glsl");
//CubeMap = setupSkybox();
//delete skyboxShader;
//glDeleteTextures(1, &CubeMap);
//
//glGenVertexArrays(1, &Cube);
//glBindBuffer(GL_ARRAY_BUFFER, Cube);
//glBufferData(GL_ARRAY_BUFFER, sizeof(myCubeVertices), skyboxVertices, GL_STATIC_DRAW);
//GLuint setupSkybox()
//{
//    std::vector<std::string> faces = {
//        "./res/textures/skybox/right.png",
//        "./res/textures/skybox/left.png",
//        "./res/textures/skybox/top.png",
//        "./res/textures/skybox/bottom.png",
//        "./res/textures/skybox/front.png",
//        "./res/textures/skybox/back.png",
//    };
//    GLuint cubeMap;
//    std::vector<unsigned char> image;
//    unsigned width, height;
//    glGenTextures(1, &cubeMap);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
//
//    for (int i = 0; i < 6; i++)
//    {
//        unsigned error = lodepng::decode(image, width, height, faces[i]);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//
//    return cubeMap;
//}

//glDepthFunc(GL_LEQUAL);
   //skyboxShader->use();
   //glUniformMatrix4fv(skyboxShader->u("M"), 1, false, glm::value_ptr(M));
   //glUniformMatrix4fv(skyboxShader->u("P"), 1, false, glm::value_ptr(P));
   //glUniformMatrix4fv(skyboxShader->u("V"), 1, false, glm::value_ptr(V));

   //glEnableVertexAttribArray(0);                      
   //glBindBuffer(GL_ARRAY_BUFFER, Cube);
   //glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

   //glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);
   //glDrawArrays(GL_TRIANGLES, 0, 36);
   //glDisableVertexAttribArray(0);
   //glDepthFunc(GL_LESS);
