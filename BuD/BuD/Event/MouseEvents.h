#pragma once

#include "Event.h"
#include <Input/MouseCode.h>

namespace BuD
{
	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(MouseCode button, int xPos, int yPos) : m_Button(button), m_PosX(xPos), m_PosY(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const MouseCode m_Button;
		const int m_PosX, m_PosY;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(MouseCode button, int xPos, int yPos) : m_Button(button), m_PosX(xPos), m_PosY(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const MouseCode m_Button;
		const int m_PosX, m_PosY;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int xOffset, int yOffset) : m_OffsetX(xOffset), m_OffsetY(yOffset) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_OffsetX, m_OffsetY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int xPos, int yPos, int wheelDelta) : m_PosX(xPos), m_PosY(yPos), m_WheelDelta(wheelDelta) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_PosX, m_PosY, m_WheelDelta;
	};

	class MouseRawInputEvent : public Event
	{
	public:
		MouseRawInputEvent() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class MouseInitializedEvent : public Event
	{
	public:
		MouseInitializedEvent() {}

		virtual void Visit(IEventDispatchable&) override;
	};
}
	