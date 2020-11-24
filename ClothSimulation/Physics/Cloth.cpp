#pragma once
#include "../stdafx.h"
#include "Cloth.h"


Cloth::Cloth()
{
}

void Cloth::Update(float deltaTime)
{
    //sum all local forces
    for(Spring& spring : _springs)
    {
        spring.ApplyForce(nullptr);  //no need to specify any simObj
    }

    //sum all global forces acting on the objects
    for(Particle& particle : _bodies)
    {
        for (IForceGenerator& forceGenerator : _forceGenerators)
        {
            forceGenerator.ApplyForce(&particle);
        }
    }

    glm::vec3 acceleration;
    for(Particle& particle : _bodies)
    {
        //find acceleration
        acceleration = particle.GetAppliedForces() / particle.GetMass();

        //integrate
        IntegrationMethod->Intergrate(&particle, acceleration, deltaTime);
    }

    //satisfy constraints
    for(int i = 0; i < ConstraintIterations; i++)
    {
        for(IConstraint& constraint : _constraints)
        {
            constraint.SatisfyConstraint();
        }
    }

    //update object
    for(Particle& particle : _bodies)
    {
        //particle.Update(deltaTime);
    }

    //reset forces on sim objects
    for (Particle& particle : _bodies)
    {
        particle.ResetAppliedForces();
    }
}

void Cloth::Initialize()
{
	ConstraintIterations = 30;
}

void Cloth::CreateParticle(int vertexId, float clothMass)
{
    int numVertices = _plane->NumberOfVertices;
    float vertexMass = clothMass / numVertices;
    _particles.resize(numVertices);
    for (int i = 0; i < numVertices; i++)
    {
        // TODO: fix 3 parameter and makes sure the texture plane stuff works in the particle class correctly
        _bodies.push_back(Particle(vertexMass, i, glm::vec3(0,0,0)));
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
            AddSpring(structalStiffness, structalDamping, _particles[vertexAId], _particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(LengthConstraint(&_particles[vertexAId], &_particles[vertexBId], length));
        }
    }

    for (int x = 0; x <= _plane->LengthSegments; x++)
    {
        for (int y = 0; y < _plane->WidthSegments; y++)
        {
            //structural spring: vertical (|)
            int vertexAId = x + y * (_plane->LengthSegments + 1);
            int vertexBId = x + (y + 1) * (_plane->LengthSegments + 1);
            AddSpring(structalStiffness, structalDamping, _particles[vertexAId], _particles[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(LengthConstraint(&_particles[vertexAId], &_particles[vertexBId], length));
        }
    }

    for (int x = 0; x < _plane->LengthSegments; x++)
    {
        for (int y = 0; y < _plane->WidthSegments; y++)
        {
            //shear spring: diagonal (/)
            int vertexAId = (x + 1) + y * (_plane->LengthSegments + 1);
            int vertexBId = x + (y + 1) * (_plane->LengthSegments + 1);
            this.AddSpring(shearStiffness, shearDamping, simVertices[vertexAId], simVertices[vertexBId]);
            float length = (_plane->GetVertexPosition(vertexAId) - _plane->GetVertexPosition(vertexBId)).length();
            _constraints.push_back(LengthConstraint(&_particles[vertexAId], &_particles[vertexBId], length));

            //shear spring: diagonal (\)
            vertexAId = x + y * (clothPlane.LengthSegments + 1);
            vertexBId = (x + 1) + (y + 1) * (clothPlane.LengthSegments + 1);
            this.AddSpring(shearStiffness, shearDamping, simVertices[vertexAId], simVertices[vertexBId]);
            length = (clothPlane.GetVertexPosition(vertexAId) - clothPlane.GetVertexPosition(vertexBId)).Length();
            _constraints.push_back(LengthConstraint(&_particles[vertexAId], &_particles[vertexBId], length));
        }
    }

    for (int x = 0; x < clothPlane.LengthSegments - 1; x++)
    {
        for (int y = 0; y <= clothPlane.WidthSegments; y++)
        {
            //bend spring: horizontal (--)
            int vertexAId = x + y * (clothPlane.LengthSegments + 1);
            int vertexBId = (x + 2) + y * (clothPlane.LengthSegments + 1);
            this.AddSpring(bendStiffness, bendDamping, simVertices[vertexAId], simVertices[vertexBId]);
            float length = (clothPlane.GetVertexPosition(vertexAId) - clothPlane.GetVertexPosition(vertexBId)).Length();
            _constraints.push_back(LengthConstraint(&_particles[vertexAId], &_particles[vertexBId], length));
        }
    }

    for (int x = 0; x <= clothPlane.LengthSegments; x++)
    {
        for (int y = 0; y < clothPlane.WidthSegments - 1; y++)
        {
            //bend spring: vertical (||)
            int vertexAId = x + y * (clothPlane.LengthSegments + 1);
            int vertexBId = x + (y + 2) * (clothPlane.LengthSegments + 1);
            this.AddSpring(bendStiffness, bendDamping, simVertices[vertexAId], simVertices[vertexBId]);
            float length = (clothPlane.GetVertexPosition(vertexAId) - clothPlane.GetVertexPosition(vertexBId)).Length();
            _constraints.push_back(LengthConstraint(&_particles[vertexAId], &_particles[vertexBId], length));
        }
    }
}

void Cloth::AddSpring(float stiffness, float damping, Particle particleA, Particle particleB)
{
	Spring spring(stiffness, damping, particleA, particleB);
	_springs.push_back(spring);
}

void Cloth::AddForceGenerator(IForceGenerator& generator)
{
	_forceGenerators.push_back(generator);
}
