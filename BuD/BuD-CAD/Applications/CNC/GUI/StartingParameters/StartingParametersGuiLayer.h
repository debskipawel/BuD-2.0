#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

class StartingParametersGuiLayer : public BaseGuiLayerCNC
{
public:
	StartingParametersGuiLayer(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual void DrawResolutionComboBox();
	virtual void DrawSizeControls();
	virtual void DrawResetSizeButton();

	std::vector<uint32_t> m_AvailableResolutions;
	uint32_t m_ResolutionSelectedIndex = 0;

	MaterialBlockParameters m_BlockParameters;
};
