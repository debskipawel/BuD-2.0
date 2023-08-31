#pragma once

#include <vector>

namespace BuD
{
	namespace Audio
	{
		class SoundEffect
		{
		public:
			SoundEffect(std::vector<float> buffer, uint32_t sampleRate, uint32_t sampleCount, uint32_t channelCount, float lengthSeconds);
			SoundEffect(const SoundEffect& other) = delete;
			SoundEffect& operator=(const SoundEffect& other) = delete;
			SoundEffect(SoundEffect&& value) = default;
			
		protected:

			std::vector<float> m_Buffer;
			
			uint32_t m_BitRate;
			uint32_t m_SampleRate;
			uint32_t m_SampleCount;
			uint32_t m_ChannelCount;
			float m_LengthInSeconds;

			friend class AudioSystem;
		};
	}
}
