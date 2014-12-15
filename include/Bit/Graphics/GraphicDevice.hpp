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

#ifndef BIT_GRAPHICS_GRAPHIC_DEVICE_HPP
#define BIT_GRAPHICS_GRAPHIC_DEVICE_HPP

#include <Bit/Build.hpp>
#include <Bit/System/Version.hpp>
#include <Bit/Window/RenderWindow.hpp>
#include <Bit/Graphics/ShaderType.hpp>
#include <Bit/System/Vector3.hpp>
#include <Bit/System/Vector4.hpp>

namespace Bit
{

	// Forward declarations
	class Framebuffer;
	class Renderbuffer;
	class VertexArray;
	class VertexBuffer;
	class Shader;
	class ShaderProgram;
	class Texture;
	class TextureProperties;
	class Model;
	class ModelRenderer;
	class PostProcessingBloom;

	////////////////////////////////////////////////////////////////
	/// \ingroup Graphics
	/// \brief Graphic device base class.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API GraphicDevice
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default model settings class
		///
		/// \see Model
		/// \see ModelRenderer
		///
		////////////////////////////////////////////////////////////////
		class BIT_API DefaultModelSettings
		{

		public:

			////////////////////////////////////////////////////////////////
			/// \brief Light source for the default model settings class.
			///
			/// \see DefaultModelSettings
			///
			////////////////////////////////////////////////////////////////
			class BIT_API Light
			{

			public:

				////////////////////////////////////////////////////////////////
				/// \brief Default constructor
				///
				////////////////////////////////////////////////////////////////
				Light( );

				////////////////////////////////////////////////////////////////
				/// \brief Set light source position
				///
				///	If the w component of the position is 0.0f, the light is treated as a directional source.
				/// Otherwise, calculations are based on the actual location of the light in eye coordinates.
				/// The position is being multiplied with the model view matrix via the MatrixManager class.
				///
				/// \see MatrixManager
				///
				////////////////////////////////////////////////////////////////
				void SetPosition( const Vector4f32 & p_Position );

				////////////////////////////////////////////////////////////////
				/// \brief Set light source color
				///
				////////////////////////////////////////////////////////////////
				void SetColor( const Vector3f32 & p_Color );

				////////////////////////////////////////////////////////////////
				/// \brief Get light source position
				///
				///	If the w component of the position is 0.0f, the light is treated as a directional source.
				/// Otherwise, calculations are based on the actual location of the light in eye coordinates.
				///
				////////////////////////////////////////////////////////////////
				const Vector4f32 & GetPosition( ) const;

				////////////////////////////////////////////////////////////////
				/// \brief Get light source color
				///
				////////////////////////////////////////////////////////////////
				const Vector3f32 & GetColor( ) const;

			private:

				// Private variables
				Vector4f32 m_Position;	///< Light position.
				Vector3f32 m_Color;		///< Light color.

			};

			////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			///
			////////////////////////////////////////////////////////////////
			DefaultModelSettings( );

			////////////////////////////////////////////////////////////////
			/// \brief Set the amount of active light sources.
			///
			////////////////////////////////////////////////////////////////
			void SetActiveLightCount( const SizeType p_Count );

			////////////////////////////////////////////////////////////////
			/// \brief Set ambient light color
			///
			////////////////////////////////////////////////////////////////
			void SetAmbientColor(const Vector3f32 & p_Color );

