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

#ifndef BIT_WINDOW_SIMPLE_RENDER_WINDOW_HPP
#define BIT_WINDOW_SIMPLE_RENDER_WINDOW_HPP

#include <Bit/Window/RenderWindow.hpp>
#include <Bit/Window/Shape.hpp>
#include <Bit/Graphics/GraphicDevice.hpp>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Window
	/// \brief Simple render window.
	///
	////////////////////////////////////////////////////////////////
	class BIT_API SimpleRenderWindow : public Window, NonCopyable
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		////////////////////////////////////////////////////////////////
		SimpleRenderWindow( );

		////////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		////////////////////////////////////////////////////////////////
		SimpleRenderWindow(	const VideoMode & p_VideoMode,
							const std::string & p_Title = "",
							const Uint32 p_Style = Style::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		////////////////////////////////////////////////////////////////
		~SimpleRenderWindow( );

		////////////////////////////////////////////////////////////////
		/// \brief Open(create) the window.
		///
		/// This function may be used instead of the custom constructor.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool Open(	const VideoMode & p_VideoMode,
							const std::string & p_Title = "",
							const Uint32 p_Style = Style::Default );

		////////////////////////////////////////////////////////////////
		/// \brief Close the window.
		///
		////////////////////////////////////////////////////////////////
		virtual void Close( );

		////////////////////////////////////////////////////////////////
		/// \brief Update the window events
		///
		/// This function must be called in your application's loop.
		///
		////////////////////////////////////////////////////////////////
		virtual void Update( );

		////////////////////////////////////////////////////////////////
		/// \brief Draw shape
		///
		////////////////////////////////////////////////////////////////
		void Draw( Shape * p_pShape );

		////////////////////////////////////////////////////////////////
		/// \brief Create shape
		///
		////////////////////////////////////////////////////////////////
		Shape * CreateShape( const Bool p_OrigoInCenter = false );

		////////////////////////////////////////////////////////////////
		/// \brief Destroy shape
		///
		////////////////////////////////////////////////////////////////
		void DestroyShape( Shape * p_pShape );

		////////////////////////////////////////////////////////////////
		/// \brief Present and clear the window
		///
		////////////////////////////////////////////////////////////////
		void Present( );

		////////////////////////////////////////////////////////////////
		/// \brief Poll a window event.
		///
		/// This function can be used in a while loop
		/// in order to make sure you poll all the events
		/// in the event queue.
		///
		/// Example:
		/// 
		/// Event e;
		/// while( window.PollEvent( e ) )
		/// {
		///		// Handle the event
		///		// ...
		///	}
		///
		/// \param p_Event The current event.
		///
		/// \return true if function succeeded, else false.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool PollEvent( Event & p_Event );

		////////////////////////////////////////////////////////////////
		/// \brief Set the window caption(title)
		///
		/// \param p_Title The new title of the window
		///
		////////////////////////////////////////////////////////////////
		virtual void SetTitle( const std::string & p_Title );

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is open( created ).
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsOpen( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Checks if the window is focused.
		///
		////////////////////////////////////////////////////////////////
		virtual Bool IsFocused( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's current video mode.
		///
		////////////////////////////////////////////////////////////////
		virtual const VideoMode & GetVideoMode( ) const;

		////////////////////////////////////////////////////////////////
		/// \brief Get the window's caption(title)
		///
		////////////////////////////////////////////////////////////////
		virtual const std::string & GetTitle( ) const;

	private:

		RenderWindow		m_RenderWindow;
		GraphicDevice *		m_pGraphicDevice;
		ShaderProgram *		m_pShaderProgram;
		Shader *			m_pVertexShader;
		Shader *			m_pFragmentShader;

	};

}

#endif