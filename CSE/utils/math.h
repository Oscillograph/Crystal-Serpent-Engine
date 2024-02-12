#ifndef CSE_MATH_H
#define CSE_MATH_H

#include <CSE/protocore.h>

// random numbers generators
#include <CSE/utils/math/random.h>

// General CSE maths library.
// It is intended to be included only there, where it is needed.
namespace CSE
{
	namespace math
	{
		typedef float real; // to allow an easy change from float to double in future
		
		struct vec3
		{
			vec3() : x(0), y(0), z(0);
			vec3(real _x) : x(_x), y(_x), z(_x) {};
			vec3(real _x, real _y, real _z) : x(_x), y(_y), z(_z) {};
			
			real x, y, z;
			
			inline vec3 operator+(const real& rhs)
			{
				return vec3(x + rhs, y + rhs, z + rhs.z);
			}
			
			inline vec3 operator+(const vec3& rhs)
			{
				return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
			}
			
			inline void operator+=(const vec3& rhs)
			{
				x += rhs.x; 
				y += rhs.y; 
				z += rhs.z;
			}
			
			inline vec3 operator-(const real& rhs)
			{
				return vec3(x - rhs, y - rhs, z - rhs.z);
			}
			
			inline vec3 operator-(const vec3& rhs)
			{
				return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
			}
			
			inline void operator-=(const vec3& rhs)
			{
				x -= rhs.x; 
				y -= rhs.y; 
				z -= rhs.z;
			}
			
			// scale by a scalar
			inline vec3 operator*(const real& rhs)
			{
				return vec3(x * rhs, y * rhs, z * rhs.z);
			}
			
			inline void operator*=(const real& rhs)
			{
				x *= rhs; 
				y *= rhs;
				z *= rhs;
			}
			
			inline vec3 operator/(const real& rhs)
			{
				return vec3((real)x / rhs, (real)y / rhs, (real)z / rhs.z);
			}
			
			inline void operator/=(const real& rhs)
			{
				x /= rhs; 
				y /= rhs;
				z /= rhs;
			}
			
			// dot product
			inline real operator*(const vec3& rhs)
			{
				return (x * rhs.x + y * rhs.y + z * rhs.z);
			}
			
			// component product
			inline vec3 ComponentProduct(const vec3& rhs)
			{
				return vec3(x * rhs.x, y * rhs.y, z * rhs.z);
			}
			
			// vector product
			inline vec3 VectorProduct(const vec3& rhs)
			{
				return vec3(
					y * rhs.z - z * rhs.y, 
					z * rhs.x - x * rhs.z, 
					x * rhs.y - y * rhs.x
					);
			}
			
			inline vec3 operator%(const vec3& rhs)
			{
				return VectorProduct(rhs);
			}
			
			// basic geometry
			inline void Invert()
			{
				x = -x;
				y = -y;
				z = -z;
			}
			
			inline real Magnitude()
			{
				return sqrt(x*x + y*y + z*z);
			}
			
			inline real SquareMagnitude()
			{
				return (x*x + y*y + z*z);
			}
			
			inline void Normalize()
			{
				real length = Magnitude();
				if (length > 0)
				{
					(*this) /= length;
				}
			}
			
			inline int MakeOrthonormalBasis(vec3& a, vec3& b, vec3& c)
			{
				a.Normalize();
				c = a % b;
				if (c.SquareMagnitude() == 0)
				{
					c.Normalize();
					b = c % a;
					return 0; // success
				} else {
					return -1; // failure
				}
			}
		};
	}
}

#endif
