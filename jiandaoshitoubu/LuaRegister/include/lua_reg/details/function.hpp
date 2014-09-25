#ifndef __LUA_REG_FUNCTION_DETAILS_HPP
#define __LUA_REG_FUNCTION_DETAILS_HPP


namespace luareg { namespace details {

	

	template < typename R, typename ...Args >
	struct free_function_t
	{
		const char *name_;

		typedef R(*function_t)(Args...);
		function_t function_;

		free_function_t(const char *name, function_t func)
			: name_(name)
			, function_(func)
		{}

		free_function_t(const free_function_t &) = delete;
		free_function_t &operator=(const free_function_t &) = delete;
	};

	
	template < typename R, typename T, typename ...Args >
	struct class_function_t
	{
		const char *name_;

		typedef R(T::*function_t)(Args...);
		T *obj_;
		function_t function_;

		class_function_t(const char *name, T *obj, function_t func)
			: name_(name)
			, obj_(obj)
			, function_(func)
		{}

		class_function_t(const class_function_t &) = delete;
		class_function_t &operator=(const class_function_t &) = delete;
	};

	template < typename ...Args >
	struct lambda_function_t;

	template < typename HandlerT, typename R, typename ...Args >
	struct lambda_function_t<HandlerT, R, std::tuple<Args...>>
	{
		using function_t = R(*)(Args...);
		using this_t = lambda_function_t<HandlerT, R, std::tuple<Args...>>;
	
		const char *name_;
		HandlerT *obj_;

		lambda_function_t(const char *name, HandlerT &&handler)
			: name_(name)
			, obj_(&handler)
		{}

		lambda_function_t(const lambda_function_t &) = delete;
		lambda_function_t &operator=(const lambda_function_t &) = delete;

		R on_handler(Args &&...args)
		{
			return HandlerT(std::forward<Args>(args)...);
		}
	};


	struct lua_function_t
	{
		using function_t = int (*) (state_t &);

		const char* name_;
		function_t function_;

		lua_function_t(const char* name, function_t func)
			:name_(name), function_(func)
		{}


		lua_function_t(const lua_function_t &) = delete;
		lua_function_t &operator=(const lua_function_t &) = delete;
	};

	template<class T>
	struct class_lua_function_t
	{
		const char *name_;

		typedef int(T::*function_t)(state_t& );
		T *obj_;
		function_t function_;

		class_lua_function_t(const char *name, T *obj, function_t func)
			: name_(name)
			, obj_(obj)
			, function_(func)
		{}

		class_lua_function_t(const class_lua_function_t &) = delete;
		class_lua_function_t &operator=(const class_lua_function_t &) = delete;
	};

	
}}


#endif