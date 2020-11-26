#include "../stdafx.h"
#include "Cloth.h"

Cloth::Cloth(int width, int height)
    : row_count(height), col_count(width)
{
    grid_size = 1.0f / (float)(std::max(height-1, width-1));
    vertex_count = height * width;
    mass = 1.0f;
    k = 2.0f;
    time = 0.0f;
    wind = false;
    
    // Vertices initialization
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            float x = grid_size * j;
            float y, z;
            //if(!p) {
            //    y = 0.0f;
            //    z = grid_size * i;
            //} else {
            //    y = grid_size * i;
            //    z = 0.0f;
            //}

            y = grid_size * i;
            z = 0.0f;

            Particle* curr_point = new Particle(glm::vec3(x, y, z), glm::vec2((float)(i / row_count), (float)(j / col_count)));
            curr_point->VertexId = i * width + j;
            if (i*width+j == 0 || i*width+j == col_count-1) 
                curr_point->IsPositionConstrained = true;
            //if (p){
            //    if(i*c+j == (row_count-1)*col_count 
            //       || i*c+j == vertex_count-1) 
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

    // Ball initialization
    ball_radius = 0.25f;
    ball_center = glm::vec3(grid_size * width * 0.5f, grid_size * width * 1.8f,
                            ball_radius * 2);

    Initialize();
}

Cloth::~Cloth()
{
}


void Cloth::CreateVertexBuffer() {
    for (int i=0; i<vertex_count; i++) {
        vertices.push_back(points[i]->CurrentPosition.x);
        vertices.push_back(points[i]->CurrentPosition.y);
        vertices.push_back(points[i]->CurrentPosition.z);
    }
}

void Cloth::ball_control(char input) {
    switch(input) {
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
            if(ball_radius > 0.2f) ball_radius -= 0.002f;
            break;
        case ']':
            if(ball_radius < 4.0f) ball_radius += 0.002f;
            break;
    }
}

void Cloth::Update(float deltaTime) {
    glm::vec3 force;  // Force on each point
    glm::vec3 gravity;  // The gravity vector
    float vertex_mass = mass / vertex_count;  // Mass of each vertex
    float damping = 0.02f;  // Damping (air resistance)
    glm::vec3 wind_force = glm::vec3(0);
    gravity = 0.1f * glm::vec3(0, 9.8f, 0);
    
    for (int i=0; i<vertex_count; i++) {
        Particle* curr_point = points[i];
        curr_point->Force += vertex_mass * gravity; // Force initialization (gravity)

        /* Add wind force */
        float x_force = 0;
        float y_force = std::abs(sin(0.1f*time) - 0.2f);
        float z_force = std::abs(cos(sin(curr_point->CurrentPosition[0]*time) - 0.8f));
        if (wind) {
            wind_force = glm::vec3(x_force, -0.0005f * y_force, -0.002f * z_force);
            curr_point->Force += wind_force;
        }

        /* Set the velocity of each point */
        curr_point->Acceleration = (curr_point->Force / vertex_mass);

        /* Reset force */
        curr_point->Force = glm::vec3(0);
    }

    /* Position update and Object collision */
    for (int i=0; i<vertex_count; i++) {
        glm::vec3 temp = points[i]->CurrentPosition;
        if(!points[i]->IsPositionConstrained) {
            points[i]->CurrentPosition = points[i]->CurrentPosition + (1.0f - damping)
                             * (points[i]->CurrentPosition - points[i]->PreviousPosition)
                             + points[i]->Acceleration * deltaTime;
            glm::vec3 offset = points[i]->CurrentPosition - ball_center;
            if (glm::length(offset) < ball_radius) {
                points[i]->CurrentPosition += glm::normalize(offset)
                                  * (ball_radius - glm::length(offset));
            }
                
        }
        points[i]->PreviousPosition = temp;
    }

    /* Satisfy constraint */
    for (int j=0; j<10; j++) {
        for (int i=0; i<constraints.size(); i++) {
            Constraint* it = constraints[i];
            Particle * a = points[it->a];
            Particle * b = points[it->b];
            //std::cout << it->a << "," << it->b << std::endl;
            //getchar();
            float rest_distance = it->rest_distance;
            float distance = glm::length(a->CurrentPosition - b->CurrentPosition);
            if (distance > rest_distance) {
                float offset = (distance - rest_distance) / distance;
                glm::vec3 correction = 0.5f * offset * (a->CurrentPosition - b->CurrentPosition);
                if(!points[it->a]->IsPositionConstrained)
                    a->CurrentPosition -= correction;
                if(!points[it->b]->IsPositionConstrained)
                    b->CurrentPosition += correction;
            }

            float tear_distance = it->rest_distance * 3.0f;
            if (distance > tear_distance) {
                float offset = (distance - rest_distance) / distance;
                glm::vec3 correction = offset * (a->CurrentPosition - b->CurrentPosition);
                if (points[it->a]->IsPositionConstrained) points[it->a]->IsPositionConstrained = false;
                if (points[it->b]->IsPositionConstrained) points[it->b]->IsPositionConstrained = false;
                constraints.erase(constraints.begin() + i);
            }
        }
    }

    for (int i=0; i<vertex_count; i++) {
        vertices[i*3] = points[i]->CurrentPosition.x;
        vertices[i*3+1] = points[i]->CurrentPosition.y;
        vertices[i*3+2] = points[i]->CurrentPosition.z;
    }

    time += 0.03f;
}

