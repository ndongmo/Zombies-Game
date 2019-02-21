#include "TouchSensor.h"
#include "Level.h"

TouchSensor::TouchSensor(AI* ai) : Sensor(ai)
{
}

void TouchSensor::updateSensor(Agent* agent, Level& level, float deltaTime)
{

}

void TouchSensor::notifyForTile()
{
	m_ai->touchNotifyForTile();
}
void TouchSensor::notifyForZombie()
{
	m_ai->touchNotifyForZombie();
}
void TouchSensor::notifyForHuman()
{
	m_ai->touchNotifyForHuman();
}
void TouchSensor::notifyForObject()
{
	m_ai->touchNotifyForObject();
}

void TouchSensor::notifyForBullet()
{
	m_ai->touchNotifyForBullet();
}