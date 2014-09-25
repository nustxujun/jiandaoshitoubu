#ifndef _SceneNode_H_
#define _SceneNode_H_

#include "Common.h"
#include "IteratorWrapper.h"
namespace JSB
{
	class SceneNode
	{
		typedef std::vector<User*> UserList;
		typedef std::map<String, SceneNode*> Children;
	public:
		typedef IteratorWrapper<UserList> Users;

	public:
		SceneNode(const String& name);
		~SceneNode();

		void attachEntity(User* u);
		void detachEntity(User* u);

		SceneNode* createChild(const String& name);
		void destroyChild(const String& name);
		SceneNode* getChild(const String& name);

		Users getUsers();
		size_t getUserCount()const;

		void broadcast(DataStream& msg);

	private:
		String mName;
		SceneNode* mParent;
		UserList mUsers;
		Children mChildren;
	};
}

#endif