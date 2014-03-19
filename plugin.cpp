/*
 * TeamSpeak 3 We are friendly Plugin
 *
 * Copyright (c) 2014 Fankserver
 */

#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

// Teamspeak includes
#include <public_errors.h>
#include <public_errors_rare.h>
#include <public_definitions.h>
#include <public_rare_definitions.h>
#include <ts3_functions.h>

#include "plugin.hpp"
#include "helper_functions.hpp"
#include "AdminTool.h"

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 20

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;

#define WAITROOM_CHANNEL_ID 123
#define WAITROOMWHITELIST_CHANNEL_ID 441
#define WAITROOMSERVERADMIN_CHANNEL_ID 419
#define WAITROOMPOLICE_CHANNEL_ID 612
#define WAITROOM_COMPLETE_CHANNEL_ID 262
#define WAITROOM_TIMEOUT_CHANNEL_ID 263

static AdminTool *adminTool;

/*********************************** Required functions ************************************/
/*
 * If any of these required functions is not implemented, TS3 will refuse to load the plugin
 */

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
	return "We are friendly Plugin";
}

/* Plugin version */
const char* ts3plugin_version() {
    return "1.0.5";
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author() {
    return "Fankserver";
}

/* Plugin description */
const char* ts3plugin_description() {
    return "This plugin is designed for the We are friendly Community";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

/*
 * Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
 * If the function returns 1 on failure, the plugin will be unloaded again.
 */
int ts3plugin_init() {
    char appPath[PATH_BUFSIZE];
    char resourcesPath[PATH_BUFSIZE];
    char configPath[PATH_BUFSIZE];
	char pluginPath[PATH_BUFSIZE];

    /* Example on how to query application, resources and configuration paths from client */
    /* Note: Console client returns empty string for app and resources path */
    ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
    ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
    ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE);

	adminTool = new AdminTool();

	unsigned int error;
	uint64 *serverConnectionHandlers;
	uint64 *serverConnectionHandler;
	if ((error = ts3Functions.getServerConnectionHandlerList(&serverConnectionHandlers)) != ERROR_ok) {
		char *errorMessage;
		if (ts3Functions.getErrorMessage(error, &errorMessage)) {
			ts3Functions.logMessage("Error getServerConnectionHandlerList", LogLevel_WARNING, ts3plugin_name(), 0);
			ts3Functions.logMessage(errorMessage, LogLevel_WARNING, ts3plugin_name(), 0);
			ts3Functions.freeMemory(errorMessage);
		}
	}
	else {
		for (serverConnectionHandler = serverConnectionHandlers; *serverConnectionHandler != (uint64)NULL; serverConnectionHandler++) {
			char *serverUID;
			if ((error = ts3Functions.getServerVariableAsString(*serverConnectionHandler, VIRTUALSERVER_UNIQUE_IDENTIFIER, &serverUID)) != ERROR_ok) {
				char *errorMessage;
				if (ts3Functions.getErrorMessage(error, &errorMessage)) {
					ts3Functions.logMessage("Error getServerVariableAsString", LogLevel_WARNING, ts3plugin_name(), 0);
					ts3Functions.logMessage(errorMessage, LogLevel_WARNING, ts3plugin_name(), 0);
					ts3Functions.freeMemory(errorMessage);
				}
			}
			else {
				if (strcmp(serverUID, "EguBkHqKJlxXcz9aRHzmy2V0Ke4=") == 0) {
					adminTool->setServer(*serverConnectionHandler);
				}
				ts3Functions.freeMemory(serverUID);
			}
		}
		ts3Functions.freeMemory(serverConnectionHandlers);
	}

    return 0;  /* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
	/* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
	 * the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
	 * For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
	/* Free pluginID if we registered it */
	if(pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

/****************************** Optional functions ********************************/
/*
 * Following functions are optional, if not needed you don't need to implement them.
 */

/*
 * If the plugin wants to use error return codes, plugin commands, hotkeys or menu items, it needs to register a command ID. This function will be
 * automatically called after the plugin was initialized. This function is optional. If you don't use these features, this function can be omitted.
 * Note the passed pluginID parameter is no longer valid after calling this function, so you must copy it and store it in the plugin.
 */
void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	_strcpy(pluginID, sz, id);  /* The id buffer will invalidate after exiting this function */
	//printf("PLUGIN: registerPluginID: %s\n", pluginID);
}

/*
* Implement the following three functions when the plugin should display a line in the server/channel/client info.
* If any of ts3plugin_infoTitle, ts3plugin_infoData or ts3plugin_freeMemory is missing, the info text will not be displayed.
*/

/* Static title shown in the left column in the info frame */
const char* ts3plugin_infoTitle() {
	return "WAF info";
}

/*
 * Dynamic content shown in the right column in the info frame. Memory for the data string needs to be allocated in this
 * function. The client will call ts3plugin_freeMemory once done with the string to release the allocated memory again.
 * Check the parameter "type" if you want to implement this feature only for specific item types. Set the parameter
 * "data" to NULL to have the client ignore the info data.
 */
void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data) {
	switch(type) {
		case PLUGIN_SERVER: {
			return;
		}
		case PLUGIN_CHANNEL: {
			return;
		}
		case PLUGIN_CLIENT: {
			char *clientWAFVersion;
			if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_META_DATA, &clientWAFVersion) != ERROR_ok) {
				printf("Error getting client WAF version\n");
				data = NULL;
				return;
			}

			*data = (char*)malloc(INFODATA_BUFSIZE * sizeof(char));
			snprintf(*data, INFODATA_BUFSIZE, "Version %s", clientWAFVersion);
			ts3Functions.freeMemory(clientWAFVersion);

			break;
		}
		default: {
			printf("Invalid item type: %d\n", type);
			data = NULL;  /* Ignore */
			return;
		}
	}
}

