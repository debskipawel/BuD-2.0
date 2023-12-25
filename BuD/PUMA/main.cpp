#include <BuD.h>
#include <Core/EntryPoint.h>

#include <PumaApp.h>

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::make_shared<PumaApp>();
}
