#pragma once

#include <DataLayers/MainDataLayer.h>

class SceneSerializer
{
public:
	SceneSerializer(MainDataLayer& dataLayer);
	
	virtual bool Save();
	virtual bool Load();

protected:
	virtual void ClearCurrentScene();

	virtual void ConstructLoadedScene();

	MainDataLayer& m_MainDataLayer;
};
