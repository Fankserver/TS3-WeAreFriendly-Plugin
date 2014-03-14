#include "AdminTool.h"

AdminTool::AdminTool() {
	this->debug = false;
}
AdminTool::~AdminTool() {

}

void AdminTool::setDebug(bool _debug) {
	this->debug = _debug;
}
bool AdminTool::getDebug() {
	return this->debug;
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
void AdminTool::validateWaitRoomStack(anyID *_clientList) {
	if (this->debug) { printf("validateWaitRoomStack()\n"); };
	std::map<anyID, bool> foundClients;
	std::map<anyID, bool> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients[*currentClient] = true;
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator	it = this->waitRoomStack.begin();
		it != this->waitRoomStack.end();
	) {
		if (foundClients[*it] != NULL) {
			this->waitRoomStack.erase(it);
			if (this->debug) { printf("Duplicate: %d\n", *it); };
		}
		else {
			if (activeClients[*it] != NULL) {
				foundClients[*it] = true;
				if (this->debug) { printf("Found: %d\n", *it); };
				it++;
			}
			else {
				this->waitRoomStack.erase(it);
				if (this->debug) { printf("Unknown: %d\n", *it); };
			}
		}
	}

	// add missing clients
	for (std::map<anyID, bool>::iterator	it = activeClients.begin();
		it != activeClients.end();
	) {
		if (foundClients[it->first] == NULL) {
			this->waitRoomStack.push_back(it->first);
			foundClients[it->first] = true;
			if (this->debug) { printf("Added: %d\n", it->first); };
		}
		it++;
	}
}

void AdminTool::addWaitRoomWhitelistStack(anyID _clientId) {
	this->waitRoomWhitelistStack.push_back(_clientId);
}
void AdminTool::removeWaitRoomWhitelistStack(anyID _clientId) {
	for (std::vector<anyID>::iterator	it = this->waitRoomWhitelistStack.begin();
		it != this->waitRoomWhitelistStack.end();
		) {
		if (*it == _clientId) {
			this->waitRoomWhitelistStack.erase(it);
		}
		else {
			it++;
		}
	}
}
std::vector<anyID> AdminTool::getWaitRoomWhitelistStack() {
	return this->waitRoomWhitelistStack;
}
void AdminTool::validateWaitRoomWhitelistStack(anyID *_clientList) {
	if (this->debug) { printf("validateWaitRoomWhitelistStack()\n"); };
	std::map<anyID, bool> foundClients;
	std::map<anyID, bool> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients[*currentClient] = true;
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator	it = this->waitRoomWhitelistStack.begin();
		it != this->waitRoomWhitelistStack.end();
		) {
		if (foundClients[*it] != NULL) {
			this->waitRoomWhitelistStack.erase(it);
			if (this->debug) { printf("Duplicate: %d\n", *it); };
		}
		else {
			if (activeClients[*it] != NULL) {
				foundClients[*it] = true;
				if (this->debug) { printf("Found: %d\n", *it); };
				it++;
			}
			else {
				this->waitRoomWhitelistStack.erase(it);
				if (this->debug) { printf("Unknown: %d\n", *it); };
			}
		}
	}

	// add missing clients
	for (std::map<anyID, bool>::iterator	it = activeClients.begin();
		it != activeClients.end();
		) {
		if (foundClients[it->first] == NULL) {
			this->waitRoomWhitelistStack.push_back(it->first);
			foundClients[it->first] = true;
			if (this->debug) { printf("Added: %d\n", it->first); };
		}
		it++;
	}
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
void AdminTool::validateWaitRoomPoliceStack(anyID *_clientList) {
	if (this->debug) { printf("validateWaitRoomPoliceStack()\n"); };
	std::map<anyID, bool> foundClients;
	std::map<anyID, bool> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients[*currentClient] = true;
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator	it = this->waitRoomPoliceStack.begin();
		it != this->waitRoomPoliceStack.end();
	) {
		if (foundClients[*it] != NULL) {
			this->waitRoomPoliceStack.erase(it);
			if (this->debug) { printf("Duplicate: %d\n", *it); };
		}
		else {
			if (activeClients[*it] != NULL) {
				foundClients[*it] = true;
				if (this->debug) { printf("Found: %d\n", *it); };
				it++;
			}
			else {
				this->waitRoomPoliceStack.erase(it);
				if (this->debug) { printf("Unknown: %d\n", *it); };
			}
		}
	}

	// add missing clients
	for (std::map<anyID, bool>::iterator	it = activeClients.begin();
		it != activeClients.end();
	) {
		if (foundClients[it->first] == NULL) {
			this->waitRoomPoliceStack.push_back(it->first);
			foundClients[it->first] = true;
			if (this->debug) { printf("Added: %d\n", it->first); };
		}
		it++;
	}
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
void AdminTool::validateWaitRoomServerAdminStack(anyID *_clientList) {
	if (this->debug) { printf("validateWaitRoomServerAdminStack()\n"); };
	std::map<anyID, bool> foundClients;
	std::map<anyID, bool> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients[*currentClient] = true;
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator	it = this->waitRoomServerAdminStack.begin();
										it != this->waitRoomServerAdminStack.end();
	) {
		if (foundClients[*it] != NULL) {
			this->waitRoomServerAdminStack.erase(it);
			if (this->debug) { printf("Duplicate: %d\n", *it); };
		}
		else {
			if (activeClients[*it] != NULL) {
				foundClients[*it] = true;
				if (this->debug) { printf("Found: %d\n", *it); };
				it++;
			}
			else {
				this->waitRoomServerAdminStack.erase(it);
				if (this->debug) { printf("Unknown: %d\n", *it); };
			}
		}
	}

	// add missing clients
	for (std::map<anyID, bool>::iterator	it = activeClients.begin();
											it != activeClients.end();
	) {
		if (foundClients[it->first] == NULL) {
			this->waitRoomServerAdminStack.push_back(it->first);
			foundClients[it->first] = true;
			if (this->debug) { printf("Added: %d\n", it->first); };
		}
		it++;
	}
}