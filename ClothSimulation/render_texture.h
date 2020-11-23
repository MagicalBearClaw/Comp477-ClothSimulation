#pragma once
#include "stdafx.h"
#include "render_context.h"

class RenderTexture
{
public:
	RenderTexture(RenderContext* renderContext);
	~RenderTexture();

	bool Create(int textureWidth, int textureHeight, float screenDepth, float screenNear);

	void Bind();
	void ClearRenderTarget(DirectX::XMFLOAT4 color);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView();
	DirectX::XMFLOAT4X4& GetProjectionMatrix();
	DirectX::XMFLOAT4X4& GetOrthoMatrix();

private:
	RenderContext * _renderContext;
	ID3D11Texture2D* _renderTargetTexture;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _shaderResourceView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11_VIEWPORT _viewport;
	DirectX::XMFLOAT4X4 _projectionMatrix;
	DirectX::XMFLOAT4X4 _orthographicMatrix;

};