void Cloth::CreateConstraints() {
    for (int i=0; i<vertex_count; i++) {
        int row = i / col_count;
        int col = i - row * col_count;
        if (col < col_count-1) {
            Constraint* right = new Constraint();
            right->rest_distance = grid_size;
            right->a = i;
            right->b = i+1;
            constraints.push_back(right);
        }
        if (row < row_count-1) {
            Constraint* down = new Constraint();
            down->rest_distance = grid_size;
            down->a = i;
            down->b = i+col_count;
            constraints.push_back(down);
        }
        if (col < col_count-1 && row < row_count-1) {
            Constraint* down_right = new Constraint();
            down_right->rest_distance = grid_size * glm::sqrt(2);
            down_right->a = i;
            down_right->b = i+col_count+1;
            constraints.push_back(down_right);
        }
        if (row > 0 && col < col_count-1) {
            Constraint* up_right = new Constraint();
            up_right->rest_distance = grid_size * glm::sqrt(2);
            up_right->a = i;
            up_right->b = i-col_count+1;
            constraints.push_back(up_right);
        }
        if (col < col_count-2) {
            Constraint* right = new Constraint();
            right->rest_distance = grid_size * 2;
            right->a = i;
            right->b = i+1;
            constraints.push_back(right);
        }
        if (row < row_count-2) {
            Constraint* down = new Constraint();
            down->rest_distance = grid_size * 2;
            down->a = i;
            down->b = i+col_count;
            constraints.push_back(down);
        }
        if (col < col_count-2 && row < row_count-2) {
            Constraint* down_right = new Constraint();
            down_right->rest_distance = grid_size * glm::sqrt(2) * 2;
            down_right->a = i;
            down_right->b = i+col_count+1;
            constraints.push_back(down_right);
        }
        if (row > 1 && col < col_count-2) {
            Constraint* up_right = new Constraint();
            up_right->rest_distance = grid_size * glm::sqrt(2) * 2;
            up_right->a = i;
            up_right->b = i-col_count+1;
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

void Cloth::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        &vertices[0], GL_DYNAMIC_DRAW);

    // Perspective projection
    // Camera matrix
    // Model matrix
    glm::mat4 model;
    float scale = 0.5f;
    model = glm::scale(model, glm::vec3(0.5f));
    model = glm::translate(model, glm::vec3(-grid_size * col_count / 2.0f,
        -grid_size * row_count / 1.2f, 0));

    // Put transformation matrics together
    glm::mat4 mvp = projection * camera.GetViewMatrix() * model;
    GLint mvp_loc = glGetUniformLocation(shader.ID, "mvp");
    GLint model_loc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cloth::Initialize()
{
    CreateVertexBuffer();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

