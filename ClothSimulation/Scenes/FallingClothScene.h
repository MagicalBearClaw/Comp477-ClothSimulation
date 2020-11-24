#pragma once

#include "../stdafx.h"
#include "Scene.h"

class FallingClothScene : public Scene
{
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void FixedUpdate(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	// Inherited via Scene
	virtual void DrawUI() override;
};


