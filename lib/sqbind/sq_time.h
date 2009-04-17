/*------------------------------------------------------------------
// sq_time.h
//
// Copyright (c) 1997
// Robert Umbehant
// winglib@wheresjames.com
// http://www.wheresjames.com
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted for commercial and
// non-commercial purposes, provided that the following
// conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to
//   endorse or promote products derived from this software without
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------*/

#pragma once

// namespace
namespace sqbind
{
    class CSqTime
    {
	public:

		SQBIND_CLASS_CTOR_BEGIN( CSqTime )
		SQBIND_CLASS_CTOR_END( CSqTime )

		CSqTime() {}

		// Copy semantics
		CSqTime( const CSqTime &r ) { m_t = r.m_t; }
		CSqTime& operator=( const CSqTime &r ) { m_t = r.m_t; return *this; }

		void GetLocalTime()
		{	m_t.GetLocalTime(); }

		void GetSystemTime()
		{	m_t.GetSystemTime(); }

		void SetTime( int year, int month, int day, int hour, int minute, int second )
		{	m_t.SetTime( year, month, day, hour, minute, second ); }

		int GetYear() { return m_t.GetYear(); }
		int GetMonth() { return m_t.GetMonth(); }
		int GetDay() { return m_t.GetDay(); }
		int GetHour() { return m_t.GetHour(); }
		int Get12Hour() { return m_t.Get12Hour(); }
		int GetMinute() { return m_t.GetMinute(); }
		int GetSecond() { return m_t.GetSecond(); }
		int GetMilliSecond() { return m_t.GetMilliSecond(); }
		int GetMicroSecond() { return m_t.GetMicroSecond(); }
		int GetNanoSecond() { return m_t.GetNanoSecond(); }

		void SetYear( int n ) { m_t.SetYear( n ); }
		void SetMonth( int n ) { m_t.SetMonth( n ); }
		void SetDay( int n ) { m_t.SetDay( n ); }
		void SetHour( int n ) { m_t.SetHour( n ); }
		void Set12Hour( int n ) { m_t.Set12Hour( n ); }
		void SetMinute( int n ) { m_t.SetMinute( n ); }
		void SetSecond( int n ) { m_t.SetSecond( n ); }
		void SetMilliSecond( int n ) { m_t.SetMilliSecond( n ); }
		void SetMicroSecond( int n ) { m_t.SetMicroSecond( n ); }
		void SetNanoSecond( int n ) { m_t.SetNanoSecond( n ); }

		int IsPM() { return m_t.IsPM(); }

		int GetUnixTime() { return m_t.GetUnixTime(); }
		void SetUnixTime( int n ) { m_t.SetUnixTime( n ); }
		int GetDosTime() { return m_t.GetDosTime(); }
		void SetDosTime( int n ) { m_t.SetDosTime( n ); }
		int GetNetTime() { return m_t.GetNetTime(); }
		void SetNetTime( int n ) { m_t.SetNetTime( n ); }

		stdString FormatTime( const stdString &sTmpl )
		{	return m_t.FormatTime( sTmpl.c_str() ).Ptr(); }

		int ParseTime( const stdString &sTmpl, const stdString &sTime )
		{	return m_t.ParseTime( sTmpl.c_str(), sTime.c_str() ); }

		static void Register( sqbind::VM vm );

	private:

		oex::CSysTime		m_t;

    };

}; // end namespace

// Declare type for use as squirrel parameters
SQBIND_DECLARE_INSTANCE( sqbind::CSqTime, CSqTime )
