#pragma once

#include <Serializable.h>

namespace MG1
{
	class Float2
	{
	public:
		float x, y;

		friend void to_json(nlohmann::ordered_json& j, const Float2& p);
		friend void from_json(const nlohmann::ordered_json& j, Float2& p);
	};
}
