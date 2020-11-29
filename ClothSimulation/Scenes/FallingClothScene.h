#pragma once
#include "../stdafx.h"
#include "Scene.h"

class FallingClothScene : public Scene
{
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void Update(bool keyState[], float deltaTime) override;
	virtual void Draw(Shader& shader, Camera& camera, glm::mat4 projection) override;
};


