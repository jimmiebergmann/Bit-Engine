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

#include <Bit/Graphics/Md2File.hpp>
#include <fstream>
#include <sstream>
#include <Bit/System/Log.hpp>
#include <string.h>
#include <Bit/System/MemoryLeak.hpp>

// Sizes in bytes.
#define MD2_MAX_SKINS				32
#define MD2_MAX_TEXCOORDS			2048
#define MD2_MAX_TEXCOORDS_SIZE		MD2_MAX_TEXCOORDS * 12
#define MD2_MAX_VERTICES			2048
#define MD2_MAX_VERTICES_SIZE		MD2_MAX_VERTICES * 12
#define MD2_MAX_TRIANGLES			4096
#define MD2_MAX_TRIANGLES_SIZE		MD2_MAX_TRIANGLES * 36
#define MD2_MAX_FRAMES				512
#define MD2_MAX_FRAME_SIZE			MD2_MAX_VERTICES_SIZE + 128
#define MD2_NORMAL_COUNT			162

namespace Bit
{
	
	// MD2 precalculated normals
	static const Float32 g_Md2Normals[ MD2_NORMAL_COUNT ][ 3 ] =
	{
		{ -0.525731f,  0.000000f,  0.850651f }, 
		{ -0.442863f,  0.238856f,  0.864188f }, 
		{ -0.295242f,  0.000000f,  0.955423f }, 
		{ -0.309017f,  0.500000f,  0.809017f }, 
		{ -0.162460f,  0.262866f,  0.951056f }, 
		{  0.000000f,  0.000000f,  1.000000f }, 
		{  0.000000f,  0.850651f,  0.525731f }, 
		{ -0.147621f,  0.716567f,  0.681718f }, 
		{  0.147621f,  0.716567f,  0.681718f }, 
		{  0.000000f,  0.525731f,  0.850651f }, 
		{  0.309017f,  0.500000f,  0.809017f }, 
		{  0.525731f,  0.000000f,  0.850651f }, 
		{  0.295242f,  0.000000f,  0.955423f }, 
		{  0.442863f,  0.238856f,  0.864188f }, 
		{  0.162460f,  0.262866f,  0.951056f }, 
		{ -0.681718f,  0.147621f,  0.716567f }, 
		{ -0.809017f,  0.309017f,  0.500000f }, 
		{ -0.587785f,  0.425325f,  0.688191f }, 
		{ -0.850651f,  0.525731f,  0.000000f }, 
		{ -0.864188f,  0.442863f,  0.238856f }, 
		{ -0.716567f,  0.681718f,  0.147621f }, 
		{ -0.688191f,  0.587785f,  0.425325f }, 
		{ -0.500000f,  0.809017f,  0.309017f }, 
		{ -0.238856f,  0.864188f,  0.442863f }, 
		{ -0.425325f,  0.688191f,  0.587785f }, 
		{ -0.716567f,  0.681718f, -0.147621f }, 
		{ -0.500000f,  0.809017f, -0.309017f }, 
		{ -0.525731f,  0.850651f,  0.000000f }, 
		{  0.000000f,  0.850651f, -0.525731f }, 
		{ -0.238856f,  0.864188f, -0.442863f }, 
		{  0.000000f,  0.955423f, -0.295242f }, 
		{ -0.262866f,  0.951056f, -0.162460f }, 
		{  0.000000f,  1.000000f,  0.000000f }, 
		{  0.000000f,  0.955423f,  0.295242f }, 
		{ -0.262866f,  0.951056f,  0.162460f }, 
		{  0.238856f,  0.864188f,  0.442863f }, 
		{  0.262866f,  0.951056f,  0.162460f }, 
		{  0.500000f,  0.809017f,  0.309017f }, 
		{  0.238856f,  0.864188f, -0.442863f }, 
		{  0.262866f,  0.951056f, -0.162460f }, 
		{  0.500000f,  0.809017f, -0.309017f }, 
		{  0.850651f,  0.525731f,  0.000000f }, 
		{  0.716567f,  0.681718f,  0.147621f }, 
		{  0.716567f,  0.681718f, -0.147621f }, 
		{  0.525731f,  0.850651f,  0.000000f }, 
		{  0.425325f,  0.688191f,  0.587785f }, 
		{  0.864188f,  0.442863f,  0.238856f }, 
		{  0.688191f,  0.587785f,  0.425325f }, 
		{  0.809017f,  0.309017f,  0.500000f }, 
		{  0.681718f,  0.147621f,  0.716567f }, 
		{  0.587785f,  0.425325f,  0.688191f }, 
		{  0.955423f,  0.295242f,  0.000000f }, 
		{  1.000000f,  0.000000f,  0.000000f }, 
		{  0.951056f,  0.162460f,  0.262866f }, 
		{  0.850651f, -0.525731f,  0.000000f }, 
		{  0.955423f, -0.295242f,  0.000000f }, 
		{  0.864188f, -0.442863f,  0.238856f }, 
		{  0.951056f, -0.162460f,  0.262866f }, 
		{  0.809017f, -0.309017f,  0.500000f }, 
		{  0.681718f, -0.147621f,  0.716567f }, 
		{  0.850651f,  0.000000f,  0.525731f }, 
		{  0.864188f,  0.442863f, -0.238856f }, 
		{  0.809017f,  0.309017f, -0.500000f }, 
		{  0.951056f,  0.162460f, -0.262866f }, 
		{  0.525731f,  0.000000f, -0.850651f }, 
		{  0.681718f,  0.147621f, -0.716567f }, 
		{  0.681718f, -0.147621f, -0.716567f }, 
		{  0.850651f,  0.000000f, -0.525731f }, 
		{  0.809017f, -0.309017f, -0.500000f }, 
		{  0.864188f, -0.442863f, -0.238856f }, 
		{  0.951056f, -0.162460f, -0.262866f }, 
		{  0.147621f,  0.716567f, -0.681718f }, 
		{  0.309017f,  0.500000f, -0.809017f }, 
		{  0.425325f,  0.688191f, -0.587785f }, 
		{  0.442863f,  0.238856f, -0.864188f }, 
		{  0.587785f,  0.425325f, -0.688191f }, 
		{  0.688191f,  0.587785f, -0.425325f }, 
		{ -0.147621f,  0.716567f, -0.681718f }, 
		{ -0.309017f,  0.500000f, -0.809017f }, 
		{  0.000000f,  0.525731f, -0.850651f }, 
		{ -0.525731f,  0.000000f, -0.850651f }, 
		{ -0.442863f,  0.238856f, -0.864188f }, 
		{ -0.295242f,  0.000000f, -0.955423f }, 
		{ -0.162460f,  0.262866f, -0.951056f }, 
		{  0.000000f,  0.000000f, -1.000000f }, 
		{  0.295242f,  0.000000f, -0.955423f }, 
		{  0.162460f,  0.262866f, -0.951056f }, 
		{ -0.442863f, -0.238856f, -0.864188f }, 
		{ -0.309017f, -0.500000f, -0.809017f }, 
		{ -0.162460f, -0.262866f, -0.951056f }, 
		{  0.000000f, -0.850651f, -0.525731f }, 
		{ -0.147621f, -0.716567f, -0.681718f }, 
		{  0.147621f, -0.716567f, -0.681718f }, 
		{  0.000000f, -0.525731f, -0.850651f }, 
		{  0.309017f, -0.500000f, -0.809017f }, 
		{  0.442863f, -0.238856f, -0.864188f }, 
		{  0.162460f, -0.262866f, -0.951056f }, 
		{  0.238856f, -0.864188f, -0.442863f }, 
		{  0.500000f, -0.809017f, -0.309017f }, 
		{  0.425325f, -0.688191f, -0.587785f }, 
		{  0.716567f, -0.681718f, -0.147621f }, 
		{  0.688191f, -0.587785f, -0.425325f }, 
		{  0.587785f, -0.425325f, -0.688191f }, 
		{  0.000000f, -0.955423f, -0.295242f }, 
		{  0.000000f, -1.000000f,  0.000000f }, 
		{  0.262866f, -0.951056f, -0.162460f }, 
		{  0.000000f, -0.850651f,  0.525731f }, 
		{  0.000000f, -0.955423f,  0.295242f }, 
		{  0.238856f, -0.864188f,  0.442863f }, 
		{  0.262866f, -0.951056f,  0.162460f }, 
		{  0.500000f, -0.809017f,  0.309017f }, 
		{  0.716567f, -0.681718f,  0.147621f }, 
		{  0.525731f, -0.850651f,  0.000000f }, 
		{ -0.238856f, -0.864188f, -0.442863f }, 
		{ -0.500000f, -0.809017f, -0.309017f }, 
		{ -0.262866f, -0.951056f, -0.162460f }, 
		{ -0.850651f, -0.525731f,  0.000000f }, 
		{ -0.716567f, -0.681718f, -0.147621f }, 
		{ -0.716567f, -0.681718f,  0.147621f }, 
		{ -0.525731f, -0.850651f,  0.000000f }, 
		{ -0.500000f, -0.809017f,  0.309017f }, 
		{ -0.238856f, -0.864188f,  0.442863f }, 
		{ -0.262866f, -0.951056f,  0.162460f }, 
		{ -0.864188f, -0.442863f,  0.238856f }, 
		{ -0.809017f, -0.309017f,  0.500000f }, 
		{ -0.688191f, -0.587785f,  0.425325f }, 
		{ -0.681718f, -0.147621f,  0.716567f }, 
		{ -0.442863f, -0.238856f,  0.864188f }, 
		{ -0.587785f, -0.425325f,  0.688191f }, 
		{ -0.309017f, -0.500000f,  0.809017f }, 
		{ -0.147621f, -0.716567f,  0.681718f }, 
		{ -0.425325f, -0.688191f,  0.587785f }, 
		{ -0.162460f, -0.262866f,  0.951056f }, 
		{  0.442863f, -0.238856f,  0.864188f }, 
		{  0.162460f, -0.262866f,  0.951056f }, 
		{  0.309017f, -0.500000f,  0.809017f }, 
		{  0.147621f, -0.716567f,  0.681718f }, 
		{  0.000000f, -0.525731f,  0.850651f }, 
		{  0.425325f, -0.688191f,  0.587785f }, 
		{  0.587785f, -0.425325f,  0.688191f }, 
		{  0.688191f, -0.587785f,  0.425325f }, 
		{ -0.955423f,  0.295242f,  0.000000f }, 
		{ -0.951056f,  0.162460f,  0.262866f }, 
		{ -1.000000f,  0.000000f,  0.000000f }, 
		{ -0.850651f,  0.000000f,  0.525731f }, 
		{ -0.955423f, -0.295242f,  0.000000f }, 
		{ -0.951056f, -0.162460f,  0.262866f }, 
		{ -0.864188f,  0.442863f, -0.238856f }, 
		{ -0.951056f,  0.162460f, -0.262866f }, 
		{ -0.809017f,  0.309017f, -0.500000f }, 
		{ -0.864188f, -0.442863f, -0.238856f }, 
		{ -0.951056f, -0.162460f, -0.262866f }, 
		{ -0.809017f, -0.309017f, -0.500000f }, 
		{ -0.681718f,  0.147621f, -0.716567f }, 
		{ -0.681718f, -0.147621f, -0.716567f }, 
		{ -0.850651f,  0.000000f, -0.525731f }, 
		{ -0.688191f,  0.587785f, -0.425325f }, 
		{ -0.587785f,  0.425325f, -0.688191f }, 
		{ -0.425325f,  0.688191f, -0.587785f }, 
		{ -0.425325f, -0.688191f, -0.587785f }, 
		{ -0.587785f, -0.425325f, -0.688191f }, 
		{ -0.688191f, -0.587785f, -0.425325f }
	};


