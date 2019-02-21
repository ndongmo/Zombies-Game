#include "LevelHandler.h"
#include "LevelEditor.h"

#include <Logger.h>
#include <CEGUI/XMLAttributes.h>

LevelHandler::LevelHandler(Level& level, NS2::EventHandler* input, NS2::Camera2D* camera) : 
m_level(level),
m_camera(camera),
m_input(input)
{
	level.clear();
}

void LevelHandler::elementStart(const CEGUI::String& element, const CEGUI::XMLAttributes& attributes)
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
		int bags = attributes.getValueAsInteger("bags");
		int ammos = attributes.getValueAsInteger("ammos");
		int flashlights = attributes.getValueAsInteger("flashlights");
		int medicines = attributes.getValueAsInteger("medicines");
		int nodes = attributes.getValueAsInteger("nodes");
		int edges = attributes.getValueAsInteger("edges");
		NS2::ColorRGBA8 color;
		getColor(color, attributes);

		m_level.init(
			width,
			height,
			nodes,
			edges,
			lights,
			humans,
			zombies,
			coffers,
			keys,
			guns,
			doors,
			medicines,
			bags,
			ammos,
			flashlights,
			name,
			color,
			NS2::ResourceManager::getMusic(music),
			m_input,
			m_camera
		);
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
		m_level.player = Player();
		Player& player = m_level.player;
		PlayerAI& ai = m_level.playerAi;
		ai.init(&player, m_input, m_camera);

		getEntity(player, attributes);

		float speed = attributes.getValueAsFloat("speed");
		float health = attributes.getValueAsFloat("health");
		int src = attributes.getValueAsInteger("src_light");
		int around = attributes.getValueAsInteger("around_light");
		int diff = attributes.getValueAsInteger("diff_light");
		bool lightOn = attributes.getValueAsBool("light_on");

		player.setAI(&ai);
		player.setSpeed(speed);
		player.setHealth(health);
		if (src > -1) player.setSourceLight(&m_level.lights[src]);
		if (diff > -1) player.setDestLight(&m_level.lights[diff]);
		if (around > -1) player.setDiffLight(&m_level.lights[around]);
		player.endInit();
	}
	else if (element == "human")
	{
		int index = attributes.getValueAsInteger("index");
		Human& human = m_level.humans[index];
		HumanAI& ai = m_level.humansAis[index];
		ai.init(&human);

		getEntity(human, attributes);

		float speed = attributes.getValueAsFloat("speed");
		float health = attributes.getValueAsFloat("health");

		human.setAI(&ai);
		human.setSpeed(speed);
		human.setHealth(health);
	}
	else if (element == "zombie")
	{
		int index = attributes.getValueAsInteger("index");
		Zombie& zombie = m_level.zombies[index];
		ZombieAI& ai = m_level.zombieAis[index];
		ai.init(&zombie);

		getEntity(zombie, attributes);

		float speed = attributes.getValueAsFloat("speed");
		float health = attributes.getValueAsFloat("health");
		float damage = attributes.getValueAsFloat("damage");

		zombie.setAI(&ai);
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
	else if (element == "bag")
	{
		int index = attributes.getValueAsInteger("index");
		Bag& bag = m_level.bags[index];
		getEntity(bag, attributes);

		int cols = attributes.getValueAsInteger("columns");
		int rows = attributes.getValueAsInteger("rows");
		bag.setColumns(cols);
		bag.setRows(rows);
	}
	else if (element == "ammo")
	{
		int index = attributes.getValueAsInteger("index");
		Ammo& ammo = m_level.ammos[index];
		getEntity(ammo, attributes);

		int amount = attributes.getValueAsInteger("amount");
		GunType guntype = (GunType)attributes.getValueAsInteger("guntype");
		ammo.setAmount(amount);
		ammo.setGunType(guntype);
	}
	else if (element == "flashlight")
	{
		int index = attributes.getValueAsInteger("index");
		Flashlight& flashlight = m_level.flashlights[index];
		getEntity(flashlight, attributes);

		float low = attributes.getValueAsFloat("low");
		float duration = attributes.getValueAsFloat("duration");
		bool on = attributes.getValueAsBool("on");

		flashlight.setOn(on);
		flashlight.setLow(low);
		flashlight.setDuration(duration);
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
		CofferMode mode = LevelEditor::getCofferModeByName(type);
		index = LevelEditor::cofferObjectIndex(mode, index);
		Object* e = nullptr;
		if (mode == CofferMode::AMMO)
			e = &m_level.ammos[index];
		else if(mode == CofferMode::BAG)
			e = &m_level.bags[index];
		else if(mode == CofferMode::FLASHLIGHT)
			e = &m_level.flashlights[index];
		else if (mode == CofferMode::GUN)
			e = &m_level.guns[index];
		else if (mode == CofferMode::KEY)
			e = &m_level.keys[index];
		else if (mode == CofferMode::MEDICINE)
			e = &m_level.medicines[index];

		m_level.coffers[parentIndex].add(e);
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
		m_level.nodes[index].setIndex(index);
		m_level.addNode(m_level.nodes[index]);
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
		m_level.edges[index].setIndex(index);

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

void LevelHandler::elementEnd(const CEGUI::String& element)
{

}

void LevelHandler::getEntity(Entity& e, const CEGUI::XMLAttributes& a)
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

	m_level.addToWorld(&e);
}
void LevelHandler::getColor(NS2::ColorRGBA8& c, const CEGUI::XMLAttributes& a)
{
	int red = a.getValueAsInteger("red");
	int green = a.getValueAsInteger("green");
	int blue = a.getValueAsInteger("blue");
	int alpha = a.getValueAsInteger("alpha");

	c.setColor(red, green, blue, alpha);
}