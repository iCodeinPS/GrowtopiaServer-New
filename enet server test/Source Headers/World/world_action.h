#pragma once

WorldInfo generateWorld(string name, int width, int height)
{
	WorldInfo world;
	world.name = name;
	world.width = width;
	world.height = height;
	world.items = new WorldItem[world.width * world.height];
	for (int i = 0; i < world.width * world.height; i++)
	{
		if (i >= 3800 && i < 5400 && !(rand() % 50)) { world.items[i].foreground = 10; }
		else if (i >= 3700 && i < 5400) {
			if (i > 5000) {
				if (i % 7 == 0) { world.items[i].foreground = 4; }
				else { world.items[i].foreground = 2; }
			}
			else { world.items[i].foreground = 2; }
		}
		else if (i >= 5400) { world.items[i].foreground = 8; }
		if (i >= 3700)
			world.items[i].background = 14;
		if (i == 3650)
			world.items[i].foreground = 6;
		else if (i >= 3600 && i < 3700)
			world.items[i].foreground = 0; //fixed the grass in the world!
		if (i == 3750)
			world.items[i].foreground = 8;
	}
	return world;
}

AWorld WorldDB::get2(string name) {
	if (worlds.size() > 200) {
#ifdef TOTAL_LOG
		cout << "Saving redundant worlds!" << endl;
#endif
		saveRedundant();
#ifdef TOTAL_LOG
		cout << "Redundant worlds are saved!" << endl;
#endif
	}
	AWorld ret;
	name = getStrUpper(name);
	if (name.length() < 1) throw 1; // too short name
	for (char c : name) {
		if ((c < 'A' || c>'Z') && (c < '0' || c>'9'))
			throw 2; // wrong name
	}
	if (name == "EXIT") {
		throw 3;
	}
	if (name == "CON" || name == "PRN" || name == "AUX" || name == "NUL" || name == "COM1" || name == "COM2" || name == "COM3" || name == "COM4" || name == "COM5" || name == "COM6" || name == "COM7" || name == "COM8" || name == "COM9" || name == "LPT1" || name == "LPT2" || name == "LPT3" || name == "LPT4" || name == "LPT5" || name == "LPT6" || name == "LPT7" || name == "LPT8" || name == "LPT9") throw 3;
	for (int i = 0; i < worlds.size(); i++) {
		if (worlds.at(i).name == name)
		{
			ret.id = i;
			ret.info = worlds.at(i);
			ret.ptr = &worlds.at(i);
			return ret;
		}

	}
	std::ifstream ifs("worlds/" + name + ".json");
	if (ifs.is_open()) {

		json j;
		ifs >> j;
		WorldInfo info;
		info.name = j["name"].get<string>();
		info.width = j["width"];
		info.height = j["height"];
		info.owner = j["owner"].get<string>();
		info.weather = j["weather"].get<int>();
		info.isPublic = j["isPublic"];
		for (int i = 0; i < j["access"].size(); i++) {
			info.acclist.push_back(j["access"][i]);
		}
		info.stuffID = j["stuffID"].get<int>();
		info.stuff_gravity = j["stuff_gravity"].get<int>();
		info.stuff_invert = j["stuff_invert"].get<bool>();
		info.stuff_spin = j["stuff_spin"].get<bool>();
		json tiles = j["tiles"];
		int square = info.width * info.height;
		info.items = new WorldItem[square];
		for (int i = 0; i < square; i++) {
			info.items[i].foreground = tiles[i]["fg"];
			info.items[i].background = tiles[i]["bg"];
		}
		worlds.push_back(info);
		ret.id = worlds.size() - 1;
		ret.info = info;
		ret.ptr = &worlds.at(worlds.size() - 1);
		return ret;
	}
	else {
		WorldInfo info = generateWorld(name, 100, 60);

		worlds.push_back(info);
		ret.id = worlds.size() - 1;
		ret.info = info;
		ret.ptr = &worlds.at(worlds.size() - 1);
		return ret;
	}
	throw 1;
}

WorldInfo WorldDB::get(string name) {

	return this->get2(name).info;
}

int WorldDB::getworldStatus(string name) {
	name = getStrUpper(name);
	if (name.length() > 24) return -1;
	return 0;
}

