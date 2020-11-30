#include "../stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include "cloth.h"


Cloth::Cloth(int width, int height, const std::string& textureFileName)
    : height(height), width(width), textureFileName(textureFileName)
{
    SegmentLength = 1.0f / (float)(std::max(height - 1, width - 1));
    vertexCount = width * height;
    Mass = 1.0f;
    Stiffness = 2.0f;
    elapsedTime = 0.0f;
    NumberOfConstraintIterations = 15;
    Color = glm::vec3(1.0f, 1.0f, 1.0f);

    Initialize();
}

Cloth::~Cloth()
{
    Reset();
}

void Cloth::AddParticlPositionConstraint(int id)
{
    particles[id]->IsPositionConstrained = true;
}

void Cloth::AddForceGenerator(IForceGenerator* forceGenerator)
{
    forceGenerators.push_back(forceGenerator);
}


void Cloth::Update(float deltaTime)
{
    int particleCount = particles.size();
    glm::vec3 windForce(0);
    for (int i = 0; i < particleCount; i++)
    {
        Particle* particle = particles[i];

        for (IForceGenerator* forceGenerator : forceGenerators)
        {
            forceGenerator->ApplyForce(particle, particles, deltaTime);
        }

        particle->Acceleration = particle->Force / particle->Mass;

        particle->Force = glm::vec3(0);
    }

    for (int i = 0; i < particleCount; i++)
    {
        glm::vec3 previousPosition = particles[i]->Position;
        
        if (!particles[i]->IsPositionConstrained) 
        {
            IntergrationMethod->Intergrate(particles[i], deltaTime);
            for (auto& handler : collisionHandlers)
            {
                handler(particles[i]);
            }
        }

        particles[i]->PreviousPosition = previousPosition;
    }

    for (int j = 0; j < NumberOfConstraintIterations; j++) 
    {
        for (int i = 0; i < constraints.size(); i++) 
        {
            Constraint* it = constraints[i];
            Particle* a = particles[it->VertexIdA];
            Particle* b = particles[it->VertexIdB];
            it->SatisfyConstraints(a, b);
        }
    }

    for (int i = 0; i < particleCount; i++)
    {
        vertices[i].Position = particles[i]->Position;
    }

    for (IForceGenerator* forceGenerator : forceGenerators)
    {
        forceGenerator->ElapsedTime += 0.03f;
    }
}

void Cloth::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{

    glm::mat4 model;
    float scale = 0.5f;
    model = glm::scale(model, glm::vec3(scale));
    model = glm::translate(model, glm::vec3(-SegmentLength * width / 2.0f,
        -SegmentLength * height / 1.2f, 0));

    glm::mat4 mvp = projection * camera.GetViewMatrix() * model;
    shader.Set("mvp", mvp);
    shader.Set("model", model);
    shader.Set("object_color", Color);
    shader.Set("diffuse", textureId);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cloth::AddCollisionHandler(std::function<void(Particle* particle)> handler)
{
    collisionHandlers.push_back(handler);
}

void Cloth::CreateVertexBuffer() 
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
     
            Vertex vertex;
            vertex.Position = glm::vec3(SegmentLength * j, 0, SegmentLength * i);
            vertex.TexCoords = glm::vec2((float)i / height, (float)j / width);
            vertices.push_back(vertex);
        }
    }
}

