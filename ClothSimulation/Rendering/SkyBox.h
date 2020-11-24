#pragma once
#include "../stdafx.h"
#include <stb_image.h>
#include "Shader.h"
#include "Camera.h"
struct VertexTexture
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class SkyBox
{
public:
	SkyBox();
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
private:
    void Initialize();
    GLuint LoadCubemap(std::vector<std::string> faces);
private:
    std::vector<VertexTexture> vertices;
    std::vector<int> indices;

    GLuint skyboxVAO;
    GLuint skyboxVBO;
    GLuint cubeTexture;
    GLuint cubemapTexture;
};