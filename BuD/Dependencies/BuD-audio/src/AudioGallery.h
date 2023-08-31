#pragma once

#include "SoundEffect.h"

#include <filesystem>
#include <map>
#include <memory>

namespace BuD
{
	namespace Audio
	{
		class AudioGallery
		{
		public:
			static std::shared_ptr<SoundEffect> Load(std::filesystem::path filepath);

		private:
			inline static std::map<std::filesystem::path, std::shared_ptr<SoundEffect>> s_SoundGallery = {};
		};
	}
}
