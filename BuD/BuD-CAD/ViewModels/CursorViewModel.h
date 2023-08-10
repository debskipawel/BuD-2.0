#pragma once

#include <memory>

#include <Objects/Additional/Cursor.h>

struct CursorViewModel
{
	CursorViewModel(std::shared_ptr<Cursor>& cursor)
		: m_Cursor(cursor)
	{
	}

	std::shared_ptr<Cursor>& m_Cursor;
};
