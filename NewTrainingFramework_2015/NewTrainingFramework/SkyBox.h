#pragma once

#include "SceneObject.h"

class Skybox : public SceneObject {
public:
    Skybox();
    ~Skybox();

    void Draw() override;
    void Update(float deltaTime) override;

    void SetFollowingCamera(const Vector3& followAxes);
    void SetCamera(Camera* cam);

private:
    GLuint cubeMapTexture;
    Vector3 followingCamera;
    Camera* camera;
};

