#ifndef __LUA_REG_STATE_HPP
#define __LUA_REG_STATE_HPP

#include "config.hpp"

#ifdef min
#undef min
#endif


namespace luareg {

	struct state_t
	{
		bool is_owner_;
		lua_State *state_;

		template < typename AllocatorT = std::allocator<char> >
		state_t(const AllocatorT &allocator = AllocatorT())
			: is_owner_(true)
			, state_(nullptr)
		{
			open(allocator);
		}

		state_t(lua_State *state)
			: is_owner_(false)
			, state_(state)
		{

		}

		state_t(const state_t &rhs)
			: is_owner_(false)
			, state_(rhs.state_)
		{}

		state_t &operator=(const state_t &rhs)
		{
			assert(&rhs != this);
			is_owner_ = false;
			state_ = rhs.state_;

			return *this;
		}

		~state_t()
		{
			close();
		}

	public:
		lua_State *get()
		{
			return state_;
		}

		operator lua_State*()
		{
			return state_;
		}

		operator lua_State*() const
		{
			return state_;
		}

		template < typename AllocatorT >
		void open(const AllocatorT &allocator)
		{
			assert(state_ == nullptr);

			static const ::luaL_Reg lualibs[] = 
			{
				{"",				::luaopen_base},
				{LUA_LOADLIBNAME,	::luaopen_package},
				{LUA_TABLIBNAME,	::luaopen_table},
				{LUA_IOLIBNAME,		::luaopen_io},
				{LUA_OSLIBNAME,		::luaopen_os},
				{LUA_STRLIBNAME,	::luaopen_string},
				{LUA_MATHLIBNAME,	::luaopen_math},
			};
	
			//state_ = ::lua_open();

			auto lambda = [](void *ud, void *ptr, std::size_t old_size, std::size_t new_size)->void *
			{
				assert(ud != nullptr);
				AllocatorT &alloc = *static_cast<AllocatorT *>(ud);

				if (old_size != 0 && new_size != 0)
				{
					auto p = alloc.allocate(new_size);
					std::memcpy(p, ptr, std::min(new_size, old_size));

					alloc.deallocate((char *)ptr, old_size);
					ptr = nullptr;

					return p;
				}

				if (new_size != 0 && old_size == 0)
				{
					auto p = alloc.allocate(new_size);
					return p;
				}

				if (old_size != 0 && new_size == 0)
				{
					assert(ptr != nullptr);
					alloc.deallocate((char *)ptr, old_size);
					return nullptr;
				}

				assert(new_size == 0 && old_size == 0);

				return nullptr;
			};
			state_ = ::lua_newstate(lambda, const_cast<AllocatorT *>(&allocator));


			for( auto i = 0; i != _countof(lualibs); ++i )
			{
				::lua_pushcfunction(state_, lualibs[i].func);
				::lua_pushstring(state_, lualibs[i].name);
				::lua_call(state_, 1, 0);
			}
		}

		void close()
		{
			if( is_owner_ )
				::lua_close(state_);
		}
	};


	struct index_t
	{
		int idx_;

		explicit operator int( ) const
		{
			return idx_;
		}
	};
}

#endif