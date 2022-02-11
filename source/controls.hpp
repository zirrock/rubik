#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

using namespace glm;

class ControlsManager {

private:
	vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;

	double lastTime;
public:
	ControlsManager();
	ControlsManager(vec3 position, float horizontalAngle, float verticalAngle, float initialFoV, float speed, float mouseSpeed);

	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
	void computeMatricesFromInput(GLFWwindow* window);


	float getHorizontalAngle();
	float getVerticalAngle();


};