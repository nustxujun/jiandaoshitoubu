#include "Resource.h"

using namespace JSB;

void Resource::setState(ResourceState state)
{
	mState = state;
}
ResourceState Resource::getState()const
{
	return mState;
}

Resource* ResourceManager::createResource(const String& name, const String& type, const CustomParams* paras )
{
	auto ex = mResources.find(name);
	if (ex != mResources.end())
	{
		JSB_EXCEPT("resource is existed.");
		return nullptr;
	}

	auto fac = mFactorys.find(type);
	if (fac == mFactorys.end())
	{
		JSB_EXCEPT("factory is not existed.");
		return nullptr;
	}

	Resource* res = fac->second->createImpl(name,paras);
	mResources[name] = res;
	return res;
}

void ResourceManager::destroyResource(const String& name)
{
	auto ret = mResources.find(name);
	if (ret == mResources.end())
		return;

	auto fac = mFactorys.find(ret->second->getType());
	if (fac == mFactorys.end())
	{
		JSB_EXCEPT("factory is not existed.");
		return ;
	}

	fac->second->destroyImpl(ret->second);
	mResources.erase(ret);
}

Resource* ResourceManager::getResource(const String& name)
{
	auto ret = mResources.find(name);
	if (ret == mResources.end())
	{
		JSB_EXCEPT(name + " is not existed.");
		return nullptr;
	}

	return ret->second;
}

bool ResourceManager::hasResource(const String& name)
{
	return mResources.find(name) != mResources.end();
}



void ResourceManager::registerFactory(const String& type, ResourceFactory* factory)
{
	auto fac = mFactorys.find(type);
	if (fac != mFactorys.end())
	{
		JSB_EXCEPT("factory is  existed.");
		return;
	}

	mFactorys[type] = factory;
}

ResourceFactory* ResourceManager::unregisterFactory(const String& type)
{
	auto fac = mFactorys.find(type);
	if (fac == mFactorys.end())
	{
		JSB_EXCEPT("factory is not existed.");
		return nullptr;
	}

	ResourceFactory* f = fac->second;
	mFactorys.erase(fac);
	return f;
}
