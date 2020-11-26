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

class Cloth 
{
public:
    Cloth(int width, int height);
    ~Cloth();
    void Update();
    void wind_on();
    void ball_control(char input);
    void add_k();
    void reduce_k();
    void get_constraints();
    float get_ball_radius();
    glm::vec3 get_ball_center();

    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
    void Initialize();

private:
    void view_transform(Shader& shader_program, float grid_size,
        int row_count, int col_count, glm::mat4 projection, Camera& camera);
    void CreateVertexBuffer();

    GLuint textureId;
    GLuint VBO, VAO, EBO;


    int row_count;  // Row count (for points)
    int col_count;  // Column count (for points)
    int vertex_count;
    float grid_size;
    float mass;
    float k;
    float time;
    glm::vec3 ball_center;
    float ball_radius;
    bool wind;
    bool pin_four;
    std::vector<Particle*> points;  
    std::vector<float> vertices;
    std::vector<int> indices;
    std::vector<Constraint*> constraints;
};
