#pragma once

#include "stdafx.h"
#include "render_context.h"

class OrthographicWindow
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	OrthographicWindow(RenderContext* renderContext);

	bool Initialize(int windowWidth, int windowHeight);
	void Render();

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	RenderContext* _renderContext;
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	
	int _vertexCount;
	int _indexCount;
};