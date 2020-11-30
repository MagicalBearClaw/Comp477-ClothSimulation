#pragma once
#include "../stdafx.h"
#include "Scene.h"
#include "MoveAbleSphere.h"

class FallingClothScene : public Scene
{
public:
	FallingClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight);
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void Update(bool keyState[], float deltaTime) override;
	virtual void Draw(Shader& shader, Camera& camera, glm::mat4 projection) override;

protected:
	// Inherited via Scene
	virtual void RecreateCloth() override;
private:
	std::unique_ptr<MoveableSphere> moveableSphere;
	std::string catTexturePath;
};


