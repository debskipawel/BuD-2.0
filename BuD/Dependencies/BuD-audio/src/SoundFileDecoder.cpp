#include "SoundFileDecoder.h"

namespace BuD::Audio::Internal
{
	void SoundFileDecoder::ReadFile(std::filesystem::path filepath, std::vector<uint8_t>& output)
	{
        auto wstringFilepath = filepath.generic_wstring();
        FILE* audioFile;
        auto res = _wfopen_s(&audioFile, wstringFilepath.c_str(), L"rb");

        if (!audioFile)
        {
            return;
        }

        fseek(audioFile, 0, SEEK_END);
        size_t lengthInBytes = ftell(audioFile);
        fseek(audioFile, 0, SEEK_SET);

        output.resize(lengthInBytes);
        size_t elementsRead = fread(output.data(), 1, lengthInBytes, audioFile);
	}
}
