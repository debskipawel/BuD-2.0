#pragma once

#include "SoundFileDecoder.h"

namespace BuD::Audio
{
	namespace Internal
	{
		class Mp3FileDecoder : public SoundFileDecoder
		{
			std::shared_ptr<SoundEffect> LoadFromPath(std::filesystem::path filepath) override;
		};
	}
}
