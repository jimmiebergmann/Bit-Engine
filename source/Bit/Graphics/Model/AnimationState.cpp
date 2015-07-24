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

#include <Bit/Graphics/Model/AnimationState.hpp>
#include <Bit/Graphics/Model.hpp>
#include <Bit/Graphics/Model/Skeleton.hpp>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	AnimationState::AnimationState(Model * p_pModel) :
		m_pParent(p_pModel),
		m_pAnimation(NULL),
		m_AnimationIndex(0),
		m_Looping(false),
		m_Interpolating(true),
		m_State(Stopped),
		m_AnimationSpeed(1.0f)
	{
	}

	void AnimationState::Play()
	{
		if (m_pAnimation == NULL)
		{
			return;
		}

		// Restart the timer
		m_Timer.Start();

		// Set the state
		m_State = Playing;
	}

	void AnimationState::Play(const SizeType p_AnimationIndex, const Time & p_StartTime)
	{
		// Get the parent's skeleton.
		Skeleton & skeleton = m_pParent->GetSkeleton();

		// Error check the index.
		if (p_AnimationIndex >= skeleton.GetAnimationCount())
		{
			return;
		}

		// Get the animation.
		m_pAnimation = skeleton.GetAnimation(p_AnimationIndex);

		// Set the time
		m_Time = p_StartTime;

		// Restart the timer
		m_Timer.Start();

		// Set the state
		m_State = Playing;
	}

	void AnimationState::Stop()
	{
		m_Time = Microseconds(0);
		m_State = Stopped;
	}

	void AnimationState::Pause()
	{
		if (m_State != Playing)
		{
			return;
		}

		m_Time += m_Timer.GetLapsedTime();
		m_State = Paused;
	}

	void AnimationState::SetLooping(const Bool p_Looping)
	{
		m_Looping = p_Looping;
	}

	void AnimationState::SetInterpolating(const Bool p_Interpolating)
	{
		m_Interpolating = p_Interpolating;
	}

	void AnimationState::SetAnimationSpeed(const Float32 p_Speed)
	{
		m_AnimationSpeed = p_Speed;
	}

	void AnimationState::SetTime(const Time & p_Time)
	{
		m_Time = p_Time;
	}

	Bool AnimationState::GetLooping() const
	{
		return m_Looping;
	}

	Bool AnimationState::GetInterpolating() const
	{
		return m_Interpolating;
	}

	AnimationState::eState AnimationState::GetState() const
	{
		return m_State;
	}

	Float32 AnimationState::GetAnimationSpeed() const
	{
		return m_AnimationSpeed;
	}

	Time AnimationState::GetTime() const
	{
		return m_Time;
	}

}