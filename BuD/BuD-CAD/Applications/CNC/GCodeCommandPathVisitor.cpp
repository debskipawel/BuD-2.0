#include "GCodeCommandPathVisitor.h"

GCodeCommandPathVisitor::GCodeCommandPathVisitor()
    : m_Points(), m_UnitScale(0.1f), m_ToolPosition(10.0f * dxm::Vector3::UnitY)
{
}

auto GCodeCommandPathVisitor::Visit(GCP::GCodeCommand& command) -> void
{
    GCP::GCodeCommandVisitor::Visit(command);
}

auto GCodeCommandPathVisitor::Visit(GCP::FastToolMoveCommand& command) -> void
{
    auto position = m_ToolPosition;

    if (command.m_X.has_value())
    {
        position.x = m_UnitScale * command.m_X.value();
    }

    if (command.m_Y.has_value())
    {
        position.y = m_UnitScale * command.m_Y.value();
    }

    if (command.m_Z.has_value())
    {
        position.z = m_UnitScale * command.m_Z.value();
    }

    m_Points.push_back(position);

    m_ToolPosition = position;
}

auto GCodeCommandPathVisitor::Visit(GCP::SlowToolMoveCommand& command) -> void
{
    auto position = m_ToolPosition;

    if (command.m_X.has_value())
    {
        position.x = m_UnitScale * command.m_X.value();
    }

    if (command.m_Y.has_value())
    {
        position.y = m_UnitScale * command.m_Y.value();
    }

    if (command.m_Z.has_value())
    {
        position.z = m_UnitScale * command.m_Z.value();
    }

    m_Points.push_back(position);

    m_ToolPosition = position;
}

auto GCodeCommandPathVisitor::Visit(GCP::InchesUnitSystemSelectionCommand& command) -> void
{
    m_UnitScale = 2.54f;
}

auto GCodeCommandPathVisitor::Visit(GCP::MillimetersUnitSystemSelectionCommand& command) -> void
{
    m_UnitScale = 0.1f;
}

auto GCodeCommandPathVisitor::Points() const -> const std::vector<dxm::Vector3>&
{
    return m_Points;
}
