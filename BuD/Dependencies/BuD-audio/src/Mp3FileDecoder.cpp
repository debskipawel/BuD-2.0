#include "Mp3FileDecoder.h"

#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_IMPLEMENTATION
#include "minimp3/minimp3.h"
#include "minimp3/minimp3_ex.h"

#include <cstdlib>
#include <cstring>

namespace BuD::Audio::Internal
{
	std::shared_ptr<SoundEffect> Mp3FileDecoder::LoadFromPath(std::filesystem::path filepath)
	{
		std::vector<uint8_t> fileBuffer;
		SoundFileDecoder::ReadFile(filepath, fileBuffer);

		if (fileBuffer.size() < 64)
		{
			return nullptr;
		}

		mp3dec_t mp3d;
		mp3dec_file_info_t info;
		mp3dec_load_buf(&mp3d, fileBuffer.data(), fileBuffer.size(), &info, 0, 0);

		auto sampleRate = info.hz;
		auto channelCount = info.channels;
		auto lengthInSeconds = ((float)info.samples / channelCount) / sampleRate;
		auto samples = std::vector<float>(info.samples);
		
		memcpy(samples.data(), info.buffer, sizeof(float) * info.samples);
		std::free(info.buffer);

		auto soundEffect = std::shared_ptr<SoundEffect>(
			new SoundEffect(samples, sampleRate, info.samples, channelCount, lengthInSeconds)
		);

		return soundEffect;
	}
}
