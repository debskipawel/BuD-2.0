#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <string>

#include <Buffers/VertexBuffer.h>
#include <Buffers/IndexBuffer.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	struct MeshMaterial
	{
		dxm::Vector3 AmbientColor;
		dxm::Vector3 DiffuseColor;
		dxm::Vector3 SpecularColor;

		float SpecularExponent;
		float OpticalDensity;
		float Dissolve;
		int Illumination;
	};

	struct MeshSegment
	{
		std::string SegmentName;

		std::shared_ptr<VertexBuffer> VertexBuffer;
		std::shared_ptr<IndexBuffer> IndexBuffer;

		MeshMaterial Material;

		std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
	};
}
