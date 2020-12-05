#pragma once
#include "../stdafx.h"
#include "Scene.h"
#include "MoveableSphere.h"
#include "Projectile.h"

class BasicClothScene : public Scene
{
public:
	BasicClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight);
	// Inherited via Scene
	virtual void Initialize() override;
	virtual void Update(bool keyState[], Camera& camera, float deltaTime) override;
	virtual void Draw(Shader& shader, Camera& camera, glm::mat4 projection) override;
	virtual void RecreateCloth() override;
private:
	std::unique_ptr<MoveableSphere> moveableSphere;
	std::unique_ptr<Projectile> projectile;
	float shootInterval;
	glm::vec3 position;
	int shootStartedTime;
	bool hasFired;
	glm::vec3 DefaultProjectilePosition;
	std::string catTexturePath;
};


