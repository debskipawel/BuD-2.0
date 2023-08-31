#pragma once

#include <queue>
#include <vector>

namespace BuD::Audio
{
	namespace Internal
	{
		struct Frame
		{
			float* FramePtr;
			size_t Size;
		};

		class SoundBuffer
		{
		public:
			SoundBuffer() : m_Buffer(), m_ReadEnd(0), m_WriteEnd(0) {}
			~SoundBuffer() = default;

			Frame ReadFrame(size_t frameSize);
			void Write(float* inputBuffer, size_t sampleCount, size_t sampleSize);

		protected:
			std::vector<float> m_Buffer;

			size_t m_ReadEnd, m_WriteEnd;
		};
	}
}
