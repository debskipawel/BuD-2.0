#pragma once

#include "SoundEffect.h"

#include <filesystem>
#include <memory>

namespace BuD::Audio
{
	namespace Internal
	{
		class SoundFileDecoder
		{
		public:
			virtual ~SoundFileDecoder() = default;
			
			virtual std::shared_ptr<SoundEffect> LoadFromPath(std::filesystem::path filepath) = 0;

		protected:
			static void ReadFile(std::filesystem::path filepath, std::vector<uint8_t>& output);
		};
	}
}
