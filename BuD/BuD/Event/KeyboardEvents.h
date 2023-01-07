#pragma once

#include "Event.h"

#include <Input/KeyboardKeys.h>

namespace BuD
{
	class KeyDownEvent : public Event
	{
	public:
		KeyDownEvent(KeyboardKeys key) : m_Key(key) {}

		virtual void Visit(IEventDispatchable&) override;

		const KeyboardKeys m_Key;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(KeyboardKeys key) : m_Key(key) {}

		virtual void Visit(IEventDispatchable&) override;

		const KeyboardKeys m_Key;
	};

	class KeyRepeatEvent : public Event
	{
	public:
		KeyRepeatEvent(KeyboardKeys key) : m_Key(key) {}

		virtual void Visit(IEventDispatchable&) override;

		const KeyboardKeys m_Key;
	};
}
