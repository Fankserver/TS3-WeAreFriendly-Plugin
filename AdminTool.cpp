#include "AdminTool.h"

AdminTool::AdminTool() {
	
}

void AdminTool::addWaitRoomStack(anyID _clientId) {
	this->waitRoomStack.push_back(_clientId);
}

void AdminTool::removeWaitRoomStack(anyID _clientId) {
	for (int i = 0; i < this->waitRoomStack.size(); i++) {
		if (this->waitRoomStack.at(i) == _clientId) {
			this->waitRoomStack.erase(this->waitRoomStack.begin() + i);
		}
	}
}

std::vector<anyID> AdminTool::getWaitRoomStack() {
	return this->waitRoomStack;
}