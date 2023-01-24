#pragma once

#include <d3d11.h>
#include <string>

#include <Buffers/VertexBuffer.h>
#include <Buffers/IndexBuffer.h>

namespace BuD
{
	struct MeshSegment
	{
		std::string SegmentName;

		std::shared_ptr<VertexBuffer> VertexBuffer;
		std::shared_ptr<IndexBuffer> IndexBuffer;

		std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
	};
}
