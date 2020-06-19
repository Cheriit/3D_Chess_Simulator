#pragma once
#include "Camera.h"
#include "../constants.h"
#include <iostream>
Camera::Camera(glm::vec3 Position, glm::vec3 Face, glm::vec3 Right)
	: Position(Position), Face(Face), Right(Right)
{
    Yaw = 0;
    Pitch = 0;
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
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
    {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float translate = CAMERA_KEY_SPEED * glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += Face * translate;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Face * translate;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position += glm::normalize(glm::cross(glm::vec3(0, 1, 0), Face)) * translate;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position -= glm::normalize(glm::cross(glm::vec3(0, 1, 0), Face)) * translate;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += glm::vec3(0, translate, 0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        Position -= glm::vec3(0, translate, 0);
    }
}

void Camera::CameraMouseCallback(GLFWwindow* window)
{
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &yPos, &xPos);
        Pitch = -1 * xPos * CAMERA_MOUSE_SPEED;
        Yaw = yPos * CAMERA_MOUSE_SPEED;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        UpdateFace();
    }
}

void Camera::UpdateFace()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Face = glm::normalize(front);
}

