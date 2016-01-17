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

#ifndef BIT_GRAPHICS_OBJ_FILE_HPP
#define BIT_GRAPHICS_OBJ_FILE_HPP

#include <Bit/Build.hpp>
#include <Bit/NonCopyable.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/System/Vector2.hpp>
#include <string>
#include <vector>
#include <Bit/System/Log.hpp>

// Win32 fix
#if defined( BIT_PLATFORM_WINDOWS )
#ifdef GetObject
#undef GetObject
#endif
#endif

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
	class BIT_API ObjFile : public NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Face corner struct. Holding face data.
		///
		/// An index that's less or equal to 0 should be ignored.
		///
		////////////////////////////////////////////////////////////////
		struct FaceCorner
		{
			Int32 VertexIndex;
			Int32 TextureCoordIndex;
			Int32 NormalIndex;
		};

		////////////////////////////////////////////////////////////////
		/// \brief Smoothing group class. Groups within a material group.
		///
		////////////////////////////////////////////////////////////////
		class Face
		{

		public:

			// Friend classes
			friend class ObjFile;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Face( );

			////////////////////////////////////////////////////////////////
			/// \brief Get face corner.
			///
			////////////////////////////////////////////////////////////////
			FaceCorner & GetFaceCorner( const SizeType p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Get face corner count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetFaceCornerCount( ) const;

		private:

			// Private typedefs
			typedef std::vector<FaceCorner> FaceCornerVector;

			// Private variables
			FaceCornerVector m_FaceCorners;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Material group class. Groups within an object group.
		///
		////////////////////////////////////////////////////////////////
		class MaterialGroup
		{

		public:

			// Friend classes
			friend class ObjFile;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			MaterialGroup( );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~MaterialGroup( );

			////////////////////////////////////////////////////////////////
			/// \brief Get material name.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetMaterialName( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get flat face.
			///
			////////////////////////////////////////////////////////////////
			Face & GetFlatFace( const SizeType p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Get smooth face.
			///
			////////////////////////////////////////////////////////////////
			Face & GetSmoothFace( const SizeType p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Get flat face count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetFlatFaceCount( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get smooth face count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetSmoothFaceCount( ) const;

		private:

			// Private typedefs
			typedef std::vector<Face *> FaceVector;

			// Private variables
			std::string m_MaterialName;
			FaceVector m_FlatFaces;
			FaceVector m_SmoothFaces;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Object group class. Groups within an object.
		///
		////////////////////////////////////////////////////////////////
		class ObjectGroup
		{

		public:

			// Friend classes
			friend class ObjFile;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			ObjectGroup( );
			
			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~ObjectGroup( );

			////////////////////////////////////////////////////////////////
			/// \brief Get group name.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;	

			////////////////////////////////////////////////////////////////
			/// \brief Get material group.
			///
			////////////////////////////////////////////////////////////////
			MaterialGroup & GetMaterialGroup( const SizeType p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Get material group count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetMaterialGroupCount( ) const;

		private:

			// Private typedefs
			typedef std::vector<MaterialGroup *> MaterialGroupVector;

			// Private variables
			std::string m_Name;
			MaterialGroupVector m_MaterialGroups;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Object class.
		///
		////////////////////////////////////////////////////////////////
		class Object
		{

		public:

			// Friend classes
			friend class ObjFile;

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			Object( );

			////////////////////////////////////////////////////////////////
			/// \brief Destructor
			///
			////////////////////////////////////////////////////////////////
			~Object( );
			
			////////////////////////////////////////////////////////////////
			/// \brief Get object name.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetName( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get vertex.
			///
			////////////////////////////////////////////////////////////////
			const Vector3f32 & GetVertex( const SizeType p_Index ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get texture coordinate.
			///
			////////////////////////////////////////////////////////////////
			const Vector2f32 & GetTextureCoord( const SizeType p_Index ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get normal.
			///
			////////////////////////////////////////////////////////////////
			const Vector3f32 & GetNormal( const SizeType p_Index ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get object group.
			///
			////////////////////////////////////////////////////////////////
			ObjectGroup & GetObjectGroup( const SizeType p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Get vertex count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetVertexCount( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get texture coordinate count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetTextureCoordCount( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get normal count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetNormalCount( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get object group count.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetObjectGroupCount( ) const;

		private:

			// Private typedefs
			typedef std::vector<Vector3f32> Vertices;
			typedef std::vector<Vector2f32> TextureCoords;
			typedef std::vector<Vector3f32> Normals;
			typedef std::vector<ObjectGroup *> ObjectGroupVector;

			// Private variables
			std::string m_Name;
			Vertices m_Vertices;
			TextureCoords m_TextureCoords;
			Normals m_Normals;
			ObjectGroupVector m_ObjectGroups;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		ObjFile( );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		/// Clears the data.
		///
		////////////////////////////////////////////////////////////////
		~ObjFile( );

		////////////////////////////////////////////////////////////////
		/// \brief Load OBJ from memory.
		///
		/// \param p_pMemory Pointer to the memory.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromMemory( const std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Load OBJ file from stream.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromStream( std::istream & p_Stream );

		////////////////////////////////////////////////////////////////
		/// \brief Load OBJ from file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool LoadFromFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Save OBJ to memory.
		///
		/// \param p_pMemory Reference to the memory.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToMemory( std::string & p_Memory );

		////////////////////////////////////////////////////////////////
		/// \brief Save OBJ to stream.
		///
		/// \param p_Stream Output stream.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToStream( std::ostream & p_Stream );

		////////////////////////////////////////////////////////////////
		/// \brief Save OBJ to file.
		///
		/// \param p_Filename Output file.
		///
		/// \return True if succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		Bool SaveToFile( const std::string & p_Filename );

		////////////////////////////////////////////////////////////////
		/// \brief Clear the OBJ data.
		///
		////////////////////////////////////////////////////////////////
		void Clear( );

		////////////////////////////////////////////////////////////////
		/// \brief Get material filename.
		///
		////////////////////////////////////////////////////////////////
		const std::string & GetMaterialFilename( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get object.
		///
		////////////////////////////////////////////////////////////////
		Object & GetObject( const SizeType p_Index );

		////////////////////////////////////////////////////////////////
		/// \brief Get object count.
		///
		////////////////////////////////////////////////////////////////
		SizeType GetObjectCount( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Help function for creating a postion buffer
		///			from the current stored data.
		///
		/// Make sure to delete the buffer when you are done with it.
		///
		/// \param p_BufferSize Size of the return buffer in bytes / size of data type(T).
		///		0 if the buffer is invalid(NULL).
		/// \param p_Object Index of the object to obtain data from.
		/// \param p_ObjectGroup Index of the object group to obtain data from.
		/// \param p_MaterialGroup Index of the material group to obtain data from.
		/// \param p_UseFlatFaces If flat faces should be used.
		/// \param p_UseSmoothFaces If smooth faces should be used.
		///
		/// \return NULL if it's not possible to create a buffer,
		///			else a pointer to the buffer or if p_UseFlatFaces and
		///			p_UseSmoothFaces is false.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		T * CreatePositionBuffer(	SizeType & p_BufferSize,
									const SizeType p_Object = 0,
									const SizeType p_ObjectGroup = 0,
									const SizeType p_MaterialGroup = 0,
									const Bool p_UseFlatFaces = true,
									const Bool p_UseSmoothFaces = true );

		////////////////////////////////////////////////////////////////
		/// \brief Help function for creating a texture coordinate buffer
		///			from the current stored data.
		///
		/// Make sure to delete the buffer when you are done with it.
		///
		/// \param p_BufferSize Size of the return buffer in bytes / size of data type(T).
		///		0 if the buffer is invalid(NULL).
		/// \param p_Object Index of the object to obtain data from.
		/// \param p_ObjectGroup Index of the object group to obtain data from.
		/// \param p_MaterialGroup Index of the material group to obtain data from.
		/// \param p_UseFlatFaces If flat faces should be used.
		/// \param p_UseSmoothFaces If smooth faces should be used.
		///
		/// \return NULL if it's not possible to create a buffer,
		///			else a pointer to the buffer or if p_UseFlatFaces and
		///			p_UseSmoothFaces is false.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		T * CreateTextureCoordBuffer(	SizeType & p_BufferSize,
										const SizeType p_Object = 0,
										const SizeType p_ObjectGroup = 0,
										const SizeType p_MaterialGroup = 0,
										const Bool p_UseFlatFaces = true,
										const Bool p_UseSmoothFaces = true );

		////////////////////////////////////////////////////////////////
		/// \brief Help function for creating a normal buffer
		///			from the current stored data.
		///
		/// Make sure to delete the buffer when you are done with it.
		///
		/// \param p_BufferSize Size of the return buffer in bytes / size of data type(T).
		///		0 if the buffer is invalid(NULL).
		/// \param p_Object Index of the object to obtain data from.
		/// \param p_ObjectGroup Index of the object group to obtain data from.
		/// \param p_MaterialGroup Index of the material group to obtain data from.
		/// \param p_UseFlatFaces If flat faces should be used.
		/// \param p_UseSmoothFaces If smooth faces should be used.
		///
		/// \return NULL if it's not possible to create a buffer,
		///			else a pointer to the buffer or if p_UseFlatFaces and
		///			p_UseSmoothFaces is false.
		///
		////////////////////////////////////////////////////////////////
		template <typename T>
		T * CreateNormalBuffer(	SizeType & p_BufferSize,
								const SizeType p_Object = 0,
								const SizeType p_ObjectGroup = 0,
								const SizeType p_MaterialGroup = 0,
								const Bool p_UseFlatFaces = true,
								const Bool p_UseSmoothFaces = true );

	private:

		// Private typedefs
		typedef std::vector<Object *> ObjectVector;

		// Private variables
		std::string m_MaterialFilename;
		ObjectVector m_Objects;

	};

	////////////////////////////////////////////////////////////////
	// Include the inline file.
	////////////////////////////////////////////////////////////////
	#include <Bit/Graphics/ObjFile.inl>

}

#endif