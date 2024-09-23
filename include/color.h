#pragma once

#include "vec3.h"
#include "interval.h"

//color is just an alias for vec3

using color = vec3;

class Draw
{
public:
    int rbyte, gbyte, bbyte;

    inline double linear_to_gamma(double linear_component)
    {
        if (linear_component > 0)
            return std::sqrt(linear_component);

        return 0 ;
    }

    inline color calColor(const vec3& pixel_color)
    {
        // Translate the [0,1] component values to the byte range [0,255].
        static const interval intensity(0.000, 0.999);
        
        rbyte = int(256 * linear_to_gamma(intensity.clamp(pixel_color.x())));
        gbyte = int(256 * linear_to_gamma(intensity.clamp(pixel_color.y())));
        bbyte = int(256 * linear_to_gamma(intensity.clamp(pixel_color.z())));                                            

        return color(rbyte, gbyte, bbyte);
    }

};