#pragma once

#include <Applications/CAD/Intersection/Structures/IntersectionResult.h>
#include <Applications/CAD/Intersection/ResultConverter/IntersectionPoint.h>

class IntersectionResultConverter
{
public:
	virtual auto ToPointVectors(const IntersectionResult& result) -> std::pair<std::vector<IntersectionPoint>, std::vector<IntersectionPoint>>;
};
