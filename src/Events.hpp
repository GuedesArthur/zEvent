#include <vector>
#include <algorithm>
#include <execution>
#include <functional>

using std::function, std::vector, std::for_each, std::remove, std::distance;

namespace Zilla
{
namespace Events
{
	
	/// @brief 
	/// @tparam ...Args 
	template<typename ...Args>
	class zEvent
	{
		struct subscriber
		{
			const void* m_callerId;
			const void* m_funcId;
			const void (*m_func)(const Args...);
			subscriber(const void (*)(const Args...));
			subscriber(const void (*)(const Args...), void* id);
			
		public:
			bool operator ==(const subscriber& rhs) const;
			void operator ()(const Args...) const;
		};

		vector<subscriber> m_subs;


	public:

		template<typename ...FArgs>
		void Subscribe(const void(*)(FArgs...,Args...), FArgs...);
		template<typename ...FArgs>
		void Subscribe(const void(*)(Args...,FArgs...), FArgs...);
		template<class T>
		void Subscribe(const void T::(*)(Args...), T);

		void Subscribe(const void(*)());
		void Subscribe(const void(*)(Args...));
		void Subscribe(const function<void(const Args...)>);

		void operator()(Args...) const;
		
		template<typename exec_policy>
		void operator()(exec_policy, Args...) const;
	};

#ifndef _RegionSubscriber

	template <typename... Args>
	inline zEvent<Args...>::subscriber::subscriber(const void (*func)(const Args...))
		: m_callerId(0), m_funcId(static_cast<void*>(func)), m_func(func){}

	template <typename... Args>
	inline zEvent<Args...>::subscriber::subscriber(const void (*func)(const Args...), void *id)
		: m_funcId(id), m_func(func) {}

	template <typename... Args>
	inline bool zEvent<Args...>::subscriber::operator==(const subscriber &rhs) const
	{
		return m_funcId == rhs.m_funcId;
	}

	template <typename... Args>
	inline void zEvent<Args...>::subscriber::operator()(const Args... args) const
	{
		m_func(args...);
	}


#endif

	
	template <typename... Args>
	inline void zEvent<Args...>::Subscribe(const void (*f)(Args...))
	{
		m_subs.emplace_back(f);
	}

	template <typename... Args>
	inline void zEvent<Args...>::Subscribe(const void (*f)())
	{
		using namespace std::placeholders;
		m_subs.emplace_back(std::bind(f, _1, _2, _3));
	}

	template <typename... Args>
	inline void zEvent<Args...>::operator()(Args... args) const
	{
		std::for_each(m_subs.begin(), m_subs.end(), [=](subscriber s){s(args...);});
	}
	
	template <typename... Args>
	template <typename exec_policy>
	inline void zEvent<Args...>::operator()(exec_policy exec, Args... args) const
	{
		static_assert(std::is_execution_policy_v<exec_policy>);
		std::for_each(exec, m_subs.begin(), m_subs.end(), [=](subscriber s){s(args...);});
	}
}
}