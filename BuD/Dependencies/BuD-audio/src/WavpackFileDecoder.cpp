#include "WavpackFileDecoder.h"

#include <wavpack\include\wavpack.h>

namespace BuD::Audio::Internal
{
	std::shared_ptr<SoundEffect> WavpackFileDecoder::LoadFromPath(std::filesystem::path filepath)
	{
		char wavPackError[80] = "";
		auto absolutePath = std::filesystem::absolute(filepath);
		auto wvContext = WavpackOpenFileInput(absolutePath.string().c_str(), wavPackError, 0, 0);

		if (!wvContext)
		{
			return nullptr;
		}

		auto channelCount = WavpackGetNumChannels(wvContext);
		auto sampleCount = WavpackGetNumSamples(wvContext);
		auto sampleRate = WavpackGetSampleRate(wvContext);

		auto lengthInSeconds = static_cast<float>(sampleCount) / sampleRate;

		std::vector<float> samples(channelCount * sampleCount);
		auto result = WavpackUnpackSamples(wvContext, reinterpret_cast<int32_t*>(samples.data()), sampleCount);

		WavpackCloseFile(wvContext);

		auto soundEffect = std::make_shared<SoundEffect>(samples, sampleRate, sampleCount, channelCount, lengthInSeconds);

		return soundEffect;
	}
}