void Cloth::CreateIndexBuffer()
{
    for (int i = 0; i < height - 1; i++) 
    {
        for (int j = 0; j < width - 1; j++) 
        {
            int currentVertex = i * width + j;
            indices.push_back(currentVertex);
            
            auto it = vertexToIndexMapping.find(currentVertex);
            if (it != vertexToIndexMapping.end())
            {
                std::vector<int> vertexIndices = vertexToIndexMapping[currentVertex];
                vertexIndices.push_back(indices.size() - 1);
                vertexToIndexMapping[currentVertex] = vertexIndices;
            }
            else 
            {
                std::vector<int> newIndiciesVector = { (int)(indices.size() - 1) };
                vertexToIndexMapping.emplace(currentVertex, newIndiciesVector);
            }

            currentVertex = i * width + j + 1;
            indices.push_back(currentVertex);

            it = vertexToIndexMapping.find(currentVertex);
            if (it != vertexToIndexMapping.end())
            {
                std::vector<int> vertexIndices = vertexToIndexMapping[currentVertex];
                vertexIndices.push_back(indices.size() - 1);
                vertexToIndexMapping[currentVertex] = vertexIndices;
            }
            else
            {
                std::vector<int> newIndiciesVector = { (int)(indices.size() - 1) };
                vertexToIndexMapping.emplace(currentVertex, newIndiciesVector);
            }
            
            currentVertex = (i + 1) * width + j;
            indices.push_back(currentVertex);

            it = vertexToIndexMapping.find(currentVertex);
            if (it != vertexToIndexMapping.end())
            {
                std::vector<int> vertexIndices = vertexToIndexMapping[currentVertex];
                vertexIndices.push_back(indices.size() - 1);
                vertexToIndexMapping[currentVertex] = vertexIndices;
            }
            else
            {
                std::vector<int> newIndiciesVector = { (int)(indices.size() - 1) };
                vertexToIndexMapping.emplace(currentVertex, newIndiciesVector);
            }

            currentVertex = i * width + j + 1;
            indices.push_back(currentVertex);
            
            it = vertexToIndexMapping.find(currentVertex);
            if (it != vertexToIndexMapping.end())
            {
                std::vector<int> vertexIndices = vertexToIndexMapping[currentVertex];
                vertexIndices.push_back(indices.size() - 1);
                vertexToIndexMapping[currentVertex] = vertexIndices;
            }
            else
            {
                std::vector<int> newIndiciesVector = { (int)(indices.size() - 1) };
                vertexToIndexMapping.emplace(currentVertex, newIndiciesVector);
            }

            currentVertex = (i + 1) * width + j;
            indices.push_back(currentVertex);
            
            it = vertexToIndexMapping.find(currentVertex);
            if (it != vertexToIndexMapping.end())
            {
                std::vector<int> vertexIndices = vertexToIndexMapping[currentVertex];
                vertexIndices.push_back(indices.size() - 1);
                vertexToIndexMapping[currentVertex] = vertexIndices;
            }
            else
            {
                std::vector<int> newIndiciesVector = { (int)(indices.size() - 1) };
                vertexToIndexMapping.emplace(currentVertex, newIndiciesVector);
            }

            currentVertex = (i + 1) * width + j + 1;
            indices.push_back(currentVertex);

            it = vertexToIndexMapping.find(currentVertex);
            if (it != vertexToIndexMapping.end())
            {
                std::vector<int> vertexIndices = vertexToIndexMapping[currentVertex];
                vertexIndices.push_back(indices.size() - 1);
                vertexToIndexMapping[currentVertex] = vertexIndices;
            }
            else
            {
                std::vector<int> newIndiciesVector = { (int)(indices.size() - 1) };
                vertexToIndexMapping.emplace(currentVertex, newIndiciesVector);
            }

        }
    }
}

void Cloth::CreateParticles()
{
    float vertexMass = Mass / vertexCount;
    for (int i = 0; i < vertexCount; i++) 
    {
        particles.push_back(new Particle(i, vertices[i].Position, vertexMass));
    }
}

void Cloth::Initialize()
{
    Reset();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateParticles();
    CreateConstraints();
    LoadTexture(textureFileName);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cloth::LoadTexture(const std::string& textureFileName)
{
    unsigned int textureId;
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


void Cloth::Reset()
{
    for (Particle* particle : particles)
    {
        if (particle)
        {
            delete particle;
            particle = nullptr;
        }
    }

    for (Constraint* constraint : constraints)
    {
        if (constraint)
        {
            delete constraint;
            constraint = nullptr;
        }
    }

    if (vertices.size() >= 0 && indices.size() >= 0)
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteTextures(1, &textureId);
    }
}

void Cloth::CreateConstraints() 
{
    int particleCount = particles.size();
    for (int i = 0; i < particleCount; i++)
    {
        int row = i / width;
        int col = i - row * width;

        if (col < width - 1)
        {
            Constraint* c = new Constraint(i, i + 1, SegmentLength);
            constraints.push_back(c);
        }
        if (row < height - 1)
        {
            Constraint* c = new Constraint(i, i + width, SegmentLength);
            constraints.push_back(c);
        }
        if (col < width - 1 && row < height - 1)
        {
            Constraint* c = new Constraint(i, i + width + 1, SegmentLength * glm::sqrt(2));
            constraints.push_back(c);
        }
        if (row > 0 && col < width - 1)
        {
            Constraint* c = new Constraint(i, i - width + 1, SegmentLength * glm::sqrt(2));
            constraints.push_back(c);
        }
        if (col < width - 2)
        {
            Constraint* c = new Constraint(i, i + 1, SegmentLength * 2);
            constraints.push_back(c);
        }
        if (row < height - 2)
        {
            Constraint* c = new Constraint(i, i + width, SegmentLength * 2);
            constraints.push_back(c);
        }
        if (col < width - 2 && row < height - 2)
        {
            Constraint* c = new Constraint(i, i + width + 1, SegmentLength * glm::sqrt(2) * 2);
            constraints.push_back(c);
        }
        if (row > 1 && col < width - 2)
        {
            Constraint* c = new Constraint(i, i - width + 1, SegmentLength * glm::sqrt(2) * 2);
            constraints.push_back(c);
        }
    }
}