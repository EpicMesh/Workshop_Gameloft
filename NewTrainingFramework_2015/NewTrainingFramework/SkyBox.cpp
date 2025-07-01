#include "stdafx.h"
#include "SkyBox.h"

Skybox::Skybox() {
   
}

Skybox::~Skybox() {
   
}

void Skybox::Draw() {
    glDepthMask(GL_FALSE);
    glUseProgram(this->shader->program);
    glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textures[6]->idTexture);

    sendCommonData();
    glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);
   
    glDepthMask(GL_TRUE);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Update(float deltaTime) {
   
    if (camera) {
        position[0] = (followingCamera.x == 1) ? camera->getPosition().x : position[0];
        position[2] = (followingCamera.z == 1) ? camera->getPosition().z : position[2];
    }
}


void Skybox::SetFollowingCamera(const Vector3& followAxes) {
    followingCamera.x = followAxes.x;
    followingCamera.y = followAxes.y;
    followingCamera.z = followAxes.z;

}

void Skybox::SetCamera(Camera* cam) {
    camera = cam;
}