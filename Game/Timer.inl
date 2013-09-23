//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

inline void Timer::Reset() throw()
{
	LARGE_INTEGER resolution;
	QueryPerformanceFrequency(&resolution);
	mResolution = (double)resolution.QuadPart;
	uint64 one60th = (uint64)(mResolution / 60.0);
	QueryPerformanceCounter((LARGE_INTEGER *)&mCurrent);
	mBeginning = mCurrent - one60th;
	mPrevious = mCurrent - one60th;
}

//////////////////////////////////////////////////////////////////////

inline void Timer::Update() throw()
{
	uint64 now;
	QueryPerformanceCounter((LARGE_INTEGER *)&now);
	mPrevious = mCurrent;
	mCurrent = now;
	mElapsed = (mCurrent - mBeginning) / mResolution;
	mDelta = (mCurrent - mPrevious) / mResolution;
}

//////////////////////////////////////////////////////////////////////

inline double Timer::Elapsed() const throw()
{
	return mElapsed;
}

//////////////////////////////////////////////////////////////////////

inline double Timer::Delta() const throw()
{
	return mDelta;
}

//////////////////////////////////////////////////////////////////////

