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

#include <Bit/Graphics/GraphicDevice.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	GraphicDevice::DefaultModelSettings::DefaultModelSettings( ) :
		m_ActiveLightCount( 0 )
	{
	}

	void GraphicDevice::DefaultModelSettings::SetActiveLightCount( const SizeType p_Count )
	{
		if( p_Count > s_MaxLightCount )
		{
			m_ActiveLightCount = s_MaxLightCount;
		}
		else
		{
			m_ActiveLightCount = p_Count;
		}
	}

	void GraphicDevice::DefaultModelSettings::SetLightPosition( const SizeType p_Index, const Vector3f32 & p_Position )
	{
		if( p_Index >= s_MaxLightCount )
		{
			return;
		}

		m_LightSources[ p_Index ].first = p_Position;
	}

	void GraphicDevice::DefaultModelSettings::SetLightColor( const SizeType p_Index, const Vector3f32 & p_Color )
	{
		if( p_Index >= s_MaxLightCount )
		{
			return;
		}

		m_LightSources[ p_Index ].second = p_Color;
	}

	void GraphicDevice::DefaultModelSettings::SetAmbientColor( const Vector3f32 & p_Color )
	{
		m_AmbientColor = p_Color;
	}

	SizeType GraphicDevice::DefaultModelSettings::GetMaxLightCount( ) const
	{
		return s_MaxLightCount;
	}

	SizeType GraphicDevice::DefaultModelSettings::GetActiveLightCount( ) const
	{
		return m_ActiveLightCount;
	}

	Vector3f32 GraphicDevice::DefaultModelSettings::GetLightPosition( const SizeType p_Index ) const
	{
		if( p_Index >= s_MaxLightCount )
		{
			return Vector3f32( 0.0f, 0.0f, 0.0f );
		}

		return m_LightSources[ p_Index ].first;
	}

	Vector3f32 GraphicDevice::DefaultModelSettings::GetLightColor( const SizeType p_Index ) const
	{
		if( p_Index >= s_MaxLightCount )
		{
			return Vector3f32( 0.0f, 0.0f, 0.0f );
		}

		return m_LightSources[ p_Index ].second;
	}

	Vector3f32 GraphicDevice::DefaultModelSettings::GetAmbientLight( ) const
	{
		return m_AmbientColor;
	}

}