#pragma once

#include <vector>		/// std::vector
#include <algorithm>	/// std::for_each
#include <functional>	/// std::function, std::bind

using std::function, std::vector, std::for_each, std::remove, std::distance;

namespace Zilla
{
namespace Events
{
	/// @brief C#-like Event class. Observer/PubSub pattern. Accepts both functions and methods
	/// @tparam ...Args Event arguments
	template<typename ...Args>
	class zEvent
	{
		/// @brief Vector containing pointers to all subscribed functions
		vector<function<void(const Args...)>> m_subs;

		/// @brief Binds T's (class) method to a function wrapper, using provided object as an argument.
		/// @tparam T Method class' type
		/// @param f Method pointer
		/// @param objPtr Class object pointer
		/// @return Wrapper function invoking the method as objPtr's
		template<class T>
		constexpr function<void(const Args...)> _bind(void (T::*)(const Args...), T*) const;
		
	public:

		/// @brief Subscribes an appropriate function to the event (regular function or static method)
		/// @tparam ...Args Event arguments
		/// @param f Function pointer
		template<class T>
		void Subscribe(void (T::*)(const Args...), T*);

		/// @brief Subscribes a method with the appropriate arguments
		/// @tparam ...Args Event arguments
		/// @param f Method's pointer ( &TClass::methodName )
		/// @param objPtr Calling object
		void Subscribe(function<void(const Args...)>);

		// TODO: Add unsubscribe methods.

		/// @brief Calls every subscribed function using provided parameters as arguments
		/// @param ...args Event parameters
		void operator()(const Args...) const;
	};

	template <typename... Args>
	template <class T>
	inline constexpr function<void(const Args...)> zEvent<Args...>::_bind(void (T::*f)(const Args...), T * objPtr) const
	{
		using namespace std::placeholders;
		
		if constexpr(sizeof...(Args) == 0) return std::bind(f, objPtr);
		if constexpr(sizeof...(Args) == 1) return std::bind(f, objPtr, _1);
		if constexpr(sizeof...(Args) == 2) return std::bind(f, objPtr, _1, _2);
		if constexpr(sizeof...(Args) == 3) return std::bind(f, objPtr, _1, _2, _3);
		if constexpr(sizeof...(Args) == 4) return std::bind(f, objPtr, _1, _2, _3, _4);
		if constexpr(sizeof...(Args) == 5) return std::bind(f, objPtr, _1, _2, _3, _4, _5);
		if constexpr(sizeof...(Args) == 6) return std::bind(f, objPtr, _1, _2, _3, _4, _5, _6);
		if constexpr(sizeof...(Args) == 7) return std::bind(f, objPtr, _1, _2, _3, _4, _5, _6, _7);
		if constexpr(sizeof...(Args) == 8) return std::bind(f, objPtr, _1, _2, _3, _4, _5, _6, _7, _8);
		if constexpr(sizeof...(Args) == 9) return std::bind(f, objPtr, _1, _2, _3, _4, _5, _6, _7, _8, _9);
		if constexpr(sizeof...(Args) == 0) return std::bind(f, objPtr, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10);
	}

	template <typename... Args>
	inline void zEvent<Args...>::Subscribe(function<void(const Args...)> f)
	{
		m_subs.emplace_back(f);
	}

	template <typename... Args>
	template <class T>
	inline void zEvent<Args...>::Subscribe(void (T::*f)(const Args...), T* objPtr)
	{
		Subscribe(_bind(f, objPtr));
	}

	template <typename... Args>
	inline void zEvent<Args...>::operator()(const Args... args) const
	{
		std::for_each(m_subs.begin(), m_subs.end(), [&](function<void(const Args...)> f){f(args...);});
	}


}
}