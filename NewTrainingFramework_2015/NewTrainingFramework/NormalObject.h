#pragma once
#include "SceneObject.h"

class NormalObject : public SceneObject {
public:
    NormalObject();
    ~NormalObject();

    void Draw() override;
    void Update(float deltaTime) override;
    void moveTerrain(float deltaTime);
};