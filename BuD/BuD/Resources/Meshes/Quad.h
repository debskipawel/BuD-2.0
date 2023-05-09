#pragma once

#include <vector>
#include <SimpleMath.h>

#include <d3d11.h>

using namespace DirectX::SimpleMath;

static const std::vector<D3D11_INPUT_ELEMENT_DESC> g_quadLayout
{
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

static const std::vector<Vector2> g_QuadVertices =
{
    { -1.0f, -1.0f },
    {  1.0f, -1.0f },
    { -1.0f,  1.0f },
    {  1.0f,  1.0f },
};

static const std::vector<unsigned short> g_QuadIndices =
{
    0, 1, 3, 0, 3, 2,
    0, 3, 1, 0, 2, 3
};
