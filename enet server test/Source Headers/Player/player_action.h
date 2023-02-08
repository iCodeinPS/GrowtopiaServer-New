#pragma once

int getState(PlayerInfo* info) {
	int val = 0;
	val |= info->canWalkInBlocks << 0;
	val |= info->canDoubleJump << 1;
	val |= info->isInvisible << 2;
	val |= info->noHands << 3;
	val |= info->noEyes << 4;
	val |= info->noBody << 5;
	val |= info->devilHorns << 6;
	val |= info->goldenHalo << 7;
	val |= info->isFrozen << 11;
	val |= info->isCursed << 12;
	val |= info->isDuctaped << 13;
	val |= info->haveCigar << 14;
	val |= info->isShining << 15;
	val |= info->isZombie << 16;
	val |= info->isHitByLava << 17;
	val |= info->haveHauntedShadows << 18;
	val |= info->haveGeigerRadiation << 19;
	val |= info->haveReflector << 20;
	val |= info->isEgged << 21;
	val |= info->havePineappleFloag << 22;
	val |= info->haveFlyingPineapple << 23;
	val |= info->haveSuperSupporterName << 24;
	val |= info->haveSupperPineapple << 25;
	return val;
}

namespace packet {
	void consolemessage(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnConsoleMessage");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void OnAddNotification(ENetPeer* peer, string text, string audiosound, string interfaceimage) {
		auto p = packetEnd(appendInt(appendString(appendString(appendString(appendString(createPacket(), "OnAddNotification"),
			interfaceimage),
			text),
			audiosound),
			0));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}
	void OnNameChanged(ENetPeer* peer, int netID, string name) {
		gamepacket_t p;
		p.Insert("OnNameChanged");
		p.Insert(netID);
		p.Insert(name);
		p.CreatePacket(peer);
	}
	void dialog(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnDialogRequest");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void onspawn(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnSpawn");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void requestworldselectmenu(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnRequestWorldSelectMenu");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void storerequest(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnStoreRequest");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void storepurchaseresult(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnStorePurchaseResult");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void SendTalkSelf(ENetPeer* peer, string text) {
		GamePacket p2 = packetEnd(appendIntx(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), ((PlayerInfo*)(peer->data))->netID), text), 0), 1));
		ENetPacket* packet2 = enet_packet_create(p2.data,
			p2.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet2);
		delete p2.data;
	}
	void OnTalkBubble(ENetPeer* peer, int netID, string text, int chatColor, bool isOverlay) {
		if (isOverlay == true) {
			GamePacket p = packetEnd(appendIntx(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"),
				((PlayerInfo*)(peer->data))->netID), text), chatColor), 1));
			ENetPacket* packet = enet_packet_create(p.data,
				p.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
			delete p.data;
		}
		else {
			GamePacket p = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"),
				((PlayerInfo*)(peer->data))->netID), text), chatColor));
			ENetPacket* packet = enet_packet_create(p.data,
				p.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
			delete p.data;
		}
	}
	void OnTextOverlay(ENetPeer* peer, string text) {
		GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnTextOverlay"), text));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}
	void OnFailedToEnterWorld(ENetPeer* peer) {
		GamePacket p = packetEnd(appendIntx(appendString(createPacket(), "OnFailedToEnterWorld"), 1));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}
	void PlayAudio(ENetPeer* peer, string audioFile, int delayMS) {
		string text = "action|play_sfx\nfile|" + audioFile + "\ndelayMS|" + to_string(delayMS) + "\n";
		BYTE* data = new BYTE[5 + text.length()];
		BYTE zero = 0;
		int type = 3;
		memcpy(data, &type, 4);
		memcpy(data + 4, text.c_str(), text.length());
		memcpy(data + 4 + text.length(), &zero, 1);
		ENetPacket* packet = enet_packet_create(data,
			5 + text.length(),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete[] data;
	}
}

void savegem(ENetPeer* peer) {
	if (((PlayerInfo*)(peer->data))->haveGrowId == true) {
		PlayerInfo* p5 = ((PlayerInfo*)(peer->data));
		string username = PlayerDB::getProperName(p5->rawName);
		ifstream fg("gemdb/" + ((PlayerInfo*)(peer->data))->rawName + ".txt");
		json j;
		fg >> j;
		fg.close();
		ofstream fs("gemdb/" + ((PlayerInfo*)(peer->data))->rawName + ".txt");
		fs << j;
		fs.close();
	}
}
void sendConsole(ENetPeer* x, string e) {
	GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), e));
	ENetPacket* packet = enet_packet_create(p.data,
		p.len,
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(x, 0, packet);
	delete p.data;
}
bool isWorldOwner(ENetPeer* peer, WorldInfo* world) {
	return ((PlayerInfo*)(peer->data))->rawName == world->owner;
}
bool isWorldAdmin(ENetPeer* peer, WorldInfo* world) {
	const auto uid = ((PlayerInfo*)(peer->data))->rawName;
	for (const auto i = 0; world->acclist.size();) {
		const auto x = world->acclist.at(i);
		if (uid == x.substr(0, x.find("|"))) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}
void savejson(ENetPeer* peer) {
	if (((PlayerInfo*)(peer->data))->haveGrowId == true) {
		PlayerInfo* p5 = ((PlayerInfo*)(peer->data));
		string username = PlayerDB::getProperName(p5->rawName);
		ifstream fg("players/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
		json j;
		fg >> j;
		fg.close();
		j["premwl"] = ((PlayerInfo*)(peer->data))->premwl;
		j["level"] = ((PlayerInfo*)(peer->data))->level;
		j["xp"] = ((PlayerInfo*)(peer->data))->xp;
		ofstream fs("players/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
		fs << j;
		fs.close();
	}
}

string PlayerDB::getProperName(string name) {
	string newS;
	for (char c : name) newS += (c >= 'A' && c <= 'Z') ? c - ('A' - 'a') : c;
	string ret;
	for (int i = 0; i < newS.length(); i++)
	{
		if (newS[i] == '`') i++; else ret += newS[i];
	}
	string ret2;
	for (char c : ret) if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) ret2 += c;

	string username = ret2;
	if (username == "prn" || username == "con" || username == "aux" || username == "nul" || username == "com1" || username == "com2" || username == "com3" || username == "com4" || username == "com5" || username == "com6" || username == "com7" || username == "com8" || username == "com9" || username == "lpt1" || username == "lpt2" || username == "lpt3" || username == "lpt4" || username == "lpt5" || username == "lpt6" || username == "lpt7" || username == "lpt8" || username == "lpt9") {
		return "";
	}

	return ret2;
}

string PlayerDB::fixColors(string text) {
	string ret = "";
	int colorLevel = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == '`')
		{
			ret += text[i];
			if (i + 1 < text.length())
				ret += text[i + 1];


			if (i + 1 < text.length() && text[i + 1] == '`')
			{
				colorLevel--;
			}
			else {
				colorLevel++;
			}
			i++;
		}
		else {
			ret += text[i];
		}
	}
	for (int i = 0; i < colorLevel; i++) {
		ret += "``";
	}
	for (int i = 0; i > colorLevel; i--) {
		ret += "`w";
	}
	return ret;
}

struct Admin {
	string username;
	string password;
	int level = 0;
	long long int lastSB = 0;
};

vector<Admin> admins;

int PlayerDB::playerLogin(ENetPeer* peer, string username, string password) {
	std::ifstream ifs("players/" + PlayerDB::getProperName(username) + ".json");
	if (ifs.is_open()) {
		json j;
		ifs >> j;
		string pss = j["password"];
		int adminLevel = j["adminLevel"];
		if (password == pss) {
			((PlayerInfo*)(peer->data))->hasLogon = true;
			bool found = false;
			for (int i = 0; i < admins.size(); i++) {
				if (admins[i].username == username) {
					found = true;
				}
			}
			if (!found) {
				if (adminLevel != 0) {
					Admin admin;
					admin.username = PlayerDB::getProperName(username);
					admin.password = pss;
					admin.level = adminLevel;
					admins.push_back(admin);
				}
			}
			ENetPeer* currentPeer;

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (currentPeer == peer)
					continue;
				if (((PlayerInfo*)(currentPeer->data))->rawName == PlayerDB::getProperName(username))
				{
					{
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("Someone else logged into this account!");
						p.CreatePacket(peer);
					}
					{
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("Someone else was logged into this account! He was kicked out now.");
						p.CreatePacket(peer);
					}
					enet_peer_disconnect_later(currentPeer, 0);
				}
			}
			return 1;
		}
		else {
			return -1;
		}
	}
	else {
		return -2;
	}
}

int PlayerDB::guildRegister(ENetPeer* peer, string guildName, string guildStatement, string guildFlagfg, string guildFlagbg) {
	if (guildName.find(" ") != string::npos || guildName.find(".") != string::npos || guildName.find(",") != string::npos || guildName.find("@") != string::npos || guildName.find("[") != string::npos || guildName.find("]") != string::npos || guildName.find("#") != string::npos || guildName.find("<") != string::npos || guildName.find(">") != string::npos || guildName.find(":") != string::npos || guildName.find("{") != string::npos || guildName.find("}") != string::npos || guildName.find("|") != string::npos || guildName.find("+") != string::npos || guildName.find("_") != string::npos || guildName.find("~") != string::npos || guildName.find("-") != string::npos || guildName.find("!") != string::npos || guildName.find("$") != string::npos || guildName.find("%") != string::npos || guildName.find("^") != string::npos || guildName.find("&") != string::npos || guildName.find("`") != string::npos || guildName.find("*") != string::npos || guildName.find("(") != string::npos || guildName.find(")") != string::npos || guildName.find("=") != string::npos || guildName.find("'") != string::npos || guildName.find(";") != string::npos || guildName.find("/") != string::npos) {
		return -1;
	}

	if (guildName.length() < 3) {
		return -2;
	}
	if (guildName.length() > 15) {
		return -3;
	}
	int fg;
	int bg;

	try {
		fg = stoi(guildFlagfg);
	}
	catch (std::invalid_argument& e) {
		return -6;
	}
	try {
		bg = stoi(guildFlagbg);
	}
	catch (std::invalid_argument& e) {
		return -5;
	}
	if (guildFlagbg.length() > 4) {
		return -7;
	}
	if (guildFlagfg.length() > 4) {
		return -8;
	}

	string fixedguildName = PlayerDB::getProperName(guildName);

	std::ifstream ifs("guilds/" + fixedguildName + ".json");
	if (ifs.is_open()) {
		return -4;
	}
	return 1;
}

int PlayerDB::playerRegister(ENetPeer* peer, string username, string password, string passwordverify, string email, string discord) {
	string name = username;
	if (name == "CON" || name == "PRN" || name == "AUX" || name == "NUL" || name == "COM1" || name == "COM2" || name == "COM3" || name == "COM4" || name == "COM5" || name == "COM6" || name == "COM7" || name == "COM8" || name == "COM9" || name == "LPT1" || name == "LPT2" || name == "LPT3" || name == "LPT4" || name == "LPT5" || name == "LPT6" || name == "LPT7" || name == "LPT8" || name == "LPT9") return -1;
	username = PlayerDB::getProperName(username);
	if (discord.find("#") == std::string::npos && discord.length() != 0) return -5;
	if (email.find("@") == std::string::npos && email.length() != 0) return -4;
	if (passwordverify != password) return -3;
	if (username.length() < 3) return -2;
	std::ifstream ifs("players/" + username + ".json");
	if (ifs.is_open()) {
		return -1;
	}

	std::ofstream o("players/" + username + ".json");
	if (!o.is_open()) {
		cout << GetLastError() << endl;
		_getch();
	}
	json j;
	j["username"] = username;
	j["password"] = password;
	j["email"] = email;
	j["discord"] = discord;
	j["adminLevel"] = 0;
	j["level"] = 1;
	j["xp"] = 0;
	j["ClothBack"] = 0;
	j["ClothHand"] = 0;
	j["ClothFace"] = 0;
	j["ClothShirt"] = 0;
	j["ClothPants"] = 0;
	j["ClothNeck"] = 0;
	j["ClothHair"] = 0;
	j["ClothFeet"] = 0;
	j["ClothMask"] = 0;
	j["ClothAnces"] = 0;
	j["guild"] = "";
	j["joinguild"] = false;
	j["premwl"] = 0;
	j["effect"] = 8421376;
	j["skinColor"] = 0x8295C3FF;
	j["worldsowned"] = ((PlayerInfo*)(peer->data))->createworldsowned;
	o << j << std::endl;

	std::ofstream oo("inventory/" + username + ".json");
	if (!oo.is_open()) {
		cout << GetLastError() << endl;
		_getch();
	}

	json items;
	json jjall = json::array();


	json jj;
	jj["aposition"] = 1;
	jj["itemid"] = 18;
	jj["quantity"] = 1;
	jjall.push_back(jj);


	jj["aposition"] = 2;
	jj["itemid"] = 32;
	jj["quantity"] = 1;
	jjall.push_back(jj);

	for (int i = 2; i < 200; i++)
	{
		jj["aposition"] = i + 1;
		jj["itemid"] = 0;
		jj["quantity"] = 0;
		jjall.push_back(jj);
	}

	items["items"] = jjall;
	oo << items << std::endl;

	return 1;
}

struct PlayerMoving {
	int packetType;
	int netID;
	float x;
	float y;
	int characterState;
	int plantingTree;
	float XSpeed;
	float YSpeed;
	int punchX;
	int punchY;

};

int level(string name) {
	std::ifstream ifff("players/" + PlayerDB::getProperName(name) + ".json");
	json j;
	ifff >> j;

	int level;
	level = j["level"];

	ifff.close();
	return level;

}

bool isHere(ENetPeer* peer, ENetPeer* peer2)
{
	return ((PlayerInfo*)(peer->data))->currentWorld == ((PlayerInfo*)(peer2->data))->currentWorld;
}

void sendInventory(ENetPeer* peer, PlayerInventory inventory)
{
	string asdf2 = "0400000009A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000000000000000";
	int inventoryLen = inventory.items.size();
	int packetLen = (asdf2.length() / 2) + (inventoryLen * 4) + 4;
	BYTE* data2 = new BYTE[packetLen];
	for (int i = 0; i < asdf2.length(); i += 2)
	{
		char x = ch2n(asdf2[i]);
		x = x << 4;
		x += ch2n(asdf2[i + 1]);
		memcpy(data2 + (i / 2), &x, 1);
	}
	int endianInvVal = _byteswap_ulong(inventoryLen);
	memcpy(data2 + (asdf2.length() / 2) - 4, &endianInvVal, 4);
	endianInvVal = _byteswap_ulong(((PlayerInfo*)(peer->data))->currentInventorySize);
	memcpy(data2 + (asdf2.length() / 2) - 8, &endianInvVal, 4);
	int val = 0;
	for (int i = 0; i < inventoryLen; i++)
	{
		val = 0;
		val |= inventory.items.at(i).itemID;
		val |= inventory.items.at(i).itemCount << 16;
		val &= 0x00FFFFFF;
		val |= 0x00 << 24;
		memcpy(data2 + (i * 4) + (asdf2.length() / 2), &val, 4);
	}
	ENetPacket* packet3 = enet_packet_create(data2,
		packetLen,
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet3);
	delete data2;
	//enet_host_flush(server);
}


