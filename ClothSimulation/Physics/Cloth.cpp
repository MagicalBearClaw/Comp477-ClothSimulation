#include "../stdafx.h"
#include "cloth.h"

#define SQRT_2 1.4142135f


Cloth::Cloth(int width, int height, const std::string& textureFileName)
    : height(height), width(width)
{
    SegmentLength = 1.0f / (float)(std::max(height - 1, width - 1));
    vertexCount = width * height;
    Mass = 1.0f;
    Stiffness = 2.0f;
    elapsedTime = 0.0f;
    IsWindForceEnabled = false;
    NumberOfConstraintIterations = 10;

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


void Cloth::Update(float deltaTime)
{
    //update_points();
    update_points_constraint();
}

void Cloth::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        &vertices[0], GL_DYNAMIC_DRAW);

    glm::mat4 model;
    float scale = 0.5f;
    model = glm::scale(model, glm::vec3(0.5f));
    model = glm::translate(model, glm::vec3(-SegmentLength * width / 2.0f,
        -SegmentLength * height / 1.2f, 0));

    // Put transformation matrics together
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

bool Cloth::update_points() {
    glm::vec3 force;  // Force on each point
    glm::vec3 gravity;  // The gravity vector
    float vertex_mass = Mass / vertexCount;  // Mass of each vertex
    float timestep = 0.00015f;  // Timestep
    Damping = 0.01f;  // Damping (air resistance)
    glm::vec3 wind_force = glm::vec3(0);
    gravity = 0.1f * glm::vec3(0, 9.8f, 0);

    for (int i = 0; i < vertexCount; i++) {
        Particle* curr_point = particles[i];
        // Force initialization (gravity)
        curr_point->Force += vertex_mass * gravity;
        /* Get the neighbors on straight directions
         * (up/down/left/right) */
        std::vector<int> s_neighbors = curr_point->get_s_neighbors(i, width,
            height);
        /* Get the neighbors on diagonal directions
         * (up_left/up_right/down_left/down_right) */
        std::vector<int> d_neighbors = curr_point->get_d_neighbors(i, width,
            height);
        // Spring force accumulation 
        for (int j = 0; j < s_neighbors.size(); j++) {
            Particle* neighbor_point = particles[s_neighbors[j]];
            glm::vec3 x = neighbor_point->Position - curr_point->Position;
            curr_point->Force += glm::normalize(x)
                * (glm::length(x) - SegmentLength)
                * Stiffness;
        }
        for (int j = 0; j < d_neighbors.size(); j++) {
            Particle* neighbor_point = particles[d_neighbors[j]];
            glm::vec3 x = neighbor_point->Position - curr_point->Position;
            curr_point->Force += glm::normalize(x)
                * (glm::length(x) - SegmentLength * SQRT_2)
                * Stiffness;
        }

        /* Add wind force */
        float x_force = 0; // cos(0.8f*elapsedTime) * (rand()/RAND_MAX-0.5f);
        float y_force = std::abs(sin(0.1f * elapsedTime) - 0.2f);
        float z_force = std::abs(cos(sin(curr_point->Position[0] * elapsedTime) - 0.8f));
        if (IsWindForceEnabled) {
            wind_force = glm::vec3(x_force,
                -0.0005f * y_force,
                -0.002f * z_force);
            curr_point->Force += wind_force;
        }

        /* Set the velocity of each point */
        curr_point->Acceleration = curr_point->Force / vertex_mass;

        /* Reset force */
        curr_point->Force = glm::vec3(0);
    }

    /* Position update and Object collision */
    for (int i = 0; i < vertexCount; i++) {
        glm::vec3 temp = particles[i]->Position;
        if (!particles[i]->IsPositionConstrained) {
            particles[i]->Position = particles[i]->Position + (1.0f - Damping)
                * (particles[i]->Position - particles[i]->PreviousPosition)
                + particles[i]->Acceleration * timestep;
            glm::vec3 offset = particles[i]->Position - ball_center;
            if (glm::length(offset) < ball_radius) {
                particles[i]->Position += glm::normalize(offset)
                    * (ball_radius - glm::length(offset));
            }

        }
        particles[i]->PreviousPosition = temp;

        vertices[i].Position = particles[i]->Position;
    }
    elapsedTime += 0.03f;
    return true;
}

void Cloth::ball_control(char input) {
    switch (input) {
    case 'I':
        ball_center -= glm::vec3(0, 0.004f, 0);
        break;
    case 'K':
        ball_center += glm::vec3(0, 0.004f, 0);
        break;
    case 'J':
        ball_center -= glm::vec3(0.004f, 0, 0);
        break;
    case 'L':
        ball_center += glm::vec3(0.004f, 0, 0);
        break;
    case 'U':
        ball_center -= glm::vec3(0, 0, 0.004f);
        break;
    case 'O':
        ball_center += glm::vec3(0, 0, 0.004f);
        break;
    case '[':
        if (ball_radius > 0.2f) ball_radius -= 0.002f;
        break;
    case ']':
        if (ball_radius < 4.0f) ball_radius += 0.002f;
        break;
    }
}

