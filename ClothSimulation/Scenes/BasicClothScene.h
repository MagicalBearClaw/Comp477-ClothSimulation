#pragma once
#include "../stdafx.h"
#include "Scene.h"

class BasicClothScene : public Scene
{
public:
	BasicClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight);
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void FixedUpdate(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};


