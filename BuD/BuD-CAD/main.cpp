#include <Core/EntryPoint.h>

#include <Applications/Switcher/ApplicationSwitcher.h>

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::make_shared<ApplicationSwitcher>();
}
