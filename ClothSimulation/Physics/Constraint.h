#pragma once
#include "../stdafx.h"
#include "Particle.h"

class Constraint 
{
public:
    
    Constraint(int vertexIdA, int vertexIdB, float restLength);
    void SatisfyConstraints(Particle* particleA, Particle* particleB);
public:
    int VertexIdA;
    int VertexIdB;
    float TearDistanceThreshold;
private:
    std::vector<Constraint*>* constraints;
    float restLength;
    int constraintIndex;
};