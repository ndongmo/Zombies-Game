#pragma once

#include "Level.h"

#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <CEGUI/XmlHandler.h>

class LevelHandler : public CEGUI::XMLHandler
{
public:
	LevelHandler(Level& level, NS2::EventHandler* input, NS2::Camera2D* camera);

	virtual void elementStart(const CEGUI::String&, const CEGUI::XMLAttributes&) override;
	virtual void elementEnd(const CEGUI::String&) override;
	const CEGUI::String& getSchemaName() const{ return m_schemaName; }
	const CEGUI::String& getDefaultResourceGroup() const{ return m_schemaName; }

private:
	void getEntity(Entity& e, const CEGUI::XMLAttributes& a);
	void getColor(NS2::ColorRGBA8& c, const CEGUI::XMLAttributes& a);

	Level& m_level;
	CEGUI::String m_schemaName = "";
	NS2::Camera2D* m_camera;
	NS2::EventHandler* m_input;
};

