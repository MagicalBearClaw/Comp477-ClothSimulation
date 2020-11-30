#include "Light.h"
#pragma once

Light::Light()
{
    Initialize();
}

Light::~Light()
{
    glDeleteVertexArrays(1, &vao);
}

void Light::Draw(Shader& shader)
{
    shader.Set("light_color", Color);
    shader.Set("light_pos", Position);
}

void Light::Initialize()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
