#include <Core/EntryPoint.h>
#include <HodographApp.h>

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::make_shared<HodographApp>();
}
