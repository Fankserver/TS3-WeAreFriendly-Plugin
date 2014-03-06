#include "AdminTool.h"

AdminTool::AdminTool() {
	
}
AdminTool::~AdminTool() {

}

void AdminTool::setServer(uint64 _serverConnectionHandlerID) {
	this->serverConnectionHandlerID = _serverConnectionHandlerID;
}
bool AdminTool::checkServer(uint64 _serverConnectionHandlerID) {
	if (this->serverConnectionHandlerID != NULL && this->serverConnectionHandlerID == _serverConnectionHandlerID) {
		return true;
	}
	else {
		return false;
	}
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

void AdminTool::addWaitRoomPoliceStack(anyID _clientId) {
	this->waitRoomPoliceStack.push_back(_clientId);
}
void AdminTool::removeWaitRoomPoliceStack(anyID _clientId) {
	for (int i = 0; i < this->waitRoomPoliceStack.size(); i++) {
		if (this->waitRoomPoliceStack.at(i) == _clientId) {
			this->waitRoomPoliceStack.erase(this->waitRoomPoliceStack.begin() + i);
		}
	}
}
std::vector<anyID> AdminTool::getWaitRoomPoliceStack() {
	return this->waitRoomPoliceStack;
}

void AdminTool::addWaitRoomServerAdminStack(anyID _clientId) {
	this->waitRoomServerAdminStack.push_back(_clientId);
}
void AdminTool::removeWaitRoomServerAdminStack(anyID _clientId) {
	for (int i = 0; i < this->waitRoomServerAdminStack.size(); i++) {
		if (this->waitRoomServerAdminStack.at(i) == _clientId) {
			this->waitRoomServerAdminStack.erase(this->waitRoomServerAdminStack.begin() + i);
		}
	}
}
std::vector<anyID> AdminTool::getWaitRoomServerAdminStack() {
	return this->waitRoomServerAdminStack;
}