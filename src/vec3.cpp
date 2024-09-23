#include "vec3.h"


vec3::vec3() : e{ 0, 0, 0 } {}

vec3::vec3(double eV) : e{ eV, eV, eV }
{
}
vec3::vec3(double e0, double e1, double e2) : e{ e0, e1, e2 }
{
}

//return x y & z components

double vec3::x() const
{
	return e[0];
}
double vec3::y() const
{
		return e[1];
}
double vec3::z() const
{
		return e[2];
}

//return (-ve)ith component
vec3 vec3::operator-() const
{
	return vec3(-e[0], -e[1], -e[2]);
}

//return ith component
double vec3::operator[](int i) const
{
	return e[i];
}

//modify ith component
double& vec3::operator[](int i)
{
	return e[i];
}

//add 2 vec3 objects with +=
vec3& vec3::operator+= (const vec3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

//multiply vec3 values with number(scalar multiplication) using *=
vec3& vec3::operator*= (double t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

//divide vec3 values with number(scalar division)
//uses *= function
vec3& vec3::operator/= (double t)
{
	return *this *= 1 / t;
}

//calculates combined sq.l of vector
double vec3::length_squared() const
{
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

//calculates euclidean length of vector
double vec3::length() const
{
	return std::sqrt(length_squared());
}

// Return true if the vector is close to zero in all dimensions.
bool vec3::near_zero() const
{
	// Return true if the vector is close to zero in all dimensions.
	auto s = 1e-8;
	return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}

//generates random arbitary vector
vec3 vec3::random()
{
	return vec3(random_double(), random_double(), random_double());
}

//generates random arbitary vector of given value
vec3 vec3::random(double min, double max)
{
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

// Vector Utility Functions

//print vec3 components
std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

//adds two vec3 vectors component-wise and returns a new vector
vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

//subtracts two vec3 vectors component-wise and returns a new vector
vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

//multiply two vec3 vectors component-wise and returns a new vector
vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

//scalar multiplication of vec3 components
vec3 operator*(double t, const vec3& v)
{
	return vec3(t * v.x(), t * v.y(), t * v.z());
}

//scalar multiplication of vec3 components
//allows scalar value to be on right side
vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

//scalar division of vec3 components
vec3 operator/(const vec3& v, double t)
{
	return (1 / t) * v;
}

//dot products of 2 vec3 vectors, returning scalar value
double dot(const vec3& u, const vec3& v)
{
	return u.x() * v.x()
		+ u.y() * v.y()
		+ u.z() * v.z();
}

//cross product of 2 vec3 vectors, returning a vector perpendicular to both
vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x());
}

//Normalizes a vector to have a length of 1
vec3 unit_vector(const vec3& v)
{
	return v / v.length();
}

vec3 random_in_unit_disk() 
{
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() < 1)
			return p;
	}
}

vec3 random_unit_vector() 
{
	while (true) 
	{
		auto p = vec3::random(-1, 1);
		auto lensq = p.length_squared();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

vec3 random_on_hemisphere(const vec3& normal) 
{
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) 
{
	auto cos_theta = std::fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

