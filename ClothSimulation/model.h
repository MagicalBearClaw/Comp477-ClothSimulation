#pragma once
#include "stdafx.h"
#include "render_context.h"

class Model
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model(RenderContext* renderContext);
	bool Initialize(const char* modelFilename, const WCHAR* textureFilename);
	void Render();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(DirectX::XMFLOAT3 position);
	DirectX::XMFLOAT3 GetPosition();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, const WCHAR* fileName);
	bool LoadModel(const char* fileName);


private:
	RenderContext* _renderContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
	ModelType* _model;

	DirectX::XMFLOAT3 _position;
	int _vertexCount;
	int _indexCount;
};
