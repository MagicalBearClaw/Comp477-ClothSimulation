#pragma once
#include "../stdafx.h"
#include "../Rendering/Plane.h"
#include "Particle.h"
#include "Forces/Spring.h"
#include "Forces/IForceGenerator.h"
#include "Integrator/IIntergrator.h"
#include "Integrator/SemiImplicitEulerIntegrator.h"
#include "Constraints/IConstraint.h"
#include "Constraints/LengthConstraint.h"

class Cloth
{
public:
	Cloth();
	void Update(float deltaTime);
public:
	float Mass;
	float StructualStiffness;
	float StructualDamping;
	float ShearStiffness;
	float ShearDamping;
	float FlexionStiffness;
	float FlexionDamping;
	int ConstraintIterations;
	IIntegrator* IntegrationMethod;
private:
	void Initialize();
	void CreateParticle(int vertexId, float clothMass);
	void ConnectSprings(float structalStiffness, float structalDamping, float shearStiffness, float shearDamping, float bendStiffness, float bendDamping);
	void AddSpring(float stiffness, float damping, Particle particleA, Particle particleB);
	void AddForceGenerator(IForceGenerator& generator);
private:
	Plane* _plane;
	std::vector<Particle> _particles;
	std::vector<Particle> _bodies;
	std::vector<IForceGenerator> _forceGenerators;
	std::vector<Spring> _springs;
	std::vector<IConstraint> _constraints;
};