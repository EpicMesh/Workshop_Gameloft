#include "stdafx.h"
#include "Camera.h"
#include "Globals.h"
#include <iostream>

Camera::Camera() {};

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat fov, GLfloat nearPlane, GLfloat farPlane, GLfloat moveSpeed, GLfloat rotateSpeed)
	: position{ position }, target{ target }, up{ up }, fov{ fov }, nearPlane{ nearPlane }, farPlane{ farPlane }, moveSpeed{ moveSpeed }, rotateSpeed{ rotateSpeed } {
	
	updatePerspectiveMatrix();
	updateWorldView();
};

void Camera::moveOx(int sens)
{
	Vector3	forward = xAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}

void Camera::moveOy(int sens)
{
	Vector3 upward = yAxis * sens;
	Vector3 vectorDeplasare = upward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}

void Camera::moveOz(int sens)
{
	Vector3 forward = zAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}

void Camera::rotateOx(int sens)
{
	GLfloat unghiRotatie = sens * rotateSpeed * deltaTime;
	Matrix mRotateOX;
	mRotateOX.SetRotationX(unghiRotatie);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOX;

	Vector4 up1 = rotatedLocalUp * worldMatrix;
	up = up1.ToVector3();
	up = up.Normalize();
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);

	Vector4 rotatedTarget = localTarget * mRotateOX;
	Vector4 target1 = rotatedTarget * worldMatrix;
	target = target1.ToVector3();
	updateWorldView();
}

void Camera::rotateOy(int sens)
{
	GLfloat unghiRotatie = sens * rotateSpeed * deltaTime;
	Matrix mRotateOY;
	mRotateOY.SetRotationY(unghiRotatie);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOY;
	Vector4 target1 = rotatedTarget * worldMatrix;
	target = target1.ToVector3();
	updateWorldView();
}

void Camera::rotateOz(int sens)
{
	GLfloat unghiRotatie = sens * rotateSpeed * deltaTime;
	Matrix mRotateOZ;
	mRotateOZ.SetRotationZ(unghiRotatie);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOZ;

	Vector4 up1 = rotatedLocalUp * worldMatrix;
	up = up1.ToVector3();
	up = up.Normalize();

	updateWorldView();

}

void Camera::updateAxes()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::updatePerspectiveMatrix()
{
	GLfloat aspectRatio = (float)Globals::screenWidth / Globals::screenHeight;
	perspectiveMatrix = perspectiveMatrix.SetPerspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::updateWorldView()
{
	updateAxes();
	Matrix R;
	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z; R.m[0][3] = 0;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z; R.m[1][3] = 0;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z; R.m[2][3] = 0;
	R.m[3][0] = 0; R.m[3][1] = 0; R.m[3][2] = 0; R.m[3][3] = 1;

	Matrix T;
	T.SetTranslation(position);


	Matrix T1;
	T1.SetTranslation(-position);

	worldMatrix = R * T;
	viewMatrix =  T1 * R.Transpose();

}

void Camera::setDeltaTime(GLfloat deltaTime)
{
	this->deltaTime = deltaTime;
}


Camera::~Camera() {};