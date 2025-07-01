#pragma once
#include "../Utilities/Math.h"

class Camera
{
private:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat nearPlane;
	GLfloat farPlane;
	GLfloat fov;
	GLfloat deltaTime;
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;
	
	Matrix worldMatrix;

public:

    void setPosition(const Vector3& newPosition) {
        position.x = newPosition.x;
		position.y = newPosition.y;
		position.z = newPosition.z;
    }

    void setTarget(const Vector3& newTarget) {
        target.x = newTarget.x;
		target.y = newTarget.y;
		target.z = newTarget.z;
    }

    void setUp(const Vector3& newUp) {
        up.x = newUp.x;
		up.y = newUp.y;
		up.z = newUp.z;
    }

    void setMoveSpeed(GLfloat newMoveSpeed) {
        moveSpeed = newMoveSpeed;
    }

    void setRotateSpeed(GLfloat newRotateSpeed) {
        rotateSpeed = newRotateSpeed;
    }

    void setNearPlane(GLfloat newNearPlane) {
        nearPlane = newNearPlane;
    }

    void setFarPlane(GLfloat newFarPlane) {
        farPlane = newFarPlane;
    }

    void setFov(GLfloat newFov) {
        fov = newFov;
    }


	Matrix viewMatrix;
	Matrix perspectiveMatrix;
	Camera();
	Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat fov, GLfloat nearPlane, GLfloat farPlane, GLfloat moveSpeed, GLfloat rotateSpeed);
	~Camera();
	void moveOx(int sens);
	void moveOy(int sens);
	void moveOz(int sens);
	void rotateOx(int sens);
	void rotateOy(int sens);
	void rotateOz(int sens);
	void updateAxes();
	void updateWorldView();
	void updatePerspectiveMatrix();
	void setDeltaTime(GLfloat deltaTime);
	Vector3 getPosition() {
		return position;
	};
};

