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

#include <Bit/System/Matrix4x4.hpp>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

#ifdef BIT_PLATFORM_WIN32
	#define isnan _isnan
#endif

namespace Bit
{

	Matrix4x4::Matrix4x4( )
	{
		for(unsigned int i = 0; i < BIT_MATRIX4X4_SIZE; i++)
		{
			m[i] = 0;
		}
	}

	Matrix4x4::Matrix4x4(	const BIT_FLOAT32 X_x, const BIT_FLOAT32 Y_x, const BIT_FLOAT32 Z_x, const BIT_FLOAT32 P_x,
				const BIT_FLOAT32 X_y, const BIT_FLOAT32 Y_y, const BIT_FLOAT32 Z_y, const  BIT_FLOAT32 P_y,
				const BIT_FLOAT32 X_z, const BIT_FLOAT32 Y_z, const BIT_FLOAT32 Z_z, const BIT_FLOAT32 P_z,
				const BIT_FLOAT32 X_a, const BIT_FLOAT32 Y_a, const BIT_FLOAT32 Z_a, const BIT_FLOAT32 P_a )
	{
		m[0] = X_x;		m[1] = X_y;		m[2] = X_z;		m[3] = X_a;
		m[4] = Y_x;		m[5] = Y_y;		m[6] = Y_z;		m[7] = Y_a;
		m[8] = Z_x;		m[9]= Z_y;		m[10] = Z_z;	m[11] = Z_a;
		m[12] = P_x;	m[13] = P_y;	m[14] = P_z;	m[15] = P_a;
	}

	void Matrix4x4::Identity( )
	{
		m[0] = 1;	m[1] = 0;		m[2] = 0;		m[3] = 0;
		m[4] = 0;	m[5] = 1;		m[6] = 0;		m[7] = 0;
		m[8] = 0;	m[9] = 0;		m[10] = 1;		m[11] = 0;
		m[12] =0;	m[13] = 0;		m[14] = 0;		m[15] = 1;
	}

	void Matrix4x4::LookAt( Vector3_f32 p_Eye, Vector3_f32 p_Center, Vector3_f32 p_Up )
	{
		Vector3_f32 Forward = p_Center - p_Eye;

		Forward.Normalize( );

		/// Side = forward x up
		Vector3_f32 Side = Vector3_f32( Vector3_f32::Cross( Forward, p_Up ) ).Normal( );

		/// Recompute up as: up = side x forward
		p_Up = Vector3_f32::Cross( Side, Forward );

		Matrix4x4 LookMatrix(
			Side.x, Side.y, Side.z, 0,
			p_Up.x, p_Up.y, p_Up.z, 0,
			-Forward.x, -Forward.y, -Forward.z, 0,
			0, 0, 0, 1
		);

		Matrix4x4 PositionMatrix(	
			1, 0, 0, -p_Eye.x,
			0, 1, 0, -p_Eye.y,
			0, 0, 1, -p_Eye.z,
			0, 0, 0, 1
		);

		Matrix4x4 ViewMatrix = LookMatrix * PositionMatrix;

		for(unsigned int i = 0; i < BIT_MATRIX4X4_SIZE; i++)
		{
			m[i] = ViewMatrix.m[i];
		}
	}

	void Matrix4x4::Orthographic(	const BIT_FLOAT32 p_Left, const BIT_FLOAT32 p_Right, const BIT_FLOAT32 p_Bottom,
									const BIT_FLOAT32 p_Top, const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar )
	{
		float TX =- ( p_Right + p_Left )/( p_Right - p_Left );
		float TY =- ( p_Top + p_Bottom )/( p_Top - p_Bottom );
		float TZ =- ( p_ZFar + p_ZNear)/( p_ZFar - p_ZNear );

		Identity();
		m[0] = 2/( p_Right - p_Left );
		m[5] = 2.0f / ( p_Top- p_Bottom );
		m[10] =- 2 / ( p_ZFar - p_ZNear );
		m[12] = TX;
		m[13] = TY;
		m[14] = TZ;
	}