/* Required to release the memory for parameter "data" allocated in ts3plugin_infoData and ts3plugin_initMenus */
void ts3plugin_freeMemory(void* data) {
	free(data);
}

/*
 * Plugin requests to be always automatically loaded by the TeamSpeak 3 client unless
 * the user manually disabled it in the plugin dialog.
 * This function is optional. If missing, no autoload is assumed.
 */
int ts3plugin_requestAutoload() {
	return 0;  /* 1 = request autoloaded, 0 = do not request autoload */
}

/* Helper function to create a menu item */
static struct PluginMenuItem* createMenuItem(enum PluginMenuType type, int id, const char* text, const char* icon) {
	struct PluginMenuItem* menuItem = (struct PluginMenuItem*)malloc(sizeof(struct PluginMenuItem));
	menuItem->type = type;
	menuItem->id = id;
	_strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
	_strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
	return menuItem;
}

/* Some makros to make the code to create menu items a bit more readable */
#define BEGIN_CREATE_MENUS(x) const size_t sz = x + 1; size_t n = 0; *menuItems = (struct PluginMenuItem**)malloc(sizeof(struct PluginMenuItem*) * sz);
#define CREATE_MENU_ITEM(a, b, c, d) (*menuItems)[n++] = createMenuItem(a, b, c, d);
#define END_CREATE_MENUS (*menuItems)[n++] = NULL; assert(n == sz);

/*
 * Menu IDs for this plugin. Pass these IDs when creating a menuitem to the TS3 client. When the menu item is triggered,
 * ts3plugin_onMenuItemEvent will be called passing the menu ID of the triggered menu item.
 * These IDs are freely choosable by the plugin author. It's not really needed to use an enum, it just looks prettier.
 */
enum {
	MENU_ID_CLIENT_INGAMENICKNAMES = 1,
	MENU_ID_GLOBAL_WAITROOMLIST,
	MENU_ID_GLOBAL_WAITROOMLIST_WHITELIST,
	MENU_ID_GLOBAL_WAITROOMLIST_SERVERADMIN,
	MENU_ID_GLOBAL_WAITROOMLIST_POLICE,
	MENU_ID_CLIENT_MODERATION_COMPLETE,
	MENU_ID_CLIENT_MODERATION_TIMEOUT
};

/*
 * Initialize plugin menus.
 * This function is called after ts3plugin_init and ts3plugin_registerPluginID. A pluginID is required for plugin menus to work.
 * Both ts3plugin_registerPluginID and ts3plugin_freeMemory must be implemented to use menus.
 * If plugin menus are not used by a plugin, do not implement this function or return NULL.
 */
