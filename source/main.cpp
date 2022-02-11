#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include "texture.hpp"
#include "controls.hpp"
#include <common/objloader.hpp>
#include "graphicsManager.hpp"


int main() {

	GraphicsManager gm;
	gm.init();
	gm.highlightWall(0);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(gm.programID);
		gm.update();
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++)
			{
				for (int z = 0; z < 3; z++) {
					gm.drawCube(x, y, z);
				}
			}
		}
		glfwSwapBuffers(gm.window);
		glfwPollEvents();

		if (glfwGetKey(gm.window, GLFW_KEY_RIGHT) == GLFW_PRESS && !gm.isRotating()) {
			gm.rotateHighlighted(1);
		}
		if (glfwGetKey(gm.window, GLFW_KEY_LEFT) == GLFW_PRESS && !gm.isRotating()) {
			gm.rotateHighlighted(-1);
		}
		if (glfwGetKey(gm.window, GLFW_KEY_TAB) == GLFW_PRESS && !gm.isRotating()) {
			gm.cycleHighlight();
		}

	} while (glfwGetKey(gm.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(gm.window) == 0);

	return 0;

}
