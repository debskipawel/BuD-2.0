#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class PathListGuiLayerCNC : public BaseGuiLayerCNC
{
public:
	PathListGuiLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual void DrawPathList();
	virtual void DrawAddPathButton();

	virtual void OpenImportPathDialog();
	virtual void DrawImportPathDialog();
};