	// Alias structures

	//Compressed alias vertex structure.
	struct AliasVertex
	{
		Uint8 Position[ 3 ];
		Uint8 NormalIndex;
	};

	// Compressed alias frame structure.
	struct AliasFrame
	{
		// Typedefs
		typedef std::vector<AliasVertex *> AliasVertexVector;
			
		// Variables
		Vector3f32 Scale;					///< Scale factor.
		Vector3f32 Translate;				///< Translation vector .
		char Name[ 16 ];					///< Name of the frame.
		AliasVertexVector AliasVertices;	///< Vector of alias vertices.
	};


	// Md2 class
	Md2File::Md2File( )
	{
	}

	Md2File::~Md2File( )
	{
		Clear( );
	}

	Bool Md2File::LoadFromMemory( const std::string & p_Memory )
	{
		// Load a string stream
		std::stringstream ss;
		ss.rdbuf( )->pubsetbuf( const_cast<char*>( p_Memory.c_str( ) ), p_Memory.size( ) );

		// Read the stream
		Bool status = LoadFromStream( ss );

		// Return the status
		return status;
	}

	Bool Md2File::LoadFromStream( std::istream & p_Stream )
	{
		// Clear the old data
		Clear( );

		// Get the stream size
		p_Stream.seekg( 0, std::istream::end );
		SizeType streamSize = p_Stream.tellg( );
		p_Stream.seekg( 0, std::istream::beg );

		// Error check the stream size, make sure the header fits.
		if( streamSize < sizeof( m_Header ) )
		{
			BitLog::NewEngine(Log::Error) << "No valid header were found." << Log::End;
			return false;
		}

		// Read the header
		p_Stream.read( reinterpret_cast<char*>( &m_Header ), sizeof( m_Header ) );

		// Error check the data
		if( m_Header.MagicNumber != 844121161 )
		{
			BitLog::NewEngine(Log::Error) << "Wrong magic number: " << m_Header.MagicNumber << ". Expecting: 844121161." << Log::End;
			return false;
		}
		if( m_Header.Version != 8 )
		{
			BitLog::NewEngine(Log::Error) << "Wrong version: " << m_Header.Version << ". Expecting: 8 " << Log::End;
			return false;
		}
		if( m_Header.TriangleCount <= 0 || m_Header.TriangleCount >= 65536 )
		{
			BitLog::NewEngine(Log::Error) << "Triagnle count error: " << m_Header.TriangleCount << Log::End;
			return false;
		}
		if( m_Header.FrameCount <= 0 )
		{
			BitLog::NewEngine(Log::Error) << "Frame count error: " << m_Header.FrameCount << Log::End;
			return false;
		}
		if( m_Header.OffsetSkins > streamSize		|| m_Header.OffsetTexCoords > streamSize ||
			m_Header.OffsetTriangles > streamSize	|| m_Header.OffsetFrames > streamSize ||
			m_Header.OffsetOpenGLCmds > streamSize )
		{
			BitLog::NewEngine(Log::Error) << "Offset error." << Log::End;
			return false;
		}

		// Error check the total stream size.
		SizeType totalMd2Data = sizeof( m_Header );
		totalMd2Data += m_Header.SkinCount * sizeof( Skin );
		totalMd2Data += m_Header.TexCoordCount * sizeof( TextureCoord );
		totalMd2Data += m_Header.TriangleCount * sizeof( Triangle );
		totalMd2Data += m_Header.FrameCount * ( 40 + ( m_Header.VertexCount * sizeof( AliasVertex ) ) );

		if( totalMd2Data > streamSize )
		{
			BitLog::NewEngine(Log::Error) << "Stream size is too small." << Log::End;
			return false;
		}

		// Read skin information
		p_Stream.seekg( m_Header.OffsetSkins, std::istream::beg );

		// Go throguh skins
		for( Int32 i = 0; i < m_Header.SkinCount; i++ )
		{
			// Create and add a new skin
			Skin * pSkin = new Skin;
			m_Skins.push_back( pSkin );

			// Read the skin name
			p_Stream.read( pSkin->Name, sizeof( pSkin->Name ) );
		}

		// Read the texture coordinates
		p_Stream.seekg( m_Header.OffsetTexCoords, std::istream::beg );
		m_TextureCoords.reserve( m_Header.TexCoordCount );
		
		// Go throguh texture coords.
		for( Int32 i = 0; i < m_Header.TexCoordCount; i++ )
		{
			// Create and add a new texture coordinate
			TextureCoord * pTextureCoord = new TextureCoord;
			pTextureCoord->s = 0;
			pTextureCoord->t = 0;
			m_TextureCoords.push_back( pTextureCoord );

			// Read the texture coord.
			p_Stream.read( reinterpret_cast<char*>( &pTextureCoord->s ), sizeof( Int16 ) );
			p_Stream.read( reinterpret_cast<char*>( &pTextureCoord->t ), sizeof( Int16 ) );
		}

		// Read the triangles
		p_Stream.seekg( m_Header.OffsetTriangles, std::istream::beg );
		m_Triangles.reserve( m_Header.TriangleCount );
		
		// Go throguh triangles.
		for( Int32 i = 0; i < m_Header.TriangleCount; i++ )
		{
			// Create and add a new triangle
			Triangle * pTriangle = new Triangle;
			m_Triangles.push_back( pTriangle );

			// Read the texture coord.
			p_Stream.read( reinterpret_cast<char*>( pTriangle ), sizeof( Triangle ) );
		}

		// Read the frames
		p_Stream.seekg( m_Header.OffsetFrames, std::istream::beg );
		m_Frames.reserve( m_Header.FrameCount );

		// Use a temporary buffer for reading the alias vertices
		Uint8 aliasVertexBuffer[ MD2_MAX_VERTICES_SIZE ];
		AliasVertex * pAliasVertices = reinterpret_cast<AliasVertex*>( aliasVertexBuffer );

		// Go throguh frames.
		for( Int32 i = 0; i < m_Header.FrameCount; i++ )
		{
			// Create and add a new frame
			Frame * pFrame = new Frame;
			pFrame->Vertices.reserve( m_Header.VertexCount );
			m_Frames.push_back( pFrame );

			// Read the frame data
			p_Stream.read( reinterpret_cast<char*>( &pFrame->Scale ), sizeof( Vector3f32 ) );
			p_Stream.read( reinterpret_cast<char*>( &pFrame->Translate ), sizeof( Vector3f32 ) );
			p_Stream.read( reinterpret_cast<char*>( pFrame->Name ), sizeof( pFrame->Name ) );

			// Read temporary alias vertices
			p_Stream.read( reinterpret_cast<char*>( pAliasVertices ), sizeof( AliasVertex ) * m_Header.VertexCount );

			// Calculate the real vertex coordinates
			for( SizeType j = 0; j < m_Header.VertexCount; j++ )
			{
				Vertex * pVertex = new Vertex;
				pFrame->Vertices.push_back( pVertex );

				// Calculate the position from the alias postion, scale and translate.
				pVertex->Position.x = static_cast<Float32>( pAliasVertices[ j ].Position[ 0 ] ) * pFrame->Scale.x + pFrame->Translate.x;
				pVertex->Position.z = -1.0f * ( static_cast<Float32>( pAliasVertices[ j ].Position[ 1 ] ) * pFrame->Scale.y + pFrame->Translate.y );
				pVertex->Position.y = static_cast<Float32>( pAliasVertices[ j ].Position[ 2 ] ) * pFrame->Scale.z + pFrame->Translate.z;
			
				// Get the precalculated normal
				if( pAliasVertices[ j ].NormalIndex >= MD2_NORMAL_COUNT )
				{
					return false;
				}

				pVertex->Normal.x = g_Md2Normals[ pAliasVertices[ j ].NormalIndex ][ 0 ];
				pVertex->Normal.z = -1.0f * g_Md2Normals[ pAliasVertices[ j ].NormalIndex ][ 1 ];
				pVertex->Normal.y = g_Md2Normals[ pAliasVertices[ j ].NormalIndex ][ 2 ];
			}
		}

		// Succeeded.
		return true;
	}

