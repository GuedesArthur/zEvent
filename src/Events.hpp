#include <vector>
#include <algorithm>
#include <functional>
#include <exception>

using std::function, std::vector, std::for_each, std::remove, std::distance;

namespace Zilla
{
namespace Events
{
	template<typename ...Args>
	class zEvent
	{
		vector<function<void(const Args...)>> m_subs;
		

		template<class T>
		constexpr function<void(const Args...)> _bind(void (T::*)(const Args...), T*) const;
		
	public:

		template<class T>
		void Subscribe(void (T::*)(const Args...), T*);
		void Subscribe(function<void(const Args...)>);

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
	inline void zEvent<Args...>::operator()(const Args... args) const
	{
		std::for_each(m_subs.begin(), m_subs.end(), [&](function<void(const Args...)> f){f(args...);});
	}

	template <typename... Args>
	template <class T>
	inline void zEvent<Args...>::Subscribe(void (T::*f)(const Args...), T* objPtr)
	{
		Subscribe(_bind(f, objPtr));
	}
}
}