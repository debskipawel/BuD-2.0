#pragma once

namespace BuD
{
	enum class CullType : int
	{
		NONE = 0,
		FRONT = 1,
		BACK = 2,
	};

	enum class FillMode : int
	{
		SOLID = 1,
		WIREFRAME = 2,
	};

	struct RasterizerDescription
	{
	public:
		CullType m_CullType = CullType::BACK;
		FillMode m_FillMode = FillMode::SOLID;

	public:
		bool operator==(const RasterizerDescription& other) const;
		bool operator!=(const RasterizerDescription& other) const;
		bool operator>(const RasterizerDescription& other) const;
		bool operator<(const RasterizerDescription& other) const;
		bool operator>=(const RasterizerDescription& other) const;
		bool operator<=(const RasterizerDescription& other) const;
	};
}
