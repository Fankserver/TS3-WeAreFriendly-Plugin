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
	for (std::vector<anyID>::iterator	it = this->waitRoomStack.begin();
										it != this->waitRoomStack.end();
		) {
		if (*it == _clientId) {
			this->waitRoomStack.erase(it);
		}
		else {
			it++;
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
	for (std::vector<anyID>::iterator	it = this->waitRoomPoliceStack.begin();
										it != this->waitRoomPoliceStack.end();
	) {
		if (*it == _clientId) {
			this->waitRoomPoliceStack.erase(it);
		}
		else {
			it++;
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
	for (std::vector<anyID>::iterator	it = this->waitRoomServerAdminStack.begin();
										it != this->waitRoomServerAdminStack.end();
		) {
		if (*it == _clientId) {
			this->waitRoomServerAdminStack.erase(it);
		}
		else {
			it++;
		}
	}
}
std::vector<anyID> AdminTool::getWaitRoomServerAdminStack() {
	return this->waitRoomServerAdminStack;
}