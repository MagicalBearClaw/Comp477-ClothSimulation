#pragma once
#include "../stdafx.h"

class Scene
{
public:
	virtual void Initialize() = 0;
	virtual void FixedUpdate(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};