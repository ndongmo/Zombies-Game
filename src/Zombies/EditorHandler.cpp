#include "EditorHandler.h"

#include <Logger.h>
#include <CEGUI/XMLAttributes.h>

EditorHandler::EditorHandler(LevelEditor& level) : m_level(level)
{
	level.clear();
}

void EditorHandler::elementStart(const CEGUI::String& element, const CEGUI::XMLAttributes& attributes)
{
	if (element == "level")
	{
		std::string name = attributes.getValueAsString("name").c_str();
		std::string music = attributes.getValueAsString("music").c_str();
		int width = attributes.getValueAsInteger("width");
		int height = attributes.getValueAsInteger("height");
		int lights = attributes.getValueAsInteger("lights");
		int humans = attributes.getValueAsInteger("humans");
		int zombies = attributes.getValueAsInteger("zombies");
		int coffers = attributes.getValueAsInteger("coffers");
		int keys = attributes.getValueAsInteger("keys");
		int guns = attributes.getValueAsInteger("guns");
		int doors = attributes.getValueAsInteger("doors");
		int medicines = attributes.getValueAsInteger("medicines"); 
		int bags = attributes.getValueAsInteger("bags");
		int ammos = attributes.getValueAsInteger("ammos");
		int flashlights = attributes.getValueAsInteger("flashlights");
		int nodes = attributes.getValueAsInteger("nodes");
		int edges = attributes.getValueAsInteger("edges");
		NS2::ColorRGBA8 color;
		getColor(color, attributes);

		m_level.name = name;
		if (!music.empty()) m_level.music = NS2::ResourceManager::getMusic(music);
		m_level.ambientColor = color;
		m_level.width = width;
		m_level.height = height;
		m_level.lights.resize(lights);
		m_level.humans.resize(humans);
		m_level.zombies.resize(zombies);
		m_level.coffers.resize(coffers);
		m_level.keys.resize(keys);
		m_level.guns.resize(guns);
		m_level.doors.resize(doors);
		m_level.medicines.resize(medicines);
		m_level.bags.resize(bags);
		m_level.ammos.resize(ammos);
		m_level.flashlights.resize(flashlights);
		m_level.tiles.resize(width * height);
		m_level.nodes.resize(nodes);
		m_level.edges.resize(edges);
	}
	else if (element == "tile")
	{
		std::string texture = attributes.getValueAsString("texture").c_str();
		int x = attributes.getValueAsInteger("x");
		int y = attributes.getValueAsInteger("y");
		int index = attributes.getValueAsInteger("index");
		bool crossable = attributes.getValueAsBool("crossable");
		NS2::GLTexture& tex = NS2::ResourceManager::getTexture(texture);
		NS2::ColorRGBA8 color;
		getColor(color, attributes);

		m_level.tiles[index].init(tex, x, y, crossable);
		m_level.tiles[index].setColor(color);
	}
	else if (element == "player")
	{
		Player& player = m_level.player;
		getEntity(player, attributes);
		
		float speed = attributes.getValueAsFloat("speed");
		float health = attributes.getValueAsFloat("health");
		int src = attributes.getValueAsInteger("src_light");
		int around = attributes.getValueAsInteger("around_light");
		int diff = attributes.getValueAsInteger("diff_light");
		
		player.setSpeed(speed);
		player.setHealth(health);
		m_level.playerSrc = src;
		m_level.playerAround = around;
		m_level.playerDiff = diff;
	}
	else if (element == "human")
	{
		int index = attributes.getValueAsInteger("index");
		Human& human = m_level.humans[index];
		getEntity(human, attributes);

		float speed = attributes.getValueAsFloat("speed");
		float health = attributes.getValueAsFloat("health");

		human.setSpeed(speed);
		human.setHealth(health);
	}
	else if (element == "zombie")
	{
		int index = attributes.getValueAsInteger("index");
		Zombie& zombie = m_level.zombies[index];
		getEntity(zombie, attributes);

		float speed = attributes.getValueAsFloat("speed");
		float health = attributes.getValueAsFloat("health");
		float damage = attributes.getValueAsFloat("damage");

		zombie.setSpeed(speed);
		zombie.setHealth(health);
		zombie.setDamage(damage);
	}
	else if (element == "key")
	{
		int index = attributes.getValueAsInteger("index");
		Key& key = m_level.keys[index];
		getEntity(key, attributes);

		std::string snd = attributes.getValueAsString("sound").c_str();
		if (!snd.empty()){
			NS2::Sound& sound = NS2::ResourceManager::getSound(snd);
			key.setSound(sound);
		}
	}
	else if (element == "door")
	{
		int index = attributes.getValueAsInteger("index");
		Door& door = m_level.doors[index];
		getEntity(door, attributes);

		bool opened = attributes.getValueAsBool("opened");
		std::string keyname = attributes.getValueAsString("keyname").c_str();
		std::string snd = attributes.getValueAsString("sound").c_str();

		if (!snd.empty()){
			NS2::Sound& sound = NS2::ResourceManager::getSound(snd);
			door.setSound(sound);
		}
		door.setKeyName(keyname);
		door.setOpened(opened);
	}
	else if (element == "medicine")
	{
		int index = attributes.getValueAsInteger("index");
		Medicine& medoc = m_level.medicines[index];
		getEntity(medoc, attributes);

		float health = attributes.getValueAsFloat("health");
		std::string snd = attributes.getValueAsString("sound").c_str();
		if (!snd.empty()){
			NS2::Sound& sound = NS2::ResourceManager::getSound(snd);
			medoc.setSound(sound);
		}
		medoc.setHealth(health);
	}
	else if (element == "ammo")
	{
		int index = attributes.getValueAsInteger("index");
		Ammo& ammo = m_level.ammos[index];
		getEntity(ammo, attributes);

		int amount = attributes.getValueAsInteger("amount");
		GunType type = (GunType)attributes.getValueAsInteger("guntype");
		
		ammo.setAmount(amount);
		ammo.setGunType(type);
	}
	else if (element == "flashlight")
	{
		int index = attributes.getValueAsInteger("index");
		Flashlight& flashlight = m_level.flashlights[index];
		getEntity(flashlight, attributes);

		bool on = attributes.getValueAsBool("on");
		float low = attributes.getValueAsFloat("low");
		float duration = attributes.getValueAsFloat("duration");

		flashlight.setOn(on);
		flashlight.setLow(low);
		flashlight.setDuration(duration);
	}
	else if (element == "bag")
	{
		int index = attributes.getValueAsInteger("index");
		Bag& bag = m_level.bags[index];
		getEntity(bag, attributes);

		int columns = attributes.getValueAsInteger("columns");
		int rows = attributes.getValueAsInteger("rows");

		bag.setColumns(columns);
		bag.setRows(rows);
	}
	else if (element == "gun")
	{
		int index = attributes.getValueAsInteger("index");
		Gun& gun = m_level.guns[index];
		getEntity(gun, attributes);

		float loader = attributes.getValueAsFloat("loader");
		GunType type = (GunType)attributes.getValueAsInteger("guntype");
		float rate = attributes.getValueAsFloat("rate");
		float spread = attributes.getValueAsFloat("spread");
		float speed = attributes.getValueAsFloat("speed");
		float damage = attributes.getValueAsFloat("damage");
		float bps = attributes.getValueAsFloat("bps");
		float bulletSize = attributes.getValueAsFloat("bullet_size");
		std::string snd = attributes.getValueAsString("sound").c_str();
		if (!snd.empty()){
			NS2::Sound& sound = NS2::ResourceManager::getSound(snd);
			gun.setSound(sound);
		}

		gun.setGunType(type);
		gun.setLoader(loader);
		gun.setRate(rate);
		gun.setSpeed(speed);
		gun.setSpread(spread);
		gun.setDamage(damage);
		gun.setBPS(bps);
		gun.setBulletSize(bulletSize);
	}
	else if (element == "coffer")
	{
		int index = attributes.getValueAsInteger("index");
		
		Coffer& coffer = m_level.coffers[index];
		getEntity(coffer, attributes);

		float health = attributes.getValueAsFloat("health");
		bool destructible = attributes.getValueAsBool("destructible");

		std::string snd1 = attributes.getValueAsString("sound").c_str();
		if (!snd1.empty()){
			NS2::Sound& sound1 = NS2::ResourceManager::getSound(snd1);
			coffer.setSound(sound1);
		}
		std::string snd2 = attributes.getValueAsString("shoot_sound").c_str();
		if (!snd2.empty()){
			NS2::Sound& sound2 = NS2::ResourceManager::getSound(snd2);
			coffer.setShootSound(sound2);
		}
		coffer.setHealth(health);
		coffer.setDestructible(destructible);
	}
	else if (element == "coffer_entity")
	{
		int parentIndex = attributes.getValueAsInteger("parent_index");
		int index = attributes.getValueAsInteger("entity_index");
		std::string type = attributes.getValueAsString("type").c_str();

		m_level.coffersObject[parentIndex][index] = m_level.getCofferModeByName(type);
	}
	else if (element == "light")
	{
		int index = attributes.getValueAsInteger("index");
		int type = attributes.getValueAsInteger("type");
		float x = attributes.getValueAsFloat("x");
		float y = attributes.getValueAsFloat("y");
		float z = attributes.getValueAsFloat("z");
		float w = attributes.getValueAsFloat("w");
		float dirX = attributes.getValueAsFloat("dir_x");
		float dirY = attributes.getValueAsFloat("dir_y");
		float percent = attributes.getValueAsFloat("percent");
		bool on = attributes.getValueAsBool("on");
		bool broken = attributes.getValueAsBool("broken_down");
		std::string snd = attributes.getValueAsString("sound").c_str();
		if (!snd.empty()){
			NS2::Sound& sound = NS2::ResourceManager::getSound(snd);
			m_level.lights[index].effect = sound;
		}
		NS2::ColorRGBA8 color;
		getColor(color, attributes);

		m_level.lights[index].init(x, y, z, index, color, (LightType)type, on, broken, percent);
		m_level.lights[index].direction.x = dirX;
		m_level.lights[index].direction.y = dirY;
	}
	else if (element == "node")
	{
		int index = attributes.getValueAsInteger("index");
		std::string type = attributes.getValueAsString("type").c_str();
		float x = attributes.getValueAsFloat("x");
		float y = attributes.getValueAsFloat("y");
		float z = attributes.getValueAsFloat("z");
		float w = attributes.getValueAsFloat("w");
		float centerX = attributes.getValueAsFloat("center_x");
		float centerY = attributes.getValueAsFloat("center_y");

		m_level.nodes[index].init(index, x, y, z, w, Shape::getTypeByName(type));
		m_level.nodes[index].setCenter(centerX, centerY);
	}
	else if (element == "edge")
	{
		int index = attributes.getValueAsInteger("index");
		int from = attributes.getValueAsInteger("from");
		int to = attributes.getValueAsInteger("to");
		float width = attributes.getValueAsFloat("width");
		float cost = attributes.getValueAsFloat("cost");

		m_level.edges[index].setNode1(from);
		m_level.edges[index].setNode2(to);
		m_level.edges[index].setWidth(width);
		m_level.edges[index].setCost(cost);

		m_level.nodes[from].addNeighbor(to, index);
		m_level.nodes[to].addNeighbor(from, index);
	}
	else if (element == "edge_point")
	{
		int parentIndex = attributes.getValueAsInteger("parent_index");
		float x = attributes.getValueAsFloat("x");
		float y = attributes.getValueAsFloat("y");

		m_level.edges[parentIndex].getPath().push_back(glm::vec2(x, y));
	}
	else
		NS2::logE << "LevelHandler::elementStart: Unknown element encountered: <" << element << std::endl;
}

