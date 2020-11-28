#pragma once

#include "../stdafx.h"
#include <stb_image.h>

#include "Particle.h"
#include "Constraint.h"
#include "Integrators/IIntergrator.h"
#include "ForceGenerators/IForceGenerator.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"

class Cloth 
{

public:
    Cloth(int width, int height, const std::string& textureFileName);
    ~Cloth();
    void Update(float deltaTime, glm::vec3 position, float raidus);
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
    void AddCollisionHandler(std::function<void(Particle* particle)> handler);
    void AddParticlPositionConstraint(int id);
    void AddForceGenerator(IForceGenerator* forceGenerator);

    void ball_control(char input);
    float get_ball_radius();
    glm::vec3 get_ball_center();

public:
    float SegmentLength;
    float Mass;
    float Stiffness;
    float Damping;
    bool IsWindForceEnabled;
    int NumberOfConstraintIterations;
    IIntegrator* IntergrationMethod;
    glm::vec3 Color;
private:
    void LoadTexture(const std::string& textureFileName);
    void Initialize();
    void Reset();
    void CreateConstraints();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateParticles();
private:
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };

    int vertexCount;
    float elapsedTime;

    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint textureId;
    int width;
    int height;

    glm::vec3 ball_center;
    float ball_radius;

    std::vector<Vertex> vertices;
    std::string textureFileName;
    std::vector<Particle*> particles;
    std::vector<int> indices;
    std::vector<Constraint*> constraints;
    std::vector<IForceGenerator*> forceGenerators;
    std::vector<std::function<void(Particle* particle)>> collisionHandlers;
};

