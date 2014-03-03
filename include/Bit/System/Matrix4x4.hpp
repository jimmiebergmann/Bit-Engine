// ///////////////////////////////////////////////////////////////////////////
// Copyright (C) 2013 Jimmie Bergmann - jimmiebergmann@gmail.com
//
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but
//    is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any
//    source distribution.
// ///////////////////////////////////////////////////////////////////////////

#ifndef BIT_SYSTEM_MATRXI4X4_HPP
#define BIT_SYSTEM_MATRXI4X4_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Math.hpp>
#include <Bit/System/Vector3.hpp>

#include <float.h>
#ifdef BIT_PLATFORM_WIN32
	#define bitIsNan _isnan
#elif BIT_PLATFORM_LINUX
	define bitIsNan isnan
#endif

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup System
	/// \brief Matrix class
	///
	////////////////////////////////////////////////////////////////
	template <typename T>
	class BIT_API Matrix4x4
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4(	const T X_x, const T Y_x, const T Z_x, const T P_x,
					const T X_y, const T Y_y, const T Z_y, const T P_y,
					const T X_z, const T Y_z, const T Z_z, const T P_z,
					const T X_a, const T Y_a, const T Z_a, const T P_a )
		{
			m[0] = X_x;		m[1] = X_y;		m[2] = X_z;		m[3] = X_a;
			m[4] = Y_x;		m[5] = Y_y;		m[6] = Y_z;		m[7] = Y_a;
			m[8] = Z_x;		m[9]= Z_y;		m[10] = Z_z;	m[11] = Z_a;
			m[12] = P_x;	m[13] = P_y;	m[14] = P_z;	m[15] = P_a;
		}
		
		////////////////////////////////////////////////////////////////
		/// \brief Turning the matrix into an identity matrix.
		///
		////////////////////////////////////////////////////////////////
		void Identity( )
		{
			m[0] = 1;	m[1] = 0;		m[2] = 0;		m[3] = 0;
			m[4] = 0;	m[5] = 1;		m[6] = 0;		m[7] = 0;
			m[8] = 0;	m[9] = 0;		m[10] = 1;		m[11] = 0;
			m[12] =0;	m[13] = 0;		m[14] = 0;		m[15] = 1;
		}

		void LookAt( Vector3<T> p_Eye, Vector3<T> p_Center, Vector3<T> p_Up )
		{
			Vector3<T> Forward = p_Center - p_Eye;

			Forward.Normalize( );

			/// Side = forward x up
			Vector3<T> Side = Vector3<T>( Vector3<T>::Cross( Forward, p_Up ) ).Normal( );

			/// Recompute up as: up = side x forward
			p_Up = Vector3<T>::Cross( Side, Forward );

			Matrix4x4<T> LookMatrix(
				Side.x, Side.y, Side.z, 0,
				p_Up.x, p_Up.y, p_Up.z, 0,
				-Forward.x, -Forward.y, -Forward.z, 0,
				0, 0, 0, 1
			);

			Matrix4x4<T> PositionMatrix(	
				1, 0, 0, -p_Eye.x,
				0, 1, 0, -p_Eye.y,
				0, 0, 1, -p_Eye.z,
				0, 0, 0, 1
			);

			Matrix4x4<T> ViewMatrix = LookMatrix * PositionMatrix;

			for( SizeType i = 0; i < 16; i++)
			{
				m[i] = ViewMatrix.m[i];
			}
		}

		void Orthographic(	const T p_Left, const T p_Right, const T p_Bottom,
							const T p_Top, const T p_ZNear, const T p_ZFar )
		{
			T TX =- ( p_Right + p_Left )	/ ( p_Right - p_Left );
			T TY =- ( p_Top + p_Bottom )	/ ( p_Top - p_Bottom );
			T TZ =- ( p_ZFar + p_ZNear)		/ ( p_ZFar - p_ZNear );

			Identity();
			m[0] = 2/( p_Right - p_Left );
			m[5] = 2.0f / ( p_Top- p_Bottom );
			m[10] =- 2 / ( p_ZFar - p_ZNear );
			m[12] = TX;
			m[13] = TY;
			m[14] = TZ;
		}

		void Perspective(	const T p_Fov, const T p_Aspect,
							const T p_ZNear, const T p_ZFar )
		{
			T Radians = p_Fov / 2.0f * static_cast<T>( Pi ) / 180.0f;

			T DeltaZ = p_ZFar - p_ZNear;
			T Sine = sin( Radians );
			if (( DeltaZ == 0) || ( Sine == 0 ) || ( p_Aspect == 0 ))
			{
				return;
			}
			T Cotangent = cos( Radians ) / Sine;

			// Calculate the identity matrix
			Identity();

			m[0] = Cotangent / p_Aspect;
			m[5] = Cotangent;
			m[10] = -(p_ZFar + p_ZNear) / DeltaZ;
			m[11] = -1;
			m[14]= -2 * p_ZNear * p_ZFar / DeltaZ;
			m[15] = 0;
		}

		void Position( const Vector3<T> & p_Postion )
		{
			m[0] = 1;	m[1] = 0;		m[2] = 0;		m[3] = 0;
			m[4] = 0;	m[5] = 1;		m[6] = 0;		m[7] = 0;
			m[8] = 0;	m[9] = 0;		m[10] = 1;		m[11] = 0;
			m[12] = p_Postion.x;	m[13] = p_Postion.y;	m[14] = p_Postion.z;	m[15] = 1;
		}

		void RotateX( const T p_Angle )
		{
			Matrix4x4<T> Dest;
			const T AngleSin = static_cast<T>( SinDegrees( p_Angle ) );
			const T AngleCos = static_cast<T>( CosDegrees( p_Angle ) );

			Dest.m[0] = 1;
			Dest.m[5] = AngleCos;
			Dest.m[6] = -AngleSin;
			Dest.m[9] = AngleSin;
			Dest.m[10] = AngleCos;
			Dest.m[15] = 1;

			*this = *this * Dest;
		}

		void RotateY( const T p_Angle )
		{
			Matrix4x4<T> Dest;
			const T AngleSin = static_cast<T>( SinDegrees( p_Angle ) );
			const T AngleCos = static_cast<T>( CosDegrees( p_Angle ) );

			Dest.m[0] = AngleCos;
			Dest.m[2] = AngleSin;
			Dest.m[5] = 1;
			Dest.m[8] = -AngleSin;
			Dest.m[10] = AngleCos;
			Dest.m[15] = 1;

			*this = *this * Dest;
		}

		void RotateZ( const T p_Angle )
		{
			Matrix4x4<T> Dest;
			const T AngleSin = static_cast<T>( SinDegrees( p_Angle ) );
			const T AngleCos = static_cast<T>( CosDegrees( p_Angle ) );

			Dest.m[0] = AngleCos;
			Dest.m[1] = -AngleSin;
			Dest.m[2] = 0;
			Dest.m[3] = 0;

			Dest.m[4] = AngleSin;
			Dest.m[5] = AngleCos;
			Dest.m[6] = 0;
			Dest.m[7] = 0;

			Dest.m[8] = 0;
			Dest.m[9] = 0;
			Dest.m[10] = 1;
			Dest.m[11] = 0;

			Dest.m[12] = 0;
			Dest.m[13] = 0;
			Dest.m[14] = 0;
			Dest.m[15] = 1;

			*this = *this * Dest;
		}

		void Scale( const T p_X, const T p_Y, const T p_Z )
		{
			m[0]	*= p_X;
			m[5]	*= p_Y;
			m[10]	*= p_Z;
		}

		void Translate( const T p_X, const T p_Y, const T p_Z )
		{
			Matrix4x4<T> dest(1, 0, 0, p_X,		0, 1, 0, p_Y,		0, 0, 1, p_Z,		0, 0, 0, 1);
			*this = *this * dest;
		}


		////////////////////////////////////////////////////////////////
		// Operators.
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		/// \brief Copy operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4 & operator = ( const Matrix4x4 & p_Mat )
		{
			for( SizeType i = 0; i < 16; i++ )
			{
				m[ i ] = p_Mat.m[ i ];
			}
			return *this;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Addition operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4 operator + ( const Matrix4x4  & p_Mat ) const
		{
			Matrix4x4 Dest = *this;

			for( SizeType i = 0; i < 16; i++ )
			{
				Dest.m[ i ] -= p_Mat.m[ i ];
			}

			return Dest;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Subtraction operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4 operator - ( const Matrix4x4  & p_Mat ) const
		{
			Matrix4x4 Dest = *this;

			for( SizeType i = 0; i < 16; i++ )
			{
				Dest.m[ i ] += p_Mat.m[ i ];
			}

			return Dest;
		}

		////////////////////////////////////////////////////////////////
		/// \brief Multiplication operator.
		///
		////////////////////////////////////////////////////////////////
		Matrix4x4 operator * ( const Matrix4x4 & p_Mat ) const
		{
			Matrix4x4 Mat1 = *this;
			Matrix4x4 Dest;

			Dest.m[0] = Mat1.m[0] * p_Mat.m[0] + Mat1.m[4] * p_Mat.m[1] + Mat1.m[8] * p_Mat.m[2] + Mat1.m[12] * p_Mat.m[3];
			Dest.m[4] = Mat1.m[0] * p_Mat.m[4] + Mat1.m[4] * p_Mat.m[5] + Mat1.m[8] * p_Mat.m[6] + Mat1.m[12] * p_Mat.m[7];
			Dest.m[8] = Mat1.m[0] * p_Mat.m[8] + Mat1.m[4] * p_Mat.m[9] + Mat1.m[8] * p_Mat.m[10] + Mat1.m[12] * p_Mat.m[11];
			Dest.m[12] = Mat1.m[0] * p_Mat.m[12] + Mat1.m[4] * p_Mat.m[13] + Mat1.m[8] * p_Mat.m[14] + Mat1.m[12] * p_Mat.m[15];

			Dest.m[1] = Mat1.m[1] * p_Mat.m[0] + Mat1.m[5] * p_Mat.m[1] + Mat1.m[9] * p_Mat.m[2] + Mat1.m[13] * p_Mat.m[3];
			Dest.m[5] = Mat1.m[1] * p_Mat.m[4] + Mat1.m[5] * p_Mat.m[5] + Mat1.m[9] * p_Mat.m[6] + Mat1.m[13] * p_Mat.m[7];
			Dest.m[9] = Mat1.m[1] * p_Mat.m[8] + Mat1.m[5] * p_Mat.m[9] + Mat1.m[9] * p_Mat.m[10] + Mat1.m[13] * p_Mat.m[11];
			Dest.m[13] = Mat1.m[1] * p_Mat.m[12] + Mat1.m[5] * p_Mat.m[13] +Mat1.m[9] * p_Mat.m[14] + Mat1.m[13] * p_Mat.m[15];

			Dest.m[2] = Mat1.m[2] * p_Mat.m[0] + Mat1.m[6] * p_Mat.m[1] + Mat1.m[10] * p_Mat.m[2] + Mat1.m[14] * p_Mat.m[3];
			Dest.m[6] = Mat1.m[2] * p_Mat.m[4] + Mat1.m[6] * p_Mat.m[5] + Mat1.m[10] * p_Mat.m[6] + Mat1.m[14] * p_Mat.m[7];
			Dest.m[10] = Mat1.m[2] * p_Mat.m[8] + Mat1.m[6] * p_Mat.m[9] + Mat1.m[10] * p_Mat.m[10] + Mat1.m[14] * p_Mat.m[11];
			Dest.m[14] = Mat1.m[2] * p_Mat.m[12] + Mat1.m[6] * p_Mat.m[13] + Mat1.m[10] * p_Mat.m[14] + Mat1.m[14] * p_Mat.m[15];

			Dest.m[3] = Mat1.m[3] * p_Mat.m[0] + Mat1.m[7] * p_Mat.m[1] + Mat1.m[11] * p_Mat.m[2] + Mat1.m[15] * p_Mat.m[3];
			Dest.m[7] = Mat1.m[3] * p_Mat.m[4] + Mat1.m[7] * p_Mat.m[5] + Mat1.m[11] * p_Mat.m[6] + Mat1.m[15] * p_Mat.m[7];
			Dest.m[11] = Mat1.m[3] * p_Mat.m[8] + Mat1.m[7] * p_Mat.m[9] + Mat1.m[11] * p_Mat.m[10] + Mat1.m[15] * p_Mat.m[11];
			Dest.m[15] = Mat1.m[3] * p_Mat.m[12] + Mat1.m[7] * p_Mat.m[13] + Mat1.m[11] * p_Mat.m[14] + Mat1.m[15] * p_Mat.m[15];

			return Dest;
		}

		////////////////////////////////////////////////////////////////
		// Public variables.
		////////////////////////////////////////////////////////////////
		T m[ 16 ]; ///< The matrix components

	};

	////////////////////////////////////////////////////////////////
	// Predefined matrix4x4 types.
	////////////////////////////////////////////////////////////////
	typedef Matrix4x4< Float32 >	Matrix4x4f32;
	typedef Matrix4x4< Float64 >	Matrix4x4f64;

}

#endif