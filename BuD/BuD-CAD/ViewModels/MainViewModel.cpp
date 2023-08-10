#include "MainViewModel.h"

MainViewModel::MainViewModel(SceneCAD& scene)
	: m_ObjectListViewModel(scene), m_PropertiesViewModel(scene), m_CursorViewModel(scene.m_MainCursor)
{
}
