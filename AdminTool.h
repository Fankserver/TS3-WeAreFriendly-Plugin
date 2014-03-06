#include <public_definitions.h>
#include <vector>

class AdminTool {
private:
	std::vector<anyID> waitRoomStack;
	std::vector<anyID> waitRoomPoliceStack;
	std::vector<anyID> waitRoomServerAdminStack;

public:
	AdminTool::AdminTool();
	~AdminTool();
	
	void addWaitRoomStack(anyID ClientId);
	void removeWaitRoomStack(anyID ClientID);
	std::vector<anyID> getWaitRoomStack();

	void addWaitRoomPoliceStack(anyID ClientId);
	void removeWaitRoomPoliceStack(anyID ClientID);
	std::vector<anyID> getWaitRoomPoliceStack();

	void addWaitRoomServerAdminStack(anyID ClientId);
	void removeWaitRoomServerAdminStack(anyID ClientID);
	std::vector<anyID> getWaitRoomServerAdminStack();
};