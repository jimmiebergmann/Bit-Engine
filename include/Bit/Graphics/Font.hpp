#ifndef BIT_GRAPHICS_FRONT_HPP
#define BIT_GRAPHICS_FRONT_HPP

#include <Bit/DataTypes.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/Graphics/VertexObject.hpp>
#include <Bit/Graphics/Texture.hpp>
#include <Bit/Graphics/ShaderProgram.hpp>
#include <Bit/Graphics/Shader.hpp>
#include <Bit/System/Vector2.hpp>
#include <Bit/System/Vector3.hpp>
///#include <freetype/freetype.h>
// Including freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include <map>
#include <string>
#include <vector>

namespace Bit
{
	// Font glypth
	struct Glyph
	{
		BIT_FLOAT32 Advance; // Total width of the character
		BIT_UCHAR8 Index; // Texture / vertex index.

	};

	// Font class
	class Font
	{

	public:

		Font( GraphicDevice * p_pRenderer );
		~Font( );
		BIT_UINT32 Load( const char * p_pFileName, BIT_UINT32 p_Size, BIT_BOOL p_Monochrome );
		BIT_UINT32 Reload( GraphicDevice * p_pRenderer );
		void Unload( );
		void Render( std::string p_String, Vector3_f32 p_Position,
			BIT_FLOAT32 p_Scale, Vector3_f32 p_Color, BIT_FLOAT32 p_Alpha);
		void SetRenderer( GraphicDevice * p_pRenderer );
		BIT_INLINE BIT_UINT32 GetHeight( ) const { return m_Size; }
		BIT_UINT32 GetStringWidth( std::string p_String, BIT_FLOAT32 p_Scale );
		BIT_BOOL IsGlyphValid( BIT_UCHAR8 p_Index );

	private:

		friend class FontLoader;

		// Private structures
		// strucutre holding data for a glyph in the row data
		struct GlypthInRowStruct
		{
			GlypthInRowStruct() { }
			FT_BitmapGlyph FTGlyph;
			FT_Glyph_Metrics Metrics;
		};


		// Stucture holding data for all glyphs for a row
		struct TextureGlyphRowStruct
		{
			TextureGlyphRowStruct() { }
			typedef std::vector< std::pair< BIT_UCHAR8, GlypthInRowStruct> > BitMapVector;
			BitMapVector BitMapGlyphs;
			Vector2_ui32 RowSize;
		};

		// private Typedefs
		typedef std::map< BIT_UCHAR8, Glyph> GlyphMap;
		typedef std::pair< BIT_UCHAR8, Glyph> GlyphMapPair;
		typedef std::vector< TextureGlyphRowStruct > GlyphRowVector;

		void ClearGlyphRows( );
		void LoadGlyphs( BIT_BYTE * p_pPixelBuffer, const Vector2_ui32 p_Size,
			GlyphRowVector * p_pGlyphRows, BIT_FLOAT32 * p_pVertexCoords, BIT_FLOAT32 * p_pTextureCoords );
		void LoadGlyphsMonochrome( BIT_BYTE * p_pPixelBuffer, const Vector2_ui32 p_Size,
			GlyphRowVector * p_pGlyphRows, BIT_FLOAT32 * p_pVertexCoords, BIT_FLOAT32 * p_pTextureCoords );
		void CreateGlyphVertexData( BIT_FLOAT32 * p_pVertexCoords, BIT_FLOAT32 * p_pTextureCoords, Vector2_ui32 p_TextureSize,
			BIT_UINT32 p_Index, FT_Bitmap * p_pBitmap, FT_BitmapGlyph * p_pGLyph, Vector2_ui32 p_GLyphPosition );
		
		BIT_UINT32 LoadGraphics( );
		BIT_UINT32 LoadShaders( );
		void UnloadGraphics( );
		
		BIT_BOOL m_Loaded;
		GraphicDevice * m_pRenderer;
		BIT_UINT32 m_Size;
		std::string m_Filename;
		BIT_BOOL m_Monochrome;
		FT_Library m_FreeTypeLibrary;
		FT_Face m_FreeTypeFace;
		Glyph * m_pGlyphs[ 256 ];
		GlyphRowVector m_GlyphRows;
		BIT_UINT32 m_TotalGlyphs;
		
		VertexObject * m_pVertexObject;
		Texture * m_pTexture;
		Vector2_ui32 m_TextureSize;
		ShaderProgram * m_pShaderProgram;
		Shader * m_pVertexShader;
		Shader * m_pFragmentShader;
		
		
		static BIT_UCHAR8 s_DefaultCharset[];

	};

}

#endif