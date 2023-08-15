#pragma once

#include "AudioDevice.h"
#include "SoundEffect.h"

#include <filesystem>
#include <vector>

namespace BuD
{
	namespace Audio
	{
		class AudioSystem
		{
		public:
			static std::vector<AudioDevice> AllAudioDevices();
			static AudioDevice DefaultAudioDevice();
			
			static void SetActiveDevice(const AudioDevice& device);

			static void Clear();

			static std::shared_ptr<SoundEffect> Load(std::filesystem::path filepath);
			static void Play(std::shared_ptr<SoundEffect> sound);

		private:
			AudioSystem() = delete;

			static void OpenStream();

			inline static AudioDevice s_ActiveDevice = AudioDevice::INVALID;
			inline static void* s_Audio = nullptr;

			inline static uint32_t s_SampleRate = 44100;
		};
	}
}
