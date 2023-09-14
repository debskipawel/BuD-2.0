#include "Torus.h"

#include <Visitors/AbstractVisitor.h>

Torus::Torus(BuD::Scene& scene)
	: Torus(scene, dxm::Vector3::Zero)
{
}

Torus::Torus(BuD::Scene& scene, dxm::Vector3 position)
	: Torus(scene, position, 3.0f, 1.0f)
{
}

Torus::Torus(BuD::Scene& scene, dxm::Vector3 position, float outerRadius, float innerRadius)
	: Torus(scene, position, outerRadius, innerRadius, 15, 30)
{
}

Torus::Torus(BuD::Scene& scene, dxm::Vector3 position, float outerRadius, float innerRadius, unsigned int segmentsU, unsigned int segmentsV)
	: ParameterizedObject2D(scene)
{
	m_Tag = std::format("Torus {}", m_SceneEntity.Id());

	m_Transform.m_Position = position;
	m_InstanceData.m_ModelMatrix = dxm::Matrix::CreateTranslation(m_Transform.m_Position);

	m_InstanceData.m_InnerRadius = innerRadius;
	m_InstanceData.m_OuterRadius = outerRadius;
	m_InstanceData.m_SegmentsU = segmentsU;
	m_InstanceData.m_SegmentsV = segmentsV;

	m_InstanceData.m_Color = dxm::Vector3::One;

	CreateInstancedRenderingPass();
	CreateTrimmedRenderingPass();

	std::vector<BuD::RenderingPass> passes = { m_InstancedRenderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(passes);
}

void Torus::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void Torus::AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	ParameterizedObject2D::AddIntersectionCurve(intersectionCurve);

	if (!m_IntersectionCurves.empty())
	{
		auto& renderingComponent = m_SceneEntity.GetComponent<BuD::IRenderable>();
		renderingComponent.RenderingPasses[0] = m_TrimmedRenderingPass;
	}
}

void Torus::RemoveIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	ParameterizedObject2D::RemoveIntersectionCurve(intersectionCurve);

	if (m_IntersectionCurves.empty())
	{
		auto& renderingComponent = m_SceneEntity.GetComponent<BuD::IRenderable>();
		renderingComponent.RenderingPasses[0] = m_InstancedRenderingPass;
	}
}

void Torus::CreateInstancedRenderingPass()
{
	auto meshLoader = BuD::MeshLoader();

	auto pointMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_TESSELLATION,
		{
			{ "INS_MODEL_X", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_Y", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 4 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_MODEL_Z", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 8 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_RADIUS", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 16 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_SEGMENTS", 0, DXGI_FORMAT_R32G32_UINT, 1, 18 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 20 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);
	pointMesh.m_BoundingBoxCallback = [this]()
	{
		BuD::AABB aabb;

		auto scale = m_Transform.m_Scale;
		auto maxScale = max(max(scale.x, scale.y), scale.z);
		auto position = m_Transform.m_Position;
		aabb.m_Max = position + maxScale * dxm::Vector3(m_InstanceData.m_OuterRadius + m_InstanceData.m_InnerRadius);
		aabb.m_Min = position - maxScale * dxm::Vector3(m_InstanceData.m_OuterRadius + m_InstanceData.m_InnerRadius);

		return aabb;
	};

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Torus/torus_instanced_vs.hlsl");
	pipeline.m_HullShader = BuD::ShaderLoader::HSLoad("Resources/Shaders/Torus/torus_instanced_hs.hlsl");
	pipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("Resources/Shaders/Torus/torus_instanced_ds.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Torus/torus_instanced_ps.hlsl");

	BuD::InstanceComponent instancing;
	instancing.m_InstanceCallback = [this]()
	{
		auto instance = BuD::InstanceData();
		instance.m_Data = (uint8_t*)(&m_InstanceData);
		instance.m_Size = sizeof(TorusInstanceData);

		return instance;
	};

	m_InstancedRenderingPass = {};
	m_InstancedRenderingPass.m_Mesh = pointMesh;
	m_InstancedRenderingPass.m_Pipeline = pipeline;
	m_InstancedRenderingPass.m_Instancing = instancing;

	m_InstancedRenderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::WIREFRAME;
	m_InstancedRenderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;

	m_InstancedRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		dxm::Matrix matrices[] = { camera->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		ds->UpdateConstantBuffer(0, matrices, 2 * sizeof(dxm::Matrix));
	};
}

void Torus::CreateTrimmedRenderingPass()
{
	auto meshLoader = BuD::MeshLoader();

	auto pointMesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::POINT_TESSELLATION);
	pointMesh.m_BoundingBoxCallback = [this]()
	{
		BuD::AABB aabb;

		auto scale = m_Transform.m_Scale;
		auto maxScale = max(max(scale.x, scale.y), scale.z);
		auto position = m_Transform.m_Position;
		aabb.m_Max = position + maxScale * dxm::Vector3(m_InstanceData.m_OuterRadius + m_InstanceData.m_InnerRadius);
		aabb.m_Min = position - maxScale * dxm::Vector3(m_InstanceData.m_OuterRadius + m_InstanceData.m_InnerRadius);

		return aabb;
	};

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Torus/Trimmed/torus_vs.hlsl");
	pipeline.m_HullShader = BuD::ShaderLoader::HSLoad("Resources/Shaders/Torus/Trimmed/torus_hs.hlsl", { sizeof(TorusInstanceData) });
	pipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("Resources/Shaders/Torus/Trimmed/torus_ds.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Torus/Trimmed/torus_ps.hlsl");

	m_TrimmedRenderingPass = {};
	m_TrimmedRenderingPass.m_Mesh = pointMesh;
	m_TrimmedRenderingPass.m_Pipeline = pipeline;
	m_TrimmedRenderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::WIREFRAME;
	m_TrimmedRenderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;

	m_TrimmedRenderingPass.m_PreRenderCallback = [this](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto device = BuD::Renderer::Device();
		auto context = device->Context();
		auto camera = scene.ActiveCamera();

		dxm::Matrix matrices[] = { camera->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };
		
		auto& hs = renderingPass.m_Pipeline.m_HullShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		hs->UpdateConstantBuffer(0, &m_InstanceData, sizeof(TorusInstanceData));
		ds->UpdateConstantBuffer(0, matrices, 2 * sizeof(dxm::Matrix));

		ID3D11ShaderResourceView* SRVs[1] = { m_ParameterSpace.has_value() ? m_ParameterSpace->SRV() : nullptr };

		context->PSSetShaderResources(0, 1, SRVs);
	};
}

dxm::Vector3 Torus::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 Torus::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
