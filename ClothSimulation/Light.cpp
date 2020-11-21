#include "stdafx.h"
#include "Light.h"

Light::Light()
{
}

Light::Light(const Light& light)
{
}

Light::~Light()
{
}

void Light::SetAmbientColor(DirectX::XMFLOAT4 color)
{
    _ambientColor = color;
}

void Light::SetDiffuseColor(DirectX::XMFLOAT4 color)
{
    _diffuseColor = color;
}

void Light::SetPosition(DirectX::XMFLOAT3 position)
{
    _position = position;
}

void Light::SetLookAt(DirectX::XMFLOAT3 lookAt)
{
    _lookAt = lookAt;
}

DirectX::XMFLOAT4 Light::GetAmbientColor()
{
    return _ambientColor;
}

DirectX::XMFLOAT4 Light::GetDiffuseColor()
{
    return _diffuseColor;
}

DirectX::XMFLOAT3 Light::GetPosition()
{
    return _position;
}

void Light::CreateViewMatrix()
{
    DirectX::XMFLOAT3 upVector(0, 1, 0);
    DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&upVector);

    DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&_lookAt);
    DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&_position);
    
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
    DirectX::XMStoreFloat4x4(&_viewMatrix, viewMatrix);
}

void Light::CreateProjectionMatrix(float screenDepth, float screenNear)
{
    float fieldOfView = DirectX::XM_PI / 2.0f;
    float screenAspect = 1.0;

    // Create the projection matrix for the light.
    DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
    return DirectX::XMStoreFloat4x4(&_projectionMatrix, projectionMatrix);
}

DirectX::XMFLOAT4X4& Light::GetViewMatrix()
{
    return _viewMatrix;
}

DirectX::XMFLOAT4X4& Light::GetProjectionMatrix()
{
    return _projectionMatrix;
}