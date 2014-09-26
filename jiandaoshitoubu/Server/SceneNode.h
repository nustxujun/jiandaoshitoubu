#ifndef _SceneNode_H_
#define _SceneNode_H_

#include "Common.h"
#include "IteratorWrapper.h"
namespace JSB
{
	class SceneNode
	{
		typedef std::vector<User*> UserList;
		typedef std::set< SceneNode*> Children;
	public:
		typedef IteratorWrapper<UserList> Users;

	public:
		SceneNode();
		~SceneNode();

		void attachEntity(User* u);
		void detachEntity(User* u);
		
		void attachScene(Scene* scene);
		void detachScene();
		Scene* getScene();

		SceneNode* createChild();
		void destroyChild(SceneNode* node);

		Users getUsers();
		size_t getUserCount()const;

		SceneNode* getParent();

		void broadcast(DataStream& msg);

	private:
		SceneNode* mParent;
		UserList mUsers;
		Children mChildren;
		Scene* mScene;
	};
}

#endif