#pragma once

#include "../stdafx.h"

class Particle {
public:
    Particle();
    Particle(float, float, float);
    glm::vec3 pos;
    glm::vec3 old_pos;
    glm::vec3 force;
    bool pined;
    void set_index(int);
    void set_acc(glm::vec3);
    glm::vec3 get_acc();
    std::vector<int> get_s_neighbors(int i, int col_count, int row_count);
    std::vector<int> get_d_neighbors(int i, int col_count, int row_count);
private:
    glm::vec3 acceleration;
    glm::vec3 color;
    int index;
};