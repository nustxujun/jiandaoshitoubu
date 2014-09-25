#ifndef _Resource_H_
#define _Resource_H_

#include "Common.h"
#include "Singleton.h"

namespace JSB
{
	typedef std::map<String, String> CustomParams;

	enum ResourceState
	{
		RS_OK,
		RS_FILE_NOT_EXISTED,
		RS_FILE_FORMAT_INVALIED,


	};

	class Resource
	{
	public:
		virtual String getType()const = 0;
		void setState(ResourceState state);
		ResourceState getState()const;

	private:
		ResourceState mState = RS_OK;
	};

	class ResourceFactory
	{
	public:
		virtual Resource* createImpl(const String& name, const CustomParams* paras) = 0;
		virtual void destroyImpl(Resource* res) = 0;
	};

	class ResourceManager: public Singleton<ResourceManager>
	{
	public:
		Resource* createResource(const String& name, const String& type, const CustomParams* paras = nullptr);
		void destroyResource(const String& name);
		Resource* getResource(const String& name);

		bool hasResource(const String& name);

		void registerFactory(const String& type, ResourceFactory* factory);
		ResourceFactory* unregisterFactory(const String& type);

	private:
		std::map<String, Resource*> mResources;
		std::map<String, ResourceFactory*> mFactorys;
	};

}

#endif
