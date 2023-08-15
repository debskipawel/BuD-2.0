#include "AudioSystem.h"

#include "AudioGallery.h"
#include "SoundBuffer.h"

#include <rtaudio-5.2.0/RtAudio.h>

#include <iostream>
#include <memory>

namespace BuD::Audio
{
	static Internal::SoundBuffer s_SoundBuffer;
	static constexpr size_t FRAME_SIZE = 256;

	static int playbackHandle(void* outBuffer, void* inBuffer, uint32_t nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userData)
	{
		auto buffer = static_cast<float*>(outBuffer);

		if (status)
		{
			return status;
		}

		auto input = s_SoundBuffer.ReadFrame(2 * nBufferFrames);

		// Playback
		if (input.FramePtr)
		{
			memcpy(outBuffer, input.FramePtr, input.Size * sizeof(float));
		}

		memset(buffer + input.Size, 0, (2 * nBufferFrames - input.Size) * sizeof(float));

		return 0;
	}

	#define CastRTAudio() reinterpret_cast<RtAudio*>(s_Audio)

	std::vector<AudioDevice> AudioSystem::AllAudioDevices()
	{
		std::unique_ptr<RtAudio> rtAudio = std::unique_ptr<RtAudio>(new RtAudio);

		auto devicesCount = rtAudio->getDeviceCount();
		auto devices = std::vector<AudioDevice>(devicesCount);

		for (int i = 0; i < devicesCount; ++i)
		{
			auto deviceInfo = rtAudio->getDeviceInfo(i);
			auto device = AudioDevice(deviceInfo.name, i, deviceInfo.outputChannels);
			
			devices[i] = device;
		}

		return devices;
	}
	
	AudioDevice AudioSystem::DefaultAudioDevice()
	{
		std::unique_ptr<RtAudio> rtAudio = std::unique_ptr<RtAudio>(new RtAudio);

		auto defaultDeviceId = rtAudio->getDefaultOutputDevice();
		auto deviceInfo = rtAudio->getDeviceInfo(defaultDeviceId);

		auto device = AudioDevice(deviceInfo.name, defaultDeviceId, deviceInfo.outputChannels);

		return device;
	}
	
	void AudioSystem::SetActiveDevice(const AudioDevice& device)
	{
		if (!s_Audio)
		{
			s_Audio = new RtAudio;
		}

		s_ActiveDevice = device;

		OpenStream();
	}
	
	void AudioSystem::Clear()
	{
		if (!s_Audio)
		{
			return;
		}

		auto audio = CastRTAudio();

		if (audio->isStreamOpen())
		{
			audio->closeStream();
		}

		delete audio;

		s_Audio = nullptr;
		s_ActiveDevice = AudioDevice::INVALID;
	}
	
	std::shared_ptr<SoundEffect> AudioSystem::Load(std::filesystem::path filepath)
	{
		return AudioGallery::Load(filepath);
	}
	
	void AudioSystem::Play(std::shared_ptr<SoundEffect> sound)
	{
		if (!s_Audio || s_ActiveDevice.Id() == AudioDevice::INVALID.Id())
		{
			std::cerr << "[ERROR] No default playback device has been set." << std::endl;
			return;
		}

		if (s_SampleRate != sound->m_SampleRate)
		{
			s_SampleRate = sound->m_SampleRate;

			OpenStream();
		}

		s_SoundBuffer.Write(sound->m_Buffer.data(), sound->m_SampleCount, sound->m_SampleRate);
	}
	
	void AudioSystem::OpenStream()
	{
		unsigned int frameSize = FRAME_SIZE;
		RtAudio::StreamParameters outputParams{ s_ActiveDevice.Id(), s_ActiveDevice.NumChannels(), 0 };

		auto rtAudio = CastRTAudio();

		if (rtAudio->isStreamOpen())
		{
			rtAudio->closeStream();
		}

		rtAudio->openStream(&outputParams, nullptr, RTAUDIO_FLOAT32, s_SampleRate, &frameSize, &playbackHandle, nullptr);
		rtAudio->startStream();
	}
}
