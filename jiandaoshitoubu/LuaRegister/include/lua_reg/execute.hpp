#ifndef __LUA_REG_EXECUTE_HPP
#define __LUA_REG_EXECUTE_HPP

#include "config.hpp"
#include "state.hpp"
#include "error.hpp"

namespace luareg {

	class Executer
	{
	public:
		static void execute(state_t &state, const char *file_name)
		{
			assert(file_name);
			int top = ::lua_gettop(state);

			if (::luaL_loadfile(state, file_name))
			{
				error_t::handler(state);
				return;
			}

			execute_impl(state);
		}

		static void execute(state_t &state, const char* buffer, size_t size, const char* name)
		{
			if (::luaL_loadbuffer(state, buffer, size, name))
			{
				error_t::handler(state);
				return;
			}

			execute_impl(state);

		}

	private:	
		static void execute_impl(state_t &state)
		{
			int error_index = 0;
			int base = ::lua_gettop(state);
			lua_pushcfunction(state, &error_t::handler);
			::lua_insert(state, base);
			error_index = base;

			int error = ::lua_pcall(state, 0, LUA_MULTRET, error_index);
		}


	};


}

#endif