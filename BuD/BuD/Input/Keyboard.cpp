#include "bud_pch.h"
#include "Keyboard.h"

namespace BuD
{
	Keyboard& Keyboard::Get()
	{
		if (!s_Instance)
		{
			s_Instance = std::unique_ptr<Keyboard>(new Keyboard());
		}

		return *s_Instance;
	}

	bool Keyboard::KeyPressed(KeyboardKeys key)
	{
		auto res = m_KeyPressedMap.find(key);

		if (res == m_KeyPressedMap.end())
		{
			m_KeyPressedMap.insert(std::make_pair(key, false));
			return false;
		}

		return res->second;
	}

	bool Keyboard::KeyReleased(KeyboardKeys key)
	{
		return !this->KeyPressed(key);
	}
}
