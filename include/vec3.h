#pragma once

#include <cmath>
#include <iostream>
#include "utility.h"

class vec3
{
public:

	vec3();
	vec3(double eV);
	vec3(double e0, double e1, double e2);

	double x() const;
	double y() const;
	double z() const;

	vec3 operator-() const;
	double operator[](int i) const;
	double& operator[](int i);
	vec3& operator+= (const vec3& v);
	vec3& operator*= (double t);
	vec3& operator/= (double t);

	double length_squared() const;
	double length() const;
	bool near_zero() const;

	static vec3 random();
	static vec3 random(double min, double max);

private:
	double e[3];
};

//vector utility functions

std::ostream& operator<<(std::ostream& out, const vec3& v);
vec3 operator+(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator*(double t, const vec3& v);
vec3 operator*(const vec3& v, double t);
vec3 operator/(const vec3& v, double t);
double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 unit_vector(const vec3& v);
vec3 random_in_unit_disk();
vec3 random_unit_vector();
vec3 random_on_hemisphere(const vec3& normal);
vec3 reflect(const vec3& v, const vec3& n);
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat);