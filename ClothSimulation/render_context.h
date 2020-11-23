#pragma once
#include "stdafx.h"

class RenderContext
{
public:
	RenderContext(HWND& windowHandle);
	RenderContext(const RenderContext& rendercontext) = delete;
	RenderContext& operator=(const RenderContext& context) = delete;
	~RenderContext();
	
	static RenderContext* CreateInstance(HWND& windowHandle, int screenWidth, int screenHeight, bool isFullScreen);

	void Shutdown();
	void Present();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();
	ID3D11RenderTargetView* GetMainRenderTargetView();
	bool CreateRenderTarget();
	void CleanupRenderTarget();
	void ClearBuffer(float r, float g, float b);
	HWND GetWindowHandle();

	DirectX::XMFLOAT4X4& GetProjectionMatrix();
	DirectX::XMFLOAT4X4& GetWorldMatrix();
	DirectX::XMFLOAT4X4& GetOrthographicMatrix();

	void SetBackBufferRenderTarget();
	void ResetViewport();

	void EnableZbuffer(bool state);

private:
	bool Initialize(int screenWidth, int screenHeight, float screenDepth, float screenNear, bool isFullScreen);

private:
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11RasterizerState* _rasterState;
	ID3D11DepthStencilState* _depthDisabledStencilState;
	
	DirectX::XMFLOAT4X4 _projectionMatrix;
	DirectX::XMFLOAT4X4 _worldMatrix;
	DirectX::XMFLOAT4X4 _orthographicMatrix;

	HWND* _windowHandle;
	D3D11_VIEWPORT _viewport;
};