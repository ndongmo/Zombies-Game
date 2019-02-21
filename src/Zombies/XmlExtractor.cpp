#include "XmlExtractor.h"
#include "LevelHandler.h"
#include "EditorHandler.h"

#include <CEGUI/XMLParserModules/Expat/XMLParser.h>
#include <fstream>

using namespace std;
using namespace CEGUI;
using namespace NS2;

bool XmlExtractor::loadLevel(Level& l, const string& filePath, string& error, NS2::EventHandler* input, NS2::Camera2D* camera)
{
	try{
		LevelHandler levelHandler(l, input, camera);
		ExpatParser parser;
		parser.initialise();
		parser.parseXMLFile(levelHandler, filePath, "xml_schema", "xml_file");
	}
	catch (exception e){
		error = e.what();
		return false;
	}
	return true;
}

bool XmlExtractor::loadLevel(LevelEditor& l, const string& filePath, string& error)
{
	try{
		EditorHandler editorHandler(l);
		ExpatParser parser;
		parser.initialise();
		parser.parseXMLFile(editorHandler, filePath, "xml_schema", "xml_file");
	}
	catch (exception e){
		error = e.what();
		return false;
	}
	return true;
}

bool XmlExtractor::saveLevel(LevelEditor& l, const string& filePath, string& error)
{
	try{
		ofstream myfile;
		myfile.open(filePath);
		XMLSerializer serializer(myfile, 4);

		serializer.openTag("level");
		{
			saveColor(serializer, l.ambientColor);
			serializer
				.attribute("name", l.name)
				.attribute("width", to_string(l.width))
				.attribute("height", to_string(l.height))
				.attribute("music", l.music.getFilePath())
				.attribute("lights", to_string(l.lights.size()))
				.attribute("humans", to_string(l.humans.size()))
				.attribute("zombies", to_string(l.zombies.size()))
				.attribute("coffers", to_string(l.coffers.size()))
				.attribute("keys", to_string(l.keys.size()))
				.attribute("guns", to_string(l.guns.size()))
				.attribute("bags", to_string(l.bags.size()))
				.attribute("ammos", to_string(l.ammos.size()))
				.attribute("flashlights", to_string(l.flashlights.size()))
				.attribute("doors", to_string(l.doors.size()))
				.attribute("medicines", to_string(l.medicines.size()))
				.attribute("nodes", to_string(l.nodes.size()))
				.attribute("edges", to_string(l.edges.size()))
				;

			for (unsigned int i = 0; i < l.tiles.size(); i++)
			{
				auto& t = l.tiles[i];

				serializer.openTag("tile");
				{
					saveColor(serializer, t.getColor());
					serializer
						.attribute("index", to_string(i))
						.attribute("x", to_string(t.getX()))
						.attribute("y", to_string(t.getY()))
						.attribute("crossable", to_string(t.isCrossable()))
						.attribute("texture", t.getTexture().filePath);
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.lights.size(); i++)
			{
				auto& li = l.lights[i];

				serializer.openTag("light");
				{
					saveColor(serializer, li.color);
					serializer
						.attribute("index", to_string(i))
						.attribute("type", to_string(li.type))
						.attribute("x", to_string(li.destRect.x))
						.attribute("y", to_string(li.destRect.y))
						.attribute("z", to_string(li.destRect.z))
						.attribute("w", to_string(li.destRect.w))
						.attribute("dir_x", to_string(li.direction.x))
						.attribute("dir_y", to_string(li.direction.y))
						.attribute("percent", to_string(li.percent))
						.attribute("on", to_string(li.on))
						.attribute("broken_down", to_string(li.brokenDown))
						.attribute("sound", li.effect.getFilePath());
				}
				serializer.closeTag();
			}

			serializer.openTag("player");
			{
				saveEntity(serializer, l.player);
				serializer
					.attribute("speed", to_string(l.player.getSpeed()))
					.attribute("health", to_string(l.player.getHealth()))
					.attribute("src_light", to_string(l.playerSrc))
					.attribute("around_light", to_string(l.playerAround))
					.attribute("diff_light", to_string(l.playerDiff));
			}
			serializer.closeTag();

			for (unsigned int i = 0; i < l.humans.size(); i++)
			{
				auto& h = l.humans[i];

				serializer.openTag("human");
				{
					saveEntity(serializer, h);
					serializer
						.attribute("index", to_string(i))
						.attribute("speed", to_string(h.getSpeed()))
						.attribute("health", to_string(h.getHealth()));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.zombies.size(); i++)
			{
				auto& z = l.zombies[i];

				serializer.openTag("zombie");
				{
					saveEntity(serializer, z);
					serializer
						.attribute("index", to_string(i))
						.attribute("speed", to_string(z.getSpeed()))
						.attribute("health", to_string(z.getHealth()))
						.attribute("damage", to_string(z.getDamage()));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.keys.size(); i++)
			{
				auto& k = l.keys[i];

				serializer.openTag("key");
				{
					saveEntity(serializer, k);
					serializer
						.attribute("index", to_string(i))
						.attribute("sound", k.getSound().getFilePath());
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.doors.size(); i++)
			{
				auto& d = l.doors[i];

				serializer.openTag("door");
				{
					saveEntity(serializer, d);
					serializer
						.attribute("index", to_string(i))
						.attribute("keyname", d.getKeyName())
						.attribute("opened", to_string(d.isOpened()))
						.attribute("sound", d.getSound().getFilePath());
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.medicines.size(); i++)
			{
				auto& m = l.medicines[i];

				serializer.openTag("medicine");
				{
					saveEntity(serializer, m);
					serializer
						.attribute("index", to_string(i))
						.attribute("health", to_string(m.getHealth()))
						.attribute("sound", m.getSound().getFilePath());
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.bags.size(); i++)
			{
				auto& m = l.bags[i];

				serializer.openTag("bag");
				{
					saveEntity(serializer, m);
					serializer
						.attribute("index", to_string(i))
						.attribute("columns", to_string(m.getColumns()))
						.attribute("rows", to_string(m.getRows()));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.ammos.size(); i++)
			{
				auto& m = l.ammos[i];

				serializer.openTag("ammo");
				{
					saveEntity(serializer, m);
					serializer
						.attribute("index", to_string(i))
						.attribute("amount", to_string(m.getAmount()))
						.attribute("guntype", to_string(m.getGunType()));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.flashlights.size(); i++)
			{
				auto& m = l.flashlights[i];

				serializer.openTag("flashlight");
				{
					saveEntity(serializer, m);
					serializer
						.attribute("index", to_string(i))
						.attribute("on", to_string(m.isOn()))
						.attribute("low", to_string(m.getLow()))
						.attribute("duration", to_string(m.getDuration()));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.coffers.size(); i++)
			{
				Coffer& c = l.coffers[i];
				serializer.openTag("coffer");
				{
					saveEntity(serializer, c);
					serializer
						.attribute("index", to_string(i))
						.attribute("objects", to_string(l.coffersObject[i].size()))
						.attribute("destructible", to_string(c.isDestructible()))
						.attribute("health", to_string(c.getHealth()))
						.attribute("shoot_sound", c.getShootSound().getFilePath())
						.attribute("sound", c.getSound().getFilePath());

					for (auto& m : l.coffersObject[i])
					{
						serializer
							.openTag("coffer_entity")
							.attribute("parent_index", to_string(i))
							.attribute("entity_index", to_string(m.first))
							.attribute("type", l.getCofferModeName(m.second))
							.closeTag();
					}
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.guns.size(); i++)
			{
				auto& g = l.guns[i];

				serializer.openTag("gun");
				{
					saveEntity(serializer, g);
					serializer
						.attribute("index", to_string(i))
						.attribute("guntype", to_string(g.getGunType()))
						.attribute("sound", g.getSound().getFilePath())
						.attribute("loader", to_string(g.getLoader()))
						.attribute("rate", to_string(g.getRate()))
						.attribute("spread", to_string(g.getSpread()))
						.attribute("speed", to_string(g.getSpeed()))
						.attribute("damage", to_string(g.getDamage()))
						.attribute("bps", to_string(g.getBPS()))
						.attribute("bullet_size", to_string(g.getBulletSize()));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.nodes.size(); i++)
			{
				auto& n = l.nodes[i];

				serializer.openTag("node");
				{
					serializer
						.attribute("index", to_string(i))
						.attribute("type", n.getShape().getTypeName())
						.attribute("x", to_string(n.getShape().destRect.x))
						.attribute("y", to_string(n.getShape().destRect.y))
						.attribute("z", to_string(n.getShape().destRect.z))
						.attribute("w", to_string(n.getShape().destRect.w))
						.attribute("center_x", to_string(n.getCenter().x))
						.attribute("center_y", to_string(n.getCenter().y));
				}
				serializer.closeTag();
			}

			for (unsigned int i = 0; i < l.edges.size(); i++)
			{
				auto& e = l.edges[i];

				serializer.openTag("edge")
					.attribute("index", to_string(i))
					.attribute("from", to_string(e.getNode1()))
					.attribute("to", to_string(e.getNode2()))
					.attribute("width", to_string(e.getWidth()))
					.attribute("cost", to_string(e.getCost()));
				{
					for (auto& p : e.getPath())
					{
						serializer.openTag("edge_point")
							.attribute("parent_index", to_string(i))
							.attribute("x", to_string(p.x))
							.attribute("y", to_string(p.y))
							.closeTag();
					}
				}
				serializer.closeTag();
			}
		}
		serializer.closeTag();
		myfile.close();
		if (!serializer)
		{
			error = "Unknow error occured";
			return false;
		}
	}
	catch (exception e){
		error = e.what();
		return false;
	}
	return true;
}

void XmlExtractor::saveColor(XMLSerializer& serializer, ColorRGBA8& color)
{
	serializer
		.attribute("red", to_string(color.r))
		.attribute("green", to_string(color.g))
		.attribute("blue", to_string(color.b))
		.attribute("alpha", to_string(color.a))
		;
}
void XmlExtractor::saveEntity(XMLSerializer& serializer, Entity& e)
{
	serializer
		.attribute("name", e.getName())
		.attribute("x", to_string(e.getShape().getLX()))
		.attribute("y", to_string(e.getShape().getBY()))
		.attribute("z", to_string(e.getShape().destRect.z))
		.attribute("w", to_string(e.getShape().destRect.w))
		.attribute("shape", e.getShape().getTypeName())
		.attribute("anim_speed", to_string(e.getAnimSpeed()))
		.attribute("dir_x", to_string(e.getDirection().x))
		.attribute("dir_y", to_string(e.getDirection().y))
		.attribute("texture", e.getTexture().texture.filePath)
		.attribute("tex_dim_x", to_string(e.getTexture().dims.x))
		.attribute("tex_dim_y", to_string(e.getTexture().dims.y));
	saveColor(serializer, e.getColor());
}