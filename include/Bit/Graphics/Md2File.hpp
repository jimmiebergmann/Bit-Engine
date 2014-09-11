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

#ifndef BIT_GRAPHICS_MD2_FILE_HPP
#define BIT_GRAPHICS_MD2_FILE_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/System/Vector2.hpp>
#include <string>
#include <vector>


namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Obj model file class.
	///
	/// The internal structure is pretty much like the file format itself.
	/// NOTE: Should the ObjectGroup class be called VertexGroup instead...?
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Md2File : public NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Header structure.
		///
		////////////////////////////////////////////////////////////////
		struct Header
		{
			Int32 MagicNumber;			///< magic number: "IDP2".
			Int32 Version;				///< version: must be 8.

			Int32 SkinWidth;			///< texture width.
			Int32 SkinHeight;			///< texture height.

			Int32 FrameSize;			///< size in bytes of a frame.

			Int32 SkinCount;			///< number of skins.
			Int32 VertexCount;			///< number of vertices per frame.
			Int32 TexCoordCount;		///< number of texture coordinates.
			Int32 TriangleCount;		///< number of triangles.
			Int32 OpenGLCmdsCount;		///< number of opengl commands.
			Int32 FrameCount;			///< number of frames.

			Int32 OffsetSkins;			///< offset skin data.
			Int32 OffsetTexCoords;		///< offset texture coordinate data.
			Int32 OffsetTriangles;		///< offset triangle data.
			Int32 OffsetFrames;			///< offset frame data.
			Int32 OffsetOpenGLCmds;		///< offset OpenGL command data.
			Int32 OffsetEnd;			///< offset end of file.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Skin structure.
		///
		////////////////////////////////////////////////////////////////
		struct Skin
		{
			char Name[ 64 ];	///< Name of the skin
		};

		////////////////////////////////////////////////////////////////
		/// \brief Texture coord structure.
		///
		////////////////////////////////////////////////////////////////
		struct TextureCoord
		{
			Int16 s;	///< S(x) Component.
			Int16 t;	///< Y(y) Component.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Triangle structure.
		///
		////////////////////////////////////////////////////////////////
		struct Triangle
		{
			Uint16 VertexIndex[ 3 ];		///< Vertex indices.
			Uint16 TextureCoordIndex[ 3 ];	///< Texture coordinate indices.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Vertex structure
		///
		////////////////////////////////////////////////////////////////
		struct Vertex
		{
			Vector3f32 Position;	///< Vertex position.
			Vector3f32 Normal;		///< Normal.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Frame structure.
		///
		////////////////////////////////////////////////////////////////
		struct Frame
		{
			// Typedefs
			typedef std::vector<Vertex *> VertexVector;
			
			// Variables
			Vector3f32 Scale;		///< Scale factor.
			Vector3f32 Translate;	///< Translation vector.
			char Name[ 16 ];		///< Name of the frame.
			VertexVector Vertices;	///< Vector of vertices.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////////
		Md2File( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		/// Clears the data.
		///
		////////////////////////////////////////////////////////////////
		~Md2File( );

		////////////////////////////////////////////////////////////////
		/// \brief Load Md2 from memory.
		///
		/// \param p_pMemory Pointer to the memory.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromMemory( const std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Load Md2 file from stream.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromStream( std::istream & p_Stream );

		////////////////////////////////////////////////////////////////
		/// \brief Load Md2 from file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Save Md2 to memory.
		///
		/// \param p_pMemory	Reference to the memory.
		/// \param p_Validate	If true, makes sure to save a valid Md2 file.
		///						Setting default values for invalid data in the Md2 structure.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToMemory( std::string & p_Memory, const Bool p_Validate = true );

		////////////////////////////////////////////////////////////////
		/// \brief Save Md2 to stream.
		///
		/// \param p_Stream		Output stream.
		/// \param p_Validate	If true, makes sure to save a valid Md2 file.
		///						Setting default values for invalid data in the Md2 structure.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToStream( std::ostream & p_Stream, const Bool p_Validate = true );

		////////////////////////////////////////////////////////////////
		/// \brief Save Md2 to file.
		///
		/// \param p_Filename Output file.
		/// \param p_Validate	If true, makes sure to save a valid MD2 file.
		///						Setting default values for invalid data in the MD2 structure.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToFile( const std::string & p_Filename, const Bool p_Validate = true );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the Md2 data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Get header.
		///
		////////////////////////////////////////////////////////////////
		Header & GetHeader( );

		////////////////////////////////////////////////////////////////
		/// \brief	Get skin count. Use this function instead of
		///			the size value found in the header stucture.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetSkinCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get skin.
		///
		/// \param p_Index Index of the skin.
		///
		/// \return NULL if index is invalid, else pointer to the requested skin.
		///
		////////////////////////////////////////////////////////////////
		Skin * GetSkin( const SizeType p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get texture coord count. Use this function instead of
		///			the size value found in the header stucture.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetTexureCoordCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get texture coordinate.
		///
		/// \param p_Index Index of the texture coordinate.
		///
		/// \return NULL if index is invalid, else pointer to the requested skin.
		///
		////////////////////////////////////////////////////////////////
		TextureCoord * GetTextureCoord( const SizeType p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get Triangle count. Use this function instead of
		///			the size value found in the header stucture.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetTriangleCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get triangle.
		///
		/// \param p_Index Index of the triangle.
		///
		/// \return NULL if index is invalid, else pointer to the requested skin.
		///
		////////////////////////////////////////////////////////////////
		Triangle * GetTriangle( const SizeType p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get frame count. Use this function instead of
		///			the size value found in the header stucture.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetFrameCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief	Get frame.
		///
		/// \param p_Index Index of the frame.
		///
		/// \return NULL if index is invalid, else pointer to the requested skin.
		///
		////////////////////////////////////////////////////////////////
		Frame * GetFrame( const SizeType p_Index ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Help function for creating a postion buffer
		///			from the current stored data.
		///
		/// Make sure to delete the buffer when you are done with it.
		///
		/// \param p_BufferSize Size of the return buffer in bytes / size of data type(T).
		///		0 if the buffer is invalid(NULL).
		/// \param p_FrameIndex Index of the frame to obtain data from.
		///
		/// \return NULL if it's not possible to create a buffer,
		///			else a pointer to the buffer.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		T * CreatePositionBuffer(	SizeType & p_BufferSize,
									const SizeType p_FrameIndex = 0 );

		////////////////////////////////////////////////////////////////
		/// \brief Help function for creating a texture coordinate buffer
		///			from the current stored data.
		///
		/// Make sure to delete the buffer when you are done with it.
		///
		/// \param p_BufferSize Size of the return buffer in bytes / size of data type(T).
		///		0 if the buffer is invalid(NULL).
		/// \param p_FrameIndex Index of the frame to obtain data from.
		///
		/// \return NULL if it's not possible to create a buffer,
		///			else a pointer to the buffer.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		T * CreateTextureCoordBuffer(	SizeType & p_BufferSize );

		////////////////////////////////////////////////////////////////
		/// \brief Help function for creating a normal buffer
		///			from the current stored data.
		///
		/// Make sure to delete the buffer when you are done with it.
		///
		/// \param p_BufferSize Size of the return buffer in bytes / size of data type(T).
		///		0 if the buffer is invalid(NULL).
		/// \param p_FrameIndex Index of the frame to obtain data from.
		///
		/// \return NULL if it's not possible to create a buffer,
		///			else a pointer to the buffer.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		T * CreateNormalBuffer(	SizeType & p_BufferSize,
								const SizeType p_FrameIndex = 0 );

	private:

		// Private typedefs
		typedef std::vector<Skin *> SkinVector;
		typedef std::vector<TextureCoord *> TextureCoordVector;
		typedef std::vector<Triangle *> TriangleVector;
		typedef std::vector<Frame *> FrameVector;

		// Private variables
		Header m_Header;					///< Header information.
		SkinVector m_Skins;					///< Skin data.
		TextureCoordVector m_TextureCoords;	///< Texture coordinates.
		TriangleVector m_Triangles;			///< Triangle data.
		FrameVector m_Frames;				///< Frames.

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/Graphics/Md2File.inl>

}

#endif