#include "bud_pch.h"
#include "InputLayoutSystem.h"

namespace BuD
{
	std::shared_ptr<InputLayout> InputLayoutSystem::GetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements)
	{
		auto description = InputLayoutDescription(layoutElements);

		auto res = s_InputLayoutCollection.find(description);

		if (res != s_InputLayoutCollection.end())
		{
			return res->second;
		}

		auto newInputLayout = std::make_shared<InputLayout>(layoutElements);

		s_InputLayoutCollection.insert(std::make_pair(description, newInputLayout));

		return newInputLayout;
	}

	std::map<InputLayoutDescription, std::shared_ptr<InputLayout>> InputLayoutSystem::s_InputLayoutCollection = {};
}
