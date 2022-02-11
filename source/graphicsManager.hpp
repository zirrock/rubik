#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "controls.hpp"
#include "cube.hpp"

class GraphicsManager {

public:
	GLFWwindow* window;
	GLuint programID;
private:
	GLuint VertexArrayID;

	std::vector<Cube> cubesBuffer;
	int cubes[3][3][3];

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint highlightID;

	ControlsManager controlsManager;

	double currentTime;

	std::pair<int, int> currentRotation;
	float rotationLeft;
	float rotationSpeed;

	int highlightedWall;
	double lastTab;

public:
	GraphicsManager();
	~GraphicsManager();
	void init();
	void update();
	void rotateWall(int id, int direction);
	void highlightWall(int id);
	bool isRotating();
	void drawCube(int x, int y, int z);
	void rotateHighlighted(int direction);
	void cycleHighlight();
private:
	void addCube(const char* texPath, GLenum gl_texture, GLuint textureID, glm::vec3 pos);
	int &cubeId(int x, int y, int z);
	static glm::vec3 getTVector(int a, int b, int axis, int direction);
};