BYTE* packPlayerMoving(PlayerMoving* dataStruct)
{
	BYTE* data = new BYTE[56];
	for (int i = 0; i < 56; i++)
	{
		data[i] = 0;
	}
	memcpy(data, &dataStruct->packetType, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 20, &dataStruct->plantingTree, 4);
	memcpy(data + 24, &dataStruct->x, 4);
	memcpy(data + 28, &dataStruct->y, 4);
	memcpy(data + 32, &dataStruct->XSpeed, 4);
	memcpy(data + 36, &dataStruct->YSpeed, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	return data;
}

BYTE* packBlockVisual(BlockVisual* dataStruct)
{
	BYTE* data = new BYTE[72];
	for (int i = 0; i < 72; i++)
	{
		data[i] = 0;
	}
	memcpy(data, &dataStruct->packetType, 4);
	memcpy(data + 8, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	memcpy(data + 52, &dataStruct->charStat, 4);
	memcpy(data + 56, &dataStruct->blockid, 4);
	//memcpy(data + 58, &dataStruct->backgroundid, 4);
	memcpy(data + 60, &dataStruct->visual, 4);
	memcpy(data + 64, &dataStruct->displayblock, 4);


	return data;
}

PlayerMoving* unpackPlayerMoving(BYTE* data)
{
	PlayerMoving* dataStruct = new PlayerMoving;
	memcpy(&dataStruct->packetType, data, 4);
	memcpy(&dataStruct->netID, data + 4, 4);
	memcpy(&dataStruct->characterState, data + 12, 4);
	memcpy(&dataStruct->plantingTree, data + 20, 4);
	memcpy(&dataStruct->x, data + 24, 4);
	memcpy(&dataStruct->y, data + 28, 4);
	memcpy(&dataStruct->XSpeed, data + 32, 4);
	memcpy(&dataStruct->YSpeed, data + 36, 4);
	memcpy(&dataStruct->punchX, data + 44, 4);
	memcpy(&dataStruct->punchY, data + 48, 4);
	return dataStruct;
}

void sendState(ENetPeer* peer) {
	//return; // TODO
	PlayerInfo* info = ((PlayerInfo*)(peer->data));
	int netID = info->netID;
	ENetPeer* currentPeer;
	int state = getState(info);
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			PlayerMoving data;
			data.packetType = 0x14;
			data.characterState = 0; // animation
			data.x = 1000;
			data.y = 100;
			data.punchX = 0;
			data.punchY = 0;
			data.XSpeed = 300;
			data.YSpeed = 600;
			data.netID = netID;
			data.plantingTree = state;
			BYTE* raw = packPlayerMoving(&data);
			int var = info->peffect; // placing and breking
			memcpy(raw + 1, &var, 3);
			float waterspeed = 125.0f;
			memcpy(raw + 16, &waterspeed, 4);
			SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
		}
	}
	// TODO
}
void SendDropSingle(ENetPeer* peer, int netID, int x, int y, int item, int count, BYTE specialEffect)
{
	if (item >= maxItems) return;
	if (item < 0) return;

	PlayerMoving data;
	data.packetType = 14;
	data.x = x;
	data.y = y;
	data.netID = netID;
	data.plantingTree = item;
	float val = count; // item count
	BYTE val2 = specialEffect;

	BYTE* raw = packPlayerMoving(&data);
	memcpy(raw + 16, &val, 4);
	memcpy(raw + 1, &val2, 1);

	SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
}

void onPeerConnect(ENetPeer* peer)
{
	ENetPeer* currentPeer;

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (peer != currentPeer)
		{
			if (isHere(peer, currentPeer))
			{
				string netIdS = std::to_string(((PlayerInfo*)(currentPeer->data))->netID);
				packet::onspawn(peer, "spawn|avatar\nnetID|" + netIdS + "\nuserID|" + netIdS + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(currentPeer->data))->x) + "|" + std::to_string(((PlayerInfo*)(currentPeer->data))->y) + "\nname|``" + ((PlayerInfo*)(currentPeer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(currentPeer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
				string netIdS2 = std::to_string(((PlayerInfo*)(peer->data))->netID);
				packet::onspawn(currentPeer, "spawn|avatar\nnetID|" + netIdS2 + "\nuserID|" + netIdS2 + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(peer->data))->x) + "|" + std::to_string(((PlayerInfo*)(peer->data))->y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
			}
		}
	}

}

void updateAllClothes(ENetPeer* peer)
{
	ENetPeer* currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			GamePacket p3 = packetEnd(appendFloat(appendIntx(appendFloat(appendFloat(appendFloat(appendString(createPacket(), "OnSetClothing"), ((PlayerInfo*)(peer->data))->cloth_hair, ((PlayerInfo*)(peer->data))->cloth_shirt, ((PlayerInfo*)(peer->data))->cloth_pants), ((PlayerInfo*)(peer->data))->cloth_feet, ((PlayerInfo*)(peer->data))->cloth_face, ((PlayerInfo*)(peer->data))->cloth_hand), ((PlayerInfo*)(peer->data))->cloth_back, ((PlayerInfo*)(peer->data))->cloth_mask, ((PlayerInfo*)(peer->data))->cloth_necklace), ((PlayerInfo*)(peer->data))->skinColor), ((PlayerInfo*)(peer->data))->cloth_ances, 0.0f, 0.0f));
			memcpy(p3.data + 8, &(((PlayerInfo*)(peer->data))->netID), 4); // ffloor
			ENetPacket* packet3 = enet_packet_create(p3.data,
				p3.len,
				ENET_PACKET_FLAG_RELIABLE);

			enet_peer_send(currentPeer, 0, packet3);
			delete p3.data;
			//enet_host_flush(server);
			GamePacket p4 = packetEnd(appendFloat(appendIntx(appendFloat(appendFloat(appendFloat(appendString(createPacket(), "OnSetClothing"), ((PlayerInfo*)(currentPeer->data))->cloth_hair, ((PlayerInfo*)(currentPeer->data))->cloth_shirt, ((PlayerInfo*)(currentPeer->data))->cloth_pants), ((PlayerInfo*)(currentPeer->data))->cloth_feet, ((PlayerInfo*)(currentPeer->data))->cloth_face, ((PlayerInfo*)(currentPeer->data))->cloth_hand), ((PlayerInfo*)(currentPeer->data))->cloth_back, ((PlayerInfo*)(currentPeer->data))->cloth_mask, ((PlayerInfo*)(currentPeer->data))->cloth_necklace), ((PlayerInfo*)(currentPeer->data))->skinColor), ((PlayerInfo*)(currentPeer->data))->cloth_ances, 0.0f, 0.0f));
			memcpy(p4.data + 8, &(((PlayerInfo*)(currentPeer->data))->netID), 4); // ffloor
			ENetPacket* packet4 = enet_packet_create(p4.data,
				p4.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet4);
			delete p4.data;
			//enet_host_flush(server);
		}
	}
}

