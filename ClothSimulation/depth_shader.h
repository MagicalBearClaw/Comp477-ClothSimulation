#pragma once
#include "stdafx.h"
#include "render_context.h"

class DepthShader
{
private:
	struct MatrixBufferType
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	DepthShader(RenderContext* renderContext);

	bool Initialize();
	bool Render(int indexCount, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

private:
	bool InitializeShader(ID3D11Device* device, HWND windowHandle, const WCHAR* vsFilename, const WCHAR* psFilename);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	RenderContext* _renderContext;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _layout;
	ID3D11Buffer* _matrixBuffer;

};