void EditorHandler::elementEnd(const CEGUI::String& element)
{
	
}

void EditorHandler::getEntity(Entity& e, const CEGUI::XMLAttributes& a)
{
	std::string name = a.getValueAsString("name").c_str();
	std::string shape = a.getValueAsString("shape").c_str();
	std::string texture = a.getValueAsString("texture").c_str();
	float x = a.getValueAsFloat("x");
	float y = a.getValueAsFloat("y");
	float z = a.getValueAsFloat("z");
	float w = a.getValueAsFloat("w");
	float dirX = a.getValueAsFloat("dir_x");
	float dirY = a.getValueAsFloat("dir_y");
	int texX = a.getValueAsInteger("tex_dim_x");
	int texY = a.getValueAsInteger("tex_dim_y");
	float animSpeed = a.getValueAsFloat("anim_speed");
	NS2::ColorRGBA8 color;
	getColor(color, a);
	NS2::GLTexture& tex = NS2::ResourceManager::getTexture(texture);

	e.setName(name);
	e.getShape().type = Shape::getTypeByName(shape);
	e.setTexture(tex, glm::ivec2(texX, texY));
	e.getShape().setDestRect(glm::vec4(x, y, z, w));
	e.setDirection(glm::vec2(dirX, dirY));
	e.setAnimSpeed(animSpeed);
	e.setColor(color);
}
void EditorHandler::getColor(NS2::ColorRGBA8& c, const CEGUI::XMLAttributes& a)
{
	int red = a.getValueAsInteger("red");
	int green = a.getValueAsInteger("green");
	int blue = a.getValueAsInteger("blue");
	int alpha = a.getValueAsInteger("alpha");

	c.setColor(red, green, blue, alpha);
}