bool Cloth::update_points_constraint() {
    glm::vec3 force;  // Force on each point
    glm::vec3 gravity;  // The gravity vector
    float vertex_mass = Mass / vertexCount;  // Mass of each vertex
    float timestep = 0.00015f;  // Timestep
    Damping = 0.02f;  // Damping (air resistance)
    glm::vec3 wind_force = glm::vec3(0);
    gravity = 0.1f * glm::vec3(0, 9.8f, 0);

    for (int i = 0; i < vertexCount; i++) {
        Particle* curr_point = particles[i];
        curr_point->Force += vertex_mass * gravity; // Force initialization (gravity)

        /* Add wind force */
        float x_force = 0;
        float y_force = std::abs(sin(0.1f * elapsedTime) - 0.2f);
        float z_force = std::abs(cos(sin(curr_point->Position[0] * elapsedTime) - 0.8f));
        if (IsWindForceEnabled) {
            wind_force = glm::vec3(x_force, -0.0005f * y_force, -0.002f * z_force);
            curr_point->Force += wind_force;
        }

        /* Set the velocity of each point */
        curr_point->Acceleration = curr_point->Force / vertex_mass;

        /* Reset force */
        curr_point->Force = glm::vec3(0);
    }

    /* Position update and Object collision */
    for (int i = 0; i < vertexCount; i++) {
        glm::vec3 temp = particles[i]->Position;
        if (!particles[i]->IsPositionConstrained) {
            particles[i]->Position = particles[i]->Position + (1.0f - Damping)
                * (particles[i]->Position - particles[i]->PreviousPosition)
                + particles[i]->Acceleration * timestep;
            glm::vec3 offset = particles[i]->Position - ball_center;
            if (glm::length(offset) < ball_radius) {
                particles[i]->Position += glm::normalize(offset)
                    * (ball_radius - glm::length(offset));
            }

        }
        particles[i]->PreviousPosition = temp;
    }

    /* Satisfy constraint */
    for (int j = 0; j < NumberOfConstraintIterations; j++) {
        for (int i = 0; i < constraints.size(); i++) {
            Constraint* it = constraints[i];
            Particle* a = particles[it->a];
            Particle* b = particles[it->b];
            //std::cout << it->a << "," << it->b << std::endl;
            //getchar();
            float rest_distance = it->rest_distance;
            float distance = glm::length(a->Position - b->Position);
            if (distance > rest_distance) {
                float offset = (distance - rest_distance) / distance;
                glm::vec3 correction = 0.5f * offset * (a->Position - b->Position);
                if (!particles[it->a]->IsPositionConstrained)
                    a->Position -= correction;
                if (!particles[it->b]->IsPositionConstrained)
                    b->Position += correction;
            }

            float tear_distance = it->rest_distance * 3.0f;
            if (distance > tear_distance) {
                float offset = (distance - rest_distance) / distance;
                glm::vec3 correction = offset * (a->Position - b->Position);
                if (particles[it->a]->IsPositionConstrained) particles[it->a]->IsPositionConstrained = false;
                if (particles[it->b]->IsPositionConstrained) particles[it->b]->IsPositionConstrained = false;
                constraints.erase(constraints.begin() + i);
            }
        }
    }

    for (int i = 0; i < vertexCount; i++) 
    {
        vertices[i].Position = particles[i]->Position;
    }

    elapsedTime += 0.03f;

    return true;
}

void Cloth::CreateConstraints() {
    for (int i = 0; i < vertexCount; i++) {
        int row = i / width;
        int col = i - row * width;
        if (col < width - 1) {
            Constraint* right = new Constraint();
            right->rest_distance = SegmentLength;
            right->a = i;
            right->b = i + 1;
            constraints.push_back(right);
        }
        if (row < height - 1) {
            Constraint* down = new Constraint();
            down->rest_distance = SegmentLength;
            down->a = i;
            down->b = i + width;
            constraints.push_back(down);
        }
        if (col < width - 1 && row < height - 1) {
            Constraint* down_right = new Constraint();
            down_right->rest_distance = SegmentLength * SQRT_2;
            down_right->a = i;
            down_right->b = i + width + 1;
            constraints.push_back(down_right);
        }
        if (row > 0 && col < width - 1) {
            Constraint* up_right = new Constraint();
            up_right->rest_distance = SegmentLength * SQRT_2;
            up_right->a = i;
            up_right->b = i - width + 1;
            constraints.push_back(up_right);
        }
        if (col < width - 2) {
            Constraint* right = new Constraint();
            right->rest_distance = SegmentLength * 2;
            right->a = i;
            right->b = i + 1;
            constraints.push_back(right);
        }
        if (row < height - 2) {
            Constraint* down = new Constraint();
            down->rest_distance = SegmentLength * 2;
            down->a = i;
            down->b = i + width;
            constraints.push_back(down);
        }
        if (col < width - 2 && row < height - 2) {
            Constraint* down_right = new Constraint();
            down_right->rest_distance = SegmentLength * SQRT_2 * 2;
            down_right->a = i;
            down_right->b = i + width + 1;
            constraints.push_back(down_right);
        }
        if (row > 1 && col < width - 2) {
            Constraint* up_right = new Constraint();
            up_right->rest_distance = SegmentLength * SQRT_2 * 2;
            up_right->a = i;
            up_right->b = i - width + 1;
            constraints.push_back(up_right);
        }
    }
}

float Cloth::get_ball_radius() {
    return ball_radius;
}

glm::vec3 Cloth::get_ball_center() {
    return ball_center;
}

