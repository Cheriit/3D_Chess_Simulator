#pragma once
#include "Camera.h"
#include "../constants.h"
#include <iostream>
Camera::Camera(glm::vec3 Position, glm::vec3 Face)
	: Position(Position), Face(Face)
{

}
glm::mat4 Camera::GetCameraMatrix()
{
	return glm::lookAt(
        Position,
        Position + Face,           // Obserwowany punkt
        glm::vec3(0.0f, 1.0f, 0.0f)   // Wektor nosa
    );
}
void Camera::CameraKeyCallback(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float translate = CAMERA_MOVEMENT_SPEED * glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        Position +=  glm::vec3(translate, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        Position -= glm::vec3(translate, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        Position += glm::vec3(0, 0, translate);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        Position -= glm::vec3(0, 0, translate);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += glm::vec3(0, translate, 0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        Position -= glm::vec3(0, translate, 0);
}
