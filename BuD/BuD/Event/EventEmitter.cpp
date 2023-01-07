#include "bud_pch.h"
#include "EventEmitter.h"

#include <Core/ApplicationCore.h>

namespace BuD
{
	void EventEmitter::Emit(Event& e)
	{
		auto app = ApplicationCore::Get();
		_ASSERT(app);

		app->OnEvent(e);
	}
}