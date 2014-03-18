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
	std::vector<anyID> foundClients;
	std::vector<anyID> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients.push_back(*currentClient);
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator
		waitRoomClient = this->waitRoomStack.begin();
		waitRoomClient != this->waitRoomStack.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *waitRoomClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// removed if duplicate client
		if (clientFound) {
			this->waitRoomStack.erase(waitRoomClient);
			if (this->debug) { printf("Duplicate: %d\n", *waitRoomClient); };
		}
		else {
			bool clientActive = false;

			// find client
			for (std::vector<anyID>::iterator
				activeClient = activeClients.begin();
				activeClient != activeClients.end();
			) {
				if (*activeClient == *waitRoomClient) {
					clientActive = true;
					break;
				}
				activeClient++;
			}

			// client found
			if (clientActive) {
				if (this->debug) { printf("Found: %d\n", *waitRoomClient); };
				foundClients.push_back(*waitRoomClient);
				waitRoomClient++;
			}

			// client not found -> delete
			else {
				this->waitRoomStack.erase(waitRoomClient);
				if (this->debug) { printf("Unknown: %d\n", *waitRoomClient); };
			}
		}
	}

	// add missing clients
	for (std::vector<anyID>::iterator
		activeClient = activeClients.begin();
		activeClient != activeClients.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *activeClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// add client if not found
		if (!clientFound) {
			this->waitRoomStack.push_back(*activeClient);
			if (this->debug) { printf("Added: %d\n", *activeClient); };
		}
		activeClient++;
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
	std::vector<anyID> foundClients;
	std::vector<anyID> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients.push_back(*currentClient);
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator
		waitRoomClient = this->waitRoomWhitelistStack.begin();
		waitRoomClient != this->waitRoomWhitelistStack.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *waitRoomClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// removed if duplicate client
		if (clientFound) {
			this->waitRoomWhitelistStack.erase(waitRoomClient);
			if (this->debug) { printf("Duplicate: %d\n", *waitRoomClient); };
		}
		else {
			bool clientActive = false;

			// find client
			for (std::vector<anyID>::iterator
				activeClient = activeClients.begin();
				activeClient != activeClients.end();
			) {
				if (*activeClient == *waitRoomClient) {
					clientActive = true;
					break;
				}
				activeClient++;
			}

			// client found
			if (clientActive) {
				if (this->debug) { printf("Found: %d\n", *waitRoomClient); };
				foundClients.push_back(*waitRoomClient);
				waitRoomClient++;
			}

			// client not found -> delete
			else {
				this->waitRoomWhitelistStack.erase(waitRoomClient);
				if (this->debug) { printf("Unknown: %d\n", *waitRoomClient); };
			}
		}
	}

	// add missing clients
	for (std::vector<anyID>::iterator
		activeClient = activeClients.begin();
		activeClient != activeClients.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *activeClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// add client if not found
		if (!clientFound) {
			this->waitRoomWhitelistStack.push_back(*activeClient);
			if (this->debug) { printf("Added: %d\n", *activeClient); };
		}
		activeClient++;
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
	std::vector<anyID> foundClients;
	std::vector<anyID> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients.push_back(*currentClient);
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator
		waitRoomClient = this->waitRoomPoliceStack.begin();
		waitRoomClient != this->waitRoomPoliceStack.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *waitRoomClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// removed if duplicate client
		if (clientFound) {
			this->waitRoomPoliceStack.erase(waitRoomClient);
			if (this->debug) { printf("Duplicate: %d\n", *waitRoomClient); };
		}
		else {
			bool clientActive = false;

			// find client
			for (std::vector<anyID>::iterator
				activeClient = activeClients.begin();
				activeClient != activeClients.end();
			) {
				if (*activeClient == *waitRoomClient) {
					clientActive = true;
					break;
				}
				activeClient++;
			}

			// client found
			if (clientActive) {
				if (this->debug) { printf("Found: %d\n", *waitRoomClient); };
				foundClients.push_back(*waitRoomClient);
				waitRoomClient++;
			}

			// client not found -> delete
			else {
				this->waitRoomPoliceStack.erase(waitRoomClient);
				if (this->debug) { printf("Unknown: %d\n", *waitRoomClient); };
			}
		}
	}

	// add missing clients
	for (std::vector<anyID>::iterator
		activeClient = activeClients.begin();
		activeClient != activeClients.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *activeClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// add client if not found
		if (!clientFound) {
			this->waitRoomPoliceStack.push_back(*activeClient);
			if (this->debug) { printf("Added: %d\n", *activeClient); };
		}
		activeClient++;
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
	std::vector<anyID> foundClients;
	std::vector<anyID> activeClients;

	// get active clients
	anyID *currentClient;
	for (currentClient = _clientList; *currentClient != (anyID)NULL; currentClient++) {
		activeClients.push_back(*currentClient);
		if (this->debug) { printf("Active: %d\n", *currentClient); };
	}

	// remove duplicate/wrong clients
	for (std::vector<anyID>::iterator
		waitRoomClient = this->waitRoomServerAdminStack.begin();
		waitRoomClient != this->waitRoomServerAdminStack.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *waitRoomClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// removed if duplicate client
		if (clientFound) {
			this->waitRoomServerAdminStack.erase(waitRoomClient);
			if (this->debug) { printf("Duplicate: %d\n", *waitRoomClient); };
		}
		else {
			bool clientActive = false;

			// find client
			for (std::vector<anyID>::iterator
				activeClient = activeClients.begin();
				activeClient != activeClients.end();
			) {
				if (*activeClient == *waitRoomClient) {
					clientActive = true;
					break;
				}
				activeClient++;
			}

			// client found
			if (clientActive) {
				if (this->debug) { printf("Found: %d\n", *waitRoomClient); };
				foundClients.push_back(*waitRoomClient);
				waitRoomClient++;
			}

			// client not found -> delete
			else {
				this->waitRoomServerAdminStack.erase(waitRoomClient);
				if (this->debug) { printf("Unknown: %d\n", *waitRoomClient); };
			}
		}
	}

	// add missing clients
	for (std::vector<anyID>::iterator
		activeClient = activeClients.begin();
		activeClient != activeClients.end();
	) {
		bool clientFound = false;

		// find client
		for (std::vector<anyID>::iterator
			foundClient = foundClients.begin();
			foundClient != foundClients.end();
		) {
			if (*foundClient == *activeClient) {
				clientFound = true;
				break;
			}
			foundClient++;
		}

		// add client if not found
		if (!clientFound) {
			this->waitRoomServerAdminStack.push_back(*activeClient);
			if (this->debug) { printf("Added: %d\n", *activeClient); };
		}
		activeClient++;
	}
}