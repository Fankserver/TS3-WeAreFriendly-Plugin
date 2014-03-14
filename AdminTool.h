#include <public_definitions.h>
#include <vector>
#include <map>

class AdminTool {
private:
	std::vector<anyID> waitRoomStack;
	std::vector<anyID> waitRoomWhitelistStack;
	std::vector<anyID> waitRoomPoliceStack;
	std::vector<anyID> waitRoomServerAdminStack;
	uint64 serverConnectionHandlerID;
	bool debug;

public:
	AdminTool::AdminTool();
	~AdminTool();

	void setDebug(bool debug);
	bool getDebug();

	void setServer(uint64 serverConnectionHandlerID);
	bool checkServer(uint64 serverConnectionHandlerID);
	
	// Waitroom methods
	void addWaitRoomStack(anyID ClientId);
	void removeWaitRoomStack(anyID ClientID);
	std::vector<anyID> getWaitRoomStack();
	void validateWaitRoomStack(anyID *clientList);

	// Waitroom whitelist methods
	void addWaitRoomWhitelistStack(anyID ClientId);
	void removeWaitRoomWhitelistStack(anyID ClientID);
	std::vector<anyID> getWaitRoomWhitelistStack();
	void validateWaitRoomWhitelistStack(anyID *clientList);

	// Waiting room police training methods
	void addWaitRoomPoliceStack(anyID ClientId);
	void removeWaitRoomPoliceStack(anyID ClientID);
	std::vector<anyID> getWaitRoomPoliceStack();
	void validateWaitRoomPoliceStack(anyID *clientList);

	// Waiting room server admin methods
	void addWaitRoomServerAdminStack(anyID ClientId);
	void removeWaitRoomServerAdminStack(anyID ClientID);
	std::vector<anyID> getWaitRoomServerAdminStack();
	void validateWaitRoomServerAdminStack(anyID *clientList);
};