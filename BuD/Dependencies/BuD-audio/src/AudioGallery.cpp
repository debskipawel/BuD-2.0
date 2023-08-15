#include "AudioGallery.h"

#include "WavpackFileDecoder.h"
#include "Mp3FileDecoder.h"

namespace BuD::Audio
{
	std::filesystem::path MP3_EXTENSION = ".mp3";
	std::filesystem::path WAV_EXTENSION = ".wav";
	std::filesystem::path WAVPACK_EXTENSION = ".wv";
	std::filesystem::path FLAC_EXTENSION = ".flac";

	std::shared_ptr<SoundEffect> AudioGallery::Load(std::filesystem::path filepath)
	{
		std::shared_ptr<Internal::SoundFileDecoder> decoder = nullptr;

		auto filename = filepath.filename();
		auto extension = filepath.extension();

		if (extension == MP3_EXTENSION)
		{
			decoder = std::shared_ptr<Internal::SoundFileDecoder>(new Internal::Mp3FileDecoder);
		}
		else if (extension == WAVPACK_EXTENSION)
		{
			decoder = std::shared_ptr<Internal::SoundFileDecoder>(new Internal::WavpackFileDecoder);
		}

		auto soundFile = decoder->LoadFromPath(filepath);

		s_SoundGallery.insert(std::make_pair(filepath, soundFile));

		return soundFile;
	}
}
