#pragma once
#include "../stdafx.h"
#include "../Rendering/Plane.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"
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
	Cloth(Plane * plane);
	~Cloth();
	void Update(float deltaTime);
	void Draw(Shader& shader, Camera& cameera, glm::mat4 projection);
	void AddForceGenerator(IForceGenerator* generator);
	void AddConstraint(IConstraint* constraint);
	void Initialize();
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
	std::vector<Particle*> Particles;
private:
	void CreateParticles(float clothMass);
	void ConnectSprings(float structalStiffness, float structalDamping, float shearStiffness, float shearDamping, float bendStiffness, float bendDamping);
	void AddSpring(float stiffness, float damping, Particle* particleA, Particle* particleB);
	void Reset();
private:
	Plane* _plane;

	std::vector<IForceGenerator*> _forceGenerators;
	std::vector<Spring> _springs;
	std::vector<IConstraint*> _constraints;
};