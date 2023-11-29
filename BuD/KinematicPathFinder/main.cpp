#include <BuD.h>
#include <Core/EntryPoint.h>

#include <KinematicPathFinderApp.h>

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::make_shared<KinematicPathFinderApp>();
}
