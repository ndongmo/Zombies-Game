#pragma once

#include "LevelEditor.h"

#include <string>
#include <CEGUI/XMLSerializer.h>
#include <Vertex.h>
#include <Camera2D.h>
#include <EventHandler.h>

class XmlExtractor
{
public:
	static bool loadLevel(Level& l, const std::string& filePath, std::string& error, NS2::EventHandler* input, NS2::Camera2D* camera);
	static bool loadLevel(LevelEditor& level, const std::string& filePath, std::string& error);
	static bool saveLevel(LevelEditor& level, const std::string& filePath, std::string& error);
private:
	static void saveColor(CEGUI::XMLSerializer& serializer, NS2::ColorRGBA8& color);
	static void saveEntity(CEGUI::XMLSerializer& serializer, Entity& e);
};

