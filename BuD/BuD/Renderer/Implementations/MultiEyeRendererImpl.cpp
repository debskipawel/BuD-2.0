#include "bud_pch.h"
#include "MultiEyeRendererImpl.h"

BuD::MultiEyeRendererImpl::MultiEyeRendererImpl(std::shared_ptr<GraphicsDevice> device)
	: BaseRendererImpl(device)
{
}

dxm::Matrix BuD::MultiEyeRendererImpl::ProjectionMatrix()
{
    if (m_ProjectionMatricesForEyes.contains(m_EyeForCurrentRendering))
    {
        return m_ProjectionMatricesForEyes.at(m_EyeForCurrentRendering);
    }

    return dxm::Matrix::Identity;
}

MultiEyeSettings BuD::MultiEyeRendererImpl::GetMultiEyeSettings() const
{
    return m_MultiEyeSettings;
}

void BuD::MultiEyeRendererImpl::SetMultiEyeSettings(const MultiEyeSettings& settings)
{
    m_MultiEyeSettings = settings;
}

void BuD::MultiEyeRendererImpl::UpdateProjectionMatrices(float aspectRatio, float fov, float nearPlane, float farPlane)
{
	float focusPlane = m_MultiEyeSettings.m_FocusPlane;
	float eyeDistance = m_MultiEyeSettings.m_EyeDistance;
	float halfEyeDistance = eyeDistance * 0.5f;

	float top = focusPlane * tanf(DirectX::XMConvertToRadians(fov * 0.5f));
	float bottom = focusPlane * tanf(-DirectX::XMConvertToRadians(fov * 0.5f));

	float width = (top - bottom) * aspectRatio;

	float L = -0.5f * width;
	float R = 0.5f * width;

	float leftEyeL = (L + halfEyeDistance) * nearPlane / focusPlane;
	float leftEyeR = (R + halfEyeDistance) * nearPlane / focusPlane;

	float rightEyeL = (L - halfEyeDistance) * nearPlane / focusPlane;
	float rightEyeR = (R - halfEyeDistance) * nearPlane / focusPlane;

	top = top * nearPlane / focusPlane;
	bottom = bottom * nearPlane / focusPlane;

	m_ProjectionMatricesForEyes[Eye::LEFT] = dxm::Matrix::CreateTranslation({ halfEyeDistance, 0.0f, 0.0f })
		* dxm::Matrix::CreatePerspectiveOffCenter(leftEyeL, leftEyeR, bottom, top, nearPlane, farPlane);

	m_ProjectionMatricesForEyes[Eye::RIGHT] = dxm::Matrix::CreateTranslation({ -halfEyeDistance, 0.0f, 0.0f })
		* dxm::Matrix::CreatePerspectiveOffCenter(rightEyeL, rightEyeR, bottom, top, nearPlane, farPlane);
}
