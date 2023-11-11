#pragma once

#include <functional>

void bresenhamAlgorithm(int x1, int y1, int x2, int y2, std::function<void(int, int, float)> onPutPixelHandler);
