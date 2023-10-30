#include "CyrusBeckClippingAlgorithm.h"

namespace egc 
{
	int lineClipCyrusBeck(std::vector<dxm::Vector2> clipWindow, dxm::Vector2& p1, dxm::Vector2& p2, float& tE, float& tL)
	{
		std::vector<dxm::Vector2> normal = std::vector<dxm::Vector2>(clipWindow.size());
		std::vector<dxm::Vector2> pei = std::vector<dxm::Vector2>(clipWindow.size());
		dxm::Vector2 D = p2 - p1;
		dxm::Vector3 z = dxm::Vector3(0.0f, 0.0f, 1.0f);
		
		if (p1 == p2) 
		{
			return 0;
		}

		for (int i = 0; i < clipWindow.size(); i++) 
		{
			pei[i] = clipWindow[i];

			auto tangent = clipWindow[(i + 1) % clipWindow.size()] - clipWindow[i];
			auto tangentCastTo3D = dxm::Vector3(tangent.x, tangent.y, 0.0f);

			auto normalIn3D = tangentCastTo3D.Cross(z);

			normal[i] = dxm::Vector2(normalIn3D.x, normalIn3D.y);
		}

		float t = 0.0f; 
		tE = 0.0f, tL = 1.0f;

		for (int i = 0; i < clipWindow.size(); i++) 
		{
			float dotProd = normal[i].Dot(D);
			
			if (dotProd != 0) 
			{
				t = -normal[i].Dot(p1 - pei[i]) / dotProd;
				
				if (dotProd < 0) 
				{
					tE = max(tE, t);
				}
				else 
				{
					tL = min(tL, t);
				}
			}
		}
		
		if (tE > tL) 
		{
			return -1;
		}

		dxm::Vector2 v1 = p1, v2 = p2;

		p1 = v1 + (v2 - v1) * tE;
		p2 = v1 + (v2 - v1) * tL;
		
		return 0;
	}
}