void WorldDB::flush(WorldInfo info)
{
	std::ofstream o("worlds/" + info.name + ".json");
	if (!o.is_open()) {
		cout << GetLastError() << endl;
	}
	json j;
	j["name"] = info.name;
	j["width"] = info.width;
	j["height"] = info.height;
	j["owner"] = info.owner;
	j["isPublic"] = info.isPublic;
	j["weather"] = info.weather;
	j["stuffID"] = info.stuffID;
	j["stuff_gravity"] = info.stuff_gravity;
	j["stuff_invert"] = info.stuff_invert;
	j["stuff_spin"] = info.stuff_spin;
	j["access"] = info.acclist;
	json tiles = json::array();
	int square = info.width * info.height;

	for (int i = 0; i < square; i++)
	{
		json tile;
		tile["fg"] = info.items[i].foreground;
		tile["bg"] = info.items[i].background;
		tiles.push_back(tile);
	}
	j["tiles"] = tiles;
	o << j << std::endl;
}

void WorldDB::flush2(AWorld info)
{
	this->flush(info.info);
}

void WorldDB::save(AWorld info)
{
	flush2(info);
	delete info.info.items;
	worlds.erase(worlds.begin() + info.id);
}

void WorldDB::saveAll()
{
	for (int i = 0; i < worlds.size(); i++) {
		flush(worlds.at(i));
		delete[] worlds.at(i).items;
	}
	worlds.clear();
}

vector<WorldInfo> WorldDB::getRandomWorlds() {
	vector<WorldInfo> ret;
	for (int i = 0; i < ((worlds.size() < 10) ? worlds.size() : 10); i++)
	{
		ret.push_back(worlds.at(i));
	}
	if (worlds.size() > 4) {
		for (int j = 0; j < 6; j++)
		{
			bool isPossible = true;
			WorldInfo world = worlds.at(rand() % (worlds.size() - 4));
			for (int i = 0; i < ret.size(); i++)
			{
				if (world.name == ret.at(i).name || world.name == "EXIT")
				{
					isPossible = false;
				}
			}
			if (isPossible)
				ret.push_back(world);
		}
	}
	return ret;
}

void WorldDB::saveRedundant()
{
	for (int i = 4; i < worlds.size(); i++) {
		bool canBeFree = true;
		ENetPeer* currentPeer;

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (((PlayerInfo*)(currentPeer->data))->currentWorld == worlds.at(i).name)
				canBeFree = false;
		}
		if (canBeFree)
		{
			flush(worlds.at(i));
			delete worlds.at(i).items;
			worlds.erase(worlds.begin() + i);
			i--;
		}
	}
}

WorldDB worldDB;

bool CheckItemMaxed(ENetPeer* peer, int fItemId, int fQuantityAdd) {
	bool isMaxed = false;
	for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++) {
		if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemId && ((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount + fQuantityAdd > 200) {
			isMaxed = true;
			break;
		}
	}
	return isMaxed;
}

void saveAllWorlds() // atexit hack plz fix
{
	GamePacket p0 = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"),
		"`4Global system message`o: Saving all worlds `oin `p5 `wseconds`o, you will be timed out for a short amount of time`w! `oDon't punch anything or you may get disconnected!``"));
	ENetPacket* packet0 = enet_packet_create(p0.data,
		p0.len,
		ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet0);
	cout << "[!] Saving worlds..." << endl;
	worldDB.saveAll();
	cout << "[!] Worlds saved!" << endl;
	Sleep(1000);

	Sleep(200);
	GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "`5 `4Global system message`o: `2Saved `oall worlds``"));
	ENetPacket* packet = enet_packet_create(p.data,
		p.len,
		ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet);
	delete p0.data;
	delete p.data;
}

