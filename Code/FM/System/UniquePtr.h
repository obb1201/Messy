#pragma once

#include <memory>
#include <type_traits>
#include "System/Memory.h"

#define FRIEND_UNIQUEPTR(T) friend struct UniquePtrDeleter<T>; friend class UniquePtr<T>;

namespace fm
{
	template<typename T>
	struct UniquePtrDeleter final
	{
		FORCEINLINE static void Destroy(T* ptr)
		{
			fmDelete(ptr);
		}

		FORCEINLINE static void DestroyArray(T* ptr)
		{
			fmDeleteArray(ptr);
		}
	};

	//------------------------------------------
	// declaration
	//----------------------------------------------
	template <typename T>
	class UniquePtr final : public std::unique_ptr<T, decltype(&UniquePtrDeleter<T>::Destroy)>
	{
	public:
		FORCEINLINE constexpr UniquePtr<T>();
		FORCEINLINE explicit UniquePtr<T>(T* ptr);
		FORCEINLINE UniquePtr<T>(UniquePtr<T>&& ptr);

		UniquePtr<T>& operator=(UniquePtr<T>&) = delete;
		FORCEINLINE UniquePtr<T>& operator=(UniquePtr<T>&& ptr);
		FORCEINLINE UniquePtr<T>& operator=(nullptr_t);
	};

	// ---- array specializtion
	template<typename T>
	class UniquePtr<T[]> final : public std::unique_ptr<T[], decltype(&UniquePtrDeleter<T>::DestroyArray)>
	{
	public:
		FORCEINLINE constexpr UniquePtr<T[]>();
		FORCEINLINE explicit UniquePtr<T[]>(T* ptr);
		FORCEINLINE UniquePtr<T[]>(UniquePtr<T[]>&& ptr);

		UniquePtr<T[]>& operator=(UniquePtr<T[]>&) = delete;
		FORCEINLINE UniquePtr<T[]>& operator=(UniquePtr<T[]>&& ptr);
		FORCEINLINE UniquePtr<T[]>& operator=(nullptr_t);
		FORCEINLINE T& operator[](size_t index) const;
	};

	//--------------------
	// implementation
	//--------------------
	template <typename T>
	constexpr UniquePtr<T>::UniquePtr()
		: std::unique_ptr<T, decltype(&UniquePtrDeleter<T>::Destroy)>(nullptr, &UniquePtrDeleter<T>::Destroy)
	{
	}

	template <typename T>
	UniquePtr<T>::UniquePtr(T* ptr)
		: std::unique_ptr<T, decltype(&UniquePtrDeleter<T>::Destroy)>(ptr, &UniquePtrDeleter<T>::Destroy)
	{
	}

	template <typename T>
	UniquePtr<T>::UniquePtr(UniquePtr<T>&& ptr)
		: UniquePtr(ptr.release())
	{
	}

	template <typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& ptr)
	{
		if (this != &ptr)
		{
			reset(ptr.release());
		}
		return (*this);
	}

	template <typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(nullptr_t)
	{
		reset();
		return (*this);
	}

	// ---- array specializtion
	template <typename T>
	constexpr UniquePtr<T[]>::UniquePtr()
		: UniquePtr(nullptr)std::unique_ptr<T[], decltype(&UniquePtrDeleter<T>::DestroyArray)>(nullptr, &UniquePtrDeleter<T>::DestroyArray)
	{
	}

	template <typename T>
	UniquePtr<T[]>::UniquePtr(T* ptr)
		: std::unique_ptr<T[], decltype(&UniquePtrDeleter<T>::DestroyArray)>(ptr, &UniquePtrDeleter<T>::DestroyArray)
	{
	}

	template <typename T>
	UniquePtr<T[]>::UniquePtr(UniquePtr<T[]>&& ptr)
		: UniquePtr(ptr.release())
	{
	}

	template <typename T>
	UniquePtr<T[]>& UniquePtr<T[]>::operator=(UniquePtr<T[]>&& ptr)
	{
		if (this != &ptr)
		{
			reset(ptr.release());
		}
		return (*this);
	}

	template <typename T>
	UniquePtr<T[]>& UniquePtr<T[]>::operator=(nullptr_t)
	{
		reset();
		return (*this);
	}

	template <typename T>
	T& UniquePtr<T[]>::operator[](size_t index) const
	{
		return std::unique_ptr<T[], decltype(&UniquePtrDeleter<T>::DestroyArray)>::operator[](index);
	}

	// ----- make unique
	template <typename T, class... Args>
	FORCEINLINE typename std::enable_if<!std::is_array<T>::value,
		UniquePtr<T> >::type MakeUnique(const eMemPool pool, const char* desc, Args&&... args)
	{
		return UniquePtr<T>(fmNew(pool, desc) T(args...));
	}

	template <typename T>
	FORCEINLINE typename std::enable_if<std::is_array<T>::value,
		UniquePtr<T> >::type MakeUnique(const eMemPool pool, const char* desc, std::size_t size)
	{
		typedef typename std::remove_extent<T>::type Elem;
		return UniquePtr<T>(fmNew(pool, desc) Elem[size]);
	}
}
