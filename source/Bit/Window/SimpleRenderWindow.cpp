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

#include <Bit/Window/SimpleRenderWindow.hpp>
#include <Bit/Graphics/OpenGL/OpenGLGraphicDevice.hpp>
#include <sstream>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	SimpleRenderWindow::SimpleRenderWindow( ) :
		m_RenderWindow( ),
		m_pGraphicDevice( NULL )
	{
	}

	SimpleRenderWindow::SimpleRenderWindow(	const VideoMode & p_VideoMode,
											const std::string & p_Title,
											const Uint32 p_Style ) :
		m_pGraphicDevice( NULL )
	{
		Open( p_VideoMode, p_Title, p_Style );
	}

	SimpleRenderWindow::~SimpleRenderWindow( )
	{
		Close( );
	}

	Bool SimpleRenderWindow::Open(	const VideoMode & p_VideoMode,
									const std::string & p_Title,
									const Uint32 p_Style )
	{
		// Close already open windows or graphic devices
		if( m_RenderWindow.IsOpen( ) || m_pGraphicDevice )
		{
			if( m_pGraphicDevice )
			{
				delete m_pGraphicDevice;
				m_pGraphicDevice = NULL;
			}

			m_RenderWindow.Close( );
		}

		// Open the window
		if( m_RenderWindow.Open( p_VideoMode, p_Title, p_Style ) == false )
		{
			return false;
		}

		// Create the graphic device
		m_pGraphicDevice = new OpenGLGraphicDevice;
		if( m_pGraphicDevice->Open( m_RenderWindow ) == false )
		{
			delete m_pGraphicDevice;
			m_pGraphicDevice = NULL;
			return false;
		}

		// Initialize the graphic device
		m_pGraphicDevice->SetViewport( Vector2u32( 0, 0 ), p_VideoMode.GetSize( ) );
		m_pGraphicDevice->SetClearColor( 0, 255, 0, 255 );

		// Succeeded
		return true;
	}

	void SimpleRenderWindow::Close( )
	{
		if( m_pGraphicDevice )
		{
			delete m_pGraphicDevice;
			m_pGraphicDevice = NULL;
		}

		m_RenderWindow.Close( );
	}

	void SimpleRenderWindow::Update( )
	{
		m_RenderWindow.Update( );
	}

	void SimpleRenderWindow::Draw( Shape * p_pShape )
	{
	}

	void SimpleRenderWindow::Present( )
	{
		if( m_pGraphicDevice == NULL )
		{
			return;
		}

		m_pGraphicDevice->Present( );
		m_pGraphicDevice->ClearColor( );
		m_pGraphicDevice->ClearDepth( );
	}

	Bool SimpleRenderWindow::PollEvent( Event & p_Event )
	{
		return m_RenderWindow.PollEvent( p_Event );
	}

	void SimpleRenderWindow::SetTitle( const std::string & p_Title )
	{
		m_RenderWindow.SetTitle( p_Title );
	}

	Bool SimpleRenderWindow::IsOpen( ) const
	{
		return m_RenderWindow.IsOpen( ) && m_pGraphicDevice;
	}

	Bool SimpleRenderWindow::IsFocused( ) const
	{
		return m_RenderWindow.IsFocused( );
	}

	const VideoMode & SimpleRenderWindow::GetVideoMode( ) const
	{
		return m_RenderWindow.GetVideoMode( );
	}

	const std::string & SimpleRenderWindow::GetTitle( ) const
	{
		return m_RenderWindow.GetTitle( );
	}

}