#ifndef _Scene_H_
#define _Scene_H_

#include "Common.h"

namespace JSB
{
	class Scene
	{
	public:
		Scene(SceneNode* node);
		virtual ~Scene();

		SceneNode* getNode() { return mNode; }

		virtual void attachEntityNotify(User* u) {}
		virtual void detachEntityNotify(User* u) {};

		virtual void update(float elapsedTime) {};
	private:
		SceneNode* mNode;
	};
}

#endif