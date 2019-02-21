#pragma once

#include "LevelEditor.h"

#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <CEGUI/XmlHandler.h>

class EditorHandler : public CEGUI::XMLHandler
{
public:
	EditorHandler(LevelEditor& level);

	virtual void elementStart(const CEGUI::String&, const CEGUI::XMLAttributes&) override;
	virtual void elementEnd(const CEGUI::String&) override;
	const CEGUI::String& getSchemaName() const{ return m_schemaName; }
	const CEGUI::String& getDefaultResourceGroup() const{ return m_schemaName; }

private:
	void getEntity(Entity& e, const CEGUI::XMLAttributes& a);
	void getColor(NS2::ColorRGBA8& c, const CEGUI::XMLAttributes& a);

	LevelEditor& m_level;
	CEGUI::String m_schemaName = "";
};