void sendClothes(ENetPeer* peer) {
	GamePacket p3 = packetEnd(appendFloat(appendIntx(appendFloat(appendFloat(appendFloat(appendString(createPacket(), "OnSetClothing"), ((PlayerInfo*)(peer->data))->cloth_hair, ((PlayerInfo*)(peer->data))->cloth_shirt, ((PlayerInfo*)(peer->data))->cloth_pants), ((PlayerInfo*)(peer->data))->cloth_feet, ((PlayerInfo*)(peer->data))->cloth_face, ((PlayerInfo*)(peer->data))->cloth_hand), ((PlayerInfo*)(peer->data))->cloth_back, ((PlayerInfo*)(peer->data))->cloth_mask, ((PlayerInfo*)(peer->data))->cloth_necklace), ((PlayerInfo*)(peer->data))->skinColor), ((PlayerInfo*)(peer->data))->cloth_ances, 0.0f, 0.0f));
	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		if (isHere(peer, currentPeer)) {
			string text = "action|play_sfx\nfile|audio/change_clothes.wav\ndelayMS|0\n";
			BYTE* data = new BYTE[5 + text.length()];
			BYTE zero = 0;
			int type = 3;
			memcpy(data, &type, 4);
			memcpy(data + 4, text.c_str(), text.length());
			memcpy(data + 4 + text.length(), &zero, 1);
			ENetPacket* packet2 = enet_packet_create(data,
				5 + text.length(),
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(currentPeer, 0, packet2);
			delete[] data;
			memcpy(p3.data + 8, &(((PlayerInfo*)(peer->data))->netID), 4); // ffloor
			ENetPacket* packet3 = enet_packet_create(p3.data,
				p3.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(currentPeer, 0, packet3);
		}
	}
	ifstream fg("players/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
	json j;
	fg >> j;
	fg.close();
	PlayerInfo* p = ((PlayerInfo*)(peer->data));
	string username = PlayerDB::getProperName(p->rawName);
	int wls = p->premwl;
	int clothback = p->cloth_back;
	int clothhand = p->cloth_hand;
	int clothface = p->cloth_face;
	int clothhair = p->cloth_hair;
	int clothfeet = p->cloth_feet;
	int clothpants = p->cloth_pants;
	int clothneck = p->cloth_necklace;
	int clothshirt = p->cloth_shirt;
	int clothmask = p->cloth_mask;
	int clothances = p->cloth_ances;
	j["ClothBack"] = clothback;
	j["ClothHand"] = clothhand;
	j["ClothFace"] = clothface;
	j["ClothShirt"] = clothshirt;
	j["ClothPants"] = clothpants;
	j["ClothNeck"] = clothneck;
	j["ClothHair"] = clothhair;
	j["ClothFeet"] = clothfeet;
	j["ClothMask"] = clothmask;
	j["ClothAnces"] = clothances;
	j["effect"] = p->peffect;
	j["premwl"] = p->premwl;
	j["worldsowned"] = ((PlayerInfo*)(peer->data))->worldsowned;
	ofstream fs("players/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
	fs << j;
	fs.close();
}
void sendEmoji(ENetPeer* peer, string emoji)
{
	GamePacket p2ssw = packetEnd(appendString(appendInt(appendString(createPacket(), "OnEmoticonDataChanged"), 0), u8"(wl)|ā|1&(yes)|Ă|1&(no)|ă|1&(love)|Ą|1&(oops)|ą|1&(shy)|Ć|1&(wink)|ć|1&(tongue)|Ĉ|1&(agree)|ĉ|1&(sleep)|Ċ|1&(punch)|ċ|1&(music)|Č|1&(build)|č|1&(megaphone)|Ď|1&(sigh)|ď|1&(mad)|Đ|1&(wow)|đ|1&(dance)|Ē|1&(see-no-evil)|ē|1&(bheart)|Ĕ|1&(heart)|ĕ|1&(grow)|Ė|1&(gems)|ė|1&(kiss)|Ę|1&(gtoken)|ę|1&(lol)|Ě|1&(smile)|Ā|1&(cool)|Ĝ|1&(cry)|ĝ|1&(vend)|Ğ|1&(bunny)|ě|1&(cactus)|ğ|1&(pine)|Ĥ|1&(peace)|ģ|1&(terror)|ġ|1&(troll)|Ġ|1&(evil)|Ģ|1&(fireworks)|Ħ|1&(football)|ĥ|1&(alien)|ħ|1&(party)|Ĩ|1&(pizza)|ĩ|1&(clap)|Ī|1&(song)|ī|1&(ghost)|Ĭ|1&(nuke)|ĭ|1&(halo)|Į|1&(turkey)|į|1&(gift)|İ|1&(cake)|ı|1&(heartarrow)|Ĳ|1&(lucky)|ĳ|1&(shamrock)|Ĵ|1&(grin)|ĵ|1&(ill)|Ķ|1&"));
	ENetPacket* packet2ssw = enet_packet_create(p2ssw.data, p2ssw.len, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet2ssw);
	delete p2ssw.data;
}
void craftItemText() {
	int current = -1;
	std::ifstream infile("effect.txt");
	for (std::string line; getline(infile, line);) {
		if (line.length() > 5 && line[0] != '/' && line[1] != '/') {
			vector<string> ex = explode("|", line);
			ItemDefinition def;
			itemDefs.at(atoi(ex[0].c_str())).effect = ex[3] + " `$(`o" + ex[1] + " `omod removed)";
			itemDefs.at(atoi(ex[0].c_str())).effects = ex[2] + " `$(`o" + ex[1] + " `omod added)";
		}
	}
}
void getAutoEffect(ENetPeer* peer) {
	PlayerInfo* info = ((PlayerInfo*)(peer->data));
	if (info->cloth_hand == 5480) {
		info->peffect = 8421456;
		craftItemText();
	}
	else if (info->cloth_necklace == 11232) {
		info->peffect = 8421376 + 224;
		craftItemText();
	}
	else if (info->cloth_hand == 11440) {
		info->peffect = 8421376 + 111;
		craftItemText();
	}
	else if (info->cloth_hand == 5276) {
		info->peffect = 8421376 + 47;
		craftItemText();
	}
	else if (info->cloth_hair == 4746) {
		info->peffect = 8421376 + 75;
		craftItemText();
	}
	else if (info->cloth_hair == 4748) {
		info->peffect = 8421376 + 75;
		craftItemText();
	}
	else if (info->cloth_hair == 4750) {
		info->peffect = 8421376 + 75;
		craftItemText();
	}
	else if (info->cloth_hand == 10652) {
		info->peffect = 8421376 + 188;
		craftItemText();
	}
	else if (info->cloth_hand == 9716) {
		info->peffect = 8421529;
		craftItemText();
	}
	else if (info->cloth_shirt == 1780) {
		info->peffect = 8421396;
		craftItemText();
	}
	else if (info->cloth_shirt == 10426) {
		info->peffect = 8421559;
		craftItemText();
	}
	else if (info->cloth_face == 1204) {
		info->peffect = 8421386;
		craftItemText();
	}
	else if (info->cloth_face == 10128) {
		info->peffect = 8421376 + 683;
		craftItemText();
	}
	else if (info->cloth_feet == 10618) {
		info->peffect = 8421376 + 699;
		craftItemText();
	}
	else if (info->cloth_face == 138) {
		info->peffect = 8421377;
		craftItemText();
	}
	else if (info->cloth_face == 2476) {
		info->peffect = 8421415;
		craftItemText();
	}
	else if (info->cloth_hand == 366 || info->cloth_hand == 1464) {
		info->peffect = 8421378;
		craftItemText();
	}
	else if (info->cloth_hand == 472) {
		info->peffect = 8421379;
		craftItemText();
	}
	else if (info->cloth_hand == 7912) {
		info->peffect = 8421487;
		craftItemText();
	}
	else if (info->cloth_hand == 594) {
		info->peffect = 8421380;
		craftItemText();
	}
	else if (info->cloth_hand == 768) {
		info->peffect = 8421381;
		craftItemText();
	}
	else if (info->cloth_hand == 900) {
		info->peffect = 8421382;
		craftItemText();
	}
	else if (info->cloth_hand == 910) {
		info->peffect = 8421383;
		craftItemText();
	}
	else if (info->cloth_hand == 930) {
		info->peffect = 8421384;
		craftItemText();
	}
	else if (info->cloth_hand == 1016) {
		info->peffect = 8421385;
		craftItemText();
	}
	else if (info->cloth_hand == 1378) {
		info->peffect = 8421387;
		craftItemText();
	}
	else if (info->cloth_hand == 1484) {
		info->peffect = 8421389;
		craftItemText();
	}
	else if (info->cloth_hand == 1512) {
		info->peffect = 8421390;
		craftItemText();
	}
	else if (info->cloth_hand == 1542) {
		info->peffect = 8421391;
		craftItemText();
	}
	else if (info->cloth_hand == 1576) {
		info->peffect = 8421392;
		craftItemText();
	}
	else if (info->cloth_hand == 1676) {
		info->peffect = 8421393;
		craftItemText();
	}
	else if (info->cloth_hand == 1710) {
		info->peffect = 8421394;
		craftItemText();
	}
	else if (info->cloth_hand == 1748) {
		info->peffect = 8421395;
		craftItemText();
	}
	else if (info->cloth_hand == 1782) {
		info->peffect = 8421397;
		craftItemText();
	}
	else if (info->cloth_hand == 1804) {
		info->peffect = 8421398;
		craftItemText();
	}
	else if (info->cloth_hand == 1868) {
		info->peffect = 8421399;
		craftItemText();
	}
	else if (info->cloth_hand == 1874) {
		info->peffect = 8421400;
		craftItemText();
	}
	else if (info->cloth_hand == 1946) {
		info->peffect = 8421401;
		craftItemText();
	}
	else if (info->cloth_hand == 1948) {
		info->peffect = 8421402;
		craftItemText();
	}
	else if (info->cloth_hand == 1956) {
		info->peffect = 8421403;
		craftItemText();
	}
	else if (info->cloth_hand == 2908) {
		info->peffect = 8421405;
		craftItemText();
	}
	else if (info->cloth_hand == 2952) {
		info->peffect = 8421405;
		craftItemText();
	}
	else if (info->cloth_hand == 6312) {
		info->peffect = 8421405;
		craftItemText();
	}
	else if (info->cloth_hand == 1980) {
		info->peffect = 8421406;
		craftItemText();
	}
	else if (info->cloth_hand == 2066) {
		info->peffect = 8421407;
		craftItemText();
	}
	else if (info->cloth_hand == 2212) {
		info->peffect = 8421408;
		craftItemText();
	}
	else if (info->cloth_hand == 2218) {
		info->peffect = 8421409;
		craftItemText();
	}
	else if (info->cloth_feet == 2220) {
		info->peffect = 8421410;
		craftItemText();
	}
	else if (info->cloth_hand == 2266) {
		info->peffect = 8421411;
		craftItemText();
	}
	else if (info->cloth_hand == 2386) {
		info->peffect = 8421412;
		craftItemText();
	}
	else if (info->cloth_hand == 2388) {
		info->peffect = 8421413;
		craftItemText();
	}
	else if (info->cloth_hand == 2450) {
		info->peffect = 8421414;
		craftItemText();
	}
	else if (info->cloth_hand == 2512) {
		info->peffect = 8421417;
		craftItemText();
	}
	else if (info->cloth_hand == 2572) {
		info->peffect = 8421418;
		craftItemText();
	}
	else if (info->cloth_hand == 2592) {
		info->peffect = 8421419;
		craftItemText();
	}
	else if (info->cloth_hand == 2720) {
		info->peffect = 8421420;
		craftItemText();
	}
	else if (info->cloth_hand == 2752) {
		info->peffect = 8421421;
		craftItemText();
	}
	else if (info->cloth_hand == 2754) {
		info->peffect = 8421422;
		craftItemText();
	}
	else if (info->cloth_hand == 2756) {
		info->peffect = 8421423;
		craftItemText();
	}
	else if (info->cloth_hand == 2802) {
		info->peffect = 8421425;
		craftItemText();
	}
	else if (info->cloth_hand == 2866) {
		info->peffect = 8421426;
		craftItemText();
	}
	else if (info->cloth_hand == 2876) {
		info->peffect = 8421427;
		craftItemText();
	}
	else if (info->cloth_hand == 2886) {
		info->peffect = 8421430;
		craftItemText();
	}
	else if (info->cloth_hand == 2890) {
		info->peffect = 8421431;
		craftItemText();
	}
	else if (info->cloth_hand == 3066) {
		info->peffect = 8421433;
		craftItemText();
	}
	else if (info->cloth_hand == 3124) {
		info->peffect = 8421434;
		craftItemText();
	}
	else if (info->cloth_hand == 3168) {
		info->peffect = 8421435;
		craftItemText();
	}
	else if (info->cloth_hand == 3214) {
		info->peffect = 8421436;
		craftItemText();
	}
	else if (info->cloth_hand == 3300) {
		info->peffect = 8421440;
		craftItemText();
	}
	else if (info->cloth_hand == 3418) {
		info->peffect = 8421441;
		craftItemText();
	}
	else if (info->cloth_hand == 3476) {
		info->peffect = 8421442;
		craftItemText();
	}
	else if (info->cloth_hand == 3686) {
		info->peffect = 8421444;
		craftItemText();
	}
	else if (info->cloth_hand == 3716) {
		info->peffect = 8421445;
		craftItemText();
	}
	else if (info->cloth_hand == 4290) {
		info->peffect = 8421447;
		craftItemText();
	}
	else if (info->cloth_hand == 4474) {
		info->peffect = 8421448;
		craftItemText();
	}
	else if (info->cloth_hand == 4464) {
		info->peffect = 8421449;
		craftItemText();
	}
	else if (info->cloth_hand == 1576) {
		info->peffect = 8421450;
		craftItemText();
	}
	else if (info->cloth_hand == 4778 || info->cloth_hand == 6026) {
		info->peffect = 8421452;
		craftItemText();
	}
	else if (info->cloth_hand == 4996) {
		info->peffect = 8421453;
		craftItemText();
	}
	else if (info->cloth_hand == 4840) {
		info->peffect = 8421454;
		craftItemText();
	}
	else if (info->cloth_hand == 5480) {
		info->peffect = 8421456;
		craftItemText();
	}
	else if (info->cloth_hand == 6110) {
		info->peffect = 8421457;
		craftItemText();
	}
	else if (info->cloth_hand == 6308) {
		info->peffect = 8421458;
		craftItemText();
	}
	else if (info->cloth_hand == 6310) {
		info->peffect = 8421459;
		craftItemText();
	}
	else if (info->cloth_hand == 6298) {
		info->peffect = 8421460;
		craftItemText();
	}
	else if (info->cloth_hand == 6756) {
		info->peffect = 8421461;
		craftItemText();
	}
	else if (info->cloth_hand == 7044) {
		info->peffect = 8421462;
		craftItemText();
	}
	else if (info->cloth_shirt == 6892) {
		info->peffect = 8421463;
		craftItemText();
	}
	else if (info->cloth_hand == 7088) {
		info->peffect = 8421465;
		craftItemText();
	}
	else if (info->cloth_hand == 7098) {
		info->peffect = 8421466;
		craftItemText();
	}
	else if (info->cloth_shirt == 7192) {
		info->peffect = 8421467;
		craftItemText();
	}
	else if (info->cloth_shirt == 7136) {
		info->peffect = 8421468;
		craftItemText();
	}
	else if (info->cloth_mask == 7216) {
		info->peffect = 8421470;
		craftItemText();
	}
	else if (info->cloth_back == 7196) {
		info->peffect = 8421471;
		craftItemText();
	}
	else if (info->cloth_back == 7392) {
		info->peffect = 8421472;
		craftItemText();
	}
	else if (info->cloth_hand == 7488) {
		info->peffect = 8421479;
		craftItemText();
	}
	else if (info->cloth_hand == 7586) {
		info->peffect = 8421480;
		craftItemText();
	}
	else if (info->cloth_hand == 7650) {
		info->peffect = 8421481;
		craftItemText();
	}
	else if (info->cloth_feet == 7950) {
		info->peffect = 8421489;
		craftItemText();
	}
	else if (info->cloth_hand == 8036) {
		info->peffect = 8421494;
		craftItemText();
	}
	else if (info->cloth_hand == 8910) {
		info->peffect = 8421505;
		craftItemText();
	}
	else if (info->cloth_hand == 8942) {
		info->peffect = 8421506;
		craftItemText();
	}
	else if (info->cloth_hand == 8948) {
		info->peffect = 8421507;
		craftItemText();
	}
	else if (info->cloth_hand == 8946) {
		info->peffect = 8421509;
		craftItemText();
	}
	else if (info->cloth_back == 9006) {
		info->peffect = 8421511;
		craftItemText();
	}
	else if (info->cloth_hand == 9116 || info->cloth_hand == 9118 || info->cloth_hand == 9120 || info->cloth_hand == 9122) {
		info->peffect = 8421376 + 111;
		craftItemText();
	}
	else {
		info->peffect = 8421376;
		craftItemText();
	}
}

void sendPData(ENetPeer* peer, PlayerMoving* data)
{
	ENetPeer* currentPeer;

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (peer != currentPeer)
		{
			if (isHere(peer, currentPeer))
			{
				data->netID = ((PlayerInfo*)(peer->data))->netID;

				SendPacketRaw(4, packPlayerMoving(data), 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
			}
		}
	}
}

int getPlayersCountInWorld(string name)
{
	int count = 0;
	ENetPeer* currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (((PlayerInfo*)(currentPeer->data))->currentWorld == name)
			count++;
	}
	return count;
}

void sendPuncheffect(ENetPeer* peer) {
	PlayerInfo* info = ((PlayerInfo*)(peer->data));
	int netID = info->netID;
	int state = getState(info);
	int pro = getState(info);
	int statey = 0;
	if (info->cloth_hand == 6028) statey = 1024;
	if (info->cloth_hand == 6262) statey = 8192;
	if (info->haveGrowId == false) statey = 50000;
	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		if (isHere(peer, currentPeer)) {
			PlayerMoving data;
			data.packetType = 0x14;
			data.characterState = statey;
			data.x = 1000;
			data.y = 100;
			data.punchX = 0;
			data.punchY = 0;
			data.XSpeed = 300;
			data.YSpeed = 600;
			data.netID = netID;
			data.plantingTree = state;
			BYTE* raw = packPlayerMoving(&data);
			int var = info->peffect;
			memcpy(raw + 1, &var, 3);
			SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
		}
	}
}
void sendPuncheffectpeer(ENetPeer* peer, int punch) {
	PlayerInfo* info = ((PlayerInfo*)(peer->data));
	int netID = info->netID;
	int state = getState(info);
	PlayerMoving data;
	float water = 125.0f;
	data.packetType = 0x14;
	data.characterState = ((PlayerInfo*)(peer->data))->characterState; // animation
	data.x = 1000;
	if (((PlayerInfo*)(peer->data))->cloth_hand == 366) {
		data.y = -400;
	}
	else {
		data.y = 400;
	}
	data.punchX = -1;
	data.punchY = -1;
	data.XSpeed = 300;
	if (((PlayerInfo*)(peer->data))->cloth_back == 9472) {
		data.YSpeed = 600;
	}
	else {
		data.YSpeed = 1150;
	}
	data.netID = netID;
	data.plantingTree = state;
	BYTE* raw = packPlayerMoving(&data);
	int var = punch;
	memcpy(raw + 1, &var, 3);
	memcpy(raw + 16, &water, 4);
	SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
}

void sendNotification(ENetPeer* peer, string song, string flag, string message) {
	GamePacket p = packetEnd(appendInt(appendString(appendString(appendString(appendString(createPacket(), "OnAddNotification"), song), message), flag), 0));
	ENetPacket* packet = enet_packet_create(p.data,
		p.len,
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
	delete p.data;
}

void sendNothingHappened(ENetPeer* peer, int x, int y) {
	PlayerMoving data;
	data.netID = ((PlayerInfo*)(peer->data))->netID;
	data.packetType = 0x8;
	data.plantingTree = 0;
	data.netID = -1;
	data.x = x;
	data.y = y;
	data.punchX = x;
	data.punchY = y;
	SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
}

string packPlayerMoving2(PlayerMoving* dataStruct)
{
	string data;
	data.resize(56);
	STRINT(data, 0) = dataStruct->packetType;
	STRINT(data, 4) = dataStruct->netID;
	STRINT(data, 12) = dataStruct->characterState;
	STRINT(data, 20) = dataStruct->plantingTree;
	STRINT(data, 24) = *(int*)&dataStruct->x;
	STRINT(data, 28) = *(int*)&dataStruct->y;
	STRINT(data, 32) = *(int*)&dataStruct->XSpeed;
	STRINT(data, 36) = *(int*)&dataStruct->YSpeed;
	STRINT(data, 44) = dataStruct->punchX;
	STRINT(data, 48) = dataStruct->punchY;
	return data;
}

void sendTileData(ENetPeer* peer, int x, int y, int visual, uint16_t fgblock, uint16_t bgblock, string tiledata) {
	PlayerMoving pmov;
	pmov.packetType = 5;
	pmov.characterState = 0;
	pmov.x = 0;
	pmov.y = 0;
	pmov.XSpeed = 0;
	pmov.YSpeed = 0;
	pmov.plantingTree = 0;
	pmov.punchX = x;
	pmov.punchY = y;
	pmov.netID = 0;

	string packetstr;
	packetstr.resize(4);
	packetstr[0] = 4;
	packetstr += packPlayerMoving2(&pmov);
	packetstr[16] = 8;
	packetstr.resize(packetstr.size() + 4);
	STRINT(packetstr, 52 + 4) = tiledata.size() + 4;
	STR16(packetstr, 56 + 4) = fgblock;
	STR16(packetstr, 58 + 4) = bgblock;
	packetstr += tiledata;

	ENetPacket* packet = enet_packet_create(&packetstr[0],
		packetstr.length(),
		ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, 0, packet);
}
string lockTileDatas(int visual, uint32_t owner, uint32_t adminLength, uint32_t* admins, bool isPublic = false, uint8_t bpm = 0) {
	string data;
	data.resize(4 + 2 + 4 + 4 + adminLength * 4 + 8);
	if (bpm) data.resize(data.length() + 4);
	data[2] = 0x01;
	if (isPublic) data[2] |= 0x80;
	data[4] = 3;
	data[5] = visual; // or 0x02
	STRINT(data, 6) = owner;
	//data[14] = 1;
	STRINT(data, 10) = adminLength;
	for (uint32_t i = 0; i < adminLength; i++) {
		STRINT(data, 14 + i * 4) = admins[i];
	}

	if (bpm) {
		STRINT(data, 10)++;
		STRINT(data, 14 + adminLength * 4) = -bpm;
	}
	return data;
}

uint8_t* lockTileData(uint32_t owner, uint32_t adminLength, uint32_t* admins) {
	uint8_t* data = new uint8_t[4 + 2 + 4 + 4 + adminLength * 4 + 8];
	memset(data, 0, 4 + 2 + 4 + 4 + adminLength * 4 + 8);
	data[2] = 0x1;
	data[4] = 3;
	*(uint32_t*)(data + 6) = owner;

	*(uint32_t*)(data + 10) = adminLength;
	for (uint32_t i = 0; i < adminLength; i++) {
		*(uint32_t*)(data + 14 + i * 4) = admins[i];
	}
	return data;
}

void resetacc(ENetPeer* peer) {
	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		string name2 = PlayerDB::getProperName(((PlayerInfo*)(currentPeer->data))->rawName);
		remove(("players/" + name2 + ".json").c_str());
		enet_peer_disconnect_later(currentPeer, 0);
	}
}
void sendPlayerLeave(ENetPeer* peer, PlayerInfo* player)
{
	ENetPeer* currentPeer;
	gamepacket_t p;
	p.Insert("OnRemove");
	p.Insert("netID|" + std::to_string(player->netID) + "\n");
	gamepacket_t p2;
	p2.Insert("OnConsoleMessage");
	p2.Insert("`5<`w" + player->displayName + "`` `5left, `w" + std::to_string(getPlayersCountInWorld(player->currentWorld) - 1) + "`` `5others here>``");
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			{
				p.CreatePacket(peer);

				{
					p.CreatePacket(currentPeer);
				}

			}
			{
				p2.CreatePacket(currentPeer);
			}
		}
	}
}

inline void sendWrongCmdLog(ENetPeer* peer)
{
	packet::consolemessage(peer, "`4Unknown command. `oEnter `$/help `ofor a list of valid commands.");
}

