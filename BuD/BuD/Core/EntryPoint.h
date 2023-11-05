#pragma once
#include <bud_pch.h>

#include <Core/ApplicationCore.h>

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
)
{
    auto app = BuD::ApplicationCore::Get();
    app->Run();

    BuD::ApplicationCore::Close();

    return 0;
}