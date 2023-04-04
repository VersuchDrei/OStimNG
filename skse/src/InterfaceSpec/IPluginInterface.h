#pragma once

class IPluginInterface
{
public:
	IPluginInterface() { };
	virtual ~IPluginInterface() { };

	virtual UINT32 GetVersion() = 0;
	virtual void Revert() = 0;
};

class IInterfaceMap
{
public:
	virtual IPluginInterface* QueryInterface(const char* name) = 0;
	virtual bool AddInterface(const char* name, IPluginInterface* pluginInterface) = 0;
	virtual IPluginInterface* RemoveInterface(const char* name) = 0;
};

struct OSAInterfaceExchangeMessage
{
	enum: uint32_t
	{
		kMessage_ExchangeInterface = 'OSA'
	};

	IInterfaceMap* interfaceMap = NULL;
};

class IMessagingRegistry : public IPluginInterface
{
public:
	virtual void RegisterForMessages(std::string newListener) = 0;
};