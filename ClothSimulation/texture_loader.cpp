#include "stdafx.h"
#include "texture_loader.h"

TextureLoader::TextureLoader(RenderContext* renderContext) : _renderContext(renderContext)
{
}

ID3D11ShaderResourceView * TextureLoader::Load(const std::string& filename)
{
    ID3D11Device* device = _renderContext->GetDevice();

	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (!surface)
	{
        std::string error(SDL_GetError());
        MessageBoxW(_renderContext->GetWindowHandle(), std::wstring(error.begin(), error.end()).c_str(), L"SDL create texture error", MB_OK);
	}

    ID3D11Texture2D*  texture2d = nullptr;

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = surface->w;
    desc.Height = surface->h;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = surface->pixels;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    HRESULT result = device->CreateTexture2D(&desc, &subResource, &texture2d);

    if (FAILED(result))
    {
        std::stringstream errorStream;
        errorStream << "A direct x error occured when creating a texture2d for file: ";
        errorStream << filename;
        errorStream << " with result code: ";
        errorStream << result;

        std::string error = errorStream.str();

        MessageBoxW(_renderContext->GetWindowHandle(), std::wstring(error.begin(), error.end()).c_str(), L"SDL create texture error", MB_OK);
    }

    ID3D11ShaderResourceView* textureView = nullptr;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    result = device->CreateShaderResourceView(texture2d, &srvDesc, &textureView);

    if (FAILED(result))
    {
        std::stringstream errorStream;
        errorStream << "A direct x error occured when creating shaderr ressource view: ";
        errorStream << filename;
        errorStream << " with result code: ";
        errorStream << result;

        std::string error = errorStream.str();

        MessageBoxW(_renderContext->GetWindowHandle(), std::wstring(error.begin(), error.end()).c_str(), L"SDL create texture error", MB_OK);
    }
	SDL_FreeSurface(surface);

    return textureView;
}
