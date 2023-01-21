#pragma once

#include "KeyboardKeys.h"

#include <map>
#include <memory>

namespace BuD
{
	class Keyboard
	{
	public:
		static Keyboard& Get();

		bool KeyPressed(KeyboardKeys key);
		bool KeyReleased(KeyboardKeys key);

	private:
		Keyboard() = default;
		Keyboard(const Keyboard& other) = delete;
		Keyboard operator=(const Keyboard& other) = delete;
		Keyboard& operator=(Keyboard& other) = default;

		void Press(KeyboardKeys key);
		void Release(KeyboardKeys key);

		inline static std::unique_ptr<Keyboard> s_Instance = nullptr;

		std::map<KeyboardKeys, bool> m_KeyPressedMap;

		friend class Win32EventFactory;
	};
}
