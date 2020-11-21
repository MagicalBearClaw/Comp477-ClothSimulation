#pragma once
#include "stdafx.h"
#include "render_context.h"


class TextureLoader
{
public:
	TextureLoader(RenderContext* renderContext);
	ID3D11ShaderResourceView* Load(const std::string& filename);
private:
	RenderContext* _renderContext;
};
