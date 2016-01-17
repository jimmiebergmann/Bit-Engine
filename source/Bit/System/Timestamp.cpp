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

#include <Bit/System/Timestamp.hpp>
#include <ctime>
#include <Bit/System/MemoryLeak.hpp>

namespace Bit
{

	// Static variables
	static const Uint32 g_MonthCount = 12;
	static const Uint8 g_DaysInMonth[g_MonthCount] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	// Timestamp class
	Timestamp::Timestamp() :
		m_Year(0),
		m_Month(0),
		m_Day(0),
		m_Hours(0),
		m_Minutes(0),
		m_Seconds(0),
		m_MicroSeconds(0)
	{

	}

	Timestamp::Timestamp(	const Uint16	p_Year,
							const Uint8		p_Month,
							const Uint8		p_Day,
							const Uint8		p_Hours,
							const Uint8		p_Minutes,
							const Uint8		p_Seconds,
							const Uint32	p_MicroSeconds) :
		m_Year(p_Year),
		m_Month(p_Month),
		m_Day(p_Day),
		m_Hours(p_Hours),
		m_Minutes(p_Minutes),
		m_Seconds(p_Seconds),
		m_MicroSeconds(p_MicroSeconds)
	{

	}

	void Timestamp::SetYear(const Uint16 p_Year)
	{
		m_Year = p_Year;
	}

	void Timestamp::SetMonth(const Uint8 p_Month)
	{
		m_Month = p_Month;
	}

	void Timestamp::SetDay(const Uint8 p_Day)
	{
		m_Day = p_Day;
	}

	void Timestamp::SetHours(const Uint8 p_Hours)
	{
		m_Hours = p_Hours;
	}

	void Timestamp::SetMinutes(const Uint8 p_Minutes)
	{
		m_Minutes = p_Minutes;
	}

	void Timestamp::SetSeconds(const Uint8 p_Seconds)
	{
		m_Seconds = p_Seconds;
	}

	void Timestamp::SetMicroseconds(const Uint32 p_MicroSeconds)
	{
		m_MicroSeconds = p_MicroSeconds;
	}

	Int32 Timestamp::GetYear() const
	{
		return static_cast<Int32>(m_Year);
	}

	Int32 Timestamp::GetMonth() const
	{
		return static_cast<Int32>(m_Month);
	}

	Int32 Timestamp::GetDay() const
	{
		return static_cast<Int32>(m_Day);
	}

	Int32 Timestamp::GetHours() const
	{
		return static_cast<Int32>(m_Hours);
	}

	Int32 Timestamp::GetMinutes() const
	{
		return static_cast<Int32>(m_Minutes);
	}

	Int32 Timestamp::GetSeconds() const
	{
		return static_cast<Int32>(m_Seconds);
	}

	Int32 Timestamp::GetMicroseconds() const
	{
		return static_cast<Int32>(m_MicroSeconds);
	}

	Bool Timestamp::Validate() const
	{
		// Validate the clock and month
		Bool valid =	m_Hours < 24 && m_Minutes < 60 && m_Seconds < 60 && 
						m_Month <= 12 && m_Month > 0;

		if (valid == false)
		{
			return false;
		}

		// Validate the day.
		// Add leap year of needed.
		Uint8 daysInMonth = g_DaysInMonth[m_Month - 1];
		if (m_Month == 2 && IsLeapYear())
		{
			daysInMonth++;
		}

		return m_Day <= daysInMonth && m_Day > 0;
	}

	Bool Timestamp::IsLeapYear() const
	{
		if ((m_Year % 400 == 0 || m_Year % 100 != 0) && (m_Year % 4 == 0))
		{
			return true;
		}

		return false;
	}

	Timestamp Timestamp::Now()
	{
		// Convert now to tm struct for local timezone
		time_t now = time(0);
		//tm * localtm = localtime(&now);
		tm * localtm = new tm;
		if (localtime_s(localtm, &now) != 0)
		{
			return Timestamp();
		}

		// Fill timestamp object.

		// Store data for timestamp.
		Timestamp timestamp(localtm->tm_year + 1900, localtm->tm_mon + 1, localtm->tm_mday,
							localtm->tm_hour, localtm->tm_min, localtm->tm_sec, 0);

		// delete the tm
		delete localtm;

		// Return timestamp.
		return timestamp;
	}

}