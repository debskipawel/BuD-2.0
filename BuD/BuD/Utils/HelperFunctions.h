#pragma once

#include <d3d11.h>

#include <concepts>
#include <sstream>

namespace BuD
{
    template <typename T>
    concept Numeric = std::is_arithmetic<T>::value;

    class HelperFunctions
    {
    public:
        static size_t BitsPerFormat(DXGI_FORMAT fmt);

        static size_t BytesPerFormat(DXGI_FORMAT format);

        template <Numeric NumericType>
        static std::string FormatWithPrecision(NumericType n, uint32_t precision)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(precision) << n;

            return stream.str();
        }
    };
}
