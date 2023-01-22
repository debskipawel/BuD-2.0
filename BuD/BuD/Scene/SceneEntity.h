#pragma once

#include <Utils/UUID.h>

#include <string>

namespace BuD
{
	class SceneEntity
	{
	public:
		uint64_t Id() const { return m_Id; }

		template <typename T>
		bool Implements() const { return dynamic_cast<T*>(this); }

		template <typename T>
		T* Component() { return dynamic_cast<T*>(this); }

	protected:
		UUID m_Id;

	public:
		std::string m_Tag;
	};
}
