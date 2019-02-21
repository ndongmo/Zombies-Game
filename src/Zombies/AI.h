#pragma once

#include "SightSensor.h"
#include "TouchSensor.h"
#include "MoveBehavior.h"

#include <string>

class AI
{
public:
	AI() : m_touch(this), m_sight(this){}
	virtual void update(Level& level, float deltaTime) = 0;
	const std::string& getName(){ return m_name; }

	virtual void manageSensors(){}
	virtual void sightNotifyForTile() {}
	virtual void sightNotifyForZombie() {}
	virtual void sightNotifyForHuman() {}
	virtual void sightNotifyForObject() {}
	virtual void sightNotifyForBullet() {}
	virtual void touchNotifyForTile() {}
	virtual void touchNotifyForZombie() {}
	virtual void touchNotifyForHuman() {}
	virtual void touchNotifyForObject() {}
	virtual void touchNotifyForBullet() {}

	/* GETTERS */
	SightSensor& getSightSensor(){ return m_sight; }
	TouchSensor& getTouchSensor(){ return m_touch; }

protected:
	std::string m_name;
	SightSensor m_sight;
	TouchSensor m_touch;
};

