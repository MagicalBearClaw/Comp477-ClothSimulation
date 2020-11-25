#include "../stdafx.h"
#include "Particle.h"

Particle::Particle(float mass, int vertexId, glm::vec3 position) : Body(mass)
{
	_currentPoistion = position;
	_previousPoistion = _currentPoistion;
	VertexId = vertexId;
}
