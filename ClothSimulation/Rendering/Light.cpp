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
    GLint light_color_loc = glGetUniformLocation(shader.ID,
        "light_color");
    GLint light_pos_loc = glGetUniformLocation(shader.ID,
        "light_pos");

    glUniform3f(light_color_loc, Color.x, Color.y, Color.z);
    glUniform3f(light_pos_loc, Position.x, Position.y, Position.z);

}

void Light::Initialize()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