void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon) {
	/*
	 * Create the menus
	 * There are three types of menu items:
	 * - PLUGIN_MENU_TYPE_CLIENT:  Client context menu
	 * - PLUGIN_MENU_TYPE_CHANNEL: Channel context menu
	 * - PLUGIN_MENU_TYPE_GLOBAL:  "Plugins" menu in menu bar of main window
	 *
	 * Menu IDs are used to identify the menu item when ts3plugin_onMenuItemEvent is called
	 *
	 * The menu text is required, max length is 128 characters
	 *
	 * The icon is optional, max length is 128 characters. When not using icons, just pass an empty string.
	 * Icons are loaded from a subdirectory in the TeamSpeak client plugins folder. The subdirectory must be named like the
	 * plugin filename, without dll/so/dylib suffix
	 * e.g. for "test_plugin.dll", icon "1.png" is loaded from <TeamSpeak 3 Client install dir>\plugins\test_plugin\1.png
	 */
	BEGIN_CREATE_MENUS(6);  /* IMPORTANT: Number of menu items must be correct! */

	// Client menus
	//CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_INGAMENICKNAMES, "InGame Nicknames", "icons/ident.png");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_MODERATION_COMPLETE, "Anliegen abgeschlossen", "icons/complete.png");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT, MENU_ID_CLIENT_MODERATION_TIMEOUT, "War zum Mod. gespraech nicht zu erreichen", "icons/timeout.png");

	// Global menus
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_WAITROOMLIST, "Wartezimmer Prioriaet", "icons/list.png");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_WAITROOMLIST_WHITELIST, "Wartezimmer Whitelist Prioritaet", "icons/list.png");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_WAITROOMLIST_SERVERADMIN, "Wartezimmer ServerAdmin Prioritaet", "icons/list.png");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_WAITROOMLIST_POLICE, "Wartezimmer Polizei Prioritaet", "icons/list.png");

	END_CREATE_MENUS;  /* Includes an assert checking if the number of menu items matched */

	/*
	 * Specify an optional icon for the plugin. This icon is used for the plugins submenu within context and main menus
	 * If unused, set menuIcon to NULL
	 */
	*menuIcon = (char*)malloc(PLUGIN_MENU_BUFSZ * sizeof(char));
	_strcpy(*menuIcon, PLUGIN_MENU_BUFSZ, "icons/app.png");

	/*
	 * Menus can be enabled or disabled with: ts3Functions.setPluginMenuEnabled(pluginID, menuID, 0|1);
	 * Test it with plugin command: /test enablemenu <menuID> <0|1>
	 * Menus are enabled by default. Please note that shown menus will not automatically enable or disable when calling this function to
	 * ensure Qt menus are not modified by any thread other the UI thread. The enabled or disable state will change the next time a
	 * menu is displayed.
	 */
	/* For example, this would disable MENU_ID_GLOBAL_2: */
	/* ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 0); */

	/* All memory allocated in this function will be automatically released by the TeamSpeak client later by calling ts3plugin_freeMemory */
}

/************************** TeamSpeak callbacks ***************************/
/*
 * Following functions are optional, feel free to remove unused callbacks.
 * See the clientlib documentation for details on each function.
 */

/* Clientlib */

void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber) {
	if (newStatus == STATUS_CONNECTION_ESTABLISHED) {
		int errorCode;
		char *serverUID;
		if (ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &serverUID) == ERROR_ok) {
			if (strcmp(serverUID, "EguBkHqKJlxXcz9aRHzmy2V0Ke4=") == 0) {
				adminTool->setServer(serverConnectionHandlerID);
			}
			ts3Functions.freeMemory(serverUID);
		}

		if ((errorCode = ts3Functions.setClientSelfVariableAsString(serverConnectionHandlerID, CLIENT_META_DATA, ts3plugin_version())) != ERROR_ok) {
			char *errorMessage;
			if (ts3Functions.getErrorMessage(errorCode, &errorMessage)) {
				ts3Functions.logMessage("Error setClientSelfVariableAsString", LogLevel_WARNING, ts3plugin_name(), 0);
				ts3Functions.logMessage(errorMessage, LogLevel_WARNING, ts3plugin_name(), 0);
				ts3Functions.freeMemory(errorMessage);
			}
		}
		ts3Functions.flushClientSelfUpdates(serverConnectionHandlerID, NULL);
	}
	if (newStatus == STATUS_DISCONNECTED) {
		char *serverUID;
		if (ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &serverUID) == ERROR_ok) {
			if (strcmp(serverUID, "EguBkHqKJlxXcz9aRHzmy2V0Ke4=") == 0) {
				adminTool->setServer(NULL);
			}
			ts3Functions.freeMemory(serverUID);
		}
	}
}

