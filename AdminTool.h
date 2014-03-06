#include <public_definitions.h>
#include <vector>

class AdminTool {
private:
	std::vector<anyID> waitRoomStack;

public:
	AdminTool::AdminTool();
	
	void addWaitRoomStack(anyID ClientId);
	void removeWaitRoomStack(anyID ClientID);
	std::vector<anyID> getWaitRoomStack();
};