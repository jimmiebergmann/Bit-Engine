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

#include <Bit/System/PathfindingAStar2.hpp>
#include <map>
#include <Bit/System/Debugger.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{
	// structs and typedefs
	struct InternalNode
	{
		InternalNode(	const BIT_UINT32 p_Index,
						const BIT_UINT32 p_Distance,
						const Bit::Vector2_ui32 p_Position ) :
			Index( p_Index ),
			Distance( p_Distance ),
			Position( p_Position )
		{ };

		BIT_UINT32 Index;
		BIT_UINT32 Distance;
		Bit::Vector2_ui32 Position;
	};

	typedef std::multimap<BIT_UINT32, InternalNode*> InternalNodeMap;
	typedef std::pair<BIT_UINT32, InternalNode*> InternalNodePair;

	// Global functions
	static BIT_UINT32 NodeDistance( Bit::Vector2_ui32 p_Position, Bit::Vector2_ui32 p_End )
	{
		BIT_UINT32 Distance = abs(static_cast<BIT_SINT32>( p_End.x ) - static_cast<BIT_SINT32>( p_Position.x ) );
		Distance += abs(static_cast<BIT_SINT32>( p_End.y ) - static_cast<BIT_SINT32>( p_Position.y ) );

		return Distance;
	}


	// Constructor/destructor
	PathfindingAStar2::PathfindingAStar2( ) :
		m_CollisionData( BIT_NULL ),
		m_Size( 0, 0 )
	{
	}

	PathfindingAStar2::~PathfindingAStar2( )
	{
		// Delete the nodes
		for( NodeList::iterator it = m_Nodes.begin( ); it != m_Nodes.end( ); it++ )
		{
			delete *it;
		}
	}

	// Public general functions
	BIT_UINT32 PathfindingAStar2::Load( BIT_BOOL * p_Data, Bit::Vector3_ui32 p_Size )
	{
		return BIT_ERROR;
	}

	BIT_UINT32 PathfindingAStar2::Generate( const Bit::Vector3_ui32 p_Start, const Bit::Vector3_ui32 p_End )
	{
		const Bit::Vector2_ui32 Start( p_Start.x, p_Start.z );
		const Bit::Vector2_ui32 End( p_End.x, p_End.z );

		// Delete the nodes
		for( NodeList::iterator it = m_Nodes.begin( ); it != m_Nodes.end( ); it++ )
		{
			delete *it;
		}

		// Create a node map for "internal" temporary nodes
		InternalNodeMap InternalNodes;

		// Add a start node
		BIT_UINT32 Distance = NodeDistance( Start, End );
		InternalNodes.insert( InternalNodePair( Distance, new InternalNode( 0, Distance, Start ) ) );

		// And so on...
		// ...


		// Delete the internal nodes
		for( InternalNodeMap::iterator it = InternalNodes.begin( ); it != InternalNodes.end( ); it++ )
		{
			delete it->second;
		}

		return BIT_OK;
	}

	// Get functions
	PathNode * PathfindingAStar2::GetNode( const BIT_UINT32 p_Inde )
	{
		return BIT_NULL;
	}

	BIT_UINT32 PathfindingAStar2::GetNodeCount( ) const
	{
		return 0;
	}


	

}