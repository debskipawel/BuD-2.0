#include "ApplicationCAD.h"

#include <string>

ApplicationCAD::ApplicationCAD()
{
	BuD::Log::RegisterLogHandle(
		[](const BuD::Log::LogRecord& record)
		{
			auto message = std::format(L"[{}] {}\n", record.time.Format(), record.message);

			OutputDebugStringW(message.c_str());
		}
	);
}

void ApplicationCAD::OnUpdate(float deltaTime)
{
}

void ApplicationCAD::OnRender()
{
}

void ApplicationCAD::OnGuiRender()
{
}
