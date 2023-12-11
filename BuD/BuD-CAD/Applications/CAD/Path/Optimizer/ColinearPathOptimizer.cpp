#include "ColinearPathOptimizer.h"

ColinearPathOptimizer::ColinearPathOptimizer(float accuracy)
    : m_Accuracy(accuracy)
{
}

auto ColinearPathOptimizer::Optimize(const MillingToolPath& toolPath) const -> MillingToolPath
{
    const auto& toolPathPoints = toolPath.GetPath();

    auto compressedPathPoints = toolPathPoints;

    const auto accuracySquared = m_Accuracy * m_Accuracy;

    for (size_t i = 1; i + 1 < compressedPathPoints.size(); ++i)
    {
        auto d1 = compressedPathPoints[i] - compressedPathPoints[i - 1];
        auto d2 = compressedPathPoints[i + 1] - compressedPathPoints[i];

        if (d1.LengthSquared() < accuracySquared || d2.LengthSquared() < accuracySquared)
        {
            compressedPathPoints.erase(compressedPathPoints.begin() + i);
            --i;

            continue;
        }

        d1.Normalize();
        d2.Normalize();

        auto crossed = d1.Cross(d2);

        if (crossed.LengthSquared() < accuracySquared)
        {
            compressedPathPoints.erase(compressedPathPoints.begin() + i);
            --i;

            continue;
        }
    }

    return MillingToolPath(toolPath.GetToolRadius(), std::move(compressedPathPoints));
}
