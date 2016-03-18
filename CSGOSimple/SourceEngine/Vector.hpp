#pragma once

#include "Definitions.hpp"
#include <sstream>

namespace SourceEngine {
	typedef float vec_t;
	// 3D Vector
	class Vector {
	public:
		// Members
		vec_t x, y, z;

		// Construction/destruction:
		Vector( void );
		Vector( vec_t X, vec_t Y, vec_t Z );
		Vector( vec_t* clr );

		// Initialization
		void Init( vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f );
		// TODO (Ilya): Should there be an init that takes a single float for consistency?

		// Got any nasty NAN's?
		bool IsValid() const;
		void Invalidate();

		// array access...
		vec_t operator[]( int i ) const;
		vec_t& operator[]( int i );

		// Base address...
		vec_t* Base();
		vec_t const* Base() const;

		// Initialization methods
		void Random( vec_t minVal, vec_t maxVal );
		void Zero(); ///< zero out a vector

		// equality
		bool operator==(const Vector& v) const;
		bool operator!=(const Vector& v) const;

		// arithmetic operations
		Vector& operator+=(const Vector& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector& operator-=(const Vector& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		Vector& operator*=(float fl) {
			x *= fl;
			y *= fl;
			z *= fl;
			return *this;
		}

		Vector& operator*=(const Vector& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		Vector& operator/=(const Vector& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		// this ought to be an opcode.
		Vector&	operator+=(float fl) {
			x += fl;
			y += fl;
			z += fl;
			return *this;
		}

		// this ought to be an opcode.
		Vector&	operator/=(float fl) {
			x /= fl;
			y /= fl;
			z /= fl;
			return *this;
		}
		Vector&	operator-=(float fl) {
			x -= fl;
			y -= fl;
			z -= fl;
			return *this;
		}

		// negate the vector components
		void	Negate();

		// Get the vector's magnitude.
		vec_t	Length() const;

		// Get the vector's magnitude squared.
		vec_t LengthSqr( void ) const {
			return (x*x + y*y + z*z);
		}

		// return true if this vector is (0,0,0) within tolerance
		bool IsZero( float tolerance = 0.01f ) const {
			return (x > -tolerance && x < tolerance &&
				y > -tolerance && y < tolerance &&
				z > -tolerance && z < tolerance);
		}

		vec_t	NormalizeInPlace();
		Vector	Normalized() const;
		bool	IsLengthGreaterThan( float val ) const;
		bool	IsLengthLessThan( float val ) const;

		// check if a vector is within the box defined by two other vectors
		bool WithinAABox( Vector const &boxmin, Vector const &boxmax );

		// Get the distance from this vector to the other one.
		vec_t	DistTo( const Vector &vOther ) const;

		// Get the distance from this vector to the other one squared.
		// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
		// may be able to tidy this up after switching to VC7
		vec_t DistToSqr( const Vector &vOther ) const {
			Vector delta;

			delta.x = x - vOther.x;
			delta.y = y - vOther.y;
			delta.z = z - vOther.z;

			return delta.LengthSqr();
		}

		// Copy
		void	CopyToArray( float* rgfl ) const;

		// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
		// is about 12% faster than the actual vector equation (because it's done per-component
		// rather than per-vector).
		void	MulAdd( const Vector& a, const Vector& b, float scalar );

		// Dot product.
		vec_t	Dot( const Vector& vOther ) const;

		// assignment
		Vector& operator=(const Vector &vOther);

		// 2d
		vec_t	Length2D( void ) const {
			return sqrt(x * x + y * y);
		}
		vec_t	Length2DSqr( void ) const;

		/// get the component of this vector parallel to some other given vector
		Vector  ProjectOnto( const Vector& onto );

		// copy constructors
		//	Vector(const Vector &vOther);

		// arithmetic operations
		Vector	operator-(void) const;

		Vector	operator+(const Vector& v) const;
		Vector	operator-(const Vector& v) const;
		Vector	operator*(const Vector& v) const;
		Vector	operator/(const Vector& v) const;
		Vector	operator*(float fl) const;
		Vector	operator/(float fl) const;

		// Cross product between two vectors.
		Vector	Cross( const Vector &vOther ) const;

		// Returns a vector with the min or max in X, Y, and Z.
		Vector	Min( const Vector &vOther ) const;
		Vector	Max( const Vector &vOther ) const;
	};

	void VectorCopy( const Vector& src, Vector& dst );
	float VectorLength( const Vector& v );
	void VectorLerp( const Vector& src1, const Vector& src2, vec_t t, Vector& dest );
	void VectorCrossProduct( const Vector& a, const Vector& b, Vector& result );
	vec_t NormalizeVector( Vector& v );
	

	class __declspec(align(16)) VectorAligned : public Vector {
	public:
		inline VectorAligned( void ) {};
		inline VectorAligned( vec_t X, vec_t Y, vec_t Z ) {
			Init( X, Y, Z );
		}

	public:
		explicit VectorAligned( const Vector &vOther ) {
			Init( vOther.x, vOther.y, vOther.z );
		}

		VectorAligned& operator=(const Vector &vOther) {
			Init( vOther.x, vOther.y, vOther.z );
			return *this;
		}

		VectorAligned& operator=(const VectorAligned &vOther) {
			_mm_store_ps( Base(), _mm_load_ps( vOther.Base() ) );
			return *this;
		}

		float w;
	};
}