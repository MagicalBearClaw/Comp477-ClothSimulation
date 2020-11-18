#include "stdafx.h"
#include "render_context.h"

RenderContext::RenderContext(HWND& windowHandle) : _windowHandle(&windowHandle), _device(nullptr), _deviceContext(nullptr), 
												  _renderTargetView(nullptr), _swapChain(nullptr)
{
}

void RenderContext::Shutdown()
{
	if (_swapChain)
	{
		_swapChain->Release();
	}

	if (_device)
	{
		_device->Release();
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
	}
}

RenderContext::~RenderContext()
{
	Shutdown();
}

RenderContext* RenderContext::CreateInstance(HWND& windowHandle, bool isFullScreen)
{
	RenderContext* renderContext = new RenderContext(windowHandle);
	if (!renderContext->Initialize(isFullScreen))
	{
		return nullptr;
	}
	return renderContext;
}

void RenderContext::CleanupRenderTarget()
{
	if (_renderTargetView) 
	{ 
		_renderTargetView->Release(); 
		_renderTargetView = nullptr; 
	}
}


bool RenderContext::Initialize(bool isFullScreen = false)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = *_windowHandle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = !isFullScreen;
	sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &_swapChain,
		&_device, &featureLevel, &_deviceContext) != S_OK)
	{
		return false;
	}

	return CreateRenderTarget();
}

bool RenderContext::CreateRenderTarget()
{
	ID3D11Texture2D* backBuffer = nullptr;
	if (_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)) != S_OK)
	{
		return false;
	}

	if (_device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView) != S_OK)
	{
		backBuffer->Release();
		return false;
	}
	
	backBuffer->Release();
	return true;
}


void RenderContext::Present()
{
	_swapChain->Present(1, 0);
}

void RenderContext::ClearBuffer(float r, float g, float b)
{
	const float colors[] = { r, g,b, 1.0f };
	_deviceContext->ClearRenderTargetView(_renderTargetView, colors);
}

ID3D11Device* RenderContext::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* RenderContext::GetDeviceContext()
{
	return _deviceContext;
}

IDXGISwapChain* RenderContext::GetSwapChain()
{
	return _swapChain;
}

ID3D11RenderTargetView* RenderContext::GetMainRenderTargetView()
{
	return _renderTargetView;
}