void sendChatMessage(ENetPeer* peer, int netID, string message)
{
	if (message.length() == 0) return;
	for (char c : message)
		if (c < 0x18 || std::all_of(message.begin(), message.end(), isspace)) {
			return;
		}
	string name = "";
	ENetPeer* currentPeer;
	for (currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		if (((PlayerInfo*)(currentPeer->data))->netID == netID)
			name = ((PlayerInfo*)(currentPeer->data))->displayName;
	}
	GamePacket p;
	GamePacket p2;
	if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> `5" + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), "`5" + message), 0));
	}
	else if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> `c" + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), "`c" + message), 0));
	}
	else if (((PlayerInfo*)(peer->data))->adminLevel == 777) {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> `2" + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), "`2" + message), 0));
	}
	else if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> `^" + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), "`^" + message), 0));
	}
	else if (((PlayerInfo*)(peer->data))->adminLevel == 444) {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> `1" + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), "`1" + message), 0));
	}
	else if (((PlayerInfo*)(peer->data))->adminLevel == 111) {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> `9" + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), "`9" + message), 0));
	}
	else {
		p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "CP:_PL:0_OID:_CT:[W]_ `o<`w" + name + "`o> " + message));
		p2 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), netID), message), 0));
	}
	for (currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		if (isHere(peer, currentPeer)) {
			ENetPacket* packet = enet_packet_create(p.data,
				p.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(currentPeer, 0, packet);
			ENetPacket* packet2 = enet_packet_create(p2.data,
				p2.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(currentPeer, 0, packet2);
		}
	}
	delete p.data;
	delete p2.data;
}

void sendDrop(ENetPeer* peer, int netID, int x, int y, int item, int count, BYTE specialEffect, bool onlyForPeer)
{
	if (item >= 7068) return;
	if (item < 0) return;
	if (onlyForPeer) {
		PlayerMoving data;
		data.packetType = 14;
		data.x = x;
		data.y = y;
		data.netID = netID;
		data.plantingTree = item;
		float val = count; // item count
		BYTE val2 = specialEffect;

		BYTE* raw = packPlayerMoving(&data);
		memcpy(raw + 16, &val, 4);
		memcpy(raw + 1, &val2, 1);

		SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
	else {
		DroppedItem dropItem;
		dropItem.x = x;
		dropItem.y = y;
		dropItem.count = count;
		dropItem.id = item;
		dropItem.uid = worldDB.get2(((PlayerInfo*)(peer->data))->currentWorld).ptr->currentItemUID++;
		worldDB.get2(((PlayerInfo*)(peer->data))->currentWorld).ptr->droppedItems.push_back(dropItem);
		ENetPeer* currentPeer;
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer)) {

				ItemSharedUID m_uid;
				m_uid.actual_uid = dropItem.uid;
				m_uid.shared_uid = (((PlayerInfo*)(currentPeer->data)))->last_uid++;
				(((PlayerInfo*)(currentPeer->data)))->item_uids.push_back(m_uid);
				PlayerMoving data;
				data.packetType = 14;
				data.x = x;
				data.y = y;
				data.netID = netID;
				data.plantingTree = item;
				float val = count; // item count
				BYTE val2 = specialEffect;

				BYTE* raw = packPlayerMoving(&data);
				memcpy(raw + 16, &val, 4);
				memcpy(raw + 1, &val2, 1);

				SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
			}
		}
	}
}

void addItemToInventory(ENetPeer* peer, int id) {
	PlayerInventory inventory = ((PlayerInfo*)(peer->data))->inventory;
	for (int i = 0; i < inventory.items.size(); i++) {
		if (inventory.items[i].itemID == id && inventory.items[i].itemCount < 200) {
			inventory.items[i].itemCount++;
			return;
		}
	}
	if (inventory.items.size() >= inventory.inventorySize)
		return;
	InventoryItem item;
	item.itemCount = 1;
	item.itemID = id;
	inventory.items.push_back(item);
}

int getSharedUID(ENetPeer* peer, int itemNetID) {
	auto v = ((PlayerInfo*)(peer->data))->item_uids;
	for (auto t = v.begin(); t != v.end(); ++t) {
		if (t->actual_uid == itemNetID) {
			return t->shared_uid;
		}
	}
	return 0;
}

int checkForUIDMatch(ENetPeer* peer, int itemNetID) {
	auto v = ((PlayerInfo*)(peer->data))->item_uids;
	for (auto t = v.begin(); t != v.end(); ++t) {
		if (t->shared_uid == itemNetID) {
			return t->actual_uid;
		}
	}
	return 0;
}

void sendCollect(ENetPeer* peer, int netID, int itemNetID) {
	ENetPeer* currentPeer;
	PlayerMoving data;
	data.packetType = 14;
	data.netID = netID;
	data.plantingTree = itemNetID;
	data.characterState = 0;
	cout << "Request collect: " << std::to_string(itemNetID) << endl;
	WorldInfo* world = getPlyersWorld(peer);
	for (auto m_item = world->droppedItems.begin(); m_item != world->droppedItems.end(); ++m_item) {
		if ((checkForUIDMatch(peer, itemNetID)) == m_item->uid) {
			cout << "Success!" << endl;
			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer)) {
					data.plantingTree = getSharedUID(currentPeer, m_item->uid);
					BYTE* raw = packPlayerMoving(&data);
					SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
				}
			}
			world->droppedItems.erase(m_item);
			m_item--;
			return;
		}
	}
}

void sendStuffweather(ENetPeer* peer, int x, int y, uint16_t itemid, uint16_t gravity, bool spin, bool invert) {
	PlayerMoving pmov;
	pmov.packetType = 5;
	pmov.characterState = 8;
	pmov.punchX = x;
	pmov.punchY = y;
	pmov.netID = -1;
	uint8_t* pmovpacked = packPlayerMoving(&pmov);
	*(uint32_t*)(pmovpacked + 52) = 10 + 8;
	uint8_t* packet = new uint8_t[4 + 56 + 10 + 8];
	memset(packet, 0, 4 + 56 + 10 + 8);
	packet[0] = 4;
	memcpy(packet + 4, pmovpacked, 56);
	*(uint16_t*)(packet + 4 + 56) = 3832; // WEATHER MACHINE - STUFF
	*(uint16_t*)(packet + 4 + 56 + 6) = 1;
	*(uint8_t*)(packet + 4 + 56 + 8) = 0x31;
	*(uint32_t*)(packet + 4 + 56 + 9) = itemid;
	*(uint32_t*)(packet + 4 + 56 + 13) = gravity;
	*(uint8_t*)(packet + 4 + 56 + 17) = spin | (invert << 1);
	ENetPacket* epacket = enet_packet_create(packet, 4 + 56 + 8 + 10, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, epacket);
	delete pmovpacked;
	delete[] packet;
}

void sendWorld(ENetPeer* peer, WorldInfo* worldInfo)
{
	int zero = 0;
	((PlayerInfo*)(peer->data))->droppeditemcount = 0;
#ifdef TOTAL_LOG
	cout << "[!] Entering a world..." << endl;
#endif
	((PlayerInfo*)(peer->data))->joinClothesUpdated = false;
	string asdf = "0400000004A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000070000000000"; // 0400000004A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000080000000000000000000000000000000000000000000000000000000000000048133A0500000000BEBB0000070000000000
	string worldName = worldInfo->name;
	int xSize = worldInfo->width;
	int ySize = worldInfo->height;
	int square = xSize * ySize;
	__int16 nameLen = (__int16)worldName.length();
	int payloadLen = asdf.length() / 2;
	int dataLen = payloadLen + 2 + nameLen + 12 + (square * 8) + 4 + 100;
	int offsetData = dataLen - 100;
	int allocMem = payloadLen + 2 + nameLen + 12 + (square * 8) + 4 + 16000 + 100 + (worldInfo->droppedCount * 20);
	BYTE* data = new BYTE[allocMem];
	memset(data, 0, allocMem);
	for (int i = 0; i < asdf.length(); i += 2)
	{
		char x = ch2n(asdf[i]);
		x = x << 4;
		x += ch2n(asdf[i + 1]);
		memcpy(data + (i / 2), &x, 1);
	}

	__int16 item = 0;
	int smth = 0;
	for (int i = 0; i < square * 8; i += 4) memcpy(data + payloadLen + i + 14 + nameLen, &zero, 4);
	for (int i = 0; i < square * 8; i += 8) memcpy(data + payloadLen + i + 14 + nameLen, &item, 2);
	memcpy(data + payloadLen, &nameLen, 2);
	memcpy(data + payloadLen + 2, worldName.c_str(), nameLen);
	memcpy(data + payloadLen + 2 + nameLen, &xSize, 4);
	memcpy(data + payloadLen + 6 + nameLen, &ySize, 4);
	memcpy(data + payloadLen + 10 + nameLen, &square, 4);
	BYTE* blockPtr = data + payloadLen + 14 + nameLen;

	int sizeofblockstruct = 8;


	for (int i = 0; i < square; i++) {

		int tile = worldInfo->items[i].foreground;
		sizeofblockstruct = 8;


		//if (world->items[x + (y*world->width)].foreground == 242 or world->items[x + (y*world->width)].foreground == 2408 or world->items[x + (y*world->width)].foreground == 5980 or world->items[x + (y*world->width)].foreground == 2950 or world->items[x + (y*world->width)].foreground == 5814 or world->items[x + (y*world->width)].foreground == 4428 or world->items[x + (y*world->width)].foreground == 1796 or world->items[x + (y*world->width)].foreground == 4802 or world->items[x + (y*world->width)].foreground == 4994 or world->items[x + (y*world->width)].foreground == 5260 or world->items[x + (y*world->width)].foreground == 7188)
		if (tile == 6) {
			int type = 0x00010000;
			memcpy(blockPtr, &tile, 2);
			memcpy(blockPtr + 4, &type, 4);
			BYTE btype = 1;
			memcpy(blockPtr + 8, &btype, 1);

			string doorText = "EXIT";
			const char* doorTextChars = doorText.c_str();
			short length = (short)doorText.size();
			memcpy(blockPtr + 9, &length, 2);
			memcpy(blockPtr + 11, doorTextChars, length);
			sizeofblockstruct += 4 + length;
			dataLen += 4 + length; // it's already 8.

		}
		else if (getItemDef(tile).blockType == BlockTypes::SIGN || tile == 1420 || tile == 6124) {
			int type = 0x00010000;
			memcpy(blockPtr, &worldInfo->items[i].foreground, 2);
			memcpy(blockPtr + 4, &type, 4);
			BYTE btype = 2;
			memcpy(blockPtr + 8, &btype, 1);
			string signText = worldInfo->items[i].text;
			const char* signTextChars = signText.c_str();
			short length = (short)signText.size();
			memcpy(blockPtr + 9, &length, 2);
			memcpy(blockPtr + 11, signTextChars, length);
			int minus1 = -1;
			memcpy(blockPtr + 11 + length, &minus1, 4);
			sizeofblockstruct += 3 + length + 4;
			dataLen += 3 + length + 4; // it's already 8.
		}
		else if (tile == 3832) {
			int type = 0x00010000;
			memcpy(blockPtr, &worldInfo->items[i].foreground, 2);
			memcpy(blockPtr + 4, &type, 4);
			BYTE btype = 0x31;
			memcpy(blockPtr + 8, &btype, 1);


			short flags = 0;
			int item = worldInfo->items[i].displayBlock;
			int gravity = worldInfo->items[i].gravity;
			flags = 3;

			memcpy(blockPtr + 9, &item, 4);
			memcpy(blockPtr + 13, &gravity, 4);
			memcpy(blockPtr + 17, &flags, 2);
			sizeofblockstruct += 10;
			dataLen += 10;
		}
		else if ((worldInfo->items[i].foreground == 0) || (worldInfo->items[i].foreground == 2) || (worldInfo->items[i].foreground == 8) || (worldInfo->items[i].foreground == 100) || (worldInfo->items[i].foreground == 4))
		{

			memcpy(blockPtr, &worldInfo->items[i].foreground, 2);
			int type = 0x00000000;

			// type 1 = locked
			if (worldInfo->items[i].activated)
				type |= 0x00200000;
			if (worldInfo->items[i].water)
				type |= 0x04000000;
			if (worldInfo->items[i].glue)
				type |= 0x08000000;
			if (worldInfo->items[i].fire)
				type |= 0x10000000;
			if (worldInfo->items[i].red)
				type |= 0x20000000;
			if (worldInfo->items[i].green)
				type |= 0x40000000;
			if (worldInfo->items[i].blue)
				type |= 0x80000000;

			// int type = 0x04000000; = water
			// int type = 0x08000000 = glue
			// int type = 0x10000000; = fire
			// int type = 0x20000000; = red color
			// int type = 0x40000000; = green color
			// int type = 0x80000000; = blue color


			memcpy(blockPtr + 4, &type, 4);
			/*if (worldInfo->items[i].foreground % 2)
			{
				blockPtr += 6;
			}*/
		}
		else
		{
			memcpy(blockPtr, &zero, 2);
		}
		memcpy(blockPtr + 2, &worldInfo->items[i].background, 2);
		blockPtr += sizeofblockstruct;


	}

	/*int increase = 20;
//TODO

	int inc = 20;
	memcpy(blockPtr, &worldInfo->droppedCount, 4);
	memcpy(blockPtr + 4, &worldInfo->droppedCount, 4);

	for (int i = 0; i < worldInfo->droppedCount; i++) {

		memcpy(blockPtr + inc - 12, &worldInfo->droppedItems.at(i).id, 2);
		memcpy(blockPtr + inc - 10, &worldInfo->droppedItems.at(i).x, 4);
		memcpy(blockPtr + inc - 6, &worldInfo->droppedItems.at(i).y, 4);
		memcpy(blockPtr + inc - 2, &worldInfo->droppedItems.at(i).count, 2);
		memcpy(blockPtr + inc, &i, 4);
		inc += 16;

	}
	blockPtr += inc;
	dataLen += inc;*/

	//((PlayerInfo*)(peer->data))->droppeditemcount = worldInfo->droppedCount;
	offsetData = dataLen - 100;

	//              0       1       2       3       4       5       6       7       8       9      10     11      12      13      14
	string asdf2 = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	BYTE* data2 = new BYTE[101];
	memcpy(data2 + 0, &zero, 4);
	for (int i = 0; i < asdf2.length(); i += 2)
	{
		char x = ch2n(asdf2[i]);
		x = x << 4;
		x += ch2n(asdf2[i + 1]);
		memcpy(data2 + (i / 2), &x, 1);
	}
	int weather = worldInfo->weather;
	memcpy(data2 + 4, &weather, 4);

	memcpy(data + offsetData, data2, 100);


	//cout << dataLen << " <- dataLen allocMem -> " << allocMem << endl;
	memcpy(data + dataLen - 4, &smth, 4);
	ENetPacket* packet2 = enet_packet_create(data,
		dataLen,
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet2);
	//enet_host_flush(server);
	for (int i = 0; i < square; i++) {
		ItemDefinition pro;
		pro = getItemDef(worldInfo->items[i].foreground);
		if ((worldInfo->items[i].foreground == 0) || (getItemDef(worldInfo->items[i].foreground).blockType) == BlockTypes::SIGN || worldInfo->items[i].foreground == 1420 || worldInfo->items[i].foreground == 6214 || (worldInfo->items[i].foreground == 3832) || (worldInfo->items[i].foreground == 2946) || (worldInfo->items[i].foreground == 6) || (worldInfo->items[i].foreground == 4) || (worldInfo->items[i].foreground == 2) || (worldInfo->items[i].foreground == 8) || (worldInfo->items[i].foreground == 100))
			; // nothing
		else if (worldInfo->items[i].foreground == 242 || worldInfo->items[i].foreground == 2408 || worldInfo->items[i].foreground == 1796 || worldInfo->items[i].foreground == 4428 || worldInfo->items[i].foreground == 7188)
		{
			ENetPeer* currentPeer;

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer)) {
					int x = i % xSize, y = i / xSize;
					sendTileData(currentPeer, x, y, 0x10, worldInfo->items[x + (y * worldInfo->width)].foreground, worldInfo->items[x + (y * worldInfo->width)].background, lockTileDatas(0x20, worldInfo->ownerID, 0, 0, false, 100));
				}
			}
		}
		else
		{
			PlayerMoving data;
			//data.packetType = 0x14;
			data.packetType = 0x3;

			//data.characterState = 0x924; // animation
			data.characterState = 0x0; // animation
			data.x = i % worldInfo->width;
			data.y = i / worldInfo->height;
			data.punchX = i % worldInfo->width;
			data.punchY = i / worldInfo->width;
			data.XSpeed = 0;
			data.YSpeed = 0;
			data.netID = -1;
			data.plantingTree = worldInfo->items[i].foreground;
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
			int x = i % xSize, y = i / xSize;
			UpdateBlockState(peer, x, y, true, worldInfo);
		}
	}
	int idx = 0;
	for (int i = 0; i < worldInfo->droppedItemUid; i++)
	{
		bool found = false;
		for (int j = 0; j < worldInfo->droppedItems.size(); j++)
		{
			if (worldInfo->droppedItems.at(j).uid == i)
			{
				SendDropSingle(peer, -1, worldInfo->droppedItems.at(j).x, worldInfo->droppedItems.at(j).y, worldInfo->droppedItems.at(j).id, worldInfo->droppedItems.at(j).count, 0);
				found = true;
				break;
			}
		}
		// temporary fix
		if (!found) SendDropSingle(peer, -1, -1000, -1000, 0, 1, 0);
	}
	((PlayerInfo*)(peer->data))->currentWorld = worldInfo->name;
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int squaresign = i + (j * 100);

			bool displaysss = std::experimental::filesystem::exists("display/" + worldInfo->name + "X" + std::to_string(squaresign) + ".txt");

			if (displaysss) {
				if (worldInfo->items[squaresign].foreground == 2946)
				{

					int x = squaresign % worldInfo->width;
					int y = squaresign / worldInfo->width;
					//cout << "[!] foundzzzzzzzzzzzzzz!";
					WorldInfo* world = getPlyersWorld(peer);
					ENetPeer* currentPeer;
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer)) {
							BlockVisual data;
							data.packetType = 0x5;
							data.characterState = 8;
							data.punchX = x;
							data.punchY = y;
							data.charStat = 13; // 13y
							data.blockid = 2946; // 2946 3794 = display shelf
							//data.netID = ((PlayerInfo*)(peer->data))->netID;
							data.backgroundid = 6864;
							data.visual = 0x00010000; //0x00210000

							std::ifstream ifs("display/" + worldInfo->name + "X" + std::to_string(squaresign) + ".txt");
							std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
							int id = atoi(content.c_str());

							world->items[x + (y * world->width)].displayblock = id;

							int n = id;
							string hex = "";
							{
								std::stringstream ss;
								ss << std::hex << n; // int decimal_value
								std::string res(ss.str());

								hex = res + "17";
							}

							if (hex == "2017") {
								continue;
							}


							int xx;
							std::stringstream ss;
							ss << std::hex << hex;
							ss >> xx;
							data.displayblock = xx;


							SendPacketRaw(192, packBlockVisual(&data), 69, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);

						}
					}
				}
			}
		}
	}

	int otherpeople = 0;
	int count = 0;
	ENetPeer* currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		count++;
		if (isHere(peer, currentPeer))
			otherpeople++;
	}
	string gstatement, gleader;
	((PlayerInfo*)(peer->data))->guildLeader = gleader;
	if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
		((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName + "`w(`eGL`w)";
	}
	int otherpeoples = otherpeople - 1;
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 0) {
				((PlayerInfo*)(peer->data))->displayName = "`2" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
			else if (((PlayerInfo*)(peer->data))->legend == true) {
				((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName + " of Legend";
			}
			else if (((PlayerInfo*)(peer->data))->level >= 125) {
				((PlayerInfo*)(peer->data))->displayName >= "`4Dr. " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 0) {
				((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName;
			}
			else if (((PlayerInfo*)(peer->data))->legend == true) {
				((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName + " of Legend";
			}
			else if (((PlayerInfo*)(peer->data))->level >= 125) {
				((PlayerInfo*)(peer->data))->displayName >= "`4Dr. " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 0) {
				((PlayerInfo*)(peer->data))->displayName = "`2" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
			else if (((PlayerInfo*)(peer->data))->legend == true) {
				((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName + " of Legend";
			}
			else if (((PlayerInfo*)(peer->data))->level >= 125) {
				((PlayerInfo*)(peer->data))->displayName >= "`4Dr. " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 0) {
				((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName;
			}
			else if (((PlayerInfo*)(peer->data))->legend == true) {
				((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName + " of Legend";
			}
			else if (((PlayerInfo*)(peer->data))->level >= 125) {
				((PlayerInfo*)(peer->data))->displayName >= "`4Dr. " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 111) {
				((PlayerInfo*)(peer->data))->displayName = "`w[`2rowPass`w] " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 111) {
				((PlayerInfo*)(peer->data))->displayName = "`w[`2GrowPass`w] " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 444) {
				((PlayerInfo*)(peer->data))->displayName = "`w[`2GrowPass`w] " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 444) {
				((PlayerInfo*)(peer->data))->displayName = "`w[`2GrowPass`w] " + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`#@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`#@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`#@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`#@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 666) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 777) {
				((PlayerInfo*)(peer->data))->displayName = "`c@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 777) {
				((PlayerInfo*)(peer->data))->displayName = "`c@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 777) {
				((PlayerInfo*)(peer->data))->displayName = "`c@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 777) {
				((PlayerInfo*)(peer->data))->displayName = "`cc@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
				((PlayerInfo*)(peer->data))->displayName = "`4@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	if (((PlayerInfo*)(peer->data))->haveGrowId) {
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
				((PlayerInfo*)(peer->data))->displayName = "`6@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
				((PlayerInfo*)(peer->data))->displayName = "`6@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}
	else
	{
		if (((PlayerInfo*)(peer->data))->rawName == worldInfo->owner)
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
				((PlayerInfo*)(peer->data))->displayName = "`6@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}

		else
			string gstatement, gleader;
		((PlayerInfo*)(peer->data))->guildLeader = gleader;
		if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
			((PlayerInfo*)(peer->data))->displayName = ((PlayerInfo*)(peer->data))->tankIDName + "`w(`eGL`w)";
		}
		else
		{
			if (((PlayerInfo*)(peer->data))->adminLevel == 1337) {
				((PlayerInfo*)(peer->data))->displayName = "`6@" + ((PlayerInfo*)(peer->data))->tankIDName;
			}
		}
	}

	string act = ((PlayerInfo*)(peer->data))->currentWorld;
	sendState(peer);
	if (worldInfo->weather == 29) {
		WorldInfo* world = getPlyersWorld(peer);
		int x = 0;
		int y = 0;
		int stuffGra = world->stuff_gravity;
		bool stuff_spin = world->stuff_spin;
		int stuffID = world->stuffID;
		bool stuff_invert = world->stuff_invert;
		world->stuff_invert = stuff_invert;
		world->stuff_spin = stuff_spin;
		world->stuff_gravity = stuffGra;
		world->stuffID = stuffID;
		sendStuffweather(currentPeer, x, y, world->stuffID, stuffGra, stuff_spin, stuff_invert);
	}
	else {
		GamePacket p7 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), worldInfo->weather));
		ENetPacket* packet7 = enet_packet_create(p7.data,
			p7.len,
			ENET_PACKET_FLAG_RELIABLE);

		enet_peer_send(peer, 0, packet7);
		delete p7.data;
	}
	string nameworld = worldInfo->name;
	string ownerworld = worldInfo->owner;
	string accessname = "";
	for (std::vector<string>::const_iterator i = worldInfo->acclist.begin(); i != worldInfo->acclist.end(); ++i) {
		accessname = *i;
	}
	if (worldInfo->owner == ((PlayerInfo*)(peer->data))->rawName)
	{

		GamePacket p3 = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "`5 `5[`0" + nameworld + " `$World Locked `oby " + ownerworld + " ``(`2ACCESS GRANTED``)`5]"));
		ENetPacket* packet3 = enet_packet_create(p3.data,
			p3.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet3);
		delete p3.data;





	}
	else if (((PlayerInfo*)(peer->data))->rawName == accessname)
	{

		GamePacket p3 = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "`5 `5[`0" + nameworld + " `$World Locked `oby " + ownerworld + " ``(`2ACCESS GRANTED``)`5]"));
		ENetPacket* packet3 = enet_packet_create(p3.data,
			p3.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet3);
		delete p3.data;





	}
	else if (((PlayerInfo*)(peer->data))->adminLevel == 1337)
	{
		if (ownerworld != "") {
			GamePacket p3 = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "`5 `5[`0" + nameworld + " `$World Locked `oby " + ownerworld + " ``(`2ACCESS GRANTED``)`5]"));
			ENetPacket* packet3 = enet_packet_create(p3.data,
				p3.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet3);
			delete p3.data;

		}





	}

	else
	{

		if (ownerworld != "") {
			GamePacket p3 = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "`5 `5[`0" + nameworld + " `$World Locked `oby " + ownerworld + "`5]"));
			ENetPacket* packet3 = enet_packet_create(p3.data,
				p3.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet3);
			delete p3.data;
		}
	}
	delete data;
}
void sendAction(ENetPeer* peer, int netID, string action)
{
	ENetPeer* currentPeer;
	string name = "";
	gamepacket_t p(0, netID);
	p.Insert("OnAction");
	p.Insert(action);
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			p.CreatePacket(currentPeer);
		}
	}
}

