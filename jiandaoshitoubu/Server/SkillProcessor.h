#ifndef _SkillProcessor_H_
#define _SkillProcessor_H_

#include "Common.h"
#include "SkillResult.h"
#include "Environment.h"
namespace JSB
{
	class SkillProcessor
	{
	public:
		static void process(const SkillResult& sr, Environment& env);
	};
}

#endif