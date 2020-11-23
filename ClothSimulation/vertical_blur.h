#pragma once
#include "stdafx.h"
#include "render_context.h"

class VerticalBlurShader
{
private:
	struct MatrixBufferType
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	struct ScreenSizeBufferType
	{
		float screenHeight;
		DirectX::XMFLOAT3 padding;
	};

public:
	VerticalBlurShader(RenderContext* renderContext);

	bool Initialize();
	bool Render(int indexCount, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix,
		DirectX::XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, float screenWidthint);
private:
	bool InitializeShader(ID3D11Device* device, HWND windowHandle, const WCHAR* vsFilename, const WCHAR* psFilename);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view,
		DirectX::XMFLOAT4X4 projection, ID3D11ShaderResourceView* texture, float screenHeight);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	RenderContext* _renderContext;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;
	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _screenSizeBuffer;
};