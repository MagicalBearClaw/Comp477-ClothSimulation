#pragma once
#include "../stdafx.h"
#include "Body.h"

class Particle : public Body
{
public:
	Particle(float mass, int vertexId, glm::vec3 position);
	int VertexId;
};