	void Matrix4x4::Perspective(	const BIT_FLOAT32 p_Fov, const BIT_FLOAT32 p_Aspect,
									const BIT_FLOAT32 p_ZNear, const BIT_FLOAT32 p_ZFar )
	{
		BIT_FLOAT32 Sine, Cotangent, DeltaZ;
		BIT_FLOAT32 Radians = p_Fov / 2.0f * BIT_FLOAT32( BIT_PI ) / 180.0f;

		DeltaZ = p_ZFar - p_ZNear;
		Sine = sin( Radians );
		if (( DeltaZ == 0) || ( Sine == 0 ) || ( p_Aspect == 0 ))
		{
			return;
		}
		Cotangent = cos( Radians ) / Sine;

		Identity();

		m[0] = Cotangent / p_Aspect;
		m[5] = Cotangent;
		m[10] = -(p_ZFar + p_ZNear) / DeltaZ;
		m[11] = -1;
		m[14]= -2 * p_ZNear * p_ZFar / DeltaZ;
		m[15] = 0;
	}

	void Matrix4x4::Position( Bit::Vector3_f32 p_Postion )
	{
		m[0] = 1;	m[1] = 0;		m[2] = 0;		m[3] = 0;
		m[4] = 0;	m[5] = 1;		m[6] = 0;		m[7] = 0;
		m[8] = 0;	m[9] = 0;		m[10] = 1;		m[11] = 0;
		m[12] = p_Postion.x;	m[13] = p_Postion.y;	m[14] = p_Postion.z;	m[15] = 1;
	}

	void Matrix4x4::RotateX( const BIT_FLOAT32 p_Angle )
	{
		Matrix4x4 Dest;
		const BIT_FLOAT32 AngleSin = static_cast<BIT_FLOAT32>( SinDegrees( p_Angle ) );
		const BIT_FLOAT32 AngleCos = static_cast<BIT_FLOAT32>( CosDegrees( p_Angle ) );

		Dest.m[0] = 1;
		Dest.m[5] = AngleCos;
		Dest.m[6] = -AngleSin;
		Dest.m[9] = AngleSin;
		Dest.m[10] = AngleCos;
		Dest.m[15] = 1;

		*this = *this * Dest;
	}

	void Matrix4x4::RotateY( const BIT_FLOAT32 p_Angle )
	{
		Matrix4x4 Dest;
		const BIT_FLOAT32 AngleSin = static_cast<BIT_FLOAT32>( SinDegrees( p_Angle ) );
		const BIT_FLOAT32 AngleCos = static_cast<BIT_FLOAT32>( CosDegrees( p_Angle ) );

		Dest.m[0] = AngleCos;
		Dest.m[2] = AngleSin;
		Dest.m[5] = 1;
		Dest.m[8] = -AngleSin;
		Dest.m[10] = AngleCos;
		Dest.m[15] = 1;

		*this = *this * Dest;
	}

	void Matrix4x4::RotateZ( const BIT_FLOAT32 p_Angle )
	{
		Matrix4x4 Dest;
		const BIT_FLOAT32 AngleSin = static_cast<BIT_FLOAT32>( SinDegrees( p_Angle ) );
		const BIT_FLOAT32 AngleCos = static_cast<BIT_FLOAT32>( CosDegrees( p_Angle ) );

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

	void Matrix4x4::Scale( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z )
	{
		m[0]	*= p_X;
		m[5]	*= p_Y;
		m[10]	*= p_Z;
	}

	void Matrix4x4::Translate( const BIT_FLOAT32 p_X, const BIT_FLOAT32 p_Y, const BIT_FLOAT32 p_Z )
	{
		Matrix4x4 dest(1, 0, 0, p_X,		0, 1, 0, p_Y,		0, 0, 1, p_Z,		0, 0, 0, 1);
		*this = *this * dest;
	}

	Matrix4x4 & Matrix4x4::operator = ( const Matrix4x4  & p_Mat )
	{
		for( BIT_UINT32 i = 0; i < BIT_MATRIX4X4_SIZE; i++ )
		{
			m[ i ] = p_Mat.m[ i ];
		}
		return *this;
	}

	Matrix4x4 Matrix4x4::operator + ( const Matrix4x4  & p_Mat ) const
	{
		Matrix4x4 Dest = *this;

		for( BIT_UINT32 i = 0; i < BIT_MATRIX4X4_SIZE; i++ )
		{
			Dest.m[ i ] -= p_Mat.m[ i ];
		}

		return Dest;
	}

	Matrix4x4 Matrix4x4::operator - ( const Matrix4x4  & p_Mat ) const
	{
		Matrix4x4 Dest = *this;

		for( BIT_UINT32 i = 0; i < BIT_MATRIX4X4_SIZE; i++ )
		{
			Dest.m[ i ] += p_Mat.m[ i ];
		}

		return Dest;
	}

	Matrix4x4 Matrix4x4::operator * ( const Matrix4x4 & p_Mat ) const
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


}