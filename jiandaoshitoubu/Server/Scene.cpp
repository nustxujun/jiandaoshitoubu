#include "Scene.h"
#include "SceneNode.h"

using namespace JSB;

Scene::Scene(SceneNode* node) :mNode(node) 
{
	mNode->attachScene(this);
}

Scene::~Scene()
{
	mNode->detachScene();
}