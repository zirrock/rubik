#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"

using namespace glm;

ControlsManager::ControlsManager() {
	ControlsManager(vec3(10, 0, 0), 0.0f, 0.0f, 45.0f, 3.0f, 0.005f);
}

ControlsManager::ControlsManager(vec3 position, float horizontalAngle, float verticalAngle, 
								 float initialFoV, float speed, float mouseSpeed) {
	ControlsManager::position = position;
	ControlsManager::horizontalAngle = horizontalAngle;
	ControlsManager::verticalAngle = verticalAngle;
	ControlsManager::initialFoV = initialFoV;
	ControlsManager::speed = speed;
	ControlsManager::mouseSpeed = mouseSpeed;
	ControlsManager::lastTime = glfwGetTime();
}

mat4 ControlsManager::getProjectionMatrix() {
	return ControlsManager::ProjectionMatrix;
}

mat4 ControlsManager::getViewMatrix() {
	return ControlsManager::ViewMatrix;
}

float ControlsManager::getHorizontalAngle() {
	return horizontalAngle;
}

float ControlsManager::getVerticalAngle() {
	return verticalAngle;
}

void ControlsManager::computeMatricesFromInput(GLFWwindow* window) {

	double currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int width, height;

	glfwGetWindowSize(window, &width, &height);

	glfwSetCursorPos(window, width / 2, height / 2);

	horizontalAngle += mouseSpeed * float(width / 2 - xpos);
	verticalAngle = max(min(verticalAngle + mouseSpeed * float(height / 2 - ypos), 3.14f/2), -3.14f/2);

	double distance = 20.0f;

	vec3 direction(
		distance * sin(horizontalAngle) * cos(verticalAngle),
		distance * sin(verticalAngle),
		distance * cos(horizontalAngle) * cos(verticalAngle)
	);

	vec3 up = vec3(
		distance * sin(horizontalAngle) * cos(verticalAngle + (3.14f/2)),
		distance * sin(verticalAngle + (3.14f/2)),
		distance * cos(horizontalAngle) * cos(verticalAngle + (3.14f/2))
	);

	float FoV = initialFoV;

	ProjectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	ViewMatrix = lookAt(
		vec3(0,0,0) + direction,
		vec3(0,0,0),
		up
	);


	lastTime = currentTime;
}