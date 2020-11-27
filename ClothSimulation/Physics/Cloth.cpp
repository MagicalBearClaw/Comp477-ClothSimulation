#include "../stdafx.h"
#include "Cloth.h"

Cloth::Cloth(int width, int height)
    : height(height), width(width)
{
    SegmentLength = 1.0f / (float)(std::max(height-1, width-1));
    vertexCount = height * width;
    Mass = 1.0f;
    Stiffness = 2.0f;
    elapsedTime = 0.0f;
    IsWindForceEnabled = false;
    NumberOfConstraintIterations = 10;

    // Vertices initialization
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            float x = SegmentLength * j;
            float y, z;
            //if(!p) {
            //    y = 0.0f;
            //    z = SegmentLength * i;
            //} else {
            //    y = SegmentLength * i;
            //    z = 0.0f;
            //}

            y = SegmentLength * i;
            z = 0.0f;

            Particle* curr_point = new Particle(glm::vec3(x, y, z), glm::vec2((float)(i / height), (float)(j / width)));
            curr_point->VertexId = i * width + j;
            if (i*width+j == 0 || i*width+j == width-1) 
                curr_point->IsPositionConstrained = true;
            //if (p){
            //    if(i*c+j == (height-1)*width 
            //       || i*c+j == vertexCount-1) 
            //        curr_point->IsPositionConstrained = true;
            //}
            points.push_back(curr_point);
        }
    }

    // Indices initialization
    int index_count = (height - 1) * (width - 1) * 6;
    for (int i=0; i< height-1; i++) {
        int stride = i * (width - 1);
        for (int j=0; j< width-1; j++) {
            indices.push_back(i * width + j);
            indices.push_back(i * width + j + 1);
            indices.push_back((i + 1) * width + j);

            indices.push_back(i * width + j + 1);
            indices.push_back((i + 1) * width + j);
            indices.push_back((i + 1) * width + j + 1);
        }
    }

    Initialize();
}

Cloth::~Cloth()
{
}

void Cloth::CreateVertexBuffer() {
    for (int i=0; i<vertexCount; i++) {
        vertices.push_back(points[i]->CurrentPosition.x);
        vertices.push_back(points[i]->CurrentPosition.y);
        vertices.push_back(points[i]->CurrentPosition.z);
    }
}



void Cloth::Update(float deltaTime) {
    glm::vec3 force;

    float vertex_mass = Mass / vertexCount;  // Mass of each vertex
    float damping = 0.02f;  // Damping (air resistance)
    
    glm::vec3 wind_force = glm::vec3(0);
    glm::vec3 gravity = 0.1f * glm::vec3(0, 9.8f, 0);
    
    for (int i=0; i<vertexCount; i++) {
        Particle* curr_point = points[i];
        curr_point->Force += vertex_mass * gravity; // Force initialization (gravity)

        /* Add wind force */
        float x_force = 0;
        float y_force = std::abs(sin(0.1f*elapsedTime) - 0.2f);
        float z_force = std::abs(cos(sin(curr_point->CurrentPosition[0]* elapsedTime) - 0.8f));
        if (IsWindForceEnabled) {
            wind_force = glm::vec3(x_force, -0.0005f * y_force, -0.002f * z_force);
            curr_point->Force += wind_force;
        }

        /* Set the velocity of each point */
        curr_point->Acceleration = (curr_point->Force / vertex_mass);

        /* Reset force */
        curr_point->Force = glm::vec3(0);
    }

    /* Position update and Object collision */
    for (int i=0; i<vertexCount; i++) 
    {
        
        // preform verlet intergration
        glm::vec3 temp = points[i]->CurrentPosition;
        if(!points[i]->IsPositionConstrained) {
            points[i]->CurrentPosition = points[i]->CurrentPosition + (1.0f - damping)
                             * (points[i]->CurrentPosition - points[i]->PreviousPosition)
                             + points[i]->Acceleration * deltaTime;

            // give function here to handle collision
            //glm::vec3 offset = points[i]->CurrentPosition - ball_center;
            //if (glm::length(offset) < ball_radius) {
            //    points[i]->CurrentPosition += glm::normalize(offset)
            //                      * (ball_radius - glm::length(offset));
            //}
                
        }
        points[i]->PreviousPosition = temp;
    }

    /* Satisfy constraint */
    for (int j=0; j<10; j++) {
        for (int i=0; i<constraints.size(); i++) {
            Constraint* it = constraints[i];
            Particle * a = points[it->VertexIdA];
            Particle * b = points[it->VertexIdB];
            it->SatisfyConstraints(a, b);
        }
    }

    for (int i=0; i<vertexCount; i++) {
        vertices[i*3] = points[i]->CurrentPosition.x;
        vertices[i*3+1] = points[i]->CurrentPosition.y;
        vertices[i*3+2] = points[i]->CurrentPosition.z;
    }

    elapsedTime += 0.03f;
}

void Cloth::CreateConstraints() {
    for (int i=0; i<vertexCount; i++) 
    {
        int row = i / width;
        int col = i - row * width;
        if (col < width-1) 
        {
            Constraint* c = new Constraint(i, i+1, SegmentLength);
            constraints.push_back(c);
        }
        if (row < height-1) 
        {
            Constraint* c = new Constraint(i, i + width, SegmentLength);
            constraints.push_back(c);
        }
        if (col < width-1 && row < height-1) 
        {
            Constraint* c = new Constraint(i, i + width + 1, SegmentLength * glm::sqrt(2));
            constraints.push_back(c);
        }
        if (row > 0 && col < width-1) 
        {
            Constraint* c = new Constraint(i, i - width + 1, SegmentLength * glm::sqrt(2));
            constraints.push_back(c);
        }
        if (col < width-2) 
        {
            Constraint* c = new Constraint(i, i + 1, SegmentLength * 2);
            constraints.push_back(c);
        }
        if (row < height-2) 
        {
            Constraint* c = new Constraint(i, i + width, SegmentLength * 2);
            constraints.push_back(c);
        }
        if (col < width-2 && row < height-2) 
        {
            Constraint* c = new Constraint(i, i + width + 1, SegmentLength * glm::sqrt(2) * 2);
            constraints.push_back(c);
        }
        if (row > 1 && col < width-2) 
        {
            Constraint* c = new Constraint(i, i - width + 1, SegmentLength * glm::sqrt(2) * 2);
            constraints.push_back(c);
        }
    }
}

glm::vec3 Cloth::CalculateWindForce()
{
    return glm::vec3();
}



void Cloth::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        &vertices[0], GL_DYNAMIC_DRAW);

    // Perspective projection
    // Camera matrix
    // Model matrix
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

void Cloth::Initialize()
{
    CreateVertexBuffer();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        &vertices[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
        &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Constaints initialization
    CreateConstraints();

}

