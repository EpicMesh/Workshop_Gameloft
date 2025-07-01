#pragma once
#include "SceneObject.h"

class Fire: public SceneObject
{
public:
	Fire();
	~Fire();

	float u_DispMax = 0.2;
	float u_Time;
	float speedFactor = 5000;

	void Draw() override;
	void Update(float deltaTime) override;


};

