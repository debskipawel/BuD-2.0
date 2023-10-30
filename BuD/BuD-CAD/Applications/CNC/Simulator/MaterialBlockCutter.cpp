#include "MaterialBlockCutter.h"

#include "Algorithms/CyrusBeckClippingAlgorithm.h"
#include "Algorithms/BresenhamAlgorithm.h"

#include <numbers>

MaterialBlockCutter::MaterialBlockCutter(MaterialBlock& materialBlock)
	: m_MaterialBlock(materialBlock)
{
}

void MaterialBlockCutter::MoveMillingTool(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition)
{
	auto radius = millingTool->Radius();
	auto startPosition = millingTool->Position();

	auto worldUp = dxm::Vector3::UnitY;

	auto direction = endPosition - startPosition;
	auto orthogonal = direction.Cross(worldUp);
	
	// edge case - vertical move (not validated yet)
	if (orthogonal.LengthSquared() < 1e-4)
	{
		MoveMillingToolVertically(millingTool, endPosition);

		millingTool->MoveTo(endPosition);

		return;
	}
	
	orthogonal.Normalize();

	auto clippingResult = ClipMillingToolLine(startPosition, endPosition, millingTool->Radius());

	if (!clippingResult.has_value())
	{
		millingTool->MoveTo(endPosition);

		return;
	}

	auto& [betterStartPosition, betterEndPosition] = clippingResult.value();

	auto A = MapWorldSpaceToPixelSpace(betterStartPosition);
	auto B = MapWorldSpaceToPixelSpace(betterEndPosition);

	auto pixelDiff = B - A;

	auto A1P = MapWorldSpaceToPixelSpace(betterStartPosition - dxm::Vector3(radius, 0.0f, radius));
	auto A2P = MapWorldSpaceToPixelSpace(betterStartPosition + dxm::Vector3(radius, 0.0f, radius));

	auto xMin = min(A1P.x, A2P.x), xMax = max(A1P.x, A2P.x);
	auto zMin = min(A1P.y, A2P.y), zMax = max(A1P.y, A2P.y);

	auto materialSize = m_MaterialBlock.Size();
	auto heightMap = m_MaterialBlock.HeightMap();
	
	auto errorMargin = 2.0f * std::numbers::sqrt2_v<float> * max(materialSize.x / heightMap->Width(), materialSize.z / heightMap->Height());

	heightMap->BeginEdit();

	auto maxIterations = 0.0;
	auto iterationsNotSkipped = 0.0f;

	for (auto dx = 0; dx < xMax - xMin; dx++)
	{
		for (auto dz = 0; dz < zMax - zMin; dz++)
		{
			maxIterations++;

			auto millingToolCenter = millingTool->CenterPoint();
			
			auto pixelInWorldSpace = MapPixelToWorldSpace(xMin + dx, zMin + dz);
			auto startToPixel = pixelInWorldSpace - startPosition;

			auto localHeight = millingTool->LocalHeight(startToPixel.x, startToPixel.z);
			auto startHeight = startPosition.y + localHeight, endHeight = endPosition.y + localHeight;

			startToPixel.y = startHeight - millingToolCenter.y;

			auto centerDistanceSq = startToPixel.LengthSquared();
			auto radiusSq = radius * radius;

			// we only consider points sufficiently close to the border
			if (centerDistanceSq - radiusSq > 0.0f || centerDistanceSq - radiusSq < -errorMargin)
			{
				continue;
			}

			auto localNormal = millingTool->LocalNormal(startToPixel.x, startToPixel.z);

			if (direction.Dot(localNormal) * direction.Dot(startToPixel) > 0.0f)
			{
				continue;
			}

			iterationsNotSkipped++;

			auto xStart = xMin + dx, xEnd = xStart + pixelDiff.x;
			auto zStart = zMin + dz, zEnd = zStart + pixelDiff.y;

			bresenhamAlgorithm(xStart, zStart, xEnd, zEnd,
				[startHeight, endHeight, heightMap](int x, int y, float t)
				{
					if (x < 0 || x >= heightMap->Width() || y < 0 || y >= heightMap->Height())
					{
						return;
					}

					auto color = heightMap->Sample(x, y);

					auto currentHeight = color.x;
					auto toolPointHeight = startHeight + t * (endHeight - startHeight);

					if (toolPointHeight < currentHeight)
					{
						heightMap->PutPixel(x, y, { toolPointHeight, toolPointHeight, toolPointHeight, toolPointHeight });
					}
				}
			);
		}
	}

	auto percentNotSkipped = 100.0f * iterationsNotSkipped / maxIterations;

	heightMap->EndEdit();

	millingTool->MoveTo(endPosition);
}

