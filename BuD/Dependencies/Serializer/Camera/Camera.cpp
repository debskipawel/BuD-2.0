#include "pch.h"
#include "Camera.h"

#include <nlohmann/json.hpp>

namespace MG1
{
	void to_json(nlohmann::ordered_json& j, const Camera& p)
	{
		j = nlohmann::ordered_json{
			{ "focusPoint", p.focusPoint },
			{ "distance", p.distance },
			{ "rotation", p.rotation },
		};
	}

	void from_json(const nlohmann::ordered_json& j, Camera& p)
	{
		p.focusPoint = j["focusPoint"];
		p.distance = j["distance"].get<float>();
		p.rotation = j["rotation"];
	}
}
