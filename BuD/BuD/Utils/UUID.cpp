#include "bud_pch.h"
#include "UUID.h"

namespace BuD
{
	UUID::UUID()
	{
		if (!m_FreedIds.empty())
		{
			m_Id = m_FreedIds.top();
			m_ReferenceCount[m_Id]++;

			m_FreedIds.pop();

			return;
		}
		
		m_Id = m_NextAvailableId++;

		if (m_ReferenceCount.size() <= m_NextAvailableId)
		{
			m_ReferenceCount.resize(m_NextAvailableId * 2);
		}

		m_ReferenceCount[m_Id] = 1;
	}
	
	UUID::UUID(const UUID& other)
	{
		m_ReferenceCount[m_Id]--;

		if (m_ReferenceCount[m_Id] == 0)
		{
			m_FreedIds.push(m_Id);
		}

		m_Id = other.m_Id;
		m_ReferenceCount[m_Id]++;
	}
	
	void UUID::operator=(const UUID& other)
	{
		m_ReferenceCount[m_Id]--;

		if (m_ReferenceCount[m_Id] == 0)
		{
			m_FreedIds.push(m_Id);
		}

		m_Id = other.m_Id;
		m_ReferenceCount[m_Id]++;
	}
	
	UUID::~UUID()
	{
		m_ReferenceCount[m_Id]--;

		if (m_ReferenceCount[m_Id] == 0)
		{
			m_FreedIds.push(m_Id);
		}
	}
	
	bool UUID::operator==(const UUID& other)
	{
		return m_Id == other.m_Id;
	}
	
	bool UUID::operator!=(const UUID& other)
	{
		return !(*this == other);
	}
}
