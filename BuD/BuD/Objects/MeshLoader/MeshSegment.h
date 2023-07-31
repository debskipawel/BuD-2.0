#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <string>

#include <Objects/MeshDetails.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	struct MeshMaterial
	{
		dxm::Vector3 AmbientColor;
		dxm::Vector3 DiffuseColor;
		dxm::Vector3 SpecularColor;

		float SpecularExponent;
		float OpticalDensity;
		float Dissolve;
		int Illumination;
	};

	struct MeshSegment
	{
		MeshDetails m_Details;
		MeshMaterial m_Material;
	};
}