	Bool Md2File::LoadFromFile( const std::string & p_Filename )
	{
		// Open the file.
		std::ifstream fin( p_Filename.c_str( ), std::ifstream::binary );
		if( fin.is_open( ) == false )
		{
			BitLog::NewEngine(Log::Error) << "Can not open the file." << Log::End;
			return false;
		}

		Bool status = LoadFromStream( fin );
		fin.close( );

		// Return the status
		return status;
	}

	Bool Md2File::SaveToMemory( std::string & p_Memory, const Bool p_Validate )
	{
		// Load a string stream
		std::stringstream ss;

		// Save the stream
		if( SaveToStream( ss, p_Validate ) == false )
		{
			return false;
		}

		// Get the string
		p_Memory = ss.str( );

		// Succeeded
		return true;
	}

	Bool Md2File::SaveToStream( std::ostream & p_Stream, const Bool p_Validate )
	{

		return false;
	}

	Bool Md2File::SaveToFile( const std::string & p_Filename, const Bool p_Validate )
	{
		// Load a string stream.
		std::stringstream ss;

		// Save the stream.
		if( SaveToStream( ss, p_Validate ) == false )
		{
			return false;
		}

		// Open the file.
		std::ofstream fout( p_Filename.c_str( ), std::fstream::binary );
		if( fout.is_open( ) == false )
		{
			BitLog::NewEngine(Log::Error) << "Can not open the file." << Log::End;
			return false;
		}

		// Write the string stream to the file
		fout.write( ss.str( ).c_str( ), ss.str( ).length( ) );

		// Close the file.
		fout.close( );

		// Succeeded.
		return true;
	}