void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {
	// only execute on We are friendly TeamSpeak
	if (!adminTool->checkServer(serverConnectionHandlerID)) {
		return;
	}

	// Add/remove clients for waitroom
	if (newChannelID == WAITROOM_CHANNEL_ID) {
		adminTool->addWaitRoomStack(clientID);
	}
	if (oldChannelID == WAITROOM_CHANNEL_ID) {
		adminTool->removeWaitRoomStack(clientID);
	}

	// Add/remove clients for waitroom whitelist
	if (newChannelID == WAITROOMWHITELIST_CHANNEL_ID) {
		adminTool->addWaitRoomWhitelistStack(clientID);
	}
	if (oldChannelID == WAITROOMWHITELIST_CHANNEL_ID) {
		adminTool->removeWaitRoomWhitelistStack(clientID);
	}

	// Add/remove clients for waitroom serveradmin
	if (newChannelID == WAITROOMSERVERADMIN_CHANNEL_ID) {
		adminTool->addWaitRoomServerAdminStack(clientID);
	}
	if (oldChannelID == WAITROOMSERVERADMIN_CHANNEL_ID) {
		adminTool->removeWaitRoomServerAdminStack(clientID);
	}

	// Add/remove clients for waitroom police
	if (newChannelID == WAITROOMPOLICE_CHANNEL_ID) {
		adminTool->addWaitRoomPoliceStack(clientID);
	}
	if (oldChannelID == WAITROOMPOLICE_CHANNEL_ID) {
		adminTool->removeWaitRoomPoliceStack(clientID);
	}
}

void ts3plugin_onClientMoveMovedEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID moverID, const char* moverName, const char* moverUniqueIdentifier, const char* moveMessage) {
	// only execute on We are friendly TeamSpeak
	if (!adminTool->checkServer(serverConnectionHandlerID)) {
		return;
	}

	// Add/remove clients for waitroom
	if (newChannelID == WAITROOM_CHANNEL_ID) {
		adminTool->addWaitRoomStack(clientID);
	}
	if (oldChannelID == WAITROOM_CHANNEL_ID) {
		adminTool->removeWaitRoomStack(clientID);
	}

	// Add/remove clients for waitroom whitelist
	if (newChannelID == WAITROOMWHITELIST_CHANNEL_ID) {
		adminTool->addWaitRoomWhitelistStack(clientID);
	}
	if (oldChannelID == WAITROOMWHITELIST_CHANNEL_ID) {
		adminTool->removeWaitRoomWhitelistStack(clientID);
	}

	// Add/remove clients for waitroom serveradmin
	if (newChannelID == WAITROOMSERVERADMIN_CHANNEL_ID) {
		adminTool->addWaitRoomServerAdminStack(clientID);
	}
	if (oldChannelID == WAITROOMSERVERADMIN_CHANNEL_ID) {
		adminTool->removeWaitRoomServerAdminStack(clientID);
	}

	// Add/remove clients for waitroom police
	if (newChannelID == WAITROOMPOLICE_CHANNEL_ID) {
		adminTool->addWaitRoomPoliceStack(clientID);
	}
	if (oldChannelID == WAITROOMPOLICE_CHANNEL_ID) {
		adminTool->removeWaitRoomPoliceStack(clientID);
	}
}

/* Client UI callbacks */

/*
 * Called when a plugin menu item (see ts3plugin_initMenus) is triggered. Optional function, when not using plugin menus, do not implement this.
 * 
 * Parameters:
 * - serverConnectionHandlerID: ID of the current server tab
 * - type: Type of the menu (PLUGIN_MENU_TYPE_CHANNEL, PLUGIN_MENU_TYPE_CLIENT or PLUGIN_MENU_TYPE_GLOBAL)
 * - menuItemID: Id used when creating the menu item
 * - selectedItemID: Channel or Client ID in the case of PLUGIN_MENU_TYPE_CHANNEL and PLUGIN_MENU_TYPE_CLIENT. 0 for PLUGIN_MENU_TYPE_GLOBAL.
 */
