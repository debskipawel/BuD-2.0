#include "bud_pch.h"
//#include "Cube.h"
//
//#include <Shaders/Loader/ShaderLoader.h>
//#include <Resources/Meshes/Cube.h>
//
//namespace BuD
//{
//	Cube::Cube(Scene& scene)
//        : SceneEntity(scene)
//	{
//        auto& positions = g_cubePositionVertices;
//        auto& indices = g_cubePositionIndices;
//
//        auto renderPass = RenderingPass{};
//
//        renderPass.VertexBuffer = std::make_shared<VertexBuffer>(positions.size() * sizeof(Vector3), g_cubePositionLayout, positions.data());
//        renderPass.IndexBuffer = std::make_shared<IndexBuffer>(DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);
//
//        renderPass.VertexShader = ShaderLoader::VSLoad(L"../x64/Debug/mvp_pos_vs.hlsl", g_cubePositionLayout, { 4 * sizeof(Matrix) });
//        renderPass.PixelShader = ShaderLoader::PSLoad(L"../x64/Debug/solid_white_ps.hlsl");
//
//        renderPass.PreRenderCallback = [this](const RenderingPass& pass)
//        {
//            auto& vs = pass.VertexShader;
//
//            auto model = dxm::Matrix::CreateFromQuaternion(m_Rotation) * dxm::Matrix::CreateTranslation(m_Position);
//            auto view = m_Scene->ActiveCamera()->ViewMatrix();
//            auto proj = Renderer::ProjectionMatrix();
//
//            dxm::Matrix cb[] =
//            {
//                model,
//                view,
//                view.Invert(),
//                proj
//            };
//
//            vs->UpdateConstantBuffer(0, cb, 4 * sizeof(Matrix));
//        };
//
//        m_RenderingPasses.push_back(renderPass);
//	}
//}
