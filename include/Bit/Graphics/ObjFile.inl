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
T * ObjFile::CreatePositionBuffer(	SizeType & p_BufferSize,
									const Bool p_UseFlatFaces,
									const Bool p_UseSmoothFaces )
{
	// Check if any faces were requested.
	if( !p_UseFlatFaces && !p_UseFlatFaces )
	{
		p_BufferSize = 0;
		return NULL;
	}

	// Calculate the size of the buffer
	SizeType bufferSize = 0;

	// Find the size by going through all the elements of the obj structure.
	for( SizeType o = 0; o < m_Objects.size( ); o++ )
	{
		ObjFile::Object & object = *m_Objects[ o ];

		// Go through the object groups
		for( SizeType og = 0; og < object.m_ObjectGroups.size( ); og++ )
		{
			ObjectGroup & objectGroup = *object.m_ObjectGroups[ og ];

			// Go through the material groups
			for( SizeType mg = 0; mg < objectGroup.m_MaterialGroups.size( ); mg++ )
			{
				MaterialGroup & materialGroup = *objectGroup.m_MaterialGroups[ mg ];

				// Add the count of the faces
				if( p_UseFlatFaces )
				{
					bufferSize += static_cast<SizeType>( materialGroup.m_FlatFaces.size( ) );
				}

				if( p_UseSmoothFaces )
				{
					bufferSize += static_cast<SizeType>( materialGroup.m_SmoothFaces.size( ) );
				}

			}
			
		}
	}

	// Error check the final computed buffer size.
	if( bufferSize == 0 )
	{
		p_BufferSize = 0;
		return NULL;
	}

	// Multiply the buffer size by 3(we are using triangles)
	bufferSize *= 9;

	// Set the buffer size paramter
	p_BufferSize = bufferSize;

	// Let's create the buffer.
	T * pBuffer = new T[bufferSize];

	// Find the size by going through all the elements of the obj structure.
	SizeType currentIndex = 0;
	for( SizeType o = 0; o < m_Objects.size( ); o++ )
	{
		ObjFile::Object & object = *m_Objects[ o ];

		// Go through the object groups
		for( SizeType og = 0; og < object.m_ObjectGroups.size( ); og++ )
		{
			ObjectGroup & objectGroup = *object.m_ObjectGroups[ og ];

			// Go through the material groups
			for( SizeType mg = 0; mg < objectGroup.m_MaterialGroups.size( ); mg++ )
			{
				MaterialGroup & materialGroup = *objectGroup.m_MaterialGroups[ mg ];

				// Go throgu the flat and smooth faces
				if( p_UseFlatFaces )
				{
					// Go through the flat faces.
					for( SizeType f = 0; f < materialGroup.m_FlatFaces.size( ); f++ )
					{
						Face & face = *materialGroup.m_FlatFaces[ f ];

						// Error check the face. Make sure it's triangular.
						if( face.m_FaceCorners.size( ) != 3 )
						{
							delete [] pBuffer;
							p_BufferSize = 0;
							return NULL;
						}

						// Go throguh the face corners
						for( SizeType fc = 0; fc < 3; fc++ )
						{
							// Get the current corner
							FaceCorner & faceCorner = face.m_FaceCorners[ fc ];

							// Get the vertex index.
							Int32 vertexIndex = faceCorner.VertexIndex - 1;

							// Error check the corner
							if( vertexIndex < 0 || vertexIndex >= object.m_Vertices.size( ) )
							{
								std::cout << "[Model::CreatePositionBuffer] Vertex index error:" << vertexIndex << std::endl;
				
								// Error, return NULL.
								delete [] pBuffer;
								p_BufferSize = 0;
								return NULL;
							}

							// Get the vertex coordinate.
							const Vector3f32 position = object.GetVertex( vertexIndex );

							// Set the buffer data
							pBuffer[ currentIndex ]		= static_cast<T>( position.x );
							pBuffer[ currentIndex + 1 ]	= static_cast<T>( position.y );
							pBuffer[ currentIndex + 2 ]	= static_cast<T>( position.z );
							currentIndex += 3;
						}
					}
					
				}
				if( p_UseSmoothFaces )
				{
					// Go through the flat faces.
					for( SizeType f = 0; f < materialGroup.m_SmoothFaces.size( ); f++ )
					{
						Face & face = *materialGroup.m_SmoothFaces[ f ];

						// Error check the face. Make sure it's triangular.
						if( face.m_FaceCorners.size( ) != 3 )
						{
							delete [] pBuffer;
							p_BufferSize = 0;
							return NULL;
						}

						// Go throguh the face corners
						for( SizeType fc = 0; fc < 3; fc++ )
						{
							// Get the current corner
							FaceCorner & faceCorner = face.m_FaceCorners[ fc ];

							// Get the vertex index.
							Int32 vertexIndex = faceCorner.VertexIndex - 1;

							// Error check the corner
							if( vertexIndex < 0 || vertexIndex >= object.m_Vertices.size( ) )
							{
								std::cout << "[Model::CreatePositionBuffer] Vertex index error:" << vertexIndex << std::endl;
				
								// Error, return NULL.
								delete [] pBuffer;
								p_BufferSize = 0;
								return NULL;
							}

							// Get the vertex coordinate.
							const Vector3f32 position = object.GetVertex( vertexIndex );

							// Set the buffer data
							pBuffer[ currentIndex ]		= static_cast<T>( position.x );
							pBuffer[ currentIndex + 1 ]	= static_cast<T>( position.y );
							pBuffer[ currentIndex + 2 ]	= static_cast<T>( position.z );
							currentIndex += 3;
						}
					}
				}

			}
			
		}
	}




	// return the buffer
	return pBuffer;
}

/*
template <typename T>
inline T Math::DegreesToRadians( const T p_Degrees)
{
	return p_Degrees * static_cast<T>( Pi / 180.0f );
}
*/