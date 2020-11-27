#pragma once

#include "../stdafx.h"
#include "Particle.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"

struct Constraint {
    int a;
    int b;
    float rest_distance;
};

class Cloth {
public:
    Cloth(int width, int height, const std::string& textureFileName);
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
    void AddCollisionHandler(std::function<void(Particle* particle)> handler);
    void ball_control(char input);
    float get_ball_radius();
    glm::vec3 get_ball_center();
public:
    float SegmentLength;
    float Mass;
    float Stiffness;
    float Damping;
    bool IsWindForceEnabled;

private:
    bool update_points(std::vector<float>& vertices);
    bool update_points_constraint(std::vector<float>& vertices);

    void CreateConstraints();
    void CreateVertexBuffer();
private:
    int vertexCount;
    float elapsedTime;

    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    int width;
    int height;

    glm::vec3 ball_center;
    float ball_radius;

    std::vector<float> vertices;
    std::string textureFileName;
    std::vector<Particle*> points;
    std::vector<int> indices;
    std::vector<Constraint*> constraints;
    std::vector<std::function<void(Particle* particle)>> collisionHandlers;
};

