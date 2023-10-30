#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlock.h>

class MaterialBlockCutter
{
public:
	MaterialBlockCutter(MaterialBlock& materialBlock);

	virtual void MoveMillingTool(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition);

protected:
	virtual std::optional<std::pair<dxm::Vector3, dxm::Vector3>> ClipMillingToolLine(const dxm::Vector3& start, const dxm::Vector3& end, float radius);

	virtual void MoveMillingToolVertically(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition);

	virtual void CutLine(int x1, int z1, float h1, int x2, int z2, float h2);
	virtual void CutLineLow(int x1, int z1, float h1, int x2, int z2, float h2);
	virtual void CutLineHigh(int x1, int z1, float h1, int x2, int z2, float h2);

	virtual dxm::Vector3 MapPixelToWorldSpace(uint32_t x, uint32_t y);
	virtual dxm::Vector2 MapWorldSpaceToPixelSpace(const dxm::Vector3& p);

	virtual dxm::Vector3 MapWorldSpaceToLocalMaterialSpace(const dxm::Vector3& p);
	virtual dxm::Vector2 MapLocalMaterialSpaceToPixelSpace(const dxm::Vector3& p);

	MaterialBlock& m_MaterialBlock;
};
