#include "../stdafx.h"
#include "cloth.h"

Cloth::Cloth(int width, int height, const std::string& textureFileName)
    : height(height), width(width)
{
    SegmentLength = 1.0f / (float)(std::max(height - 1, width - 1));
    vertexCount = width * height;
    Mass = 1.0f;
    Stiffness = 2.0f;
    elapsedTime = 0.0f;
    IsWindForceEnabled = true;
    NumberOfConstraintIterations = 15;
    Damping = 0.001f;
    // Ball initialization
    ball_radius = 0.25f;
    ball_center = glm::vec3(SegmentLength * width * 0.5f, SegmentLength * height * 1.8f,
        ball_radius * 2);

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


void Cloth::Update(float deltaTime, glm::vec3 ballPosition, float ballRadius)
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

            glm::vec3 offset = particles[i]->Position - ballPosition;
            if (glm::length(offset) < ballRadius) {
                particles[i]->Position += glm::normalize(offset)
                    * (ballRadius - glm::length(offset));
            }

            //for (auto& handler : collisionHandlers)
            //{
            //    handler(part)
            //}
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

    for (int i = 0; i < vertexCount; i++)
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        &vertices[0], GL_DYNAMIC_DRAW);

    glm::mat4 model;
    float scale = 0.5f;
    model = glm::scale(model, glm::vec3(scale));
    model = glm::translate(model, glm::vec3(-SegmentLength * width / 2.0f,
        -SegmentLength * height / 1.2f, 0));


    glm::mat4 mvp = projection * camera.GetViewMatrix() * model;
    GLint mvp_loc = glGetUniformLocation(shader.ID, "mvp");
    GLint model_loc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

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
            indices.push_back(i * width + j);
            indices.push_back(i * width + j + 1);
            indices.push_back((i + 1) * width + j);

            indices.push_back(i * width + j + 1);
            indices.push_back((i + 1) * width + j);
            indices.push_back((i + 1) * width + j + 1);
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
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
        //glDeleteTextures(1, &textureId);
    }
}

void Cloth::ball_control(char input) 
{
    float speed = 0.05f;
    switch (input) {
    case 'I':
        ball_center -= glm::vec3(0, speed, 0);
        break;
    case 'K':
        ball_center += glm::vec3(0, speed, 0);
        break;
    case 'J':
        ball_center -= glm::vec3(speed, 0, 0);
        break;
    case 'L':
        ball_center += glm::vec3(speed, 0, 0);
        break;
    case 'U':
        ball_center -= glm::vec3(0, 0, speed);
        break;
    case 'O':
        ball_center += glm::vec3(0, 0, speed);
        break;
    case '[':
        if (ball_radius > 0.2f) ball_radius -= 0.002f;
        break;
    case ']':
        if (ball_radius < 4.0f) ball_radius += 0.002f;
        break;
    }
}

void Cloth::CreateConstraints() 
{
    for (int i = 0; i < vertexCount; i++) 
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

float Cloth::get_ball_radius() {
    return ball_radius;
}

glm::vec3 Cloth::get_ball_center() {
    return ball_center;
}

