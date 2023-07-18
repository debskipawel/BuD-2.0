#pragma once

#include <map>
#include <memory>

#include <Buffers/InputLayout.h>
#include <Systems/InputLayoutDescription.h>

namespace BuD
{
	class InputLayoutSystem
	{
	public:
		static std::shared_ptr<InputLayout> GetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements);

	private:
		static std::map<InputLayoutDescription, std::shared_ptr<InputLayout>> s_InputLayoutCollection;
	};
}
