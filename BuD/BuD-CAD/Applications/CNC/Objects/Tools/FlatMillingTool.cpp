#include "FlatMillingTool.h"

#include <numbers>

#include <Applications/CNC/Objects/Tools/Visitors/MillingToolVisitor.h>

FlatMillingTool::FlatMillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters)
	: MillingTool(scene, millingToolParameters)
{
	auto meshLoader = BuD::MeshLoader();
	auto meshSegments = meshLoader.LoadMeshFromFile("Resources/Meshes/flat_tool_r1_h4.obj");

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/MillingTool/Flat/flatTool_vs.hlsl", { 3 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/MillingTool/Flat/flatTool_ps.hlsl");

	std::vector<BuD::RenderingPass> renderingPasses; 
	renderingPasses.reserve(meshSegments.size());

	constexpr float BASE_MESH_MILLING_TOOL_HEIGHT = 4.0f;

	for (auto& segment : meshSegments)
	{
		auto renderingPass = BuD::RenderingPass();
		renderingPass.m_Mesh = segment.m_Details;
		renderingPass.m_Pipeline = shaderPipeline;

		renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;
		renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::FRONT;

		renderingPass.m_PreRenderCallback = [this](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
		{
			auto modelMatrix = dxm::Matrix::CreateScale(m_Parameters.m_Radius, m_Parameters.m_Height / BASE_MESH_MILLING_TOOL_HEIGHT, m_Parameters.m_Radius) 
				* dxm::Matrix::CreateTranslation(m_Position);
			auto viewMatrix = scene.ActiveCamera()->ViewMatrix();
			auto projMatrix = BuD::Renderer::ProjectionMatrix();

			dxm::Matrix cbVS[] = { modelMatrix, viewMatrix, projMatrix };

			auto& vs = renderingPass.m_Pipeline.m_VertexShader;
			vs->UpdateConstantBuffer(0, cbVS, 3 * sizeof(dxm::Matrix));
		};

		renderingPasses.push_back(renderingPass);
	}

	m_MillEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void FlatMillingTool::Accept(MillingToolVisitor& visitor)
{
	visitor.Visit(*this);
}

void FlatMillingTool::EnableRendering()
{
	auto& renderableComponent = m_MillEntity.GetComponent<BuD::IRenderable>();

	for (auto& renderingPass : renderableComponent.RenderingPasses)
	{
		renderingPass.m_ShouldSkip = false;
	}
}

void FlatMillingTool::DisableRendering()
{
	auto& renderableComponent = m_MillEntity.GetComponent<BuD::IRenderable>();

	for (auto& renderingPass : renderableComponent.RenderingPasses)
	{
		renderingPass.m_ShouldSkip = true;
	}
}

dxm::Vector3 FlatMillingTool::CenterPoint() const
{
	return m_Position;
}

dxm::Vector3 FlatMillingTool::GetCuttingPointInDirection(float x, float y, dxm::Vector3 direction)
{
	auto directionOnXZ = dxm::Vector3(direction.x, 0.0f, direction.z);
	auto worldUp = dxm::Vector3::UnitY;
	
	auto pointOnDiameter = dxm::Vector3(x, LocalHeight(x, y), y);

	if (direction.LengthSquared() < 1e-3f || directionOnXZ.LengthSquared() < 1e-3f)
	{
		return pointOnDiameter;
	}

	direction.Normalize();

	auto radius = Radius();
	
	auto shiftSq = std::fmaxf(radius * radius - (x * x + y * y), 0.0f);
	auto shift = std::sqrtf(shiftSq);
	auto scale = (worldUp.Dot(direction) >= 0 ? 1.0f : -1.0f);

	auto newPoint = pointOnDiameter + scale * shift * direction;

	return newPoint;
}

float FlatMillingTool::LocalHeight(float x, float y)
{
	if (x * x + y * y > m_Parameters.m_Radius * m_Parameters.m_Radius)
	{
		return INFINITY;
	}

	return 0.0f;
}

dxm::Vector3 FlatMillingTool::LocalNormal(float x, float y)
{
	if (x * x + y * y > m_Parameters.m_Radius * m_Parameters.m_Radius)
	{
		return dxm::Vector3::Zero;
	}

	return { 0.0f, -1.0f, 0.0f };
}