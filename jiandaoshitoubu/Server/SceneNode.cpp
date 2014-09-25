#include "SceneNode.h"
#include "User.h"
using namespace JSB;

SceneNode::SceneNode(const String& name)
	:mParent(0), mName(name)
{
}

SceneNode::~SceneNode()
{
	for (auto& i : mChildren)
	{
		delete i.second;
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
}

void SceneNode::detachEntity(User* u)
{
	mUsers.erase(std::find(mUsers.begin(), mUsers.end(), u));
	u->setSceneNode(nullptr);
}

SceneNode* SceneNode::createChild(const String& name)
{
	SceneNode* node = new SceneNode(name);
	auto ret = mChildren.insert(std::make_pair(name, node));
	if (!ret.second)
	{
		delete node;
		JSB_EXCEPT("same scenenode name is existed.");
		return nullptr;
	}

	node->mParent = this;

	return node;

}

void SceneNode::destroyChild(const String& name)
{
	auto ret = mChildren.find(name);
	if (ret == mChildren.end())
		return;
	delete ret->second;
	mChildren.erase(ret);
}

SceneNode* SceneNode::getChild(const String& name)
{
	auto ret = mChildren.find(name);
	if (ret == mChildren.end())
		return nullptr;
	return ret->second;
}


SceneNode::Users SceneNode::getUsers()
{
	return Users(mUsers.begin(), mUsers.end());
}

size_t SceneNode::getUserCount()const
{
	return mUsers.size();
}

void SceneNode::broadcast(DataStream& msg)
{
	for (auto& i : mUsers)
	{
		i->send(msg);
	}
}

