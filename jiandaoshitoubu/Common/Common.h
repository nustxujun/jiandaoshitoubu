#ifndef _Common_H_
#define _Common_H_

#include <iostream>
#include <xstring>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

namespace JSB
{
	typedef char Char;

	typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char> > String;
	typedef std::basic_stringstream<Char, std::char_traits<Char>,std::allocator<Char> > Stringstream;

	typedef std::exception Exception;

	typedef int Identity;

	class Battle;

	class CommonFile;
	
	class DataStream;

	class Entity;
	class Environment;

	class Mod;
	class Monster;

	class Profile;

	class Resource;
	class ResourceManager;

	class Scene;
	class SceneNode;
	class Server;
	class ServerEntity;
	class Skill;
	class SkillContainer;
	class SkillEffect;
	class SkillInstance;
	class SkillLua;
	struct SkillResult;
	class SkillTrigger;
	class Socket;

	class User;
	class UserManager;
}
#define JSB_EXCEPT(x) { std::stringstream ss; ss << std::endl << __FILE__ << " Line: " << __LINE__ << "  " << (x) << std::endl; std::cout << ss.str() ; throw Exception(ss.str().c_str());}
#define Assert(x) {if (!(x)) JSB_EXCEPT(#x##" :failed");}

#define STR(x) (x)

#endif