#pragma once

#include "Sensor.h"

class TouchSensor : public Sensor
{
public:
	TouchSensor(AI* ai);

	virtual void notifyForTile() override;
	virtual void notifyForZombie() override;
	virtual void notifyForHuman() override;
	virtual void notifyForObject() override;
	virtual void notifyForBullet() override;

protected:
	virtual void updateSensor(Agent* agent, Level& level, float deltaTime) override;
};

