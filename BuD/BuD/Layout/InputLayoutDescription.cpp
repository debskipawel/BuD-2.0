#include "bud_pch.h"
#include "InputLayoutDescription.h"

#include <Utils/HelperFunctions.h>

namespace BuD
{
	InputLayoutDescription::InputLayoutDescription(std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements)
		: m_LayoutElements(layoutElements)
	{
		CalculateAccumulatedByteSize();
	}

	InputLayoutDescription::InputLayoutDescription(std::initializer_list<D3D11_INPUT_ELEMENT_DESC> layoutElements)
		: m_LayoutElements(layoutElements)
	{
		CalculateAccumulatedByteSize();
	}

	bool InputLayoutDescription::operator==(const InputLayoutDescription& other) const
	{
		// order of comparison:
		// 1. accumulated size
		// 2. number of elements
		// 3. all elements 

		if (m_AccumulatedByteSize != other.m_AccumulatedByteSize || m_LayoutElements.size() != other.m_LayoutElements.size())
		{
			return false;
		}

		for (int i = 0; i < m_LayoutElements.size(); i++)
		{
			auto& elementLeft = m_LayoutElements[i];
			auto& elementRight = other.m_LayoutElements[i];

			auto compare = CompareInputElements(elementLeft, elementRight);

			if (compare != 0)
			{
				return false;
			}
		}

		return true;
	}

	bool InputLayoutDescription::operator<(const InputLayoutDescription& other) const
	{
		if (m_AccumulatedByteSize < other.m_AccumulatedByteSize 
			|| (m_AccumulatedByteSize == other.m_AccumulatedByteSize && m_LayoutElements.size() < other.m_LayoutElements.size()))
		{
			return true;
		}

		if (m_AccumulatedByteSize > other.m_AccumulatedByteSize 
			|| (m_AccumulatedByteSize == other.m_AccumulatedByteSize && m_LayoutElements.size() > other.m_LayoutElements.size()))
		{
			return false;
		}

		for (int i = 0; i < m_LayoutElements.size(); i++)
		{
			auto& elementLeft = m_LayoutElements[i];
			auto& elementRight = other.m_LayoutElements[i];

			int compare = CompareInputElements(elementLeft, elementRight);

			if (compare != 0)
			{
				return compare < 0;
			}
		}

		return false;
	}

	bool InputLayoutDescription::operator>(const InputLayoutDescription& other) const
	{
		if (m_AccumulatedByteSize > other.m_AccumulatedByteSize 
			|| (m_AccumulatedByteSize == other.m_AccumulatedByteSize && m_LayoutElements.size() > other.m_LayoutElements.size()))
		{
			return true;
		}

		if (m_AccumulatedByteSize < other.m_AccumulatedByteSize 
			|| (m_AccumulatedByteSize == other.m_AccumulatedByteSize && m_LayoutElements.size() < other.m_LayoutElements.size()))
		{
			return false;
		}

		for (int i = 0; i < m_LayoutElements.size(); i++)
		{
			auto& elementLeft = m_LayoutElements[i];
			auto& elementRight = other.m_LayoutElements[i];

			int compare = CompareInputElements(elementLeft, elementRight);

			if (compare != 0)
			{
				return compare > 0;
			}
		}

		return false;
	}

	bool InputLayoutDescription::operator!=(const InputLayoutDescription& other) const
	{
		return !(*this == other);
	}

	bool InputLayoutDescription::operator<=(const InputLayoutDescription& other) const
	{
		return !(*this > other);
	}

	bool InputLayoutDescription::operator>=(const InputLayoutDescription& other) const
	{
		return !(*this < other);
	}

	void InputLayoutDescription::CalculateAccumulatedByteSize()
	{
		m_AccumulatedByteSize = std::accumulate(
			m_LayoutElements.begin(), m_LayoutElements.end(), (size_t)0,
			[](size_t a, D3D11_INPUT_ELEMENT_DESC desc) { return a + HelperFunctions::BytesPerFormat(desc.Format); }
		);
	}

	int InputLayoutDescription::CompareInputElements(const D3D11_INPUT_ELEMENT_DESC& left, const D3D11_INPUT_ELEMENT_DESC& right) const
	{
		// order of comparison:
		// 1. format
		// 2. name
		// 3. semantic index
		// 4. byte offset
		// 5. input slot
		// 6. input slot class
		// 7. instance data step rate, whatever tf that is

		// 1
		int formatComparison = (int)left.Format - (int)right.Format;
		
		if (formatComparison != 0)
		{
			return formatComparison / abs(formatComparison);
		}

		// 2
		int nameComparison = strcmp(left.SemanticName, right.SemanticName);

		if (nameComparison != 0)
		{
			return nameComparison;
		}

		// 3
		int indexComparison = (int)left.SemanticIndex - (int)right.SemanticIndex;

		if (indexComparison != 0)
		{
			return indexComparison / abs(indexComparison);
		}

		// 4
		int offsetComparison = (int)left.AlignedByteOffset - (int)right.AlignedByteOffset;

		if (offsetComparison != 0)
		{
			return offsetComparison / abs(offsetComparison);
		}

		// 5
		int inputSlotComparison = (int)left.InputSlot - (int)right.InputSlot;

		if (inputSlotComparison != 0)
		{
			return inputSlotComparison / abs(inputSlotComparison);
		}

		// 6
		int inputSlotClassComparison = (int)left.InputSlotClass - (int)right.InputSlotClass;

		if (inputSlotClassComparison != 0)
		{
			return inputSlotClassComparison / abs(inputSlotClassComparison);
		}

		// 7
		int instanceDataStepRateComparison = (int)left.InputSlotClass - (int)right.InputSlotClass;
		
		if (instanceDataStepRateComparison != 0)
		{
			return instanceDataStepRateComparison / abs(instanceDataStepRateComparison);
		}

		return 0;
	}
}
