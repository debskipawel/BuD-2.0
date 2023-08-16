#pragma once

#include <functional>
#include <optional>
#include <string>

#include <Buffers/VertexBuffer.h>
#include <Buffers/IndexBuffer.h>
#include <Layout/InputLayout.h>
#include <Objects/AABB.h>

namespace BuD
{
	struct MeshDetails
	{
	public:
		MeshDetails();

		std::string m_Name;

		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		std::shared_ptr<InputLayout> m_InputLayout;

		std::optional<std::function<AABB()>> m_BoundingBoxCallback;

	public:
		bool operator==(const MeshDetails& other) const;
		bool operator!=(const MeshDetails& other) const;
		bool operator<=(const MeshDetails& other) const;
		bool operator>=(const MeshDetails& other) const;
		bool operator<(const MeshDetails& other) const;
		bool operator>(const MeshDetails& other) const;
	};
}
