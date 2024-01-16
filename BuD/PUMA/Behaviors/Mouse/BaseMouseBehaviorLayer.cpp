#include "BaseMouseBehaviorLayer.h"

BaseMouseBehaviorLayer::BaseMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: m_MainDataLayer(mainDataLayer)
{
}

auto BaseMouseBehaviorLayer::OnLeftButtonDown(int x, int y) -> void
{
}

auto BaseMouseBehaviorLayer::OnMiddleButtonDown(int x, int y) -> void
{
}

auto BaseMouseBehaviorLayer::OnRightButtonDown(int x, int y) -> void
{
}

auto BaseMouseBehaviorLayer::OnLeftButtonUp(int x, int y) -> void
{
}

auto BaseMouseBehaviorLayer::OnMiddleButtonUp(int x, int y) -> void
{
}

auto BaseMouseBehaviorLayer::OnRightButtonUp(int x, int y) -> void
{
}

auto BaseMouseBehaviorLayer::OnMove(int x, int y, int dx, int dy) -> void
{
}

auto BaseMouseBehaviorLayer::OnScroll(int x, int y, int delta) -> void
{
}
