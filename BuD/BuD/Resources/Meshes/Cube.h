#pragma once

#include <vector>
#include <SimpleMath.h>

#include <d3d11.h>

using namespace DirectX::SimpleMath;

static const std::vector<D3D11_INPUT_ELEMENT_DESC> g_cubePositionLayout
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

static const std::vector<Vector3> g_cubePositionVertices =
{
    { -0.05f, -0.05f, -0.05f },
    {  0.05f, -0.05f, -0.05f },
    { -0.05f,  0.05f, -0.05f },
    {  0.05f,  0.05f, -0.05f },
    { -0.05f, -0.05f,  0.05f },
    {  0.05f, -0.05f,  0.05f },
    { -0.05f,  0.05f,  0.05f },
    {  0.05f,  0.05f,  0.05f },
};

static const std::vector<unsigned short> g_cubePositionIndices =
{
    0, 1, 3, 0, 3, 2,
    0, 4, 5, 0, 5, 1,
    1, 5, 7, 1, 7, 3,
    2, 6, 4, 2, 4, 0,
    3, 7, 6, 3, 6, 2, 
    4, 6, 7, 4, 7, 5
};
