#include <bud_pch.h>

#include <Core/ApplicationCore.h>

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
)
{
    try
    {
        auto app = BuD::ApplicationCore::Get();
        app->Run();

        BuD::ApplicationCore::Close();
    }
    catch (std::exception e)
    {
        auto& logs = BuD::Log::GetAllLogs();
    }

    return 0;
}