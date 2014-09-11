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

template <typename T>
T * Md2File::CreatePositionBuffer(	SizeType & p_BufferSize,
									const SizeType p_FrameIndex )
{
	// Set the buffer size paramter to 0, if anything fail.
	p_BufferSize = 0;

	// Error check the frame index
	if( p_FrameIndex >= static_cast<SizeType>( m_Frames.size( ) ) )
	{
		return NULL;
	}

	// Get the frame
	Frame * pFrame = m_Frames[ p_FrameIndex ];

	// Calculate the buffer size
	SizeType bufferSize = static_cast<SizeType>( m_Triangles.size( ) ) * 9;

	// Error check the buffer size
	if( bufferSize == 0 )
	{
		return NULL;
	}

	// Create the buffer
	T * pBuffer = new T[ bufferSize ];

	// Go throguh the triangles and calculate the right vertex coordinate
	SizeType currentPos = 0;
	for( TriangleVector::size_type t = 0; t < m_Triangles.size( ); t++ )
	{
		for( SizeType v = 0; v < 3; v++ )
		{
			pBuffer[ currentPos++ ] = static_cast<T>( pFrame->Vertices[ m_Triangles[ t ]->VertexIndex[ v ] ]->Position.x );
			pBuffer[ currentPos++ ] = static_cast<T>( pFrame->Vertices[ m_Triangles[ t ]->VertexIndex[ v ] ]->Position.y );
			pBuffer[ currentPos++ ] = static_cast<T>( pFrame->Vertices[ m_Triangles[ t ]->VertexIndex[ v ] ]->Position.z );
		}

	}

	 // Set the buffer size parameter p_BufferSize
	p_BufferSize = bufferSize;

	// Return the buffer
	return pBuffer;
}

template <typename T>
T * Md2File::CreateNormalBuffer(	SizeType & p_BufferSize,
										const SizeType p_FrameIndex )
{
	// Set the buffer size paramter to 0, if anything fail.
	p_BufferSize = 0;

	// Error check the frame index
	if( p_FrameIndex >= static_cast<SizeType>( m_Frames.size( ) ) )
	{
		return NULL;
	}

	// Get the frame
	Frame * pFrame = m_Frames[ p_FrameIndex ];

	// Calculate the buffer size
	SizeType bufferSize = static_cast<SizeType>( m_Triangles.size( ) ) * 9;

	// Error check the buffer size
	if( bufferSize == 0 )
	{
		return NULL;
	}

	// Create the buffer
	T * pBuffer = new T[ bufferSize ];

	// Go throguh the triangles and calculate the right texture coordinate
	SizeType currentPos = 0;
	for( TriangleVector::size_type t = 0; t < m_Triangles.size( ); t++ )
	{
		for( SizeType v = 0; v < 3; v++ )
		{
			pBuffer[ currentPos++ ] = static_cast<T>( pFrame->Vertices[ m_Triangles[ t ]->VertexIndex[ v ] ]->Normal.x );
			pBuffer[ currentPos++ ] = static_cast<T>( pFrame->Vertices[ m_Triangles[ t ]->VertexIndex[ v ] ]->Normal.y );
			pBuffer[ currentPos++ ] = static_cast<T>( pFrame->Vertices[ m_Triangles[ t ]->VertexIndex[ v ] ]->Normal.z );
		}

	}

	 // Set the buffer size parameter p_BufferSize
	p_BufferSize = bufferSize;

	// Return the buffer
	return pBuffer;
}