void sendWorldOffers(ENetPeer* peer)
{
	if (!((PlayerInfo*)(peer->data))->isIn) return;
	vector<WorldInfo> worlds = worldDB.getRandomWorlds();
	string worldOffers = "default|";
	if (worlds.size() > 0) {
		worldOffers += worlds[0].name;
	}

	worldOffers += "\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\n";
	for (int i = 0; i < worlds.size(); i++) {
		worldOffers += "add_floater|" + worlds[i].name + "|" + std::to_string(getPlayersCountInWorld(worlds[i].name)) + "|0.55|3529161471\n";
	}
	packet::requestworldselectmenu(peer, worldOffers);
}


void addAdmin(string username, string password, int level)
{
	Admin admin;
	admin.username = username;
	admin.password = password;
	admin.level = level;
	admins.push_back(admin);
}

int getAdminLevel(string username, string password) {
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password) {
			return admin.level;
		}
	}
	return 0;
}

int adminlevel(string name) {
	std::ifstream ifff("players/" + PlayerDB::getProperName(name) + ".json");
	json j;
	ifff >> j;

	int adminlevel;
	adminlevel = j["adminLevel"];

	ifff.close();
	if (adminlevel == 0) {
		return 0;
	}
	else {
		return adminlevel;
	}

}

void showWrong(ENetPeer* peer, string listFull, string itemFind) {
	packet::dialog(peer, "add_label_with_icon|big|`wFind item: " + itemFind + "``|left|3802|\nadd_spacer|small|\n" + listFull + "add_textbox|Enter a word below to find the item|\nadd_text_input|item|Item Name||30|\nend_dialog|findid|Cancel|Find the item!|\n");
}
void PathFindingCore(ENetPeer* peer, int xs, int ys)
{

	try {
		int Square = xs + (ys * 100); // 100 = Width World lu
		string gayname = ((PlayerInfo*)(peer->data))->rawName;
		WorldInfo* world = getPlyersWorld(peer);
		if (world->items[Square].PosFind == Square)
		{
			packet::consolemessage(peer, "`4[CHEAT DETECTED] " + gayname + " STOP CHEATING!");
			enet_peer_disconnect_later(peer, 0);
		}

	}
	catch (const std::out_of_range& e) {
		std::cout << e.what() << std::endl;
	}
}

void LoadCaptCha() {
	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		Sleep(300);
		int userMaxRand = 50, userLowRand = 0;
		resultnbr2 = rand() % userMaxRand + userLowRand + 1;
		srand(time(0));
		resultnbr1 = rand() % userMaxRand + userLowRand + 1;
		hasil = resultnbr1 + resultnbr2;
		string captcha = "set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following numbers|left|\nadd_textbox|" + to_string(resultnbr1) + " + " + to_string(resultnbr2) + "|left|\nadd_text_input|captcha_answer|Answer:||32|\nadd_button|Submit_|Submit|";
		packet::dialog(currentPeer, captcha);
	}
}

void craftItemDescriptions() {
	int current = -1;
	std::ifstream infile("Descriptions.txt");
	for (std::string line; getline(infile, line);)
	{
		if (line.length() > 3 && line[0] != '/' && line[1] != '/')
		{
			vector<string> ex = explode("|", line);
			ItemDefinition def;
			if (atoi(ex[0].c_str()) + 1 < itemDefs.size())
			{
				itemDefs.at(atoi(ex[0].c_str())).description = ex[1];
				if (!(atoi(ex[0].c_str()) % 2))
					itemDefs.at(atoi(ex[0].c_str()) + 1).description = "This is a tree.";
			}
		}
	}
}

void sendGazette(ENetPeer* peer) {
	std::ifstream news("news.txt");
	std::stringstream buffer;
	buffer << news.rdbuf();
	std::string newsString(buffer.str());
	packet::dialog(peer, newsString);
}

