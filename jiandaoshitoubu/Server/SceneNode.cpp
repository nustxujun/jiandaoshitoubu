#include "SceneNode.h"
#include "User.h"
#include "Scene.h"
using namespace JSB;

SceneNode::SceneNode()
	:mParent(0), mScene(0)
{
}

SceneNode::~SceneNode()
{
	for (auto& i : mChildren)
	{
		delete i;
	}

}

void SceneNode::attachEntity(User* u)
{
	if (u->getSceneNode() != nullptr)
	{
		JSB_EXCEPT("user has a node ");
	}

	auto ret = std::find(mUsers.begin(), mUsers.end(), u);
	if (ret != mUsers.end())
	{
		JSB_EXCEPT("same user is existed");
	}
	mUsers.push_back(u);
	u->setSceneNode(this);
	if (mScene)
		mScene->attachEntityNotify(u);
}

void SceneNode::detachEntity(User* u)
{
	auto ret = std::find(mUsers.begin(), mUsers.end(), u);
	if (ret == mUsers.end()) return;
	
	if (mScene)
		mScene->detachEntityNotify(u);

	mUsers.erase(ret);
	u->setSceneNode(nullptr);

}

void SceneNode::attachScene(Scene* scene)
{
	if (mScene != nullptr)
	{
		JSB_EXCEPT("scenenode has a scene");
	}

	mScene = scene;
}
void SceneNode::detachScene()
{
	mScene = nullptr;
}

Scene* SceneNode::getScene()
{
	return mScene;
}

SceneNode* SceneNode::createChild()
{
	SceneNode* node = new SceneNode();
	mChildren.insert(node);
	node->mParent = this;

	return node;

}

void SceneNode::destroyChild(SceneNode* node)
{
	auto ret = mChildren.find(node);
	if (ret == mChildren.end())
		return;
	delete node;
	mChildren.erase(ret);
}

SceneNode::Users SceneNode::getUsers()
{
	return Users(mUsers.begin(), mUsers.end());
}

size_t SceneNode::getUserCount()const
{
	return mUsers.size();
}

SceneNode* SceneNode::getParent()
{
	return mParent;
}


void SceneNode::broadcast(DataStream& msg)
{
	for (auto& i : mUsers)
	{
		i->send(msg);
	}
}

