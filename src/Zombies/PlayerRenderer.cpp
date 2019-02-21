#include "PlayerRenderer.h"
#include "EventConfig.h"

#include <ResourceManager.h>

PlayerRenderer::PlayerRenderer() : m_tmpRect(4)
{

}
void PlayerRenderer::init(float screenWidth, float screenHeight)
{
	m_fontColor.setColor(0, 0, 0, 255);
	m_dialogFontColor.setColor(0, 0, 0, 255);
	m_color.setColor(255, 255, 255, 255);
	m_dialogColor.setColor(255, 255, 255, 128);
	m_staminaLowColor.setColor(255, 0, 0, 128);

	m_staminaBack = NS2::ResourceManager::getTexture("Assets/staminaBarBackground.png");
	m_playerBack = NS2::ResourceManager::getTexture("Assets/playerBackground_1.png");
	m_lifeBack = NS2::ResourceManager::getTexture("Assets/lifeBarBackground.png");
	m_dialogBack = NS2::ResourceManager::getTexture("Assets/dialog.png");
	m_lifeFront = NS2::ResourceManager::getTexture("Assets/lifeBarForeground.png");
	m_staminaFront = NS2::ResourceManager::getTexture("Assets/staminaBarForeground.png");
	m_stuffBack = NS2::ResourceManager::getTexture("Assets/stuffBackground.png");
	m_moneyBack = NS2::ResourceManager::getTexture("Assets/moneyBackground.png");
	m_spriteFont = NS2::ResourceManager::getFont("Fonts/chintzy.ttf", 12);
	m_smallFont = NS2::ResourceManager::getFont("Fonts/chintzy.ttf", 9);
	m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	float margin = 10.0f;
	float width = screenWidth * 0.20f;
	float heigh = screenHeight * 0.15f;
	float x = (screenWidth - width) / 2.0f;
	float y = (screenHeight - heigh) / 2.0f;

	m_dialogRect = glm::vec4(x, y, width, heigh);
	
	width = screenWidth * 0.04f;
	heigh = screenHeight * 0.05f;
	x = margin;
	y = screenHeight - margin - heigh;

	m_playerRect = glm::vec4(x, y, width, heigh);

	float tmp = screenHeight * 0.02f;
	x = x + width;
	y = y + (heigh / 2.0f) - tmp;
	width = screenWidth * 0.2f;
	heigh = tmp;
	m_staminaRect = glm::vec4(x, y, width, heigh);

	y = y + heigh;
	m_lifeRect = glm::vec4(x, y, width, heigh);

	width = screenWidth * 0.04f;
	heigh = screenHeight * 0.05f;
	x = margin;
	y = (screenWidth - heigh) / 2.0f;
	m_leftStuffRect = glm::vec4(x, y, width, heigh);

	x = x + width + (margin * 2);
	m_rightStuffRect = glm::vec4(x, y, width, heigh);

	x = (x + margin) / 2.0f;
	y = y + heigh + margin;
	m_topStuffRect = glm::vec4(x, y, width, heigh);

	x = margin;
	y = margin;
	width = screenWidth * 0.2f;
	heigh = screenHeight * 0.04f;
	m_moneyRect = glm::vec4(x, y, width, heigh);
}
void PlayerRenderer::render(NS2::SpriteBatch& spriteBatch, Level& level, const glm::vec4& box)
{
	static char buffer[50];
	Player& p = level.getPlayer();

	if (level.isDark())
		m_fontColor.setColor(255, 255, 255, 255);
	else
		m_fontColor.setColor(0, 0, 0, 255);

	updatePoints(m_playerRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_playerBack.id, 0.0f, m_color);
	updatePoints(m_staminaRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_staminaBack.id, 0.0f, m_color);
	updatePoints(m_lifeRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_lifeBack.id, 0.0f, m_color);
	updatePoints(m_leftStuffRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_stuffBack.id, 0.0f, m_color);
	updatePoints(m_rightStuffRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_stuffBack.id, 0.0f, m_color);
	updatePoints(m_topStuffRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_stuffBack.id, 0.0f, m_color);
	updatePoints(m_staminaRect, box, level.player.getCurrentStamina() / level.player.getStamina());
	if (level.player.getPenality())
		spriteBatch.draw(m_tmpRect, m_uvRect, m_staminaFront.id, 0.0f, m_staminaLowColor);
	else
		spriteBatch.draw(m_tmpRect, m_uvRect, m_staminaFront.id, 0.0f, m_color);
	updatePoints(m_lifeRect, box, level.player.getHealth() / level.player.getInitHealth());
	spriteBatch.draw(m_tmpRect, m_uvRect, m_lifeFront.id, 0.0f, m_color);
	updatePoints(m_moneyRect, box);
	spriteBatch.draw(m_tmpRect, m_uvRect, m_moneyBack.id, 0.0f, m_color);

	sprintf_s(buffer, "H : %d   Z : %d", level.nbHumans, level.nbZombies);
	m_tmpRect[0].x += m_moneyRect.z / 2.0f;
	m_tmpRect[0].y += m_moneyRect.w / 2.0f;
	m_tmpRect[1].x = 1.0f;
	m_tmpRect[1].y = 1.0f;
	m_spriteFont.draw(spriteBatch, buffer, m_tmpRect[0],
		m_tmpRect[1], 0.0f, m_fontColor, NS2::Justification::MIDDLE);

	if (p.geEquipAmmo() != nullptr) {
		updatePoints(m_leftStuffRect, box);
		spriteBatch.draw(m_tmpRect, m_uvRect, p.geEquipAmmo()->getTexture().texture.id, 0.0f, m_color);
		m_tmpRect[0].y -= 10;
		m_tmpRect[1].x = 1.0f;
		m_tmpRect[1].y = 1.0f;
		sprintf_s(buffer, "%d", p.geEquipAmmo()->getAmount());
		m_smallFont.draw(spriteBatch, buffer, m_tmpRect[0],
			m_tmpRect[1], 0.0f, m_fontColor, NS2::Justification::LEFT);
	}
	if (p.getEquipWeapon() != nullptr) {
		updatePoints(m_rightStuffRect, box);
		spriteBatch.draw(m_tmpRect, m_uvRect, p.getEquipWeapon()->getTexture().texture.id, 0.0f, m_color);
		m_tmpRect[0].x += 5;
		m_tmpRect[0].y -= 5;
		m_tmpRect[1].x = 1.0f;
		m_tmpRect[1].y = 1.0f;
		sprintf_s(buffer, "%d", p.getEquipWeapon()->getCurrentLoader());
		m_smallFont.draw(spriteBatch, buffer, m_tmpRect[0],
			m_tmpRect[1], 0.0f, m_fontColor, NS2::Justification::MIDDLE);
	}
	if (p.getEquipObject() != nullptr) {
		std::string str = "";
		p.getEquipObject()->textValue(str);
		updatePoints(m_topStuffRect, box);
		spriteBatch.draw(m_tmpRect, m_uvRect, p.getEquipObject()->getTexture().texture.id, 0.0f, m_color);
		if (!str.empty())
		{
			m_tmpRect[0].y += m_topStuffRect.w + 10;
			m_tmpRect[1].x = 1.0f;
			m_tmpRect[1].y = 1.0f;
			sprintf_s(buffer, "%s", str.c_str());
			m_smallFont.draw(spriteBatch, buffer, m_tmpRect[0],
				m_tmpRect[1], 0.0f, m_fontColor, NS2::Justification::MIDDLE);
		}
	}
	if (p.getInterAction() != nullptr)
	{
		std::string str = "";
		p.getInterAction()->textAction(&p, str);
		if (str.empty()) return;

		std::string strAction = "";
		strAction = level.input->getMapping(MyEvent::ACTION);

		glm::vec2& pos = p.getInterAction()->getShape().getTL();
		glm::vec2& mes1 = m_spriteFont.measure(str.c_str());
		glm::vec2& mes2 = m_spriteFont.measure(strAction.c_str());

		m_dialogRect.x = pos.x;
		m_dialogRect.y = pos.y;
		m_dialogRect.z = (mes1.x > mes2.x) ? mes1.x + 60 : mes2.x + 60;
		m_dialogRect.w = mes1.y + mes2.y + 40;
		spriteBatch.draw(m_dialogRect, m_uvRect, m_dialogBack.id, 0.0f, m_dialogColor);

		m_tmpRect[0].x = m_dialogRect.x  + (m_dialogRect.z / 2.0f);
		m_tmpRect[0].y = 10 + m_dialogRect.y + (m_dialogRect.w / 2.0f);
		m_tmpRect[1].x = 1.0f;
		m_tmpRect[1].y = 1.0f;
		
		sprintf_s(buffer, "%s", str.c_str());
		m_spriteFont.draw(spriteBatch, buffer, m_tmpRect[0],
			m_tmpRect[1], 0.0f, m_dialogFontColor, NS2::Justification::MIDDLE);

		m_tmpRect[0].y -= 20;

		sprintf_s(buffer, "Press %s", strAction.c_str());
		m_spriteFont.draw(spriteBatch, buffer, m_tmpRect[0],
			m_tmpRect[1], 0.0f, m_dialogFontColor, NS2::Justification::MIDDLE);
	}
}

void PlayerRenderer::updatePoints(const glm::vec4& points, const glm::vec4& box)
{
	m_tmpRect[0].x = box.x + points.x;
	m_tmpRect[0].y = box.y + points.y;
	m_tmpRect[1].x = box.x + points.x + points.z;
	m_tmpRect[1].y = box.y + points.y;
	m_tmpRect[2].x = box.x + points.x + points.z;
	m_tmpRect[2].y = box.y + points.y + points.w;
	m_tmpRect[3].x = box.x + points.x;
	m_tmpRect[3].y = box.y + points.y + points.w;
}
void PlayerRenderer::updatePoints(const glm::vec4& points, const glm::vec4& box, float epsilon)
{
	m_tmpRect[0].x = box.x + points.x;
	m_tmpRect[0].y = box.y + points.y;
	m_tmpRect[1].x = box.x + points.x + (points.z * epsilon);
	m_tmpRect[1].y = box.y + points.y;
	m_tmpRect[2].x = box.x + points.x + (points.z * epsilon);
	m_tmpRect[2].y = box.y + points.y + points.w;
	m_tmpRect[3].x = box.x + points.x;
	m_tmpRect[3].y = box.y + points.y + points.w;
}