#include "../stdafx.h"
#include "Sphere.h"

Sphere::Sphere(int resolution, float radius, glm::vec3 position, const std::string textureFileName)
              : textureFileName(textureFileName), Radius(radius), resolution(resolution), Position(position)
{
    Initialize();
}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &textureId);
}

void Sphere::Draw(Shader& shader, Camera& camera, float radius, glm::vec3 position)
{
    //shader.Use();
    //// Color input
    //GLint object_color_loc = glGetUniformLocation(shader.ID,
    //    "object_color");

    //glUniform3f(object_color_loc, 1.0f, 0.5f, 0.2f);

    //glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 mvp = projection * camera.GetViewMatrix() * model;

    //shader.Set("model", model);
    //shader.Set("mvp", mvp);
    //shader.Set("projection", projection);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
    //    &vertices[0], GL_DYNAMIC_DRAW);
    //glBindVertexArray(vao);
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);


    float* sphere_vertex, * sphere_normal;
    GLuint* sphere_indices;
    int sphere_vertex_size, sphere_indices_size;
    int res = 50;
    GLuint vao, vbo, nbo, ebo;

    sphere_vertex_size = 3 * (res + 1) * (res + 1);
    sphere_indices_size = 6 * res * res;

    sphere_vertex = new float[sphere_vertex_size];
    sphere_normal = new float[sphere_vertex_size];
    sphere_indices = new GLuint[sphere_indices_size];
    std::vector<float> uvs;
    for (int j = 0; j <= res; j++) {
        for (int i = 0; i <= res; i++) {
            int k = i + j * (res + 1);
            sphere_vertex[3 * k] = (float)i / res;
            sphere_vertex[3 * k + 1] = .0f;
            sphere_vertex[3 * k + 2] = (float)j / res;
            const float M_PI = glm::pi<float>();
            float theta = sphere_vertex[3 * k + 0] * 2 * M_PI;
            float phi = sphere_vertex[3 * k + 2] * M_PI;

            sphere_normal[3 * k + 0] = sphere_vertex[3 * k + 0] = position.x + radius * glm::cos(theta) * glm::sin(phi);
            sphere_normal[3 * k + 1] = sphere_vertex[3 * k + 1] = position.y + radius * glm::sin(theta) * glm::sin(phi);
            sphere_normal[3 * k + 2] = sphere_vertex[3 * k + 2] = position.z + radius * glm::cos(phi);
        }
    }

    int k = 0;
    for (int j = 0; j < res; j++) {
        for (int i = 0; i < res; i++) {
            sphere_indices[k++] = i + j * (res + 1);
            sphere_indices[k++] = i + (j + 1) * (res + 1);
            sphere_indices[k++] = i + 1 + (j + 1) * (res + 1);

            sphere_indices[k++] = i + j * (res + 1);
            sphere_indices[k++] = i + 1 + (j + 1) * (res + 1);
            sphere_indices[k++] = i + 1 + j * (res + 1);
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere_vertex_size * sizeof(float), sphere_vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, sphere_vertex_size * sizeof(float), sphere_normal, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_indices_size * sizeof(GLuint), sphere_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sphere_indices_size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Sphere::Update(glm::vec3 newPosition)
{
    int index = 0;
    for (int j = 0; j <= resolution; j++)
    {
        for (int i = 0; i <= resolution; i++)
        {
            Vertex& vertex = vertices[index];
            vertex.Position.x = (float)i / resolution;
            vertex.Position.y = 0.0f;
            vertex.Position.z = (float)j / resolution;

            float theta = vertex.Position.x * 2 * glm::pi<float>();
            float phi = vertex.Position.z * glm::pi<float>();

            vertex.Normal.x = vertex.Position.x = newPosition.x + Radius * glm::cos(theta) * glm::sin(phi);
            vertex.Normal.y = vertex.Position.y = newPosition.y + Radius * glm::sin(theta) * glm::sin(phi);
            vertex.Normal.z = vertex.Position.z = newPosition.z + Radius * glm::cos(phi);
            
            index++;
        }
    }
}

void Sphere::Initialize()
{
    //Create();
    //LoadTexture(textureFileName);


    //glGenVertexArrays(1, &vao);
    //glGenBuffers(1, &vbo);
    //glGenBuffers(1, &ebo);

    //glBindVertexArray(vao);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    //// vertex positions
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //// vertex normals
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    ////// vertex texture coords
    ////glEnableVertexAttribArray(2);
    ////glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    //glBindVertexArray(0);
}

void Sphere::Create()
{
#pragma region  Previous Spher Code
    //constexpr float PI = glm::pi<float>();
    //float sectorStep = 2 * PI / sectorCount;
    //float stackStep = PI / stackCount;
    //float sectorAngle, stackAngle;
    //float xy = 0;
    //for (int i = 0; i <= stackCount; ++i)
    //{
    //    stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
    //    xy = radius * cosf(stackAngle);             // r * cos(u)
    //    float z = radius * sinf(stackAngle);              // r * sin(u)
    //    float iverseRadius = 1.0f / radius;
    //    // add (sectorCount+1) vertices per stack
    //    // the first and last vertices have same position and normal, but different tex coords
    //    for (int j = 0; j <= sectorCount; ++j)
    //    {
    //        sectorAngle = j * sectorStep;           // starting from 0 to 2pi

    //        // vertex position
    //        float x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
    //        float y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
    //       
    //        // normalized vertex normal
    //        float nx = x * iverseRadius;
    //        float ny = y * iverseRadius;
    //        float nz = z * iverseRadius;
    //
    //        // vertex tex coord between [0, 1]
    //        float u = (float)j / sectorCount;
    //        float v = (float)i / stackCount;

    //        Vertex vertex;
    //        vertex.Position = glm::vec3(x, y, z);
    //        vertex.Normal = glm::vec3(nx, ny, nz);
    //        vertex.TexCoords = glm::vec2(u, v);
    //        vertices.push_back(vertex);
    //    }
    //}

    //unsigned int k1 = 0;
    //unsigned int k2 = 0;
    //for (int i = 0; i < stackCount; ++i)
    //{
    //    k1 = i * (sectorCount + 1);     // beginning of current stack
    //    k2 = k1 + sectorCount + 1;      // beginning of next stack

    //    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
    //    {
    //        // 2 triangles per sector excluding 1st and last stacks
    //        if (i != 0)
    //        {
    //            indices.push_back(k1);
    //            indices.push_back(k2);
    //            indices.push_back(k1 + 1);
    //        }

    //        if (i != (stackCount - 1))
    //        {
    //            indices.push_back(k1 + 1);
    //            indices.push_back(k2);
    //            indices.push_back(k2 + 1);
    //        }
    //    }
    //}
#pragma endregion


    //for (int j = 0; j <= resolution; j++) 
    //{
    //    for (int i = 0; i <= resolution; i++) 
    //    {
    //        Vertex vertex;
    //        vertex.Position.x = (float)i / resolution;
    //        vertex.Position.y = 0.0f;
    //        vertex.Position.z = (float)j / resolution;

    //        float theta = vertex.Position.x * 2 * glm::pi<float>();
    //        float phi = vertex.Position.z * glm::pi<float>();

    //        vertex.Normal.x = vertex.Position.x = position.x + Radius * glm::cos(theta) * glm::sin(phi);
    //        vertex.Normal.y = vertex.Position.y = position.y + Radius * glm::sin(theta) * glm::sin(phi);
    //        vertex.Normal.z = vertex.Position.z = position.z + Radius * glm::cos(phi);

    //        vertex.TexCoords = glm::vec2((float)i / resolution, (float)j / resolution);
    //        vertices.push_back(vertex);
    //    }
    //}

    //for (int j = 0; j < resolution; j++) 
    //{
    //    for (int i = 0; i < resolution; i++) 
    //    {
    //        indices.push_back(i + j * (resolution + 1));
    //        indices.push_back(i + (j + 1) * (resolution + 1));
    //        indices.push_back(i + 1 + (j + 1) * (resolution + 1));
    //        indices.push_back(i + j * (resolution + 1));
    //        indices.push_back(i + 1 + (j + 1) * (resolution + 1));
    //        indices.push_back(i + 1 + j * (resolution + 1));
    //    }
    //}
}

void Sphere::LoadTexture(const std::string& textureFileName)
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