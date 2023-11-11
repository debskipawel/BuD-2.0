#pragma once

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>

class SceneSerializer
{
public:
	SceneSerializer(MainDataLayerCAD& dataLayer);
	
	virtual bool Save();
	virtual bool Load();

protected:
	virtual void ClearCurrentScene();

	virtual void ConstructLoadedScene();

	MainDataLayerCAD& m_MainDataLayer;
};