			////////////////////////////////////////////////////////////////
			/// \brief Get the maximum amount of light sources.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetMaxLightCount( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get the amount of active light sources.
			///
			////////////////////////////////////////////////////////////////
			SizeType GetActiveLightCount( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Get light source
			///
			////////////////////////////////////////////////////////////////
			Light & GetLight( const SizeType p_Index );

			////////////////////////////////////////////////////////////////
			/// \brief Get ambient light color
			///
			////////////////////////////////////////////////////////////////
			Vector3f32 GetAmbientLight( ) const;

		private:

			// Private typedefs
			//typedef std::pair<Vector4f32, Vector3f32> Vector3f32Pair; ///< First = position, second = color.

			// Private variables
			static const SizeType s_MaxLightCount = 8;
			Vector3f32 m_AmbientColor;
			Light m_LightSources[ s_MaxLightCount ];
			SizeType m_ActiveLightCount;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Enumerator for back or front face culling.
		///
		////////////////////////////////////////////////////////////////
		enum eCulling
		{
			FrontFace = 0,
			BackFace = 1
		};

		////////////////////////////////////////////////////////////////
		/// \brief Enumerator for default shader programs.
		///
		////////////////////////////////////////////////////////////////
		enum eDefaultShaders
		{
			InitialPoseShader = 0,
			SkeletalAnimationShader = 1,
			VertexAnimationShader = 2
		};

		enum ePostProcessingEffects
		{
			Bloom = 0,
			/*
			GaussianBlur,
			MotionBlur,
			DepthOfField
			*/
		};
		

		////////////////////////////////////////////////////////////////
		/// \brief Virtual destructor.
		///
		////////////////////////////////////////////////////////////////
		virtual ~GraphicDevice( ) { }

		////////////////////////////////////////////////////////////////
		/// \brief Open(create) the graphic device.
		///
		/// \param p_RenderOutput The output window.
		/// \param p_Version The version of the graphic device context
		///		that you would like to open.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Open(	const RenderWindow & p_RenderOutput,
							const Version & p_Version = Version::Default ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Close the graphic device.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Making this graphic device to the current one.
		///
		/// This means that you can have multiple graphic devices
		/// for a single render output, for example a window.
		///
		////////////////////////////////////////////////////////////////
		virtual void MakeCurrent( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Present the graphics, swap buffers if double-buffering.
		///
		/// Call this function when you want to swap the color buffers.
		/// This function wont clear the buffers.
		///
		/// \see ClearColor
		/// \see ClearDepth
		///
		////////////////////////////////////////////////////////////////
		virtual void Present( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Clearing the render outputs color buffer
		///
		////////////////////////////////////////////////////////////////
		virtual void ClearColor( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Clearing the render outputs depth buffer
		///
		////////////////////////////////////////////////////////////////
		virtual void ClearDepth( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Enable depth test.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableDepthTest( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Enable textures.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableTexture( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Enable multisampling
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableMultisampling( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Enable face culling.
		///
		/// \param p_FaceCulling Front of back face culling.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableFaceCulling( eCulling p_FaceCulling ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Enable smooth lines.
		///
		/// For primitive mode Lines and LineStrip.
		///
		////////////////////////////////////////////////////////////////
		virtual void EnableSmoothLines( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Disable depth test.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableDepthTest( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Disable textures.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableTexture( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Disable face culling.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableFaceCulling( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Disable smooth lines.
		///
		/// For primitive mode Lines and LineStrip.
		///
		////////////////////////////////////////////////////////////////
		virtual void DisableSmoothLines( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a framebuffer.
		///
		/// \return A pointer to the new framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Framebuffer * CreateFramebuffer( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a renderbuffer.
		///
		/// \return A pointer to the new renderbuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual Renderbuffer * CreateRenderbuffer( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a vertex array.
		///
		/// \return A pointer to the new VAO
		///
		////////////////////////////////////////////////////////////////
		virtual VertexArray * CreateVertexArray( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a vertex buffer.
		///
		/// \return A pointer to the new VBO
		///
		////////////////////////////////////////////////////////////////
		virtual VertexBuffer * CreateVertexBuffer( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a shader
		///
		/// \param p_Type Enumerator of the shader type.
		///
		/// \return A pointer to the new shader
		///
		////////////////////////////////////////////////////////////////
		virtual Shader * CreateShader( ShaderType::eType p_Type ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a shader program
		///
		/// \return A pointer to the new shader program
		///
		////////////////////////////////////////////////////////////////
		virtual ShaderProgram * CreateShaderProgram( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a texture
		///
		/// \return A pointer to the new texture
		///
		////////////////////////////////////////////////////////////////
		virtual Texture * CreateTexture( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a model
		///
		/// \return A pointer to the new model.
		///
		/// \see CreateModelRenderer
		///
		////////////////////////////////////////////////////////////////
		virtual Model * CreateModel( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a model
		///
		/// \return A pointer to the new model.
		///
		/// \see CreateModel
		///
		////////////////////////////////////////////////////////////////
		virtual ModelRenderer * CreateModelRenderer( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Create a bloom post-processing effect.
		///
		/// \return A pointer to the new post-process.
		///
		////////////////////////////////////////////////////////////////
		virtual PostProcessingBloom * CreatePostProcessingBloom( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Setting the viewport area.
		///
		/// \param p_Position The position of the viewport.
		/// \param p_Size The size of the viewport.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetViewport( const Vector2u32 & p_Position, const Vector2u32 & p_Size ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Setting the clear color.
		///
		/// All params are in the range of 0 to 255.
		///
		/// \param p_Red Red color component.
		/// \param p_Green Green color component.
		/// \param p_Blue Blue color component.
		/// \param p_Alpha Alpha component.
		///
		////////////////////////////////////////////////////////////////
		virtual void SetClearColor( const Uint8 p_Red, const Uint8 p_Green,
									const Uint8 p_Blue, const Uint8 p_Alpha ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the graphic device is open.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the context version
		///
		////////////////////////////////////////////////////////////////
		virtual Version GetVersion( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual const Framebuffer & GetDefaultFramebuffer( ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default framebuffer.
		///
		////////////////////////////////////////////////////////////////
		virtual ShaderProgram * GetDefaultShaderProgram( const eDefaultShaders p_DefaultShader ) const = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default model settings.
		///
		////////////////////////////////////////////////////////////////
		virtual DefaultModelSettings & GetDefaultModelSettings( ) = 0;

		////////////////////////////////////////////////////////////////
		/// \brief Get the default texture properties.
		///
		////////////////////////////////////////////////////////////////
		virtual TextureProperties & GetDefaultTextureProperties( ) = 0;

	};

	/*
	// Forward declarations
	class Framebuffer;
	class VertexObject;
	class ShaderProgram;
	class Texture;
	class PostProcessingBloom;


	// Graphic Device class
	class BIT_API GraphicDevice
	{

	public:

		// Public enums
		enum eDevice
		{
			Device_Any = 0,			// Picking the most fitting and newest one.
			Device_OpenGL_2_1 = 1,	// OpenGL 2.1 context
			Device_OpenGL_3_1 = 2,	// OpenGL 3.1 context
		};

		enum eCulling
		{
			Culling_FrontFace = 0,
			Culling_BackFace = 1
		};


		// Constructors/destructors
		virtual ~GraphicDevice( ) { }

		// Public general functions
		virtual BIT_UINT32 Open( const Window & p_Window, const BIT_UINT32 p_Devices ) = 0;
		virtual BIT_UINT32 Close( ) = 0;
		virtual void Present( ) = 0;
		virtual void BindDefaultFramebuffer( ) = 0;
		virtual void BindDefaultShaderProgram( ) = 0;

		// Public functions
		BIT_INLINE BIT_BOOL IsOpen( ) const { return m_Open; }

		// Create functions for different renderer elements
		virtual Framebuffer * CreateFramebuffer( ) const = 0;
		//virtual Renderbuffer * CreateRenderbuffer( ) const = 0;
		virtual VertexObject * CreateVertexObject( ) const = 0;
		virtual ShaderProgram * CreateShaderProgram( ) const = 0;
		virtual Shader * CreateShader( const Shader::eShaderType p_ShaderType ) const = 0;
		virtual Texture * CreateTexture( ) const = 0;
		virtual Model * CreateModel( Model::eModelType p_Type ) const = 0;
		virtual PostProcessingBloom * CreatePostProcessingBloom( VertexObject * p_pVertexObject, Texture * p_pTexture ) = 0;

		// Clear functions
		virtual void ClearBuffers( const BIT_UINT32 p_ClearBits ) = 0;
		virtual void ClearColor( ) = 0;
		virtual void ClearDepth( ) = 0;

		// Enable functions
		virtual void EnableTexture( ) = 0;
		virtual void EnableAlpha( ) = 0;
		virtual void EnableDepthTest( ) = 0;
		virtual void EnableStencilTest( ) = 0;
		virtual void EnableFaceCulling( eCulling p_FaceCulling ) = 0;
		virtual void EnableSmoothLines( ) = 0;

		// Disable functions
		virtual void DisableTexture( ) = 0;
		virtual void DisableAlpha( ) = 0;
		virtual void DisableDepthTest( ) = 0;
		virtual void DisableStencilTest( ) = 0;
		virtual void DisableFaceCulling( ) = 0;
		virtual void DisableSmoothLines( ) = 0;

		// Set functions
		virtual void SetClearColor( const BIT_FLOAT32 p_R, const BIT_FLOAT32 p_G,
			const BIT_FLOAT32 p_B, const BIT_FLOAT32 p_A ) = 0;
		virtual void SetClearDepth( BIT_FLOAT32 p_Depth ) = 0;
		virtual void SetClearStencil( BIT_UINT32 p_Stencil ) = 0;
		virtual void SetViewport( const BIT_UINT32 p_LX, const BIT_UINT32 p_LY,
			const BIT_UINT32 p_HX, const BIT_UINT32 p_HY ) = 0; // Lower and higher coordinates
		virtual void SetLineWidth( const BIT_FLOAT32 p_Width ) = 0;

		// Get functions(inlines)
		BIT_INLINE BIT_UINT32 GetDeviceType( ) const { return m_DeviceType; }
		BIT_INLINE BIT_BOOL GetTextureStatus( ) const { return m_TextureStatus; }
		BIT_INLINE BIT_BOOL GetAlphaStatus( ) const { return m_AlphaStatus; }
		BIT_INLINE BIT_BOOL GetDepthTestStatus( ) const { return m_DepthTestStatus; }
		BIT_INLINE BIT_BOOL GetStencilTestStatus( ) const { return m_StencilTestStatus; }
		BIT_INLINE BIT_BOOL GetFaceCullingStatus( ) const { return m_FaceCullingStatus; }
		BIT_INLINE BIT_UINT32 GetFaceCullingType( ) const { return m_FaceCullingType; }
		BIT_INLINE BIT_BOOL GetSmoothLinesStatus( ) const { return m_SmoothLinesStatus; }
		BIT_INLINE Vector2_si32 GetViewportSize( ) const { return m_ViewportHigh - m_ViewportLow; }
		BIT_INLINE Vector2_si32 GetViewportLow( ) const { return m_ViewportLow; }
		BIT_INLINE Vector2_si32 GetViewportHigh( ) const { return m_ViewportHigh; }

	protected:

		// Protected functions
		BIT_BOOL m_Open;
		eDevice m_DeviceType;
		Vector2_si32 m_ViewportLow;
		Vector2_si32 m_ViewportHigh;

		// Enable / Disable statuses
		BIT_BOOL m_TextureStatus;
		BIT_BOOL m_AlphaStatus;
		BIT_BOOL m_DepthTestStatus;
		BIT_BOOL m_StencilTestStatus;
		BIT_BOOL m_FaceCullingStatus;
		BIT_UINT32 m_FaceCullingType;
		BIT_BOOL m_SmoothLinesStatus;

	};

	// Create a cross platform renderer via this function
	BIT_API GraphicDevice * CreateGraphicDevice( );
*/
}

#endif
