#include "ColinearPathOptimizer.h"

#include <numbers>

ColinearPathOptimizer::ColinearPathOptimizer(float lengthAccuracy, float angleAccuracy)
    : m_LengthAccuracy(lengthAccuracy)
{
    m_AngleAccuracy = sinf(angleAccuracy * std::numbers::pi_v<float> / 180.0f);
}

auto ColinearPathOptimizer::Optimize(const MillingToolPath& toolPath) const -> MillingToolPath
{
    auto compressedPathPoints = toolPath.GetPath();

    const auto accuracySquared = m_LengthAccuracy * m_LengthAccuracy;

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

        if (crossed.LengthSquared() <= m_AngleAccuracy * m_AngleAccuracy)
        {
            compressedPathPoints.erase(compressedPathPoints.begin() + i);
            --i;

            continue;
        }
    }

    return MillingToolPath(toolPath.GetToolRadius(), std::move(compressedPathPoints));
}
