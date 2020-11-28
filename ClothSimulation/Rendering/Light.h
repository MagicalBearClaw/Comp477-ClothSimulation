#pragma once

#include "../stdafx.h"
#include "./Shader.h"

class Light
{
public:
	Light();
	~Light();
	glm::vec3 Position;
	glm::vec3 Color;
	void Draw(Shader& shader);
private:
	void Initialize();
	GLuint vao;
};