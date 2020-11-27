#include "../stdafx.h"
#include "Cloth.h"

Cloth::Cloth(int width, int height, const std::string& textureFileName)
    : height(height), width(width), textureFileName(textureFileName)
{
    SegmentLength = 1.0f / (float)(std::max(height-1, width-1));
    vertexCount = height * width;
    Mass = 1.0f;
    Stiffness = 2.0f;
    elapsedTime = 0.0f;
    IsWindForceEnabled = false;
    NumberOfConstraintIterations = 10;

    Initialize();
}

Cloth::~Cloth()
{
    Reset();
}

void Cloth::CreateVertexBuffer() 
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Vertex vertex;
            vertex.Position = glm::vec3(SegmentLength * j, SegmentLength * i, 0);
            vertex.TexCoords = glm::vec2((float)i / height, (float)j / width);
            vertices.push_back(vertex);
        }
    }
}

void Cloth::CreateIndexBuffer()
{
    int index_count = (height - 1) * (width - 1) * 6;
    for (int i = 0; i < height - 1; i++) {
        int stride = i * (width - 1);
        for (int j = 0; j < width - 1; j++) {
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
    for (int i = 0; i < vertexCount; i++) 
    {
        particles.push_back(new Particle(vertices[i].Position, i));
    }
}

void Cloth::Update(float deltaTime) {
    glm::vec3 force;

    float vertex_mass = Mass / vertexCount;  // Mass of each vertex
    float damping = 0.02f;  // Damping (air resistance)
    
    glm::vec3 wind_force = glm::vec3(0);
    glm::vec3 gravity = 0.1f * glm::vec3(0, 9.8f, 0);
    
    for (int i=0; i<vertexCount; i++) {
        Particle* curr_point = particles[i];
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
        glm::vec3 temp = particles[i]->CurrentPosition;
        if(!particles[i]->IsPositionConstrained) {
            particles[i]->CurrentPosition = particles[i]->CurrentPosition + (1.0f - damping)
                             * (particles[i]->CurrentPosition - particles[i]->PreviousPosition)
                             + particles[i]->Acceleration * deltaTime;

            // give function here to handle collision
            //glm::vec3 offset = particles[i]->CurrentPosition - ball_center;
            //if (glm::length(offset) < ball_radius) {
            //    particles[i]->CurrentPosition += glm::normalize(offset)
            //                      * (ball_radius - glm::length(offset));
            //}
                
        }
        particles[i]->PreviousPosition = temp;
    }

    /* Satisfy constraint */
    for (int j=0; j<10; j++) {
        for (int i=0; i<constraints.size(); i++) {
            Constraint* it = constraints[i];
            Particle * a = particles[it->VertexIdA];
            Particle * b = particles[it->VertexIdB];
            it->SatisfyConstraints(a, b);
        }
    }

    for (int i=0; i<vertexCount; i++) {
        vertices[i].Position = particles[i]->CurrentPosition;
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

void Cloth::Reset()
{
    for(Particle* particle : particles)
    {
        if (particle)
        {
            delete particle;
            particle = nullptr;
        }
    }

    for(Constraint* constraint : constraints)
    {
        if (constraint)
        {
            delete constraint;
            constraint = nullptr;
        }
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Cloth::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    float scale = 0.5f;

    glm::mat4 model;
    model = glm::scale(model, glm::vec3(scale));
    model = glm::translate(model, glm::vec3(-SegmentLength * width / 2.0f, -SegmentLength * height / 1.2f, 0));

    glm::mat4 mvp = projection * camera.GetViewMatrix() * model;
    
    GLint mvp_loc = glGetUniformLocation(shader.ID, "mvp");
    GLint model_loc = glGetUniformLocation(shader.ID, "model");
    GLint diffuse_loc = glGetUniformLocation(shader.ID, "diffuse");
    glUniform1i(diffuse_loc, textureId);
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    shader.Use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cloth::AddParticlPositionConstraint(unsigned int id)
{
    particles[id]->IsPositionConstrained = true;
}

void Cloth::Initialize()
{
    Reset();
    CreateVertexBuffer();
    CreateIndexBuffer();
    LoadTexture(textureFileName);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Vertex), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    CreateParticles();
    CreateConstraints();

}


void Cloth::LoadTexture(const std::string& textureFileName)
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