#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>
#include <Applications/CNC/Factories/MillingTool/MillingToolFactory.h>

class PathListGuiLayerCNC : public BaseGuiLayerCNC
{
public:
	PathListGuiLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual void DrawPathList();
	virtual void DrawPathSelectable(std::shared_ptr<PathProgram> toolPath, float selectableWidth);

	virtual void DrawAddPathButton();

	virtual void OpenImportPathDialog();
	virtual void DrawImportPathDialog();

	std::unique_ptr<MillingToolFactory> m_MillingToolFactory;
};
