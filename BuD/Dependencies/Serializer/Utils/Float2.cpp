#include "pch.h"
#include "Float2.h"

#include <nlohmann/json.hpp>

namespace MG1
{
	void to_json(nlohmann::ordered_json& j, const Float2& p)
	{
		j = {
			{ "x", p.x },
			{ "y", p.y },
		};
	}

	void from_json(const nlohmann::ordered_json& j, Float2& p)
	{
		p.x = j["x"].get<float>();
		p.y = j["y"].get<float>();
	}
}
