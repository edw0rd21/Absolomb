#pragma once

#include "vec3.h"

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


class ray 
{
public:
    ray();

    ray(const point3& origin, const vec3& direction);

    const point3& origin() const;
    const vec3& direction() const;
    point3 at(double t) const;
    point3 at(float t) const;

private:
    point3 orig;
    vec3 dir;
};

