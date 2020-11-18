#pragma once
#include "stdafx.h"

class RenderContext
{
public:
	RenderContext(HWND& windowHandle);
	RenderContext(const RenderContext& rendercontext) = delete;
	RenderContext& operator=(const RenderContext& context) = delete;
	~RenderContext();
	
	static RenderContext* CreateInstance(HWND& windowHandle, bool isFullScreen);
	
	void Shutdown();
	void Present();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();
	ID3D11RenderTargetView* GetMainRenderTargetView();
	bool CreateRenderTarget();
	void CleanupRenderTarget();
	void ClearBuffer(float r, float g, float b);

private:

	bool Initialize(bool isFullScreen);

private:
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	HWND* _windowHandle;
};