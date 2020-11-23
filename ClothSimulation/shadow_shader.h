#pragma once
#include "stdafx.h"
#include "render_context.h"

class ShadowShader
{
private:
	struct MatrixBufferType
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 lightView;
		DirectX::XMFLOAT4X4 lightProjection;
	};

	struct LightBufferType2
	{
		DirectX::XMFLOAT3 lightPosition;
		float padding;
	};

public:
	ShadowShader(RenderContext* renderContext);

	bool Initialize();

	bool Render(int indexCount, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix,
				DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 lightViewMatrix, DirectX::XMFLOAT4X4 lightProjectionMatrix,
				ID3D11ShaderResourceView* depthMapTexture, DirectX::XMFLOAT3 lightPosition);

private:
	bool InitializeShader(ID3D11Device* device, HWND windowHandle, const WCHAR* vsFilename, const WCHAR* psFilename);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix,
							 DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 lightViewMatrix, DirectX::XMFLOAT4X4 lightProjectionMatrix,
							 ID3D11ShaderResourceView* depthMapTexture, DirectX::XMFLOAT3 lightPosition);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	RenderContext* _renderContext;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;
	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _lightBuffer;

};