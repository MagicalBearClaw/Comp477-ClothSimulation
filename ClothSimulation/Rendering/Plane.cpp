#include "../stdafx.h"
#include "Plane.h"

Plane::Plane(int width, int height, std::string textureFileName)
{
	Width = width;
	Height = height;
    _textureFileName = textureFileName;
	PlaneStride = 1.0f / (float)(std::max(Height - 1, Width - 1));
	NumberOfVertices = height * width;
    Initialize();
}

void Plane::CreateBuffers()
{
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            float x = PlaneStride * j;
            float y = PlaneStride * i;
            float z = 0.0f;

            Vertex v;
            v.Position = glm::vec3(x, y, z);
            v.TexCoords = glm::vec2((float)i / Width, (float)j / Height);
            vertices.push_back(v);
        }
    }

    NumberOfIndicies = (Height - 1) * (Width - 1) * 6;
    for (int i = 0; i < Height - 1; i++) {
        int stride = i * (Width - 1);
        for (int j = 0; j < Width - 1; j++) {
            indices.push_back(i * Width + j);
            indices.push_back(i * Width + j + 1);
            indices.push_back((i + 1) * Width + j);

            indices.push_back(i * Width + j + 1);
            indices.push_back((i + 1) * Width + j);
            indices.push_back((i + 1) * Width + j + 1);
        }
    }
}

void Plane::Initialize()
{
    CreateBuffers();
    LoadTexture(_textureFileName);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Plane::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    glm::mat4 view = camera.GetViewMatrix();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Plane::LoadTexture(const std::string& textureFileName)
{
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(textureFileName.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << textureFileName << std::endl;
        stbi_image_free(data);
    }
}
