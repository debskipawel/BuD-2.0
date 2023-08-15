#pragma once

#include <cstdint>
#include <string>

namespace BuD
{
	namespace Audio
	{
		class AudioDevice
		{
		public:
			AudioDevice() : AudioDevice("", -1, -1) {}

			std::string Name() const { return m_Name; }

			uint32_t Id() const { return m_Id; }
			uint32_t NumChannels() const { return m_NumChannels; }

			bool operator==(const AudioDevice& other)
			{
				return m_Id == other.m_Id;
			}

		protected:
			std::string m_Name;

			uint32_t m_Id;
			uint32_t m_NumChannels;

			static AudioDevice INVALID;

			AudioDevice(std::string name, uint32_t id, uint32_t numChannels)
				: m_Name(name), m_Id(id), m_NumChannels(numChannels)
			{}

			friend class AudioSystem;
		};
	}
}
