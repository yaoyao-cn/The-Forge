// The MIT License (MIT)
// 
// 	Copyright (c) 2015 Sergey Makeev, Vadim Slyusarev
// 
// 	Permission is hereby granted, free of charge, to any person obtaining a copy
// 	of this software and associated documentation files (the "Software"), to deal
// 	in the Software without restriction, including without limitation the rights
// 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// 	copies of the Software, and to permit persons to whom the Software is
// 	furnished to do so, subject to the following conditions:
// 
//  The above copyright notice and this permission notice shall be included in
// 	all copies or substantial portions of the Software.
// 
// 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// 	THE SOFTWARE.

#pragma once

#include "MTConfig.h"

#if MT_MSVC_COMPILER_FAMILY || MT_PLATFORM_DURANGO
#include "Platform/Windows/MTAtomic.h"
#elif MT_PLATFORM_ORBIS
#include "../../../../../../PS4/Common_3/ThirdParty/OpenSource/TaskScheduler/Scheduler/Include/Platform/Orbis/MTAtomic.h"
#elif MT_PLATFORM_NX64
#include "Platform/Posix/MTAtomic.h"
#elif MT_PLATFORM_POSIX || MT_PLATFORM_OSX
#include "Platform/Posix/MTAtomic.h"
#else
#endif



namespace MT
{
	inline bool IsPointerAligned( const volatile void* p, const uint32 align )
	{
		return !((uintptr_t)p & (align - 1));
	}


	//
	// Atomic int (type with constructor)
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class Atomic32 : public Atomic32Base<T>
	{

	public:

		Atomic32()
		{
			static_assert(sizeof(Atomic32<T>) == sizeof(T), "Invalid atomic type size");
			MT_ASSERT(IsPointerAligned(this, __alignof(T)), "Invalid atomic alignment");

			Atomic32Base<T>::Store(0);
		}

		explicit Atomic32(T v)
		{
			static_assert(sizeof(Atomic32<T>) == sizeof(T), "Invalid atomic type size");
			MT_ASSERT(IsPointerAligned(this, __alignof(T)), "Invalid atomic alignment");

			Atomic32Base<T>::Store(v);
		}
	};


	//
	// Atomic pointer (type with constructor)
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class AtomicPtr : public AtomicPtrBase<T>
	{
	public:
		AtomicPtr()			
		{
			static_assert(sizeof(AtomicPtr<T>) == sizeof(T*), "Invalid atomic type size");
			MT_ASSERT(IsPointerAligned(this, sizeof(T*)), "Invalid atomic ptr alignment");

			AtomicPtrBase<T>::Store(nullptr);
		}

		explicit AtomicPtr(T* v)
		{
			static_assert(sizeof(AtomicPtr<T>) == sizeof(T*), "Invalid atomic type size");
			MT_ASSERT(IsPointerAligned(this, sizeof(T*)), "Invalid atomic ptr alignment");

			AtomicPtrBase<T>::Store(v);
		}

	};
}