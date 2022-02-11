#include <cstdio>

#include "cube.hpp"
#include <common/objloader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.hpp"

Cube::Cube(const char* objpath, const char* texpath, GLuint programID, GLenum gl_texture, GLuint textureID, glm::mat4 position) {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	loadOBJ(objpath, vertices, uvs, normals);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	texture = loadDDS(texpath);
	if (texture == 0) {
		printf("Error loading texture\n");
	}

	Cube::textureID = textureID;
	Cube::gl_texture = gl_texture;
	Cube::position = position;
	Cube::highlight = -0.2;
}

Cube::~Cube() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteTextures(1, &texture);
}

glm::mat4 Cube::getModelView() {
	return position * rotation;
}

void Cube::changeHighlight(int on) {
	if (on) highlight = 0.5f;
	else highlight = -0.2f;
}

void Cube::draw(GLuint highlightID) {
	glUniform1f(highlightID, highlight);

	glBindVertexArray(VertexArrayID);
	glActiveTexture(gl_texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Cube::rotate(float angle, glm::vec3 axis) {
	rotation = glm::rotate(glm::mat4(), angle, axis) * rotation;
}

void Cube::translate(glm::vec3 translation) {
	position = glm::translate(position, translation);
}