bool SaveConvertedItem(int fItemid, int fQuantity, ENetPeer* peer)
{
	int invsizee = ((PlayerInfo*)(peer->data))->currentInventorySize;

	bool doesItemInInventryAlready = false;

	for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
	{
		if (((PlayerInfo*)(peer->data))->inventory.items[i].itemID == fItemid)
		{
			doesItemInInventryAlready = true;
			if (((PlayerInfo*)(peer->data))->inventory.items[i].itemCount >= 101) return false;
			break;
		}
	}

	std::ifstream iffff("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");

	json jj;

	if (iffff.fail()) {
		iffff.close();
		cout << "SaveConvertedItem funkcijoje (ifstream dalyje) error: itemid - " << fItemid << ", kiekis - " << fQuantity << endl;

	}
	if (iffff.is_open()) {


	}

	iffff >> jj; //load


	std::ofstream oo("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
	if (!oo.is_open()) {
		cout << GetLastError() << " SaveConvertedItem funkcijoje (ofstream dalyje) error: itemid - " << fItemid << ", kiekis - " << fQuantity << endl;
		_getch();
	}
	int howManyHasNow = 0;

	for (int i = 0; i < ((PlayerInfo*)(peer->data))->currentInventorySize; i++)
	{
		int itemidFromJson = jj["items"][i]["itemid"];
		int quantityFromJson = jj["items"][i]["quantity"];;
		if (doesItemInInventryAlready)
		{
			if (itemidFromJson == fItemid)
			{
				howManyHasNow = jj["items"][i]["quantity"];
				howManyHasNow += fQuantity;
				jj["items"][i]["quantity"] = howManyHasNow;

				for (int k = 0; k < ((PlayerInfo*)(peer->data))->inventory.items.size(); k++)
				{
					if (((PlayerInfo*)(peer->data))->inventory.items[k].itemID == fItemid)
					{
						((PlayerInfo*)(peer->data))->inventory.items[k].itemCount += (byte)fQuantity;
						break;
					}
				}
				sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);

				break;
			}
		}
		else if (itemidFromJson == 0 && quantityFromJson == 0)
		{
			jj["items"][i]["quantity"] = fQuantity;
			jj["items"][i]["itemid"] = fItemid;

			InventoryItem item;
			item.itemID = fItemid;
			item.itemCount = fQuantity;
			((PlayerInfo*)(peer->data))->inventory.items.push_back(item);

			sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);

			break;
		}
	}

	oo << jj << std::endl;
	return true;
}
void RemoveInventoryItem(int fItemid, int fQuantity, ENetPeer* peer)
{
	std::ifstream iffff("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");

	json jj;

	if (iffff.fail()) {
		iffff.close();

	}
	if (iffff.is_open()) {


	}

	iffff >> jj; //load


	std::ofstream oo("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
	if (!oo.is_open()) {
		cout << GetLastError() << " Gak bisa delete " << fItemid << " bruh" << fQuantity << endl;
		_getch();
	}

	//jj["items"][aposition]["aposition"] = aposition;


	for (int i = 0; i < ((PlayerInfo*)(peer->data))->currentInventorySize; i++)
	{
		int itemid = jj["items"][i]["itemid"];
		int quantity = jj["items"][i]["quantity"];
		if (itemid == fItemid)
		{
			if (quantity - fQuantity == 0)
			{
				jj["items"][i]["itemid"] = 0;
				jj["items"][i]["quantity"] = 0;
			}
			else
			{
				jj["items"][i]["itemid"] = itemid;
				jj["items"][i]["quantity"] = quantity - fQuantity;
			}

			break;
		}

	}
	oo << jj << std::endl;

	for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
	{
		if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemid)
		{
			if ((unsigned int)((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount > fQuantity && (unsigned int)((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount != fQuantity)
			{
				((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount -= fQuantity;
			}
			else
			{
				((PlayerInfo*)(peer->data))->inventory.items.erase(((PlayerInfo*)(peer->data))->inventory.items.begin() + i);
			}
			sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);
		}
	}


}
void SearchInventoryItem(ENetPeer* peer, int fItemid, int fQuantity, bool& iscontains)
{
	iscontains = false;
	for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
	{
		if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemid && ((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount >= fQuantity) {

			iscontains = true;
			break;
		}
	}
}
void SaveFindsItem(int fItemid, int fQuantity, ENetPeer* peer)
{

	std::ifstream iffff("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");

	json jj;

	if (iffff.fail()) {
		iffff.close();


	}
	if (iffff.is_open()) {


	}

	iffff >> jj; //load


	std::ofstream oo("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
	if (!oo.is_open()) {
		cout << GetLastError() << endl;
		_getch();
	}

	//jj["items"][aposition]["aposition"] = aposition;

	for (int i = 0; i < ((PlayerInfo*)(peer->data))->currentInventorySize; i++)
	{
		int itemid = jj["items"][i]["itemid"];
		int quantity = jj["items"][i]["quantity"];
		if (itemid == 0 && quantity == 0)
		{
			jj["items"][i]["itemid"] = fItemid;
			jj["items"][i]["quantity"] = fQuantity;
			break;
		}

	}
	oo << jj << std::endl;


	InventoryItem item;
	item.itemID = fItemid;
	item.itemCount = fQuantity;
	((PlayerInfo*)(peer->data))->inventory.items.push_back(item);

	sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);
}
void SaveShopsItemMoreTimes(int fItemid, int fQuantity, ENetPeer* peer, bool& success)
{
	size_t invsizee = ((PlayerInfo*)(peer->data))->currentInventorySize;
	bool invfull = false;
	bool alreadyhave = false;


	if (((PlayerInfo*)(peer->data))->inventory.items.size() == invsizee) {

		GamePacket ps = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "add_label_with_icon|big|`4Whoops!|left|1432|\nadd_spacer|small|\nadd_textbox|`oSorry! Your inventory is full! You can purchase an inventory upgrade in the shop.|\nadd_spacer|small|\nadd_button|close|`5Close|0|0|"));
		ENetPacket* packet = enet_packet_create(ps.data,
			ps.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete ps.data;


		alreadyhave = true;
	}

	bool isFullStock = false;
	bool isInInv = false;
	for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
	{

		if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemid && ((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount >= 200) {


			GamePacket ps = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "add_label_with_icon|big|`4Whoops!|left|1432|\nadd_spacer|small|\nadd_textbox|`oSorry! You already have full stock of this item!|\nadd_spacer|small|\nadd_button|close|`5Close|0|0|"));
			ENetPacket* packet = enet_packet_create(ps.data,
				ps.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
			delete ps.data;


			isFullStock = true;
		}

		if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemid && ((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount < 200)	isInInv = true;

	}

	if (isFullStock == true || alreadyhave == true)
	{
		success = false;
	}
	else
	{
		success = true;

		std::ifstream iffff("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");

		json jj;

		if (iffff.fail()) {
			iffff.close();


		}
		if (iffff.is_open()) {


		}

		iffff >> jj; //load


		std::ofstream oo("inventory/" + ((PlayerInfo*)(peer->data))->rawName + ".json");
		if (!oo.is_open()) {
			cout << GetLastError() << endl;
			_getch();
		}

		//jj["items"][aposition]["aposition"] = aposition;

		if (isInInv == false)
		{

			for (int i = 0; i < ((PlayerInfo*)(peer->data))->currentInventorySize; i++)
			{
				int itemid = jj["items"][i]["itemid"];
				int quantity = jj["items"][i]["quantity"];

				if (itemid == 0 && quantity == 0)
				{
					jj["items"][i]["itemid"] = fItemid;
					jj["items"][i]["quantity"] = fQuantity;
					break;
				}

			}
			oo << jj << std::endl;


			InventoryItem item;
			item.itemID = fItemid;
			item.itemCount = fQuantity;
			((PlayerInfo*)(peer->data))->inventory.items.push_back(item);

			sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);
		}
		else
		{
			for (int i = 0; i < ((PlayerInfo*)(peer->data))->currentInventorySize; i++)
			{
				int itemid = jj["items"][i]["itemid"];
				int quantity = jj["items"][i]["quantity"];

				if (itemid == fItemid)
				{
					jj["items"][i]["quantity"] = quantity + fQuantity;
					break;
				}

			}
			oo << jj << std::endl;


			for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
			{
				if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemid)
				{
					((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount += fQuantity;
					sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);
				}
			}

		}
	}
}

string getRankText(string name) {
	int lvl = 0;
	lvl = adminlevel(name);
	if (lvl == 0) {
		return "`wPlayer";
	}
	if (lvl == 444) {
		return "`w[`1VIP`w]";
	}
	else if (lvl == 666) {
		return "`#Moderator";
	}
	else if (lvl == 777) {
		return "`4Administrator";
	}
	else if (lvl == 999) {
		return "`4CO-Creator";
	}
	else if (lvl == 1337) {
		return "`cServer-Creator";
	}
}
string getRankId(string name) {
	int lvl = 0;
	lvl = adminlevel(name);
	if (lvl == 0) {
		return "18";
	}
	if (lvl == 444) {
		return "274";
	}
	else if (lvl == 666) {
		return "278";
	}
	else if (lvl == 777) {
		return "276";
	}
	else if (lvl == 999) {
		return "1956";
	}
	else if (lvl == 1337) {
		return "2376";
	}
}
string getRankTexts(string name) {
	int lvl = 0;
	lvl = level(name);
	if (lvl <= 10) {
		return "`2Newbie";
	}
	if (lvl >= 11) {
		return "`1Advance";
	}
	if (lvl >= 50) {
		return "`cPro";
	}
	if (lvl >= 100) {
		return "`eMaster";
	}
	if (lvl >= 150) {
		return "`9Expert";
	}
	if (lvl >= 200) {
		return "`5A`4C`qE";
	}
}
string getRankIds(string name) {
	int lvl = 0;
	lvl = level(name);
	if (lvl <= 10) {
		return "3900";
	}
	if (lvl >= 11) {
		return "3192";
	}
	if (lvl >= 50) {
		return "7832";
	}
	if (lvl >= 100) {
		return "7586";
	}
	if (lvl >= 150) {
		return "6312";
	}
	if (lvl >= 200) {
		return "1956";
	}
}

bool canSB(string username, string password) {
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password && admin.level > 1) {
			using namespace std::chrono;
			if (admin.lastSB + 900000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() || admin.level == 999)
			{
				admins[i].lastSB = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}

bool canClear(string username, string password) {
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password) {
			return admin.level > 0;
		}
	}
	return false;
}

bool isSuperAdmin(string username, string password) {
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password && admin.level == 999) {
			return true;
		}
	}
	return false;
}

void sendGrowmojis(ENetPeer* peer) {
	std::ifstream emoji("growmoji.txt");
	std::stringstream buffer;
	buffer << emoji.rdbuf();
	std::string newsString(buffer.str());
	packet::dialog(peer, newsString);
}

void removeAcc(ENetPeer* peer, string name) {
	string resetacc;
	std::ifstream ifff("players/" + PlayerDB::getProperName(name) + ".json");
	json j;
	ifff >> j;
}

void PlayAudioWorld(ENetPeer* peer, string audioFile) {
	string text = "action|play_sfx\nfile|" + audioFile + "\ndelayMS|0\n";
	BYTE* data = new BYTE[5 + text.length()];
	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);
	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		if (isHere(peer, currentPeer)) {
			ENetPacket* packet2 = enet_packet_create(data,
				5 + text.length(),
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(currentPeer, 0, packet2);
		}
	}
	delete[] data;
}

void OnSetCurrentWeather(ENetPeer* peer, int weather) {
	GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), weather));
	ENetPacket* packet2 = enet_packet_create(p2.data,
		p2.len,
		ENET_PACKET_FLAG_RELIABLE);
	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		if (isHere(peer, currentPeer)) {
			enet_peer_send(currentPeer, 0, packet2);
		}
	}
	delete p2.data;
}

void sendRoulete(ENetPeer* peer, int x, int y)
{
	ENetPeer* currentPeer;
	int val = rand() % 37;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			gamepacket_t p(2500);
			p.Insert("OnTalkBubble");
			p.Insert(((PlayerInfo*)(peer->data))->netID);
			p.Insert("`w[" + ((PlayerInfo*)(peer->data))->displayName + " `wspun the wheel and got `4" + std::to_string(val) + "`w!]");
			p.Insert(0);
			p.CreatePacket(currentPeer);
		}
	}
}

