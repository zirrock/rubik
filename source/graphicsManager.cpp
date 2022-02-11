#include <cstdio>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include <common/objloader.hpp>
#include <common/shader.hpp>
#include "texture.hpp"
#include "graphicsManager.hpp"

GraphicsManager::GraphicsManager() {
	glewExperimental = true;

	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	controlsManager = ControlsManager(vec3(5, 3, 1), 0.0f, 0.0f, 45.0f, 3.0f, 0.005f);

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++) {
				cubes[x][y][z] = x * 9 + y * 3 + z;
			}
		}
	}
	lastTab = 0.0f;
}

void GraphicsManager::init() {
	window = glfwCreateWindow(1024, 768, "Rubik", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	GLuint textureID = glGetUniformLocation(programID, "myTextureSampler");

	cubesBuffer.reserve(30);
	addCube("textures/orangeGreenWhite.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, -2.0f, -2.0f));
	addCube("textures/orangeWhite.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, -2.0f, 0.0f));
	addCube("textures/orangeBlueWhite.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, -2.0f, 2.0f));

	addCube("textures/orangeGreen.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, 0.0f, -2.0f));
	addCube("textures/middleOrange.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, 0.0f, 0.0f));
	addCube("textures/orangeBlue.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, 0.0f, 2.0f));

	addCube("textures/orangeGreenYellow.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, 2.0f, -2.0f));
	addCube("textures/orangeYellow.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, 2.0f, 0.0f));
	addCube("textures/orangeBlueYellow.dds", GL_TEXTURE0, textureID, glm::vec3(-2.0f, 2.0f, 2.0f));

	addCube("textures/greenWhite.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, -2.0f, -2.0f));
	addCube("textures/middleWhite.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, -2.0f, 0.0f));
	addCube("textures/blueWhite.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, -2.0f, 2.0f));

	addCube("textures/middleGreen.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, 0.0f, -2.0f));
	addCube("textures/black.dds", GL_TEXTURE0, textureID, glm::vec3(0, 0, 0));
	addCube("textures/middleBlue.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, 0.0f, 2.0f));

	addCube("textures/greenYellow.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, 2.0f, -2.0f));
	addCube("textures/middleYellow.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, 2.0f, 0.0f));
	addCube("textures/blueYellow.dds", GL_TEXTURE0, textureID, glm::vec3(0.0f, 2.0f, 2.0f));

	addCube("textures/redWhiteGreen.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, -2.0f, -2.0f));
	addCube("textures/redWhite.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, -2.0f, 0.0f));
	addCube("textures/redWhiteBlue.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, -2.0f, 2.0f));

	addCube("textures/redGreen.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, 0.0f, -2.0f));
	addCube("textures/middleRed.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, 0.0f, 0.0f));
	addCube("textures/redBlue.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, 0.0f, 2.0f));

	addCube("textures/redYellowGreen.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, 2.0f, -2.0f));
	addCube("textures/redYellow.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, 2.0f, 0.0f));
	addCube("textures/redBlueYellow.dds", GL_TEXTURE0, textureID, glm::vec3(2.0f, 2.0f, 2.0f));

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	programID = LoadShaders("shaders/TransformVertexShader.vertexshader", "shaders/TextureFragmentShader.fragmentshader");


	MatrixID = glGetUniformLocation(programID, "MVP");
	highlightID = glGetUniformLocation(programID, "highlight");

	glUseProgram(programID);

	currentTime = glfwGetTime();
	rotationSpeed = 3.0f;
	rotationLeft = 0.0f;
	highlightedWall = 0;
}

void GraphicsManager::update() {
	float lastTime = currentTime;
	currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	if (rotationLeft > 0) {
		int id = currentRotation.first;
		int direction = currentRotation.second;
		glm::vec3 axis;
		switch (id / 3) {
		case 0:
			axis = glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
		case 1:
			axis = glm::vec3(0.0f, 0.0f, -1.0f);
			break;
		case 2:
			axis = glm::vec3(0.0f, -1.0f, 0.0f);
		}

		if (rotationSpeed * deltaTime < rotationLeft)
		{ 
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					cubesBuffer[cubeId(id, x, y)].translate(rotationSpeed * deltaTime * getTVector(x, y, id / 3, direction));
					cubesBuffer[cubeId(id, x, y)].rotate(rotationSpeed * deltaTime * direction * glm::radians(90.0f), axis);
				}
			}
			rotationLeft -= rotationSpeed * deltaTime;
		}
		else {
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					cubesBuffer[cubeId(id, x, y)].translate(rotationLeft * getTVector(x, y, id / 3, direction));
					cubesBuffer[cubeId(id, x, y)].rotate(rotationLeft * direction * glm::radians(90.0f), axis);
				}
			}
			if (direction == 1) {
				std::swap(cubeId(id, 0, 0), cubeId(id, 0, 2));
				std::swap(cubeId(id, 0, 0), cubeId(id, 2, 2));
				std::swap(cubeId(id, 0, 0), cubeId(id, 2, 0));
				std::swap(cubeId(id, 0, 1), cubeId(id, 1, 2));
				std::swap(cubeId(id, 0, 1), cubeId(id, 2, 1));
				std::swap(cubeId(id, 0, 1), cubeId(id, 1, 0));
			}
			else {
				std::swap(cubeId(id, 0, 1), cubeId(id, 1, 0));
				std::swap(cubeId(id, 0, 1), cubeId(id, 2, 1));
				std::swap(cubeId(id, 0, 1), cubeId(id, 1, 2));
				std::swap(cubeId(id, 0, 0), cubeId(id, 2, 0));
				std::swap(cubeId(id, 0, 0), cubeId(id, 2, 2));
				std::swap(cubeId(id, 0, 0), cubeId(id, 0, 2));
			}
			rotationLeft = 0;
			highlightWall(highlightedWall);
		}
	}

}

void GraphicsManager::highlightWall(int id) {
	for (int x = 0; x < 3; x ++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				cubesBuffer[cubeId(x, y, z)].changeHighlight(0);
			}
		}
	}
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			cubesBuffer[cubeId(id, x, y)].changeHighlight(1);
		}
	}
	highlightedWall = id;
}

void GraphicsManager::cycleHighlight() {
	double newTime = glfwGetTime();
	if (newTime - lastTab > 0.15f)
	{ 
		lastTab = newTime;
		highlightedWall = (highlightedWall + 1) % 9;
		highlightWall(highlightedWall);
	}

}

void GraphicsManager::rotateHighlighted(int direction) {
	if (highlightedWall < 6) rotateWall(highlightedWall, direction);
	else rotateWall(highlightedWall, -direction);
}

void GraphicsManager::addCube(const char* texPath, GLenum gl_texture, GLuint textureID, glm::vec3 pos) {
	glm::vec3 xyz = (pos - vec3(1.0f, 1.0f, 1.0f)) * 2.0f;
	glm::mat4 matrix = glm::translate(glm::mat4(), pos);
	cubesBuffer.emplace_back("objects/cube.obj", texPath, programID, gl_texture, textureID, matrix);
}


void GraphicsManager::drawCube(int x, int y, int z) {
	glBindVertexArray(VertexArrayID);

	controlsManager.computeMatricesFromInput(window);

	mat4 Projection = controlsManager.getProjectionMatrix();
	mat4 View = controlsManager.getViewMatrix();
	mat4 Model = cubesBuffer[cubes[x][y][z]].getModelView();
	mat4 mvp = Projection * View * Model;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	cubesBuffer[cubes[x][y][z]].draw(highlightID);
}

void GraphicsManager::rotateWall(int id, int direction) {
	rotationLeft = 1;
	currentRotation = std::make_pair ( id, direction );
}

bool GraphicsManager::isRotating() {
	return rotationLeft > 0;
}

int &GraphicsManager::cubeId(int x, int y, int z) { 
	switch (x / 3) {
	case 0:
		return cubes[x%3][y][z];
	case 1:
		return cubes[y][z][x%3];
	case 2:
		return cubes[z][x%3][y];
	}
}

glm::vec3 GraphicsManager::getTVector(int a, int b, int axis, int direction) {
	if (direction == -1) return getTVector(b, 2 - a, axis, 1);
	switch (axis) {
	case 0:
		return glm::vec3(0.0f, (float)2 * b - 2 * a, (float)4 - 2 * a - 2 * b);
	case 1:
		return glm::vec3((float)2 * b - 2 * a, (float)4 - 2 * a - 2 * b, 0.0f);
	case 2:
		return glm::vec3((float)4 - 2 * a - 2 * b, 0.0f, (float)2 * b - 2 * a);
	default:
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

}

GraphicsManager::~GraphicsManager() {
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glfwTerminate();
}