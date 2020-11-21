#include "stdafx.h"
#include "Camera.h"

Camera::Camera() : _position(DirectX::XMFLOAT3(0,0,0)), _rotation(DirectX::XMFLOAT3(0, 0, 0))
{
}

Camera::Camera(const Camera&)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(DirectX::XMFLOAT3& position)
{
	_position = position;
}

void Camera::SetRotation(DirectX::XMFLOAT3& rotation)
{
	_rotation = rotation;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return _position;
}

DirectX::XMFLOAT3 Camera::GetRotation()
{
	return _rotation;
}

void Camera::Update()
{
	DirectX::XMFLOAT3 upVector(0, 1, 0);
	DirectX::XMFLOAT3 lookAtVector(0, 0, 1);

	DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtVector);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&upVector);


	float yaw, pitch, roll;

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&_rotation));

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.

	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&_position);
	lookAt = DirectX::XMVectorAdd(position, up);

	// Finally create the view matrix from the three updated vectors.
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
	DirectX::XMStoreFloat4x4(&_viewMatrix, viewMatrix);
}

// todo: remember this must be a ref back to caller so changes persists
DirectX::XMFLOAT4X4& Camera::GetViewMatrix()
{
	return _viewMatrix;
}

void Camera::UpdateBaseViewMatrix()
{
	DirectX::XMFLOAT3 upVector(0, 1, 0);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&upVector);
	DirectX::XMFLOAT3 lookAtVector;

	float radians = _rotation.y;

	// Setup where the camera is looking.
	lookAtVector.x = sinf(radians) + _position.x;
	lookAtVector.y = _position.y;
	lookAtVector.z = cosf(radians) + _position.z;

	// Create the base view matrix from the three vectors.
	DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtVector);
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&_position);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
	DirectX::XMStoreFloat4x4(&_baseViewMatrix, viewMatrix);
}

DirectX::XMFLOAT4X4& Camera::GetBaseViewMatrix()
{
	return _baseViewMatrix;
}
