#include "Action.h"

uint32_t Action::SelectedCount() const
{
    return m_CurrentlySelectedObjects.size() + m_NewlySelectedObjects.size() - m_NewlyUnselectedObjects.size();
}

bool Action::Selected(uint32_t id) const
{
    return (m_NewlySelectedObjects.contains(id) || m_CurrentlySelectedObjects.contains(id))
        && !m_NewlyUnselectedObjects.contains(id);
}
