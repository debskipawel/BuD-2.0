#include "bud_pch.h"

#pragma once

#ifdef _DEBUG
#include <filesystem>
#include <shlobj.h>

static std::wstring GetLatestWinPixGpuCapturerPath()
{
    LPWSTR programFilesPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

    std::filesystem::path pixInstallationPath = programFilesPath;
    pixInstallationPath /= "Microsoft PIX";

    if (!std::filesystem::exists(pixInstallationPath))
    {
        return std::wstring();
    }

    std::wstring newestVersionFound;

    for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
    {
        if (directory_entry.is_directory())
        {
            if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
            {
                newestVersionFound = directory_entry.path().filename().c_str();
            }
        }
    }

    if (newestVersionFound.empty())
    {
        return std::wstring();
    }

    return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
}

static void LoadPixDebugLayer()
{
    // Check to see if a copy of WinPixGpuCapturer.dll has already been injected into the application.
    // This may happen if the application is launched through the PIX UI. 
    if (GetModuleHandle(L"WinPixGpuCapturer.dll") != 0)
    {
        return;
    }

    auto pixPath = GetLatestWinPixGpuCapturerPath();

    if (!pixPath.empty())
    {
        LoadLibrary(pixPath.c_str());
    }
}

#endif