void sendTileUpdate(int x, int y, int tile, int causedBy, ENetPeer* peer)
{
	if (tile > itemDefs.size()) {
		return;
	}
	bool isLock = false;
	PlayerMoving data;
	//data.packetType = 0x14;
	data.packetType = 0x3;

	//data.characterState = 0x924; // animation
	data.characterState = 0x0; // animation
	data.x = x;
	data.y = y;
	data.punchX = x;
	data.punchY = y;
	data.XSpeed = 0;
	data.YSpeed = 0;
	data.netID = causedBy;
	data.plantingTree = tile;

	WorldInfo* world = getPlyersWorld(peer);

	string gay = world->items[x + (y * world->width)].text;
	int gay2 = world->items[x + (y * world->width)].foreground;
	if (getItemDef(world->items[x + (y * world->width)].foreground).blockType == BlockTypes::SIGN || world->items[x + (y * world->width)].foreground == 1420 || world->items[x + (y * world->width)].foreground == 6214)
	{
		if (world->owner != "") {
			if (((PlayerInfo*)(peer->data))->rawName == world->owner) {
				if (tile == 32) {
					((PlayerInfo*)(peer->data))->wrenchx = x;
					((PlayerInfo*)(peer->data))->wrenchy = y;
					GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "set_default_color|`o\n\nadd_label_with_icon|big|`wEdit " + getItemDef(world->items[x + (y * world->width)].foreground).name + "``|left|" + to_string(gay2) + "|\n\nadd_textbox|`oWhat would you like to write on this sign?|\nadd_text_input|ch3||" + gay + "|100|\nembed_data|tilex|" + std::to_string(((PlayerInfo*)(peer->data))->wrenchx) + "\nembed_data|tiley|" + std::to_string(((PlayerInfo*)(peer->data))->wrenchy) + "\nend_dialog|sign_edit|Cancel|OK|"));
					ENetPacket* packet = enet_packet_create(p.data,
						p.len,
						ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
					delete p.data;
				}
			}
		}
	}
	if (getItemDef(world->items[x + (y * world->width)].foreground).blockType == BlockTypes::GATEWAY) {
		if (tile == 32) {
			if (((PlayerInfo*)(peer->data))->rawName == world->owner) {
				((PlayerInfo*)(peer->data))->wrenchx = x;
				((PlayerInfo*)(peer->data))->wrenchy = y;
				int id = getItemDef(world->items[x + (y * world->width)].foreground).id;
				if (world->items[x + (y * world->width)].isOpened == false) {
					packet::dialog(peer, "add_label_with_icon|big|Edit " + getItemDef(world->items[x + (y * world->width)].foreground).name + "|left|" + to_string(id) + "|\nadd_checkbox|opentopublic|`ois Open to Public?|0|\nend_dialog|entrance|Cancel|OK|");
				}
				else {
					packet::dialog(peer, "add_label_with_icon|big|Edit " + getItemDef(world->items[x + (y * world->width)].foreground).name + "|left|" + to_string(id) + "|\nadd_checkbox|opentopublic|`ois Open to Public?|1|\nend_dialog|entrance|Cancel|OK|");
				}
			}
		}
	}
	if (getItemDef(tile).blockType == BlockTypes::CONSUMABLE) {
		if (tile == 11398) {
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL) continue;
				if (isHere(peer, currentPeer)) {
					if (x == ((PlayerInfo*)(peer->data))->x / 32 && y == ((PlayerInfo*)(peer->data))->y / 32) {
						RemoveInventoryItem(11398, 1, peer);
						vector<int> alien_pod{ 10990, 11000, 11410, 11426, 10952, 10956, 10954, 10958, 10960, 10996, 11408, 11412, 11414, 10998, 11422, 10994 };
						int rand_item = alien_pod[rand() % alien_pod.size()];
						int count = 1;
						if (rand_item == 10990) count = 1;
						if (rand_item == 11000) count = 1;
						if (rand_item == 11410) count = 1;
						if (rand_item == 11426) count = 1;
						if (rand_item == 10952) count = 1;
						if (rand_item == 10956) count = 1;
						if (rand_item == 10954) count = 1;
						if (rand_item == 10958) count = 1;
						if (rand_item == 10960) count = 1;
						if (rand_item == 10958) count = 1;
						if (rand_item == 10996) count = 1;
						if (rand_item == 11408) count = 1;
						if (rand_item == 11412) count = 1;
						if (rand_item == 11414) count = 1;
						if (rand_item == 10998) count = 1;
						if (rand_item == 11422) count = 1;
						if (rand_item == 10994) count = 1;
						if (rand_item == 10960 || rand_item == 10956 || rand_item == 10958 || rand_item == 10954) {
							int target = 5;
							if (tile == 9286) target = 10;
							if ((rand() % 1000) <= target) {}
							else rand_item = 11422;
						}
						packet::SendTalkSelf(peer, "You received `2" + to_string(count) + " " + getItemDef(rand_item).name + "`` from the Alien Landing Pod.");
						packet::consolemessage(peer, "You received `2" + to_string(count) + " " + getItemDef(rand_item).name + "`` from the Alien Landing Pod.");
						bool success = true;
						SaveShopsItemMoreTimes(rand_item, count, peer, success);
						return;
					}
					else if (x == ((PlayerInfo*)(currentPeer->data))->x / 32 && y == ((PlayerInfo*)(currentPeer->data))->y / 32) {
						packet::SendTalkSelf(peer, "You can only use that on yourself.");
						return;
					}
					else {
						packet::SendTalkSelf(peer, "Must be used on a person.");
						return;
					}
				}
			}
			if (tile == 10456) {
				if (x == ((PlayerInfo*)(peer->data))->x / 32 && y == ((PlayerInfo*)(peer->data))->y / 32) {
					if (((PlayerInfo*)(peer->data))->cloth_back != 10456) {
						((PlayerInfo*)(peer->data))->cloth_back = 10456;
						((PlayerInfo*)(peer->data))->peffect = 8421559;
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->peffect);
						sendClothes(peer);
					}
					else {
						((PlayerInfo*)(peer->data))->cloth_back = 10426;
						((PlayerInfo*)(peer->data))->peffect = 8421559;
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->peffect);
						sendClothes(peer);
					}
				}
				else {
					packet::SendTalkSelf(peer, "Must be used on a person.");
				}
			}
		}
		return;
	}

	if (world == NULL) return;
	if (x<0 || y<0 || x>world->width - 1 || y>world->height - 1 || tile > itemDefs.size()) return; // needs - 1
	sendNothingHappened(peer, x, y);
	if (((PlayerInfo*)(peer->data))->adminLevel < 665)
	{
		if (world->items[x + (y * world->width)].foreground == 6 || world->items[x + (y * world->width)].foreground == 8 || world->items[x + (y * world->width)].foreground == 3760)
			return;
		if (tile == 6 || tile == 8 || tile == 3760 || tile == 6864)
			return;
	}
	if (world->name == "ADMIN" && !getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass))
	{
		if (world->items[x + (y * world->width)].foreground == 758)
			sendRoulete(peer, x, y);
		return;
	}
	if (world->name != "ADMIN") {
		if (world->owner != "") {

			string name = ((PlayerInfo*)(peer->data))->rawName;
			if (((PlayerInfo*)(peer->data))->rawName == world->owner || (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end() || ((PlayerInfo*)(peer->data))->adminLevel == 1337)) {
				if (((PlayerInfo*)(peer->data))->rawName == "") return;
				// WE ARE GOOD TO GO

				if (world->items[x + (y * world->width)].foreground == 242 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 1796 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 4428 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 2408 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 7188 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 5980 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 2950 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()) || world->items[x + (y * world->width)].foreground == 5638 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end()))
				{
					return;
				}

				if (tile == 32 && (find(world->acclist.begin(), world->acclist.end(), name) != world->acclist.end())) {
					return;
				}
				string offlinelist = "";
				string offname = "";
				int ischecked;
				int ischecked1;
				int ischecked2;
				for (std::vector<string>::const_iterator i = world->acclist.begin(); i != world->acclist.end(); ++i) {
					offname = *i;
					offlinelist += "\nadd_checkbox|isAccessed|" + offname + "|0|\n";

				}


				if (world->isPublic == true) {
					ischecked = 1;
				}
				else {
					ischecked = 0;
				}
				int noclap = 0;
				bool casin = world->isCasino;
				if (casin == true) {
					noclap = 1;
				}
				else {
					noclap = 0;
				}
				if (tile == 32) {
					if (world->items[x + (y * world->width)].foreground == 242 || world->items[x + (y * world->width)].foreground == 5814 || world->items[x + (y * world->width)].foreground == 2408 || world->items[x + (y * world->width)].foreground == 1796 || world->items[x + (y * world->width)].foreground == 4428 || world->items[x + (y * world->width)].foreground == 7188) {

						GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "set_default_color|`o\n\nadd_label_with_icon|big|`wEdit " + getItemDef(world->items[x + (y * world->width)].foreground).name + "``|left|" + to_string(world->items[x + (y * world->width)].foreground) + "|\nadd_textbox|`wAccess list:|left|\nadd_spacer|small|" + offlinelist + "add_spacer|small|\nadd_player_picker|netid|`wAdd|\nadd_spacer|small|\nadd_checkbox|isWorldPublic|Allow anyone to build|" + std::to_string(ischecked) + "|\nadd_checkbox|allowNoclip|Disable noclip|" + std::to_string(noclap) + "|\nend_dialog|wlmenu|Cancel|OK|"));
						ENetPacket* packet = enet_packet_create(p.data,
							p.len,
							ENET_PACKET_FLAG_RELIABLE);
						enet_peer_send(peer, 0, packet);
						delete p.data;
					}
				}
			}

			else if (find(world->acclist.begin(), world->acclist.end(), ((PlayerInfo*)(peer->data))->rawName) != world->acclist.end())
			{
				if (world->items[x + (y * world->width)].foreground == 242 || world->items[x + (y * world->width)].foreground == 5814 || world->items[x + (y * world->width)].foreground == 2408 || world->items[x + (y * world->width)].foreground == 1796 || world->items[x + (y * world->width)].foreground == 4428 || world->items[x + (y * world->width)].foreground == 7188 || world->items[x + (y * world->width)].foreground == 4802 || world->items[x + (y * world->width)].foreground == 10410)
				{


					string ownername = world->owner;
					GamePacket p2 = packetEnd(appendIntx(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), ((PlayerInfo*)(peer->data))->netID), "`0" + ownername + "'s `$World Lock`0. (`2Access Granted`w)"), 0), 1));


					ENetPacket* packet2 = enet_packet_create(p2.data,
						p2.len,
						ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet2);
					delete p2.data;
					string text = "action|play_sfx\nfile|audio/punch_locked.wav\ndelayMS|0\n";
					BYTE* data = new BYTE[5 + text.length()];
					BYTE zero = 0;
					int type = 3;
					memcpy(data, &type, 4);
					memcpy(data + 4, text.c_str(), text.length()); // change memcpy here
					memcpy(data + 4 + text.length(), &zero, 1); // change memcpy here, revert to 4

					ENetPacket* packetsou = enet_packet_create(data,
						5 + text.length(),
						ENET_PACKET_FLAG_RELIABLE);

					enet_peer_send(peer, 0, packetsou);


					return;
				}

			}
			else if (world->isPublic)
			{
				if (world->items[x + (y * world->width)].foreground == 242 || world->items[x + (y * world->width)].foreground == 5814 || world->items[x + (y * world->width)].foreground == 2408 || world->items[x + (y * world->width)].foreground == 1796 || world->items[x + (y * world->width)].foreground == 4428 || world->items[x + (y * world->width)].foreground == 7188)
				{
					string ownername = world->owner;
					GamePacket p2 = packetEnd(appendIntx(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), ((PlayerInfo*)(peer->data))->netID), "`0" + ownername + "'s `$World Lock`0. (`2Access Granted`w)"), 0), 1));


					ENetPacket* packet2 = enet_packet_create(p2.data,
						p2.len,
						ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet2);
					delete p2.data;
					string text = "action|play_sfx\nfile|audio/punch_locked.wav\ndelayMS|0\n";
					BYTE* data = new BYTE[5 + text.length()];
					BYTE zero = 0;
					int type = 3;
					memcpy(data, &type, 4);
					memcpy(data + 4, text.c_str(), text.length()); // change memcpy here
					memcpy(data + 4 + text.length(), &zero, 1); // change memcpy here, revert to 4

					ENetPacket* packetsou = enet_packet_create(data,
						5 + text.length(),
						ENET_PACKET_FLAG_RELIABLE);

					enet_peer_send(peer, 0, packetsou);


					return;
				}

			}
			else {
				ItemDefinition pro;
				pro = getItemDef(world->items[x + (y * world->width)].foreground);
				if (world->items[x + (y * world->width)].foreground == 242 || world->items[x + (y * world->width)].foreground == 5814 || world->items[x + (y * world->width)].foreground == 2408 || world->items[x + (y * world->width)].foreground == 1796 || world->items[x + (y * world->width)].foreground == 4428 || world->items[x + (y * world->width)].foreground == 7188) {
					string ownername = world->owner;
					GamePacket p2 = packetEnd(appendIntx(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), ((PlayerInfo*)(peer->data))->netID), "`0" + ownername + "'s `$World Lock`0. (`4No access`w)"), 0), 1));


					ENetPacket* packet2 = enet_packet_create(p2.data,
						p2.len,
						ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet2);
					delete p2.data;
					string text = "action|play_sfx\nfile|audio/punch_locked.wav\ndelayMS|0\n";
					BYTE* data = new BYTE[5 + text.length()];
					BYTE zero = 0;
					int type = 3;
					memcpy(data, &type, 4);
					memcpy(data + 4, text.c_str(), text.length()); // change memcpy here
					memcpy(data + 4 + text.length(), &zero, 1); // change memcpy here, revert to 4

					ENetPacket* packetsou = enet_packet_create(data,
						5 + text.length(),
						ENET_PACKET_FLAG_RELIABLE);

					enet_peer_send(peer, 0, packetsou);


					return;
				}
				else
				{
					string text = "action|play_sfx\nfile|audio/punch_locked.wav\ndelayMS|0\n";
					BYTE* data = new BYTE[5 + text.length()];
					BYTE zero = 0;
					int type = 3;
					memcpy(data, &type, 4);
					memcpy(data + 4, text.c_str(), text.length()); // change memcpy here
					memcpy(data + 4 + text.length(), &zero, 1); // change memcpy here, revert to 4

					ENetPacket* packetsou = enet_packet_create(data,
						5 + text.length(),
						ENET_PACKET_FLAG_RELIABLE);

					enet_peer_send(peer, 0, packetsou);


					return;
				}

			} /*lockeds*/
			if (tile == 242 || tile == 2408 || tile == 1796 || tile == 4428 || tile == 7188) {



				GamePacket p3 = packetEnd(appendIntx(appendString(appendIntx(appendString(createPacket(), "OnTalkBubble"), ((PlayerInfo*)(peer->data))->netID), "`0Only one `$World Lock`0 can be placed in a world!"), 0));


				ENetPacket* packet3 = enet_packet_create(p3.data,
					p3.len,
					ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer, 0, packet3);
				delete p3.data;
				return;
			}
		}
	}

	if (world->items[x + (y * world->width)].foreground == 5958) {
		if (tile == 32) {
			packet::dialog(peer, "add_label_with_icon|big|`wEpoch Machine|left|5958|\nadd_spacer|small|\nadd_textbox|`oSelect Your Doom:|\nadd_spacer|small|\nadd_checkbox|epochice|Ice Age|" + to_string(world->ice) + "|\nadd_checkbox|epochvol|Volcano|" + to_string(world->volcano) + "|\nadd_checkbox|epochland|Floating Islands|" + to_string(world->land) + "|\nadd_text_input|timedilation|Cycle time (minutes): |0|3|\nend_dialog|epochweather|Cancel|Okay|");
		}
	}
	if (world->items[x + (y * world->width)].foreground == 5818) {
		if (tile == 32) {
			string leader = ((PlayerInfo*)(peer->data))->guildLeader;
			string rawname = ((PlayerInfo*)(peer->data))->rawName;
			((PlayerInfo*)(peer->data))->wrenchx = x;
			((PlayerInfo*)(peer->data))->wrenchy = y;
			int id = getItemDef(world->items[x + (y * world->width)].foreground).id;
			if (rawname == leader || isWorldOwner(peer, world) || isWorldAdmin(peer, world)) {
				if (world->items[x + (y * world->width)].isOpened == false) {
					packet::dialog(peer, "add_label_with_icon|big|Edit " + getItemDef(world->items[x + (y * world->width)].foreground).name + "|left|" + to_string(id) + "|\nadd_checkbox|gopentopublic|`ois Open to Public?|0|\nend_dialog|gentrance|Cancel|OK|");
				}
				else {
					packet::dialog(peer, "add_label_with_icon|big|Edit " + getItemDef(world->items[x + (y * world->width)].foreground).name + "|left|" + to_string(id) + "|\nadd_checkbox|gopentopublic|`ois Open to Public?|1|\nend_dialog|gentrance|Cancel|OK|");
				}
			}
		}
	}
	if (world->items[x + (y * world->width)].foreground == 5000) {
		if (tile == 32) {
			if (world->owner == "" || isWorldOwner(peer, world) || isWorldAdmin(peer, world) || getPlyersWorld(peer)->isPublic) {
				string fyg = "set_default_color|`o\nadd_label_with_icon|big|`wWeather Machine - Background``|left|5000|\nadd_spacer|small|\nadd_textbox|`oYou can scan any Background Block to set it up in your weather machine.|\nadd_item_picker|bg_pick|Item: `2" + getItemDef(world->bgID).name + "|Select any Background Block|\nadd_quick_exit|\nend_dialog|bg_weather|Cancel|Okay|";
				packet::dialog(peer, fyg);
				((PlayerInfo*)(peer->data))->wrenchedBlockLocation = x + (y * world->width);
			}
			else {
				packet::SendTalkSelf(peer, "That area is owned by " + world->owner + "");
				return;
			}
		}
	}
	if (tile == 32) {
		if (world->items[x + (y * world->width)].foreground == 3832) {
			if (tile == 32) {
				if (world->owner == "" || isWorldOwner(peer, world) || isWorldAdmin(peer, world) || getPlyersWorld(peer)->isPublic) {
					string gaekid = "set_default_color|`o\nadd_label_with_icon|big|`wWeather Machine - Stuff``|left|3832|\nadd_spacer|small|\nadd_item_picker|stuff_pick|Item: `2" + getItemDef(world->stuffID).name + "|Select any item to rain down|\nadd_text_input|stuff_gravity|Gravity:|" + to_string(world->stuff_gravity) + "|3|\nadd_checkbox|stuff_spin|Spin Items|" + to_string(world->stuff_spin) + "|\nadd_checkbox|stuff_invert|Invert Sky Colors|" + to_string(world->stuff_invert) + "|\nadd_textbox|`oSelect item and click okay to continue!|\nend_dialog|stuff_weather|Cancel|Okay|";
					packet::dialog(peer, gaekid);
					((PlayerInfo*)(peer->data))->wrenchedBlockLocation = x + (y * world->width);
				}
				else {
					packet::SendTalkSelf(peer, "That area is owned by " + world->owner + "");
					return;
				}
			}
		}
	}

	if (tile == 18) {
		if (world->owner == "" || isWorldOwner(peer, world) || isWorldAdmin(peer, world) || getPlyersWorld(peer)->isPublic) {
			if (world->items[x + (y * world->width)].foreground == 340) {
				world->items[x + (y * world->width)].foreground = 0;
				int valgems = 0;
				valgems = rand() % 1000;
				int valgem = rand() % 1000;
				valgem = valgems + 1;
				std::ifstream ifs("gemdb/" + ((PlayerInfo*)(peer->data))->rawName + ".txt");
				std::string content((std::istreambuf_iterator<char>(ifs)),
					(std::istreambuf_iterator<char>()));


				int gembux = atoi(content.c_str());
				int fingembux = gembux + valgem;
				ofstream myfile;
				myfile.open("gemdb/" + ((PlayerInfo*)(peer->data))->rawName + ".txt");
				myfile << fingembux;
				myfile.close();

				int gemcalc = gembux + valgem;
				GamePacket pp = packetEnd(appendInt(appendString(createPacket(), "OnSetBux"), gemcalc));
				ENetPacket* packetpp = enet_packet_create(pp.data,
					pp.len,
					ENET_PACKET_FLAG_RELIABLE);

				enet_peer_send(peer, 0, packetpp);
				delete pp.data;
			}
			if (world->items[x + (y * world->width)].foreground == 1210) {
				if (world->weather == 8) {
					world->weather = 0;
				}
				else {
					world->weather = 8;
				}
				OnSetCurrentWeather(peer, world->weather);
			}
			if (world->items[x + (y * world->width)].foreground == 5958) {
				if (world->weather == 38) {
					world->weather = 0;
				}
				else {
					world->weather = 38;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 3832) {
				if (world->weather == 29) {
					world->weather = 0;
				}
				else {
					world->weather = 29;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 984) {
				if (world->weather == 5) {
					world->weather = 0;
				}
				else {
					world->weather = 5;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 946) {
				if (world->weather == 3) {
					world->weather = 0;
				}
				else {
					world->weather = 3;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 934) {
				if (world->weather == 2) {
					world->weather = 0;
				}
				else {
					world->weather = 2;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 3252) {
				if (world->weather == 20) {
					world->weather = 0;
				}
				else {
					world->weather = 20;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 7644) {
				if (world->weather == 44) {
					world->weather = 0;
				}
				else {
					world->weather = 44;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 6854) {
				if (world->weather == 42) {
					world->weather = 0;
				}
				else {
					world->weather = 42;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 5716) {
				if (world->weather == 37) {
					world->weather = 0;
				}
				else {
					world->weather = 37;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 5654) {
				if (world->weather == 36) {
					world->weather = 0;
				}
				else {
					world->weather = 36;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 8896) {
				if (world->weather == 47) {
					world->weather = 0;
				}
				else {
					world->weather = 47;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 8836) {
				if (world->weather == 48) {
					world->weather = 0;
				}
				else {
					world->weather = 48;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 5112) {
				if (world->weather == 35) {
					world->weather = 0;
				}
				else {
					world->weather = 35;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 5000) {
				if (world->weather == 34) {
					world->weather = 0;
				}
				else {
					world->weather = 34;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 4892) {
				if (world->weather == 33) {
					world->weather = 0;
				}
				else {
					world->weather = 33;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 4776) {
				if (world->weather == 32) {
					world->weather = 0;
				}
				else {
					world->weather = 32;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 4486) {
				if (world->weather == 31) {
					world->weather = 0;
				}
				else {
					world->weather = 31;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 4242) {
				if (world->weather == 30) {
					world->weather = 0;
				}
				else {
					world->weather = 30;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 1490) {
				if (world->weather == 10) {
					world->weather = 0;
				}
				else {
					world->weather = 10;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 3694) {
				if (world->weather == 28) {
					world->weather = 0;
				}
				else {
					world->weather = 28;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 3534) {
				if (world->weather == 22) {
					world->weather = 0;
				}
				else {
					world->weather = 22;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 3446) {
				if (world->weather == 21) {
					world->weather = 0;
				}
				else {
					world->weather = 21;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 2284) {
				if (world->weather == 18) {
					world->weather = 0;
				}
				else {
					world->weather = 18;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 2046) {
				if (world->weather == 17) {
					world->weather = 0;
				}
				else {
					world->weather = 17;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 1750) {
				if (world->weather == 15) {
					world->weather = 0;
				}
				else {
					world->weather = 15;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 1364) {
				if (world->weather == 11) {
					world->weather = 0;
				}
				else {
					world->weather = 11;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 932) {
				if (world->weather == 4) {
					world->weather = 0;
				}
				else {
					world->weather = 4;
				}
				OnSetCurrentWeather(peer, world->weather);
			}

			if (world->items[x + (y * world->width)].foreground == 10286) {
				if (world->weather == 51) {
					world->weather = 0;
				}
				else {
					world->weather = 51;
				}
				OnSetCurrentWeather(peer, world->weather);
			}
		}
	}

	if (tile == 822) {
		world->items[x + (y * world->width)].water = !world->items[x + (y * world->width)].water;
		return;
	}
	if (tile == 1136) {
		packet::dialog(peer, "set_default_color|`w\n\nadd_label_with_icon|big|`wMars Blast|left|1136|\nadd_textbox|`oThis item creates a new world! Enter a unique name for it.|left|\nadd_text_input|marstext|`oNew World Name||30|\nend_dialog|sendmb|Cancel|`5Create!|\n");
		return;
	}
	if (tile == 3062)
	{
		world->items[x + (y * world->width)].fire = !world->items[x + (y * world->width)].fire;
		return;
	}
	if (tile == 1866)
	{
		world->items[x + (y * world->width)].glue = !world->items[x + (y * world->width)].glue;
		return;
	}
	ItemDefinition def;
	try {
		def = getItemDef(tile);
		if (def.blockType == BlockTypes::CLOTHING) return;
	}
	catch (int e) {
		def.breakHits = 4;
		def.blockType = BlockTypes::UNKNOWN;
#ifdef TOTAL_LOG
		cout << "Ugh, unsupported item " << tile << endl;
#endif
	}

	if (tile == 18) {
		if (world->items[x + (y * world->width)].background == 6864 && world->items[x + (y * world->width)].foreground == 0) return;
		if (world->items[x + (y * world->width)].background == 0 && world->items[x + (y * world->width)].foreground == 0) return;
		//data.netID = -1;
		int tool = ((PlayerInfo*)(peer->data))->cloth_hand;
		data.packetType = 0x8;
		data.plantingTree = (tool == 98 || tool == 1438 || tool == 4956) ? 8 : 6;
		int block = world->items[x + (y * world->width)].foreground > 0 ? world->items[x + (y * world->width)].foreground : world->items[x + (y * world->width)].background;
		//if (world->items[x + (y*world->width)].foreground == 0) return;
		using namespace std::chrono;
		if ((duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() - world->items[x + (y * world->width)].breakTime >= 4000)
		{
			world->items[x + (y * world->width)].breakTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
			world->items[x + (y * world->width)].breakLevel = 0; // TODO
			if (world->items[x + (y * world->width)].foreground == 758)
				sendRoulete(peer, x, y);
		}
		if (y < world->height)
		{
			world->items[x + (y * world->width)].breakTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
			world->items[x + (y * world->width)].breakLevel += (int)((tool == 98 || tool == 1438 || tool == 4956) ? 8 : 6); // TODO
		}


		if (y < world->height && world->items[x + (y * world->width)].breakLevel >= getItemDef(block).breakHits * 6) { // TODO
			data.packetType = 0x3;// 0xC; // 0xF // World::HandlePacketTileChangeRequest
			data.plantingTree = 18;
			world->items[x + (y * world->width)].breakLevel = 0;
			if (world->items[x + (y * world->width)].foreground != 0)
			{
				if (world->items[x + (y * world->width)].foreground == 242)
				{
					world->owner = "";
					world->isPublic = false;
				}
				world->items[x + (y * world->width)].foreground = 0;
			}
			else {
				world->items[x + (y * world->width)].background = 6864;
			}

		}


	}
	else {
		for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
		{
			if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == tile)
			{
				if ((unsigned int)((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount > 1)
				{
					((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount--;
				}
				else {
					((PlayerInfo*)(peer->data))->inventory.items.erase(((PlayerInfo*)(peer->data))->inventory.items.begin() + i);

				}
			}
		}
		if (def.blockType == BlockTypes::BACKGROUND)
		{
			world->items[x + (y * world->width)].background = tile;
		}
		else {
			if (world->items[x + (y * world->width)].foreground != 0)return;
			world->items[x + (y * world->width)].foreground = tile;
			ItemDefinition pro;
			pro = getItemDef(tile);
			if (tile == 242 || tile == 2408 || tile == 1796 || tile == 4428 || tile == 7188 || tile == 10410) {
				if (((PlayerInfo*)(peer->data))->rawName == world->owner) return;
				world->owner = ((PlayerInfo*)(peer->data))->rawName;
				world->ownerID = ((PlayerInfo*)(peer->data))->netID;
				world->isPublic = false;
				isLock = true;
				ENetPeer* currentPeer;

				for (currentPeer = server->peers;
					currentPeer < &server->peers[server->peerCount];
					++currentPeer)
				{
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
						continue;
					if (isHere(peer, currentPeer)) {
						packet::consolemessage(peer, "`5[`w" + world->name + " `ohas been World Locked by " + ((PlayerInfo*)(peer->data))->displayName + "`5]");
						packet::SendTalkSelf(peer, "`5[`w" + world->name + " `ohas been World Locked by " + ((PlayerInfo*)(peer->data))->displayName + "`5]");
						packet::PlayAudio(peer, "audio/use_lock.wav", 0);
					}
				}
			}

		}

		world->items[x + (y * world->width)].breakLevel = 0;
	}

	ENetPeer* currentPeer;

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);

	}
	string name = ((PlayerInfo*)(peer->data))->rawName;
	if (isLock) {
		ENetPeer* currentPeer;

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer)) {
				sendTileData(currentPeer, x, y, 0x10, tile, world->items[x + (y * world->width)].background, lockTileDatas(0x20, ((PlayerInfo*)(peer->data))->userID, 0, 0, false, 100));
			}
		}
	}
}

void sendBackground(ENetPeer* peer, int x, int y, uint16_t itemid) {
	PlayerMoving pmov;
	pmov.packetType = 5;
	pmov.characterState = 8;
	pmov.punchX = x;
	pmov.punchY = y;
	pmov.netID = -1;
	uint8_t* pmovpacked = packPlayerMoving(&pmov);
	*(uint32_t*)(pmovpacked + 52) = 5 + 8;
	uint8_t* packet = new uint8_t[4 + 56 + 5 + 8];
	memset(packet, 0, 4 + 56 + 5 + 8);
	packet[0] = 4;
	memcpy(packet + 4, pmovpacked, 56);
	*(uint16_t*)(packet + 4 + 56) = 5000; // WEATHER MACHINE - BACKGROUND
	*(uint16_t*)(packet + 4 + 56 + 6) = 1;
	*(uint8_t*)(packet + 4 + 56 + 8) = 0x28;
	*(uint16_t*)(packet + 4 + 56 + 9) = itemid;
	ENetPacket* epacket = enet_packet_create(packet, 4 + 56 + 8 + 5, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, epacket);
	delete pmovpacked;
	delete[] packet;
}

void updateDoor(ENetPeer* peer, int foreground, int x, int y, string text)
{
	PlayerMoving sign;
	sign.packetType = 0x3;
	sign.characterState = 0x0;
	sign.x = x;
	sign.y = y;
	sign.punchX = x;
	sign.punchY = y;
	sign.XSpeed = 0;
	sign.YSpeed = 0;
	sign.netID = -1;
	sign.plantingTree = foreground;
	SendPacketRaw(4, packPlayerMoving(&sign), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	int hmm = 8;
	int text_len = text.length();
	int lol = 0;
	int wut = 5;
	int yeh = hmm + 3 + 1;
	int idk = 15 + text_len;
	int is_locked = 0;
	int bubble_type = 1;
	int ok = 52 + idk;
	int kek = ok + 4;
	int yup = ok - 8 - idk;
	int four = 4;
	int magic = 56;
	int wew = ok + 5 + 4;
	int wow = magic + 4 + 5;

	BYTE* data = new BYTE[kek];
	ENetPacket* p = enet_packet_create(0, wew, ENET_PACKET_FLAG_RELIABLE);
	for (int i = 0; i < kek; i++) data[i] = 0;
	memcpy(data, &wut, four); //4
	memcpy(data + yeh, &hmm, four); //8
	memcpy(data + yup, &x, 4); //12
	memcpy(data + yup + 4, &y, 4); //16
	memcpy(data + 4 + yup + 4, &idk, four); //20
	memcpy(data + magic, &foreground, 2); //22
	memcpy(data + four + magic, &lol, four); //26
	memcpy(data + magic + 4 + four, &bubble_type, 1); //27
	memcpy(data + wow, &text_len, 2); //data + wow = text_len, pos 29
	memcpy(data + 2 + wow, text.c_str(), text_len); //data + text_len_len + text_len_offs = text, pos 94
	memcpy(data + ok, &is_locked, four); //98
	memcpy(p->data, &four, four); //4
	memcpy((char*)p->data + four, data, kek); //kek = data_len
	ENetPeer* currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			enet_peer_send(currentPeer, 0, p);
		}
	}
	delete data;
}

void updateSign(ENetPeer* peer, int foreground, int x, int y, string text, int background)
{
	PlayerMoving sign;
	sign.packetType = 0x3;
	sign.characterState = 0x0;
	sign.x = x;
	sign.y = y;
	sign.punchX = x;
	sign.punchY = y;
	sign.XSpeed = 0;
	sign.YSpeed = 0;
	sign.netID = -1;
	sign.plantingTree = foreground;
	SendPacketRaw(4, packPlayerMoving(&sign), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	int hmm = 8, wot = text.length(), lol = 0, wut = 5;
	int yeh = hmm + 3 + 1, idk = 15 + wot, lmao = -1, yey = 2; //idk = text_len + 15, wut = type(?), wot = text_len, yey = len of text_len
	int ok = 52 + idk;
	int kek = ok + 4, yup = ok - 8 - idk;
	int thonk = 4, magic = 56, wew = ok + 5 + 4;
	int wow = magic + 4 + 5;
	BYTE* data = new BYTE[kek];
	ENetPacket* p = enet_packet_create(0, wew, ENET_PACKET_FLAG_RELIABLE);
	for (int i = 0; i < kek; i++) data[i] = 0;
	memcpy(data, &wut, thonk);
	memcpy(data + yeh, &hmm, thonk); //read discord
	memcpy(data + yup, &x, 4);
	memcpy(data + yup + 4, &y, 4);
	memcpy(data + 4 + yup + 4, &idk, thonk);
	memcpy(data + magic, &foreground, yey);
	memcpy(data + magic + 2, &background, yey); //p100 fix by the one and only lapada
	memcpy(data + thonk + magic, &lol, thonk);
	memcpy(data + magic + 4 + thonk, &yey, 1);
	memcpy(data + wow, &wot, yey); //data + wow = text_len
	memcpy(data + yey + wow, text.c_str(), wot); //data + text_len_len + text_len_offs = text
	memcpy(data + ok, &lmao, thonk); //end ?
	memcpy(p->data, &thonk, thonk);
	memcpy((char*)p->data + thonk, data, kek); //kek = data_len
	ENetPeer* currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			enet_peer_send(currentPeer, 0, p);
		}
	}
	delete data;
}

void DailyMath() {
	while (DailyMath) {
		using namespace std::chrono;
		if (quest + 400000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
			quest = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
			int userMaxRand = 100, userLowRand = 0;
			resultnbr2 = rand() % userMaxRand + userLowRand + 1;
			srand(time(0));
			resultnbr1 = rand() % userMaxRand + userLowRand + 1;
			hasil = resultnbr1 + resultnbr2;
			prize = rand() % 10000;
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
				sendConsole(currentPeer, "`9** Growtopia Daily Math [Questions : `3'" + to_string(resultnbr1) + " + " + to_string(resultnbr2) + "'`9 = ?] Prize: `2" + to_string(prize) + "`9 (gems) ! `o(/c <answer>).");
				SendConsole("(DailyMath) Growtopia Daily Math : " + to_string(resultnbr1) + " + " + to_string(resultnbr2) + " = ? Prize: " + to_string(prize) + "! `o(/c <answer>).", "DAILYMATH");
				packet::PlayAudio(currentPeer, "startopia_tool_droid.wav", 0);
				DailyMaths = true;
			}
		}
	}
}

void sendWho(ENetPeer* peer)
{
	ENetPeer* currentPeer;
	string name = "";
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			if (((PlayerInfo*)(currentPeer->data))->isGhost)
				continue;

			gamepacket_t p;
			p.Insert("OnTalkBubble");
			p.Insert(((PlayerInfo*)(currentPeer->data))->netID);
			p.Insert(((PlayerInfo*)(currentPeer->data))->displayName);
			p.Insert(1);
			p.CreatePacket(peer);
		}
	}
}

void Captcha() {
	while (1) {
		Sleep(300);
		LoadCaptCha();
	}
}

void updateEntrance(ENetPeer* peer, int foreground, int x, int y, bool open, int bg) {
	BYTE* data = new BYTE[69];// memset(data, 0, 69);
	for (int i = 0; i < 69; i++) data[i] = 0;
	int four = 4; int five = 5; int eight = 8;
	int huhed = (65536 * bg) + foreground; int loled = 128;

	memcpy(data, &four, 4);
	memcpy(data + 4, &five, 4);
	memcpy(data + 16, &eight, 4);
	memcpy(data + 48, &x, 4);
	memcpy(data + 52, &y, 4);
	memcpy(data + 56, &eight, 4);
	memcpy(data + 60, &foreground, 4);
	memcpy(data + 62, &bg, 4);

	if (open) {
		int state = 0;
		memcpy(data + 66, &loled, 4);
		memcpy(data + 68, &state, 4);
	}
	else {
		int state = 100;
		int yeetus = 25600;
		memcpy(data + 67, &yeetus, 5);
		memcpy(data + 68, &state, 4);
	}
	ENetPacket* p = enet_packet_create(data, 69, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);
}
void updateSignSound(ENetPeer* peer, int foreground, int x, int y, string text, int background)
{
	int hmm = 8, wot = text.length(), lol = 0, wut = 5;
	int yeh = hmm + 3 + 1, idk = 15 + wot, lmao = -1, yey = 2; //idk = text_len + 15, wut = type(?), wot = text_len, yey = len of text_len
	int ok = 52 + idk;
	int kek = ok + 4, yup = ok - 8 - idk;
	int thonk = 4, magic = 56, wew = ok + 5 + 4;
	int wow = magic + 4 + 5;
	BYTE* data = new BYTE[kek];
	ENetPacket* p = enet_packet_create(0, wew, ENET_PACKET_FLAG_RELIABLE);
	for (int i = 0; i < kek; i++) data[i] = 0;
	memcpy(data, &wut, thonk);
	memcpy(data + yeh, &hmm, thonk); //read discord
	memcpy(data + yup, &x, 4);
	memcpy(data + yup + 4, &y, 4);
	memcpy(data + 4 + yup + 4, &idk, thonk);
	memcpy(data + magic, &foreground, yey);
	memcpy(data + magic + 2, &background, yey); //p100 fix by the one and only lapada
	memcpy(data + thonk + magic, &lol, thonk);
	memcpy(data + magic + 4 + thonk, &yey, 1);
	memcpy(data + wow, &wot, yey); //data + wow = text_len
	memcpy(data + yey + wow, text.c_str(), wot); //data + text_len_len + text_len_offs = text
	memcpy(data + ok, &lmao, thonk); //end ?
	memcpy(p->data, &thonk, thonk);
	memcpy((char*)p->data + thonk, data, kek); //kek = data_len
	ENetPeer* currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			enet_peer_send(currentPeer, 0, p);
		}
	}
	delete data;
}