void autosave()
{
	bool exist = std::experimental::filesystem::exists("save.txt");
	if (!exist)
	{
		ofstream save("save.txt");
		save << 0;
		save.close();
	}
	std::ifstream ok("save.txt");
	std::string limits((std::istreambuf_iterator<char>(ok)),
		(std::istreambuf_iterator<char>()));
	int a = atoi(limits.c_str());
	if (a == 0)
	{
		ofstream ok;
		ok.open("save.txt");
		ok << 50;
		ok.close();
		worldDB.saveAll();
		cout << "[!]Auto Saving Worlds" << endl;
	}
	else
	{
		int aa = a - 1;
		ofstream ss;
		ss.open("save.txt");
		ss << aa;
		ss.close();
		if (aa == 0)
		{
			ofstream ok;
			ok.open("save.txt");
			ok << 50;
			ok.close();
			worldDB.saveAll();
			cout << "[!]Auto Saving Worlds" << endl;
		}
	}
}
bool CheckItemExists(ENetPeer* peer, const int fItemId) {
	auto isExists = false;
	for (auto i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++) {
		if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == fItemId) {
			isExists = true;
			break;
		}
	}
	return isExists;
}

WorldInfo* getPlyersWorld(ENetPeer* peer)
{
	try {
		return worldDB.get2(((PlayerInfo*)(peer->data))->currentWorld).ptr;
	}
	catch (int e) {
		return NULL;
	}
}

BYTE* packBlockVisual222(TileExtra* dataStruct)
{

	BYTE* data = new BYTE[104]; // 96
	for (int i = 0; i < 100; i++)
	{
		data[i] = 0;
	}
	memcpy(data, &dataStruct->packetType, 4);
	memcpy(data + 8, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 16, &dataStruct->objectSpeedX, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	memcpy(data + 52, &dataStruct->charStat, 4);
	memcpy(data + 56, &dataStruct->blockid, 2);
	memcpy(data + 58, &dataStruct->backgroundid, 2);
	memcpy(data + 60, &dataStruct->visual, 4);
	memcpy(data + 64, &dataStruct->displayblock, 4);


	return data;
}
BYTE* packStuffVisual(TileExtra* dataStruct, int options, int gravity)
{
	BYTE* data = new BYTE[102];
	for (int i = 0; i < 102; i++)
	{
		data[i] = 0;
	}
	memcpy(data, &dataStruct->packetType, 4);
	memcpy(data + 8, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	memcpy(data + 52, &dataStruct->charStat, 4);
	memcpy(data + 56, &dataStruct->blockid, 2);
	memcpy(data + 58, &dataStruct->backgroundid, 2);
	memcpy(data + 60, &dataStruct->visual, 4);
	memcpy(data + 64, &dataStruct->displayblock, 4);
	memcpy(data + 68, &gravity, 4);
	memcpy(data + 70, &options, 4);

	return data;
}

ItemDefinition getItemDef(int id)
{
	if (id < itemDefs.size() && id > -1)
		return itemDefs.at(id);
	throw 0;
	return itemDefs.at(0);
}

void UpdateBlockState(ENetPeer* peer, int x, int y, bool forEveryone, WorldInfo* worldInfo) {

	if (!worldInfo) return;

	int i = y * worldInfo->width + x;

	int blockStateFlags = 0;


	if (worldInfo->items[i].flipped)
		blockStateFlags |= 0x00200000;
	if (worldInfo->items[i].water)
		blockStateFlags |= 0x04000000;
	if (worldInfo->items[i].glue)
		blockStateFlags |= 0x08000000;
	if (worldInfo->items[i].fire)
		blockStateFlags |= 0x10000000;
	if (worldInfo->items[i].red)
		blockStateFlags |= 0x20000000;
	if (worldInfo->items[i].green)
		blockStateFlags |= 0x40000000;
	if (worldInfo->items[i].blue)
		blockStateFlags |= 0x80000000;
	if (worldInfo->items[i].active)
		blockStateFlags |= 0x00400000;
	if (worldInfo->items[i].silenced)
		blockStateFlags |= 0x02400000;
}
inline void autoSaveWorlds()
{
	while (true)
	{
		Sleep(300000);
		ENetPeer* currentPeer;
		serverIsFrozen = true;
		for (currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
		}
		GamePacket p0 = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"),
			"`2Saving all worlds."));
		ENetPacket* packet0 = enet_packet_create(p0.data,
			p0.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(server, 0, packet0);
		saveAllWorlds();

	}
	serverIsFrozen = false;

}