std::optional<std::pair<dxm::Vector3, dxm::Vector3>> MaterialBlockCutter::ClipMillingToolLine(const dxm::Vector3& start, const dxm::Vector3& end, float radius)
{
	auto worldUp = dxm::Vector3::UnitY;
	auto worldDirection = end - start;

	auto tangent = worldDirection.Cross(worldUp);

	if (tangent.LengthSquared() < 1e-3f)
	{
		return std::nullopt;
	}

	tangent.Normalize();

	auto leftStart = MapWorldSpaceToLocalMaterialSpace(start + radius * tangent);
	auto rightStart = MapWorldSpaceToLocalMaterialSpace(start - radius * tangent);
	
	auto leftEnd = MapWorldSpaceToLocalMaterialSpace(end + radius * tangent);
	auto rightEnd = MapWorldSpaceToLocalMaterialSpace(end - radius * tangent);

	const auto clipWindow = std::vector<dxm::Vector2>{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	auto leftStartPlane = dxm::Vector2(leftStart.x, leftStart.z);
	auto leftEndPlane = dxm::Vector2(leftEnd.x, leftEnd.z);

	auto rightStartPlane = dxm::Vector2(rightStart.x, rightStart.z);
	auto rightEndPlane = dxm::Vector2(rightEnd.x, rightEnd.z);

	float tE1, tE2, tL1, tL2;

	auto clipResultLeft = egc::lineClipCyrusBeck(clipWindow, leftStartPlane, leftEndPlane, tE1, tL1);
	auto clipResultRight = egc::lineClipCyrusBeck(clipWindow, rightStartPlane, rightEndPlane, tE2, tL2);

	if (clipResultLeft == -1 && clipResultRight == -1)
	{
		return std::nullopt;
	}

	auto tE = min(
		clipResultLeft == -1 ? 1000.0f : tE1,
		clipResultRight == -1 ? 1000.0f : tE2
	);

	auto tL = max(
		clipResultLeft == -1 ? -1000.0f : tL1,
		clipResultRight == -1 ? -1000.0f : tL2
	);

	return std::make_pair(start + tE * worldDirection, start + tL * worldDirection);
}

void MaterialBlockCutter::MoveMillingToolVertically(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition)
{
	auto radius = millingTool->Radius();
	auto heightMap = m_MaterialBlock.HeightMap();

	auto pixelWidth = heightMap->Width(), pixelHeight = heightMap->Height();

	auto minPoint = endPosition - dxm::Vector3(radius, 0.0f, radius);
	auto maxPoint = endPosition + dxm::Vector3(radius, 0.0f, radius);

	auto minPointPixel = MapWorldSpaceToPixelSpace(minPoint);
	auto maxPointPixel = MapWorldSpaceToPixelSpace(maxPoint);

	auto minPixelX = std::clamp(static_cast<int>(minPointPixel.x), 0, static_cast<int>(pixelWidth - 1));
	auto maxPixelX = std::clamp(static_cast<int>(maxPointPixel.x), 0, static_cast<int>(pixelWidth - 1));
	auto minPixelZ = std::clamp(static_cast<int>(minPointPixel.x), 0, static_cast<int>(pixelHeight - 1));
	auto maxPixelZ = std::clamp(static_cast<int>(maxPointPixel.x), 0, static_cast<int>(pixelHeight - 1));

	heightMap->BeginEdit();

	for (auto x = min(minPixelX, maxPixelX); x <= max(minPixelX, maxPixelX); ++x)
	{
		for (auto z = min(minPixelZ, maxPixelZ); z <= max(minPixelZ, maxPixelZ); ++z)
		{
			auto pixel = MapPixelToWorldSpace(x, z);
			auto toPixel = pixel - endPosition;
			
			toPixel.y = 0.0f;

			if (toPixel.LengthSquared() > radius * radius)
			{
				continue;
			}

			auto color = heightMap->Sample(x, z);

			auto localHeight = millingTool->LocalHeight(toPixel.x, toPixel.z);
			
			auto previousHeight = color.x;
			auto resultHeight = min(endPosition.y + localHeight, previousHeight);

			heightMap->PutPixel(x, z, { resultHeight, resultHeight, resultHeight, resultHeight });
		}
	}

	heightMap->EndEdit();
}

dxm::Vector3 MaterialBlockCutter::MapPixelToWorldSpace(uint32_t x, uint32_t y)
{
	auto heightMap = m_MaterialBlock.HeightMap();

	auto pixelWidth = heightMap->Width(), pixelHeight = heightMap->Height();

	auto materialPosition = m_MaterialBlock.Position();
	auto materialSize = m_MaterialBlock.Size();

	auto xLocal = -1.0f + 2.0f * static_cast<float>(x) / pixelWidth;
	auto zLocal = -1.0f + 2.0f * static_cast<float>(y) / pixelHeight;

	auto xWorld = 0.5f * materialSize.x * xLocal + materialPosition.x;
	auto zWorld = 0.5f * materialSize.z * zLocal + materialPosition.z;

	return dxm::Vector3(xWorld, materialPosition.y, zWorld);
}

dxm::Vector2 MaterialBlockCutter::MapWorldSpaceToPixelSpace(const dxm::Vector3& p)
{
	auto localMaterialPoint = MapWorldSpaceToLocalMaterialSpace(p);
	auto pixelPoint = MapLocalMaterialSpaceToPixelSpace(localMaterialPoint);

	return pixelPoint;
}

dxm::Vector3 MaterialBlockCutter::MapWorldSpaceToLocalMaterialSpace(const dxm::Vector3& p)
{
	auto materialPosition = m_MaterialBlock.Position();
	auto materialSize = m_MaterialBlock.Size();

	auto x = p.x, z = p.z;

	auto xLocal = 0.5f + (x - materialPosition.x) / materialSize.x;
	auto zLocal = 0.5f + (z - materialPosition.z) / materialSize.z;

	return dxm::Vector3(xLocal, p.y, zLocal);
}

dxm::Vector2 MaterialBlockCutter::MapLocalMaterialSpaceToPixelSpace(const dxm::Vector3& p)
{
	auto heightMap = m_MaterialBlock.HeightMap();
	auto pixelWidth = heightMap->Width(), pixelHeight = heightMap->Height();

	auto result = dxm::Vector2(p.x * pixelWidth, p.z * pixelHeight);

	return result;
}
