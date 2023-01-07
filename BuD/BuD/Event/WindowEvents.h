#pragma once

#include "Event.h"

namespace BuD
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height, bool minimized) : m_Width(width), m_Height(height), m_Minimized(minimized) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_Width, m_Height;
		const bool m_Minimized;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int xPos, int yPos) : m_PosX(xPos), m_PosY(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_PosX, m_PosY;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		virtual void Visit(IEventDispatchable&) override;
	};

	class WindowEnterSizeMoveEvent : public Event
	{
	public:
		WindowEnterSizeMoveEvent() = default;

		virtual void Visit(IEventDispatchable&) override;
	};

	class WindowExitSizeMoveEvent : public Event
	{
	public:
		WindowExitSizeMoveEvent() = default;

		virtual void Visit(IEventDispatchable&) override;
	};

	class ActivateAppEvent : public Event
	{
	public:
		ActivateAppEvent(bool isActive) : m_IsActive(isActive) {}

		virtual void Visit(IEventDispatchable&) override;

		const bool m_IsActive;
	};

	class ToggleFullscreenEvent : public Event
	{
	public:
		ToggleFullscreenEvent() = default;

		virtual void Visit(IEventDispatchable&) override;
	};
}
