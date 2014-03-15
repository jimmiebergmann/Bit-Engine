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

#include <Bit/System/Vector3.hpp>

template <typename T>
Matrix4x4<T>::Matrix4x4( )
{
	
}

template <typename T>
Matrix4x4<T>::Matrix4x4( const T p_Val )
{
	for( SizeType i = 0; i < 16; i++ )
	{
		m[ i ] = p_Val;
	}
}

template <typename T>
Matrix4x4<T>::Matrix4x4(	const T X_x, const T Y_x, const T Z_x, const T P_x,
							const T X_y, const T Y_y, const T Z_y, const T P_y,
							const T X_z, const T Y_z, const T Z_z, const T P_z,
							const T X_a, const T Y_a, const T Z_a, const T P_a )
{
	m[0] = X_x;		m[1] = X_y;		m[2] = X_z;		m[3] = X_a;
	m[4] = Y_x;		m[5] = Y_y;		m[6] = Y_z;		m[7] = Y_a;
	m[8] = Z_x;		m[9]= Z_y;		m[10] = Z_z;	m[11] = Z_a;
	m[12] = P_x;	m[13] = P_y;	m[14] = P_z;	m[15] = P_a;
}
		
template <typename T>
void Matrix4x4<T>::Identity( )
{
	m[0]	= static_cast<T>( 1 );
	m[1]	= static_cast<T>( 0 );
	m[2]	= static_cast<T>( 0 );
	m[3]	= static_cast<T>( 0 );
	m[4]	= static_cast<T>( 0 );
	m[5]	= static_cast<T>( 1 );
	m[6]	= static_cast<T>( 0 );
	m[7]	= static_cast<T>( 0 );
	m[8]	= static_cast<T>( 0 );
	m[9]	= static_cast<T>( 0 );
	m[10]	= static_cast<T>( 1 );
	m[11]	= static_cast<T>( 0 );
	m[12]	= static_cast<T>( 0 );
	m[13]	= static_cast<T>( 0 );
	m[14]	= static_cast<T>( 0 );
	m[15]	= static_cast<T>( 1 );
}

template <typename T>
void Matrix4x4<T>::LookAt( Vector3<T> p_Eye, Vector3<T> p_Center, Vector3<T> p_Up )
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

template <typename T>
void Matrix4x4<T>::Orthographic(	const T p_Left, const T p_Right, const T p_Bottom,
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

template <typename T>
void Matrix4x4<T>::Perspective(	const T p_Fov, const T p_Aspect,
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

template <typename T>
void Matrix4x4<T>::Position( const Vector3<T> & p_Postion )
{
	m[0] = 1;	m[1] = 0;		m[2] = 0;		m[3] = 0;
	m[4] = 0;	m[5] = 1;		m[6] = 0;		m[7] = 0;
	m[8] = 0;	m[9] = 0;		m[10] = 1;		m[11] = 0;
	m[12] = p_Postion.x;	m[13] = p_Postion.y;	m[14] = p_Postion.z;	m[15] = 1;
}

template <typename T>
void Matrix4x4<T>::RotateX( const T p_Angle )
{
	Matrix4x4<T> rotMat;
	const T AngleSin = static_cast<T>( SinDegrees( p_Angle ) );
	const T AngleCos = static_cast<T>( CosDegrees( p_Angle ) );

	rotMat.m[0] = 1;
	rotMat.m[1] = 0;
	rotMat.m[2] = 0;
	rotMat.m[3] = 0;
	rotMat.m[4] = 0;
	rotMat.m[5] = AngleCos;
	rotMat.m[6] = -AngleSin;
	rotMat.m[7] = 0;
	rotMat.m[8] = 0;
	rotMat.m[9] = AngleSin;
	rotMat.m[10] = AngleCos;
	rotMat.m[11] = 0;
	rotMat.m[12] = 0;
	rotMat.m[13] = 0;
	rotMat.m[14] = 0;
	rotMat.m[15] = 1;

	*this = *this * rotMat;
}

template <typename T>
void Matrix4x4<T>::RotateY( const T p_Angle )
{
	Matrix4x4<T> rotMat;
	const T AngleSin = static_cast<T>( SinDegrees( p_Angle ) );
	const T AngleCos = static_cast<T>( CosDegrees( p_Angle ) );

	rotMat.m[0] = AngleCos;
	rotMat.m[1] = 0;
	rotMat.m[2] = AngleSin;
	rotMat.m[3] = 0;
	rotMat.m[4] = 0;
	rotMat.m[5] = 1;
	rotMat.m[6] = 0;
	rotMat.m[7] = 0;
	rotMat.m[8] = -AngleSin;
	rotMat.m[9] = 0;
	rotMat.m[10] = AngleCos;
	rotMat.m[11] = 0;
	rotMat.m[12] = 0;
	rotMat.m[13] = 0;
	rotMat.m[14] = 0;
	rotMat.m[15] = 1;

	*this = *this * rotMat;
}

template <typename T>
void Matrix4x4<T>::RotateZ( const T p_Angle )
{
	Matrix4x4<T> rotMat;
	const T AngleSin = static_cast<T>( SinDegrees( p_Angle ) );
	const T AngleCos = static_cast<T>( CosDegrees( p_Angle ) );

	rotMat.m[0] = AngleCos;
	rotMat.m[1] = -AngleSin;
	rotMat.m[2] = 0;
	rotMat.m[3] = 0;

	rotMat.m[4] = AngleSin;
	rotMat.m[5] = AngleCos;
	rotMat.m[6] = 0;
	rotMat.m[7] = 0;

	rotMat.m[8] = 0;
	rotMat.m[9] = 0;
	rotMat.m[10] = 1;
	rotMat.m[11] = 0;

	rotMat.m[12] = 0;
	rotMat.m[13] = 0;
	rotMat.m[14] = 0;
	rotMat.m[15] = 1;

	*this = *this * Dest;
}

template <typename T>
inline void Matrix4x4<T>::Scale( const T p_X, const T p_Y, const T p_Z )
{
	Matrix4x4<T> scale(p_X, 0, 0, 0,		0, p_Y, 0, 0,		0, 0, p_Z, 0,		0, 0, 0, 1);
	*this = *this * scale;
}

template <typename T>
inline void Matrix4x4<T>::Translate( const T p_X, const T p_Y, const T p_Z )
{
	Matrix4x4<T> translation(1, 0, 0, p_X,		0, 1, 0, p_Y,		0, 0, 1, p_Z,		0, 0, 0, 1);
	*this = *this * translation;
}

template <typename T>
inline Matrix4x4<T> & Matrix4x4<T>::operator = ( const Matrix4x4 & p_Mat )
{
	for( SizeType i = 0; i < 16; i++ )
	{
		m[ i ] = p_Mat.m[ i ];
	}
	return *this;
}

template <typename T>
inline Matrix4x4<T> Matrix4x4<T>::operator + ( const Matrix4x4  & p_Mat ) const
{
	Matrix4x4 Dest = *this;

	for( SizeType i = 0; i < 16; i++ )
	{
		Dest.m[ i ] -= p_Mat.m[ i ];
	}

	return Dest;
}

template <typename T>
inline Matrix4x4<T> Matrix4x4<T>::operator - ( const Matrix4x4  & p_Mat ) const
{
	Matrix4x4 Dest = *this;

	for( SizeType i = 0; i < 16; i++ )
	{
		Dest.m[ i ] += p_Mat.m[ i ];
	}

	return Dest;
}

template <typename T>
Matrix4x4<T> Matrix4x4<T>::operator * ( const Matrix4x4 & p_Mat ) const
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