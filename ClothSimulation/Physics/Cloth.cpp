#pragma once
#include "../stdafx.h"
#include "Cloth.h"

Cloth::Cloth(Plane* plane) : _plane(plane)
{
}

Cloth::~Cloth()
{
    Reset();
}

void Cloth::Update(float deltaTime)
{
    //sum all local forces
    for(Spring& spring : _springs)
    {
        spring.ApplyForce(nullptr);  //no need to specify any simObj
    }

    //sum all global forces acting on the objects
    for(Particle* particle : Particles)
    {
        for (IForceGenerator* forceGenerator : _forceGenerators)
        {
            forceGenerator->ApplyForce(particle);
        }
    }

    glm::vec3 acceleration;
    for(Particle* particle : Particles)
    {
        //find acceleration
        acceleration = particle->GetAppliedForces() / particle->GetMass();

        //integrate
        IntegrationMethod->Intergrate(particle, acceleration, deltaTime);
    }

    //satisfy constraints
    for(int i = 0; i < ConstraintIterations; i++)
    {
        for(IConstraint* constraint : _constraints)
        {
            constraint->SatisfyConstraint();
        }
    }

    //update object
    for(Particle* particle : Particles)
    {
        _plane->SetVertexPosition(particle->VertexId, particle->GetCurrentPosition());
    }

    //reset forces on sim objects
    for (Particle* particle : Particles)
    {
        particle->ResetAppliedForces();
    }
}

void Cloth::Draw(Shader& shader, Camera& cameera, glm::mat4 projection)
{
    _plane->Draw(shader, cameera, projection);
}

void Cloth::Initialize()
{
	ConstraintIterations = 30;
    Reset();
    CreateParticles(Mass);
    ConnectSprings(StructualStiffness, StructualDamping, ShearStiffness, ShearDamping, FlexionStiffness, FlexionDamping);
}

void Cloth::CreateParticles(float clothMass)
{
    int numVertices = _plane->NumberOfVertices;
    float vertexMass = clothMass / numVertices;
    for (int i = 0; i < numVertices; i++)
    {
        Particle*  p =  new Particle(vertexMass, i, _plane->GetVertexPosition(i));
        Particles.push_back(p);
    }
}

void Cloth::ConnectSprings(float structalStiffness, float structalDamping, float shearStiffness, float shearDamping, float bendStiffness, float bendDamping)
{
    for (int x = 0; x < _plane->NumberOfLengthSegments; x++)
    {
        for (int y = 0; y <= _plane->NumberOfWidthSegments; y++)
        {
            //structural spring: horizontal (-)
            int vertexAId = x + y * (_plane->NumberOfLengthSegments + 1);
            int vertexBId = (x + 1) + y * (_plane->NumberOfLengthSegments + 1);
            AddSpring(structalStiffness, structalDamping, Particles[vertexAId], Particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(new LengthConstraint(Particles[vertexAId], Particles[vertexBId], length));
        }
    }

    for (int x = 0; x <= _plane->NumberOfLengthSegments; x++)
    {
        for (int y = 0; y < _plane->NumberOfWidthSegments; y++)
        {
            //structural spring: vertical (|)
            int vertexAId = x + y * (_plane->NumberOfLengthSegments + 1);
            int vertexBId = x + (y + 1) * (_plane->NumberOfLengthSegments + 1);
            AddSpring(structalStiffness, structalDamping, Particles[vertexAId], Particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(new LengthConstraint(Particles[vertexAId], Particles[vertexBId], length));
        }
    }

    for (int x = 0; x < _plane->NumberOfLengthSegments; x++)
    {
        for (int y = 0; y < _plane->NumberOfWidthSegments; y++)
        {
            //shear spring: diagonal (/)
            int vertexAId = (x + 1) + y * (_plane->NumberOfLengthSegments + 1);
            int vertexBId = x + (y + 1) * (_plane->NumberOfLengthSegments + 1);
            AddSpring(shearStiffness, shearDamping, Particles[vertexAId], Particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(new LengthConstraint(Particles[vertexAId], Particles[vertexBId], length));

            //shear spring: diagonal (\)
            vertexAId = x + y * (_plane->NumberOfLengthSegments + 1);
            vertexBId = (x + 1) + (y + 1) * (_plane->NumberOfLengthSegments + 1);
            AddSpring(shearStiffness, shearDamping, Particles[vertexAId], Particles[vertexBId]);
            length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(new LengthConstraint(Particles[vertexAId], Particles[vertexBId], length));
        }
    }

    for (int x = 0; x < _plane->NumberOfLengthSegments - 1; x++)
    {
        for (int y = 0; y <= _plane->NumberOfWidthSegments; y++)
        {
            //bend spring: horizontal (--)
            int vertexAId = x + y * (_plane->NumberOfLengthSegments + 1);
            int vertexBId = (x + 2) + y * (_plane->NumberOfLengthSegments + 1);
            AddSpring(bendStiffness, bendDamping, Particles[vertexAId], Particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(new LengthConstraint(Particles[vertexAId], Particles[vertexBId], length));
        }
    }

    for (int x = 0; x <= _plane->NumberOfLengthSegments; x++)
    {
        for (int y = 0; y < _plane->NumberOfWidthSegments - 1; y++)
        {
            //bend spring: vertical (||)
            int vertexAId = x + y * (_plane->NumberOfLengthSegments + 1);
            int vertexBId = x + (y + 2) * (_plane->NumberOfLengthSegments + 1);
            AddSpring(bendStiffness, bendDamping, Particles[vertexAId], Particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(new LengthConstraint(Particles[vertexAId], Particles[vertexBId], length));
        }
    }
}

void Cloth::AddSpring(float stiffness, float damping, Particle* particleA, Particle* particleB)
{
	Spring spring(stiffness, damping, (particleA->GetCurrentPosition() - particleB->GetCurrentPosition()).length());
	_springs.push_back(spring);
}

void Cloth::AddForceGenerator(IForceGenerator* generator)
{
	_forceGenerators.push_back(generator);
}

void Cloth::AddConstraint(IConstraint* constraint)
{
    _constraints.push_back(constraint);
}

void Cloth::Reset()
{
    for (Particle* particle : Particles)
    {
        if (particle)
            delete particle;
    }

    for (IConstraint* constraint : _constraints)
    {
        if (constraint)
            delete constraint;
    }

    for (IForceGenerator* force : _forceGenerators)
    {
        if (force)
            delete force;
    }
}
