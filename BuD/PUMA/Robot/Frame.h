#pragma once

#include <BuD.h>

class Frame
{
public:
	Frame(const dxm::Vector3& position = dxm::Vector3::Zero, const dxm::Vector3& front = -dxm::Vector3::UnitZ, const dxm::Vector3& up = dxm::Vector3::UnitY);

	virtual auto Translate(const dxm::Vector3& translation) const -> Frame;
	virtual auto Rotate(const dxm::Matrix& rotationMatrix) const -> Frame;

	virtual auto Front() const -> dxm::Vector3;
	virtual auto Up() const -> dxm::Vector3;
	virtual auto Right() const -> dxm::Vector3;

	virtual auto Position() const -> dxm::Vector3;

protected:
	dxm::Vector3 m_Position;

	dxm::Vector3 m_Front;
	dxm::Vector3 m_Up;
};