void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
	char *pluginName = "PLUGIN_NAME";
	strstr((const char *)pluginName, ts3plugin_name());

	// only execute on We are friendly TeamSpeak
	if (!adminTool->checkServer(serverConnectionHandlerID)) {
		return;
	}

	switch(type) {
		case PLUGIN_MENU_TYPE_GLOBAL:
			/* Global menu item was triggered. selectedItemID is unused and set to zero. */
			switch (menuItemID) {
				case MENU_ID_GLOBAL_WAITROOMLIST: {
					char *channelName;

					// get channel name
					if (ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, WAITROOM_CHANNEL_ID, CHANNEL_NAME, &channelName) == ERROR_ok) {
						anyID *clientList;

						// get all clients in channel
						if (ts3Functions.getChannelClientList(serverConnectionHandlerID, WAITROOM_CHANNEL_ID, &clientList) == ERROR_ok) {
							adminTool->validateWaitRoomStack(clientList);
							ts3Functions.freeMemory(clientList);
						};

						std::wstringstream waitRoomList;
						std::wstringstream waitRoomName;
						std::vector<anyID> waitRoomStock = adminTool->getWaitRoomStack();

						// create dialog title
						waitRoomName << channelName << " Priorität";

						// build priority list
						for (int i = 0; i < waitRoomStock.size(); i++) {
							char *clientNickname;
							if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, waitRoomStock[i], CLIENT_NICKNAME, &clientNickname) == ERROR_ok) {
								waitRoomList << "" << (i + 1) << ". " << clientNickname << "\n";
							}
						}

						// show dialog
						int nResult = MessageBox(NULL, waitRoomList.str().c_str(), waitRoomName.str().c_str(), MB_ICONINFORMATION | MB_OK);

						ts3Functions.freeMemory(channelName);
					}
					break;
				}
				case MENU_ID_GLOBAL_WAITROOMLIST_WHITELIST: {
					char *channelName;

					// get channel name
					if (ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, WAITROOMWHITELIST_CHANNEL_ID, CHANNEL_NAME, &channelName) == ERROR_ok) {
						anyID *clientList;

						// get all clients in channel
						if (ts3Functions.getChannelClientList(serverConnectionHandlerID, WAITROOMWHITELIST_CHANNEL_ID, &clientList) == ERROR_ok) {
							adminTool->validateWaitRoomWhitelistStack(clientList);
							ts3Functions.freeMemory(clientList);
						};

						std::wstringstream waitRoomList;
						std::wstringstream waitRoomName;
						std::vector<anyID> waitRoomStock = adminTool->getWaitRoomWhitelistStack();

						// create dialog title
						waitRoomName << channelName << " Priorität";

						// build priority list
						for (int i = 0; i < waitRoomStock.size(); i++) {
							char *clientNickname;
							if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, waitRoomStock[i], CLIENT_NICKNAME, &clientNickname) == ERROR_ok) {
								waitRoomList << "" << (i + 1) << ". " << clientNickname << "\n";
							}
						}

						// show dialog
						int nResult = MessageBox(NULL, waitRoomList.str().c_str(), waitRoomName.str().c_str(), MB_ICONINFORMATION | MB_OK);

						ts3Functions.freeMemory(channelName);
					}
					break;
				}
				case MENU_ID_GLOBAL_WAITROOMLIST_POLICE: {
					char *channelName;

					// get channel name
					if (ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, WAITROOMPOLICE_CHANNEL_ID, CHANNEL_NAME, &channelName) == ERROR_ok) {
						anyID *clientList;

						// get all clients in channel
						if (ts3Functions.getChannelClientList(serverConnectionHandlerID, WAITROOMPOLICE_CHANNEL_ID, &clientList) == ERROR_ok) {
							adminTool->validateWaitRoomPoliceStack(clientList);
							ts3Functions.freeMemory(clientList);
						};

						std::wstringstream waitRoomList;
						std::wstringstream waitRoomName;
						std::vector<anyID> waitRoomStock = adminTool->getWaitRoomPoliceStack();

						// create dialog title
						waitRoomName << channelName << " Priorität";

						// build priority list
						for (int i = 0; i < waitRoomStock.size(); i++) {
							char *clientNickname;
							if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, waitRoomStock[i], CLIENT_NICKNAME, &clientNickname) == ERROR_ok) {
								waitRoomList << "" << (i + 1) << ". " << clientNickname << "\n";
							}
						}

						// show dialog
						int nResult = MessageBox(NULL, waitRoomList.str().c_str(), waitRoomName.str().c_str(), MB_ICONINFORMATION | MB_OK);

						ts3Functions.freeMemory(channelName);
					}
					break;
				}
				case MENU_ID_GLOBAL_WAITROOMLIST_SERVERADMIN: {
					char *channelName;

					// get channel name
					if (ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, WAITROOMSERVERADMIN_CHANNEL_ID, CHANNEL_NAME, &channelName) == ERROR_ok) {
						anyID *clientList;

						// get all clients in channel
						if (ts3Functions.getChannelClientList(serverConnectionHandlerID, WAITROOMSERVERADMIN_CHANNEL_ID, &clientList) == ERROR_ok) {
							adminTool->validateWaitRoomServerAdminStack(clientList);
							ts3Functions.freeMemory(clientList);
						};

						std::wstringstream waitRoomList;
						std::wstringstream waitRoomName;
						std::vector<anyID> waitRoomStock = adminTool->getWaitRoomServerAdminStack();

						// create dialog title
						waitRoomName << channelName << " Priorität";

						// build priority list
						for (int i = 0; i < waitRoomStock.size(); i++) {
							char *clientNickname;
							if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, waitRoomStock[i], CLIENT_NICKNAME, &clientNickname) == ERROR_ok) {
								waitRoomList << "" << (i + 1) << ". " << clientNickname << "\n";
							}
						}

						// show dialog
						int nResult = MessageBox(NULL, waitRoomList.str().c_str(), waitRoomName.str().c_str(), MB_ICONINFORMATION | MB_OK);

						ts3Functions.freeMemory(channelName);
					}
				}
				default: {
					break;
				}
			}
			break;
		case PLUGIN_MENU_TYPE_CLIENT: {
			/* Client contextmenu item was triggered. selectedItemID is the clientID of the selected client */
			switch (menuItemID) {
				case MENU_ID_CLIENT_INGAMENICKNAMES: {
					/* Menu client InGame Nicknames was triggered */
					char *clientUID = NULL;
					if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)selectedItemID, 0, &clientUID) != ERROR_ok) {
						ts3Functions.logMessage("Error getClientVariableAsString", LogLevel_INFO, pluginName, serverConnectionHandlerID);
					}
					/*
					if (clientUID != NULL) {
						anyID clientId;
						ts3Functions.getClientID(serverConnectionHandlerID, &clientId);
						CURL *curl;
						CURLcode res;
						curl = curl_easy_init();

						// CURL init success
						if (curl) {
							// build url
							std::string requestUrl = "";
							requestUrl.append(clientUID);
							requestUrl = urlDecode(requestUrl);
							requestUrl = "http://www.fankserver.com/request/ts3/" + requestUrl;
							curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());

							// catch response
							std::string response;
							curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
							curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

							// perform & cleanup
							res = curl_easy_perform(curl);
							curl_easy_cleanup(curl);

							const char *returnCode;
							ts3Functions.requestSendPrivateTextMsg(serverConnectionHandlerID, response.c_str(), clientId, returnCode);
						}
						else {
							ts3Functions.logMessage("curl_easy_init() failed", LogLevel_CRITICAL, pluginName, serverConnectionHandlerID);
						}
					}*/
					ts3Functions.freeMemory(clientUID);
					break;
				}
				case MENU_ID_CLIENT_MODERATION_COMPLETE: {
					// move client in complete room
					int errorCode;
					if ((errorCode = ts3Functions.requestClientMove(serverConnectionHandlerID, (anyID)selectedItemID, WAITROOM_COMPLETE_CHANNEL_ID, "", NULL)) != ERROR_ok) {
						char *errorMessage;
						if (ts3Functions.getErrorMessage(errorCode, &errorMessage)) {
							ts3Functions.logMessage("Error requestClientMove", LogLevel_WARNING, "We Are Friendly Plugin", 0);
							ts3Functions.logMessage(errorMessage, LogLevel_WARNING, "We Are Friendly Plugin", 0);
							ts3Functions.freeMemory(errorMessage);
						}
					}
					break;
				}
				case MENU_ID_CLIENT_MODERATION_TIMEOUT: {
					// move client in not reachable room
					int errorCode;
					if ((errorCode = ts3Functions.requestClientMove(serverConnectionHandlerID, (anyID)selectedItemID, WAITROOM_TIMEOUT_CHANNEL_ID, "", NULL)) != ERROR_ok) {
						char *errorMessage;
						if (ts3Functions.getErrorMessage(errorCode, &errorMessage)) {
							ts3Functions.logMessage("Error requestClientMove", LogLevel_WARNING, "We Are Friendly Plugin", 0);
							ts3Functions.logMessage(errorMessage, LogLevel_WARNING, "We Are Friendly Plugin", 0);
							ts3Functions.freeMemory(errorMessage);
						}
					}
					break;
				}
				default: {
					break;
				}
			}
			break;
		}
		default:
			break;
	}
}