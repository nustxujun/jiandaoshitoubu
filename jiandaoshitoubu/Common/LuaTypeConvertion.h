#ifndef _LuaTypeConvertion_H_
#define _LuaTypeConvertion_H_

#include "Property.h"
#include "lua_reg.hpp"
#include "Common.h"
#include "LogicType.h"


namespace luareg  {

	template <>
	struct convertion_t<JSB::Property>
	{
		static JSB::Property from(state_t &state, int index)
		{
			LUAREG_ERROR(lua_istable(state, index) != 0, LUA_TTABLE, index);

			JSB::Property prop;

			lua_pushstring(state, "name");
			lua_gettable(state, -2);
			prop.name = lua_tostring(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "hp");
			lua_gettable(state, -2);
			prop.hp = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "str");
			lua_gettable(state, -2);
			prop.str = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "dex");
			lua_gettable(state, -2);
			prop.dex = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "inte");
			lua_gettable(state, -2);
			prop.inte = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "speed");
			lua_gettable(state, -2);
			prop.speed = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "level");
			lua_gettable(state, -2);
			prop.level = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "exp");
			lua_gettable(state, -2);
			prop.exp = lua_tointeger(state, -1);
			lua_pop(state, 1);

			lua_pushstring(state, "progress");
			lua_gettable(state, -2);
			prop.progress = lua_tointeger(state, -1);
			lua_pop(state, 1);

			return prop;
		}

		static std::uint32_t to(state_t &state, JSB::Property val)
		{
			::lua_createtable(state, 2, 0);

			::lua_pushstring(state, "name");
			::lua_pushstring(state, val.name.c_str());
			::lua_settable(state, -3);

			::lua_pushstring(state, "hp");
			::lua_pushinteger(state, val.hp);
			::lua_settable(state, -3);

			::lua_pushstring(state, "str");
			::lua_pushinteger(state, val.str);
			::lua_settable(state, -3);

			::lua_pushstring(state, "dex");
			::lua_pushinteger(state, val.dex);
			::lua_settable(state, -3);

			::lua_pushstring(state, "inte");
			::lua_pushinteger(state, val.inte);
			::lua_settable(state, -3);

			::lua_pushstring(state, "speed");
			::lua_pushinteger(state, val.speed);
			::lua_settable(state, -3);

			::lua_pushstring(state, "level");
			::lua_pushinteger(state, val.level);
			::lua_settable(state, -3);

			::lua_pushstring(state, "exp");
			::lua_pushinteger(state, val.exp);
			::lua_settable(state, -3);

			::lua_pushstring(state, "progress");
			::lua_pushinteger(state, val.progress);
			::lua_settable(state, -3);

			//convertion_t<int>::to(state, val.str);
			//::lua_rawseti(state, -2, 1);


			return 1;
		}
	};


	template <>
	struct convertion_t<const JSB::Property&>
	{
		static std::uint32_t to(state_t &state, const JSB::Property& val)
		{
			::lua_createtable(state, 2, 0);

			::lua_pushstring(state, "name");
			::lua_pushstring(state, val.name.c_str());
			::lua_settable(state, -3);

			::lua_pushstring(state, "hp");
			::lua_pushinteger(state, val.hp);
			::lua_settable(state, -3);

			::lua_pushstring(state, "str");
			::lua_pushinteger(state, val.str);
			::lua_settable(state, -3);

			::lua_pushstring(state, "dex");
			::lua_pushinteger(state, val.dex);
			::lua_settable(state, -3);

			::lua_pushstring(state, "inte");
			::lua_pushinteger(state, val.inte);
			::lua_settable(state, -3);

			::lua_pushstring(state, "speed");
			::lua_pushinteger(state, val.speed);
			::lua_settable(state, -3);

			::lua_pushstring(state, "level");
			::lua_pushinteger(state, val.level);
			::lua_settable(state, -3);

			::lua_pushstring(state, "exp");
			::lua_pushinteger(state, val.exp);
			::lua_settable(state, -3);

			::lua_pushstring(state, "progress");
			::lua_pushinteger(state, val.progress);
			::lua_settable(state, -3);

			//convertion_t<int>::to(state, val.str);
			//::lua_rawseti(state, -2, 1);


			return 1;
		}
	};

	//template < >
	//struct convertion_t< JSB::String >
	//{
	//	static JSB::String from(state_t &state, int index)
	//	{
	//		LUAREG_ERROR(::lua_isstring(state, index) != 0, LUA_TSTRING, index);
	//		return ::lua_tostring(state, index);
	//	}

	//	static std::uint32_t to(state_t &state, const JSB::String& val)
	//	{
	//		::lua_pushstring(state, val.c_str());
	//		return 1;
	//	}
	//};

	//template <>
	//struct convertion_t<JSB::SkillResult>
	//{
	//	static JSB::SkillResult from(state_t &state, int index)
	//	{
	//		LUAREG_ERROR(lua_istable(state, index) != 0, LUA_TTABLE, index);


	//		JSB::SkillResult sr;


	//		lua_pushstring(state, "type");
	//		lua_gettable(state, -2);
	//		sr.type = (JSB::CastType)lua_tointeger(state, -1);
	//		lua_pop(state, 1);

	//		lua_pushstring(state, "value");
	//		lua_gettable(state, -2);
	//		sr.value = lua_tointeger(state, -1);
	//		lua_pop(state, 1);


	//		return sr;
	//	}

	//	static std::uint32_t to(state_t &state, const JSB::SkillResult& val)
	//	{
	//		::lua_createtable(state, 2, 0);

	//		::lua_pushstring(state, "type");
	//		::lua_pushinteger(state, val.type);
	//		::lua_settable(state, -3);

	//		::lua_pushstring(state, "value");
	//		::lua_pushinteger(state, val.value);
	//		::lua_settable(state, -3);


	//		return 1;
	//	}
	//};


	template <>
	struct convertion_t<const JSB::SkillResult&>
	{
		static std::uint32_t to(state_t &state, const JSB::SkillResult& val)
		{



			return 1;
		}
	};


}


#endif