	void Md2File::Clear( )
	{
		// Clear skins
		for( SkinVector::size_type i = 0; i < m_Skins.size( ); i++ )
		{
			delete m_Skins[ i ];
		}
		m_Skins.clear( );

		// Clear texture coords
		for( SkinVector::size_type i = 0; i < m_TextureCoords.size( ); i++ )
		{
			delete m_TextureCoords[ i ];
		}
		m_TextureCoords.clear( );

		// Clear triangles
		for( SkinVector::size_type i = 0; i < m_Triangles.size( ); i++ )
		{
			delete m_Triangles[ i ];
		}
		m_Triangles.clear( );

		// Clear frames
		for( FrameVector::size_type i = 0; i < m_Frames.size( ); i++ )
		{
			// Clear vertices
			for( Frame::VertexVector::size_type j = 0; j < m_Frames[ i ]->Vertices.size( ); j++ )
			{
				delete m_Frames[ i ]->Vertices[ j ];
			}

			delete m_Frames[ i ];
		}
		m_Frames.clear( );
	}

	Md2File::Header & Md2File::GetHeader( )
	{
		return m_Header;
	}

	SizeType Md2File::GetSkinCount( ) const
	{
		return static_cast<SizeType>( m_Skins.size( ) );
	}

	Md2File::Skin * Md2File::GetSkin( const SizeType p_Index ) const
	{
		if( p_Index >= static_cast<SizeType>( m_Skins.size( ) ) )
		{
			return NULL;
		}

		return m_Skins[ p_Index ];
	}

	SizeType Md2File::GetTexureCoordCount( ) const
	{
		return static_cast<SizeType>( m_TextureCoords.size( ) );
	}

	Md2File::TextureCoord * Md2File::GetTextureCoord( const SizeType p_Index ) const
	{
		if( p_Index >= static_cast<SizeType>( m_TextureCoords.size( ) ) )
		{
			return NULL;
		}

		return m_TextureCoords[ p_Index ];
	}

	SizeType Md2File::GetTriangleCount( ) const
	{
		return static_cast<SizeType>( m_Triangles.size( ) );
	}

	Md2File::Triangle * Md2File::GetTriangle( const SizeType p_Index ) const
	{
		if( p_Index >= static_cast<SizeType>( m_Triangles.size( ) ) )
		{
			return NULL;
		}

		return m_Triangles[ p_Index ];
	}

	SizeType Md2File::GetFrameCount( ) const
	{
		return static_cast<SizeType>( m_Frames.size( ) );
	}

	Md2File::Frame * Md2File::GetFrame( const SizeType p_Index ) const
	{
		if( p_Index >= static_cast<SizeType>( m_Frames.size( ) ) )
		{
			return NULL;
		}

		return m_Frames[ p_Index ];
	}

}