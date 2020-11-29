#pragma once
#include "../stdafx.h"
#include "Scene.h"
#include "MoveAbleSphere.h"


class BasicClothScene : public Scene
{
public:
	BasicClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight);
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void Update(bool keyState[], float deltaTime) override;
	virtual void Draw(Shader& shader, Camera& camera, glm::mat4 projection) override;

private:
	std::unique_ptr<MoveableSphere> moveableSphere;
};


