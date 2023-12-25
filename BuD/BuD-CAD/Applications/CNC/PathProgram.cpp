#include "PathProgram.h"

#include <Applications/CNC/GCodeCommandPathVisitor.h>

PathProgram::PathProgram(BuD::Scene& scene, std::string name, const GCP::GCodeProgram& program, std::shared_ptr<MillingTool> millingTool)
	: m_PathName(name), m_Program(program), m_Tool(millingTool), m_PathEntity(scene)
{
	auto visitor = GCodeCommandPathVisitor();

	for (const auto& command : m_Program.m_Commands)
	{
		visitor.Visit(*command);
	}

	const auto& vertices = visitor.Points();

	auto indices = std::vector<unsigned short>();

	std::transform(vertices.begin(), vertices.end(), std::back_inserter(indices),
		[&vertices](const dxm::Vector3& vertex)
		{
			auto idx = &vertex - vertices.data();

			return idx;
		}
	);

	auto inputLayoutDesc = std::vector<D3D11_INPUT_ELEMENT_DESC>
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	auto mesh = BuD::MeshDetails();
	mesh.m_VertexBuffer = std::make_shared<BuD::VertexBuffer>(vertices.size() * sizeof(dxm::Vector3), inputLayoutDesc, vertices.data());
	mesh.m_IndexBuffer = std::make_shared<BuD::IndexBuffer>(DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mesh.m_InputLayout = std::make_shared<BuD::InputLayout>(inputLayoutDesc);
	mesh.m_Name = name;

	auto pipeline = BuD::ShaderPipeline();
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Path/path_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Path/path_ps.hlsl");

	auto renderingPass = BuD::RenderingPass();
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_ShouldSkip = true;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;

		dxm::Matrix vsCB[] =
		{
			camera->ViewMatrix(),
			BuD::Renderer::ProjectionMatrix()
		};

		vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>{ renderingPass };

	m_PathEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

auto PathProgram::DisableRendering() -> void
{
	m_Tool->DisableRendering();

	auto& renderable = m_PathEntity.GetComponent<BuD::IRenderable>();
	renderable.RenderingPasses[0].m_ShouldSkip = true;
}

auto PathProgram::EnableRendering() -> void
{
	m_Tool->EnableRendering();

	auto& renderable = m_PathEntity.GetComponent<BuD::IRenderable>();
	renderable.RenderingPasses[0].m_ShouldSkip = false;
}
