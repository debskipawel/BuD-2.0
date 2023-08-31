#pragma once

#include <Utils/Float2.h>
#include <Utils/Float3.h>

namespace MG1
{
	class Camera
	{
	public:
		explicit Camera() = default;

		Float3 focusPoint;
		float distance;
		Float2 rotation;

		friend void to_json(nlohmann::ordered_json& j, const Camera& p);
		friend void from_json(const nlohmann::ordered_json& j, Camera& p);
	};
}
