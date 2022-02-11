#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class Cube {
private:

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint texture;
	GLuint textureID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	GLuint VertexArrayID;

	GLenum gl_texture;

	glm::mat4 position;
	glm::mat4 rotation;
public:
	float highlight;

	Cube(const char* objpath, const char* texpath, GLuint programID, GLenum gl_texture, GLuint textureID, glm::mat4 position);
	~Cube();
	glm::mat4 getModelView();
	void draw(GLuint highlightID);
	void rotate(float angle, glm::vec3 axis);
	void translate(glm::vec3 translation);
	void changeHighlight(int on);
};