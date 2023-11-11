#include <BuD.h>
#include <Core/EntryPoint.h>

#include "InterpolationApp.h"

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::make_shared<InterpolationApp>();
}
