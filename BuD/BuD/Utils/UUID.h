#pragma once

#include <cstdint>
#include <stack>
#include <vector>

namespace BuD
{
	class UUID
	{
	public:
		UUID();
		UUID(const UUID& other);
		UUID(UUID&& other) = default;
		void operator=(const UUID& other);

		~UUID();

		bool operator==(const UUID& other);
		bool operator!=(const UUID& other);

		operator uint64_t() const { return m_Id; }

	private:
		uint64_t m_Id;

		inline static uint64_t m_NextAvailableId = 0;
		inline static std::stack<uint64_t> m_FreedIds = {};
		inline static std::vector<int> m_ReferenceCount = {};
	};
}
