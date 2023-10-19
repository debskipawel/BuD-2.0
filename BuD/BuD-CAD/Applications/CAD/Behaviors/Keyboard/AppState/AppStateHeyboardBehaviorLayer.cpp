#include "AppStateHeyboardBehaviorLayer.h"

AppStateKeyboardBehaviorLayer::AppStateKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayerCAD(dataLayer)
{
}

bool AppStateKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return false;
	}

	HandleAppStateChange(key);

	return false;
}

void AppStateKeyboardBehaviorLayer::HandleAppStateChange(BuD::KeyboardKeys key)
{
	auto result = s_AppStateMap.find(key);

	if (result != s_AppStateMap.end())
	{
		auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

		auto& centroidCursor = scene.m_CentroidCursor;
		centroidCursor->SetAppState(result->second);

		m_MainDataLayer.m_AppStateDataLayer.SetAppState(result->second);
	}
}
