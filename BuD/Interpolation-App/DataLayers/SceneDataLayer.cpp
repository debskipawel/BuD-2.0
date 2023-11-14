#include "SceneDataLayer.h"

SceneDataLayer::SceneDataLayer()
	: m_EulerScene(), m_EulerFrame(m_EulerScene), m_QuaternionScene(), m_QuaternionFrame(m_QuaternionScene), m_EulerGrid(m_EulerScene), m_QuaternionGrid(m_QuaternionScene)
{
}
