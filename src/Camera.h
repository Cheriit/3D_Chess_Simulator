#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

class Camera
{
private:
	glm::vec3 Position;
	glm::vec3 Face;
public:
	Camera(glm::vec3 Position, glm::vec3 Face);
	glm::mat4 GetCameraMatrix();
	void CameraKeyCallback(GLFWwindow* window);
};

#endif