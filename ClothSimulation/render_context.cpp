#include "stdafx.h"
#include "render_context.h"

RenderContext::RenderContext(HWND& windowHandle) : _windowHandle(&windowHandle), _device(nullptr), _deviceContext(nullptr), 
												  _renderTargetView(nullptr), _swapChain(nullptr)
{
	_depthStencilBuffer = nullptr;
	_depthStencilState = nullptr;
	_depthStencilView = nullptr;
	_rasterState = nullptr;
	_depthDisabledStencilState = nullptr;
}

void RenderContext::Shutdown()
{
}

RenderContext::~RenderContext()
{
	Shutdown();
}

RenderContext* RenderContext::CreateInstance(HWND& windowHandle, int screenWidth, int screenHeight, bool isFullScreen)
{
	RenderContext* renderContext = new RenderContext(windowHandle);
	if (!renderContext->Initialize(screenWidth, screenHeight, 0.00001, 10000000, isFullScreen))
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


bool RenderContext::Initialize(int screenWidth, int screenHeight, float screenDepth, float screenNear, bool isFullScreen = false)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView = 0;
	float aspectRatio = 0;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;



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

	if (!CreateRenderTarget())
	{
		return false;
	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRESULT result = _device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = _device->CreateDepthStencilView(_depthStencilBuffer.Get(), &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView.Get());

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	_deviceContext->RSSetState(_rasterState.Get());

	// Setup the viewport for rendering.
	_viewport.Width = (float)screenWidth;
	_viewport.Height = (float)screenHeight;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	_deviceContext->RSSetViewports(1, &_viewport);

	// Setup the projection matrix.
	fieldOfView = (float)DirectX::XM_PI / 4.0f;
	aspectRatio = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, screenNear, screenDepth);
	DirectX::XMStoreFloat4x4(&_projectionMatrix, projectionMatrix);
	// Initialize the world matrix to the identity matrix.
	DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&_worldMatrix, identityMatrix);
	// Create an orthographic projection matrix for 2D rendering.
	DirectX::XMMATRIX orthographicMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	DirectX::XMStoreFloat4x4(&_orthographicMatrix, orthographicMatrix);
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = _device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
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
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), colors);

	// Clear the depth buffer.
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

HWND RenderContext::GetWindowHandle()
{
	return *_windowHandle;
}

DirectX::XMFLOAT4X4& RenderContext::GetProjectionMatrix()
{
	return _projectionMatrix;
}

DirectX::XMFLOAT4X4& RenderContext::GetWorldMatrix()
{
	return _worldMatrix;
}

DirectX::XMFLOAT4X4& RenderContext::GetOrthographicMatrix()
{
	return _orthographicMatrix;
}

void RenderContext::SetBackBufferRenderTarget()
{
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView.Get());
}

void RenderContext::ResetViewport()
{
	_deviceContext->RSSetViewports(1, &_viewport);
}

void RenderContext::EnableZbuffer(bool state)
{
	if (state)
	{
		_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 1);
	}
	else 
	{
		_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState.Get(), 1);
	}
}

ID3D11Device* RenderContext::GetDevice()
{
	return _device.Get();
}

ID3D11DeviceContext* RenderContext::GetDeviceContext()
{
	return _deviceContext.Get();
}

IDXGISwapChain* RenderContext::GetSwapChain()
{
	return _swapChain.Get();
}

ID3D11RenderTargetView* RenderContext::GetMainRenderTargetView()
{
	return _renderTargetView.Get();
}
