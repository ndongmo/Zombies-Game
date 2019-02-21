#include "GameRenderer.h"
#include "Level.h"

#include <ImageLoader.h>

/*************************************Game Renderer*********************************************************/
void GameRenderer::init(float screenWidth, float screenHeight)
{
	m_debugColor = NS2::ColorRGBA8(255, 0, 0, 255);
	m_lineWidth = 2.0f;
	m_debug = false;

	// Compile color shader
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	// Compile light shader
	m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();

	m_spriteBatch.init();
	m_lightSpriteBatch.init();
	m_fontSpriteBatch.init();
	m_debugRenderer.init();

	m_lightMap = NS2::ImageLoader::create(screenWidth, screenHeight);

	m_entityRenderer.init();
	m_playerRenderer.init(screenWidth, screenHeight);
}

void GameRenderer::dispose()
{
	m_textureProgram.dispose();
	m_lightProgram.dispose();
	m_spriteBatch.dispose();
	m_lightSpriteBatch.dispose();
	m_fontSpriteBatch.dispose();
	m_debugRenderer.dispose();
}

void GameRenderer::clearAmbientColor(const NS2::ColorRGBA8& color)
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}

void GameRenderer::render(Level& level, NS2::Camera2D& camera, NS2::Particle2DEngine& particleEngine)
{
	glm::vec4 box;
	camera.setScreenBox(box);

	clearAmbientColor(level.ambientColor);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_lightProgram.use();
	{
		GLint cmLocation = m_lightProgram.getUniformLocation("cameraMatrix");
		glm::mat4 cameraMatrix = camera.getCameraMatrix();
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		m_spriteBatch.begin();
		{
			m_lightRenderer.render(m_spriteBatch, camera, level.lights);
		}
		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
	}
	m_lightProgram.unuse();

	glBindTexture(GL_TEXTURE_2D, m_lightMap.id);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_lightMap.width, m_lightMap.height);
	glClear(GL_COLOR_BUFFER_BIT);

	m_textureProgram.use();
	{
		glActiveTexture(GL_TEXTURE0);

		// Make sure the shader uses texture 0
		GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
		glUniform1i(textureLocation, 0);

		// set the camera matrix
		GLint cmLocation = m_textureProgram.getUniformLocation("cameraMatrix");
		glm::mat4 cameraMatrix = camera.getCameraMatrix();
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		// Begin drawing entities
		m_spriteBatch.begin();
		{
			//m_mapRenderer.render(m_spriteBatch, camera, level.tiles);
			m_entityRenderer.render(m_spriteBatch, camera, level);
		}
		m_spriteBatch.end();
		m_spriteBatch.renderBatch();

	}
	m_textureProgram.unuse();

	glBlendFunc(GL_DST_COLOR, GL_ZERO);

	m_textureProgram.use();
	{
		glActiveTexture(GL_TEXTURE0);

		// Make sure the shader uses texture 0
		GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
		glUniform1i(textureLocation, 0);

		// set the camera matrix
		GLint cmLocation = m_textureProgram.getUniformLocation("cameraMatrix");
		glm::mat4 cameraMatrix = camera.getCameraMatrix();
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		// Begin drawing entities
		m_spriteBatch.begin();
		{
			
			const glm::vec4 uv(0, 0, 1, -1);
			const NS2::ColorRGBA8 color(255, 255, 255, 255);
			m_spriteBatch.draw(box, uv, m_lightMap.id, 0, color);
		}
		m_spriteBatch.end();
		m_spriteBatch.renderBatch();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_spriteBatch.begin();
		{
			// Draw player stuff
			m_playerRenderer.render(m_spriteBatch, level, box);
		}
		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
		particleEngine.draw(m_spriteBatch);
	}
	m_textureProgram.unuse();

	// Draw map
	m_mapRenderer.render(m_debugRenderer, camera, level);

	if (m_debug)
	{
		//m_tileRenderer.render(m_debugRenderer, camera, level.tiles);
		//m_gridRenderer.render(m_debugRenderer, camera, level.grid);
		m_entityRenderer.render(m_debugRenderer, camera, level.entities);
		m_aiRenderer.render(m_debugRenderer, camera, level.entities);
		m_graphRenderer.render(m_debugRenderer, camera, level.nodes, level.edges);
	}
	m_debugRenderer.end();
	m_debugRenderer.render(camera.getCameraMatrix(), m_lineWidth);
}

void GameRenderer::render(LevelEditor& levelEditor, NS2::Camera2D& camera, bool isMouseInMap)
{
	glClearColor(levelEditor.ambientColor.r / 255.0f, levelEditor.ambientColor.g / 255.0f,
		levelEditor.ambientColor.b / 255.0f, levelEditor.ambientColor.a / 255.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_lightProgram.use();
	{
		GLint cmLocation = m_textureProgram.getUniformLocation("cameraMatrix");
		glm::mat4 cameraMatrix = camera.getCameraMatrix();
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		m_spriteBatch.begin();
		{
			if (levelEditor.selectionMode == SelectionMode::PLACE &&
				levelEditor.objectMode == ObjectMode::LIGHT && isMouseInMap)
			{
				levelEditor.light.draw(m_spriteBatch);
			}

			m_lightRenderer.render(m_spriteBatch, camera, levelEditor.lights);
		}
		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
	}
	m_lightProgram.unuse();

	// Additive blending : Destination alpha will be always the same
	glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

	m_textureProgram.use();
	{
		glActiveTexture(GL_TEXTURE0);

		// Make sure the shader uses texture 0
		GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
		glUniform1i(textureLocation, 0);

		// set the camera matrix
		GLint cmLocation = m_textureProgram.getUniformLocation("cameraMatrix");
		glm::mat4 cameraMatrix = camera.getCameraMatrix();
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		// Begin drawing entities
		m_spriteBatch.begin();
		{
			if (levelEditor.selectionMode == SelectionMode::PLACE && isMouseInMap)
			{
				if (levelEditor.objectMode == ObjectMode::HUMAN)
				{
					levelEditor.human.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::ZOMBIE)
				{
					levelEditor.zombie.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::COFFER)
				{
					levelEditor.coffer.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::KEY)
				{
					levelEditor.key.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::GUN)
				{
					levelEditor.gun.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::DOOR)
				{
					levelEditor.door.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::BAG)
				{
					levelEditor.bag.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::AMMO)
				{
					levelEditor.ammo.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::FLASHLIGHT)
				{
					levelEditor.flashlight.draw(m_spriteBatch);
				}
				else if (levelEditor.objectMode == ObjectMode::MEDICINE)
				{
					levelEditor.medicine.draw(m_spriteBatch);
				}
			}

			if (levelEditor.objectMode == ObjectMode::TILE && isMouseInMap &&
				((levelEditor.selectionMode == SelectionMode::PLACE) ||
				(levelEditor.selectionMode == SelectionMode::SELECT && levelEditor.tileSelected)))
			{
				levelEditor.tile.draw(m_spriteBatch);
			}

			levelEditor.player.draw(m_spriteBatch);
			m_tileRenderer.render(m_spriteBatch, camera, levelEditor.tiles);
			m_entityRenderer.render(m_spriteBatch, camera, levelEditor);
		}
		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
	}
	m_textureProgram.unuse();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_debug)
	{
		if (levelEditor.objectMode == ObjectMode::TILE) {
			m_tileRenderer.render(m_debugRenderer, camera, levelEditor.tiles);
		}
		else if (levelEditor.objectMode == ObjectMode::LIGHT) {
			m_lightRenderer.render(m_debugRenderer, camera, levelEditor.lights);
		}
		else if (levelEditor.objectMode == ObjectMode::NODE) {
			m_graphRenderer.render(m_debugRenderer, camera, levelEditor.nodes, levelEditor.edges);
		}
		else {
			m_entityRenderer.render(m_debugRenderer, camera, levelEditor);
		}
	}

	if (levelEditor.selectionMode == SelectionMode::PLACE && isMouseInMap)
	{
		if (levelEditor.objectMode == ObjectMode::TILE) {
			m_debugRenderer.drawBox(levelEditor.tile.getDestRect(), m_debugColor, 0.0f);
		}
		else if (levelEditor.objectMode == ObjectMode::LIGHT) {
			const float radius = levelEditor.light.destRect.z / 2.0f;
			const glm::vec2 center(levelEditor.light.destRect.x + radius, levelEditor.light.destRect.y + radius);
			m_debugRenderer.drawCircle(center, m_debugColor, radius);
		}
		else if (levelEditor.objectMode == ObjectMode::NODE)
		{
			levelEditor.node.getShape().draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::HUMAN) {
			levelEditor.human.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::ZOMBIE) {
			levelEditor.zombie.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::COFFER) {
			levelEditor.coffer.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::KEY) {
			levelEditor.key.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::GUN) {
			levelEditor.gun.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::DOOR) {
			levelEditor.door.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::BAG) {
			levelEditor.bag.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::AMMO) {
			levelEditor.ammo.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::FLASHLIGHT) {
			levelEditor.flashlight.draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::MEDICINE) {
			levelEditor.medicine.draw(m_debugRenderer, m_debugColor);
		}
	}
	else if (levelEditor.selectionMode == SelectionMode::SELECT)
	{
		if (levelEditor.objectMode == ObjectMode::TILE && levelEditor.tileSelected) {
			m_debugRenderer.drawBox(levelEditor.tile.getDestRect(), m_debugColor, 0.0f);
		}
		else if (levelEditor.objectMode == ObjectMode::LIGHT && levelEditor.indexLight != NO_SELECT) {
			const float radius = levelEditor.lights[levelEditor.indexLight].destRect.z / 2.0f;
			const glm::vec2 center(levelEditor.lights[levelEditor.indexLight].destRect.x + radius,
				levelEditor.lights[levelEditor.indexLight].destRect.y + radius);
			m_debugRenderer.drawCircle(center, m_debugColor, radius);
		}
		else if (levelEditor.objectMode == ObjectMode::NODE && levelEditor.indexNode != NO_SELECT)
		{
			levelEditor.nodes[levelEditor.indexNode].getShape().draw(m_debugRenderer, m_debugColor);
			if (levelEditor.nodeMode == NodeMode::EDGE)
			{
				levelEditor.edge.draw(m_debugRenderer, m_debugColor);
			}
		}
		else if (levelEditor.objectMode == ObjectMode::HUMAN && levelEditor.indexHuman != NO_SELECT) {
			levelEditor.humans[levelEditor.indexHuman].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::ZOMBIE && levelEditor.indexZombie != NO_SELECT) {
			levelEditor.zombies[levelEditor.indexZombie].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::KEY && levelEditor.indexKey != NO_SELECT) {
			levelEditor.keys[levelEditor.indexKey].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::GUN && levelEditor.indexGun != NO_SELECT) {
			levelEditor.guns[levelEditor.indexGun].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::DOOR && levelEditor.indexDoor != NO_SELECT) {
			levelEditor.doors[levelEditor.indexDoor].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::BAG && levelEditor.indexBag != NO_SELECT) {
			levelEditor.bags[levelEditor.indexBag].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::AMMO && levelEditor.indexAmmo != NO_SELECT) {
			levelEditor.ammos[levelEditor.indexAmmo].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::FLASHLIGHT && levelEditor.indexFlashlight != NO_SELECT) {
			levelEditor.flashlights[levelEditor.indexFlashlight].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::MEDICINE && levelEditor.indexMedoc != NO_SELECT) {
			levelEditor.medicines[levelEditor.indexMedoc].draw(m_debugRenderer, m_debugColor);
		}
		else if (levelEditor.objectMode == ObjectMode::COFFER && levelEditor.indexCoffer != NO_SELECT) {
			levelEditor.coffers[levelEditor.indexCoffer].draw(m_debugRenderer, m_debugColor);
			NS2::ColorRGBA8 color(m_debugColor);
			color.a = 128;
			for (auto& m : levelEditor.coffersObject[levelEditor.indexCoffer])
			{
				if (m.second == CofferMode::AMMO) {
					int index = m.first - (CofferMode::AMMO * COFFER_OBJECT_OFFSET);
					levelEditor.ammos[index].draw(m_debugRenderer, color);
				}
				if (m.second == CofferMode::BAG) {
					int index = m.first - (CofferMode::BAG * COFFER_OBJECT_OFFSET);
					levelEditor.bags[index].draw(m_debugRenderer, color);
				}
				if (m.second == CofferMode::FLASHLIGHT) {
					int index = m.first - (CofferMode::FLASHLIGHT * COFFER_OBJECT_OFFSET);
					levelEditor.flashlights[index].draw(m_debugRenderer, color);
				}
				else if (m.second == CofferMode::GUN) {
					int index = m.first - (CofferMode::GUN * COFFER_OBJECT_OFFSET);
					levelEditor.guns[index].draw(m_debugRenderer, color);
				}
				else if (m.second == CofferMode::KEY) {
					int index = m.first - (CofferMode::KEY * COFFER_OBJECT_OFFSET);
					levelEditor.keys[index].draw(m_debugRenderer, color);
				}
				else if (m.second == CofferMode::MEDICINE) {
					int index = m.first - (CofferMode::MEDICINE * COFFER_OBJECT_OFFSET);
					levelEditor.medicines[index].draw(m_debugRenderer, color);
				}
			}
		}
	}

	if (levelEditor.objectMode == ObjectMode::PLAYER) {
		levelEditor.player.draw(m_debugRenderer, m_debugColor);
		NS2::ColorRGBA8 color(m_debugColor);
		color.a = 128;

		if (levelEditor.playerGun1 != NO_SELECT)
			levelEditor.guns[levelEditor.playerGun1].draw(m_debugRenderer, color);
		if (levelEditor.playerGun2 != NO_SELECT)
			levelEditor.guns[levelEditor.playerGun2].draw(m_debugRenderer, color);
		if (levelEditor.playerSrc != NO_SELECT)
		{
			const float radius = levelEditor.lights[levelEditor.playerSrc].destRect.z / 2.0f;
			const glm::vec2 center(levelEditor.lights[levelEditor.playerSrc].destRect.x + radius,
				levelEditor.lights[levelEditor.playerSrc].destRect.y + radius);
			m_debugRenderer.drawCircle(center, color, radius);
		}
		if (levelEditor.playerAround != NO_SELECT)
		{
			const float radius = levelEditor.lights[levelEditor.playerAround].destRect.z / 2.0f;
			const glm::vec2 center(levelEditor.lights[levelEditor.playerAround].destRect.x + radius,
				levelEditor.lights[levelEditor.playerAround].destRect.y + radius);
			m_debugRenderer.drawCircle(center, color, radius);
		}
		if (levelEditor.playerDiff != NO_SELECT)
		{
			const float radius = levelEditor.lights[levelEditor.playerDiff].destRect.z / 2.0f;
			const glm::vec2 center(levelEditor.lights[levelEditor.playerDiff].destRect.x + radius,
				levelEditor.lights[levelEditor.playerDiff].destRect.y + radius);
			m_debugRenderer.drawCircle(center, color, radius);
		}
	}

	if (levelEditor.objectMode == ObjectMode::LEVEL && levelEditor.onScreenShoot &&
		levelEditor.screenRect.x > 0 && levelEditor.screenRect.y > 0)
	{
		const NS2::ColorRGBA8 color(255, 255, 0, 255);
		glm::vec4 destRect = levelEditor.screenRect;
		if (destRect.z < 0.0f){
			destRect.x += destRect.z;
			destRect.z *= -1;
		}
		if (destRect.w < 0.0f){
			destRect.y += destRect.w;
			destRect.w *= -1;
		}
		m_debugRenderer.drawBox(destRect, color, 0);
	}
	const glm::vec4 destRect(0.0f, 0.0f, levelEditor.width * TILESIZE, levelEditor.height * TILESIZE);
	m_debugRenderer.drawBox(destRect, m_debugColor, 0.0f);

	m_debugRenderer.end();
	m_debugRenderer.render(camera.getCameraMatrix(), m_lineWidth);
}

/*************************************Map Renderer*********************************************************/
TileRenderer::TileRenderer()
{
	m_debugColor = NS2::ColorRGBA8(255, 0, 0, 50);
}

void TileRenderer::render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, std::vector<Tile>& tiles)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		Tile& t = tiles[i];

		if (camera.isBoxInView(t.getDestRect()))
		{
			t.draw(spriteBatch);
		}
	}
}

void TileRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Tile>& tiles)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		Tile& t = tiles[i];

		if (camera.isBoxInView(t.getDestRect()))
		{
			t.draw(debugRenderer, m_debugColor);
		}
	}
}

/*************************************Entity Renderer*********************************************************/
void EntityRenderer::init()
{
	m_showAgentBlood = true;
	m_offset = 5.0f;
	m_bloodWidth = 40.0f;
	m_bloodHeight = 5.0f;
	m_playerBloodWidth = 180.0f;
	m_playerBloodHeight = 20.0f;
	m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_debugColor.setColor(0, 0, 255, 128);
	m_clFont.setColor(255, 255, 255, 255);
	m_clBlood.setColor(0, 255, 0, 128);
	m_clBloodBack.setColor(255, 0, 0, 128);
	m_clBloodBackground.setColor(255, 255, 255, 128);
	m_bloodTexture = NS2::ResourceManager::getTexture("Assets/blood.png");
	m_bloodBackTexture = NS2::ResourceManager::getTexture("Assets/bloodBack.png");
	m_spriteFont = NS2::ResourceManager::getFont("Fonts/arial.ttf", 12);
}

void EntityRenderer::render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, Level& level)
{
	glm::vec4 box;
	camera.setScreenBox(box);

	int minXgrid = (int)floor(box.x / level.grid.getCellSize());
	int maxXgrid = (int)ceil((box.x + box.z) / level.grid.getCellSize());
	int minYgrid = (int)floor(box.y / level.grid.getCellSize());
	int maxYgrid = (int)ceil((box.y + box.w) / level.grid.getCellSize());

	if (minXgrid < 0) minXgrid = 0;
	if (minYgrid < 0) minYgrid = 0;
	if (maxXgrid > level.grid.getNbCellX()) maxXgrid = level.grid.getNbCellX();
	if (maxYgrid > level.grid.getNbCellY()) maxYgrid = level.grid.getNbCellY();

	int minXtile = (int)floor(box.x / TILESIZE);
	int maxXtile = (int)ceil((box.x + box.z) / TILESIZE);
	int minYtile = (int)floor(box.y / TILESIZE);
	int maxYtile = (int)ceil((box.y + box.w) / TILESIZE);

	if (minXtile < 0) minXtile = 0;
	if (minYtile < 0) minYtile = 0;
	if (maxXtile > level.width) maxXtile = level.width;
	if (maxYtile > level.height) maxYtile = level.height;

	for (unsigned int j = minYtile; j < maxYtile; j++){
		for (unsigned int i = minXtile; i < maxXtile; i++){
			level.getTile(i, j)->draw(spriteBatch);
		}
	}

	for (unsigned int j = minYgrid; j < maxYgrid; j++){
		for (unsigned int i = minXgrid; i < maxXgrid; i++){
			Cell<Entity>& cell = level.grid.getCell(i, j);
			for (auto& e : cell.entities){
				e->draw(spriteBatch);
			}
		}
	}

	if (m_showAgentBlood) {
		for (unsigned int j = minYgrid; j < maxYgrid; j++){
			for (unsigned int i = minXgrid; i < maxXgrid; i++){
				Cell<Entity>& cell = level.grid.getCell(i, j);
				for (auto& e : cell.entities){
					if ((e->getType() == EntityType::HUMAN || e->getType() == EntityType::ZOMBIE) && e != &level.player) {
						Agent* a = static_cast<Agent*> (e);

						m_desRect.z = m_bloodWidth;
						m_desRect.w = m_bloodHeight;
						m_desRect.x = e->getShape().getLX() + ((e->getShape().getWidth() - m_bloodWidth) / 2.0f);
						m_desRect.y = e->getShape().getTY() + m_offset;
						spriteBatch.draw(m_desRect, m_uvRect, m_bloodTexture.id, 0.0f, m_clBloodBack);

						m_desRect.z = (m_bloodWidth * a->getHealth()) / a->getInitHealth();
						spriteBatch.draw(m_desRect, m_uvRect, m_bloodTexture.id, 0.0f, m_clBlood);
					}
				}
			}
		}
	}
}

void EntityRenderer::renderPalyerStuff(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, Level& level)
{
	glm::vec4 box;
	const float border = 8.0f;
	static char buffer[100];
	camera.setScreenBox(box);

	// Draw player's blood.
	m_desRect.z = m_playerBloodWidth + border;
	m_desRect.w = m_playerBloodHeight + border;
	m_desRect.x = box.x + m_offset - (border/2.0f);
	m_desRect.y = box.y + box.w - m_playerBloodHeight - m_offset - (border / 2.0f);
	spriteBatch.draw(m_desRect, m_uvRect, m_bloodBackTexture.id, 0.0f, m_clBloodBackground);

	m_desRect.z = m_playerBloodWidth;
	m_desRect.w = m_playerBloodHeight;
	m_desRect.x = box.x + m_offset;
	m_desRect.y = box.y + box.w - m_playerBloodHeight - m_offset;
	spriteBatch.draw(m_desRect, m_uvRect, m_bloodTexture.id, 0.0f, m_clBloodBack);

	m_desRect.z = (m_playerBloodWidth * level.player.getHealth()) / level.player.getInitHealth();
	spriteBatch.draw(m_desRect, m_uvRect, m_bloodTexture.id, 0.0f, m_clBlood);

	// Draw the number of remaining humans.
	m_desRect.z = m_playerBloodWidth + border;
	m_desRect.w = m_spriteFont.getFontHeight() + border + m_offset;
	m_desRect.x = box.x + m_offset - (border / 2.0f);
	m_desRect.y = box.y + box.w - m_playerBloodHeight - m_offset - ((m_offset + (border / 2.0f)) * 2) - m_spriteFont.getFontHeight();
	spriteBatch.draw(m_desRect, m_uvRect, m_bloodBackTexture.id, 0.0f, m_clBloodBackground);

	m_desRect.x = box.x + m_offset;
	m_desRect.y = m_desRect.y + m_offset;
	m_desRect.z = 1.0f;
	m_desRect.w = 1.0f;
	sprintf_s(buffer, "Humans : %d / %d", level.nbHumans, level.humans.size());
	m_spriteFont.draw(spriteBatch, buffer, m_desRect, 0.0f, m_clFont);

	// Draw the number of remaining zombies.
	m_desRect.z = m_playerBloodWidth + border;
	m_desRect.w = m_spriteFont.getFontHeight() + border + m_offset;
	m_desRect.x = box.x + m_offset - (border / 2.0f);
	m_desRect.y = box.y + box.w - m_playerBloodHeight - m_offset - ((m_offset + (border / 2.0f)) * 6) - m_spriteFont.getFontHeight();
	spriteBatch.draw(m_desRect, m_uvRect, m_bloodBackTexture.id, 0.0f, m_clBloodBackground);

	m_desRect.x = box.x + m_offset;
	m_desRect.y = m_desRect.y + m_offset;
	m_desRect.z = 1.0f;
	m_desRect.w = 1.0f;
	sprintf_s(buffer, "Zombies : %d / %d", level.nbZombies, level.nbTotalZom);
	m_spriteFont.draw(spriteBatch, buffer, m_desRect, 0.0f, m_clFont);
}

void EntityRenderer::render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, std::vector<Entity*>& entities)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (camera.isBoxInView(entities[i]->getShape().destRect))
		{
			entities[i]->draw(spriteBatch);
		}
	}
}

void EntityRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Entity*>& entities)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (camera.isBoxInView(entities[i]->getShape().destRect))
		{
			entities[i]->draw(debugRenderer, m_debugColor);
		}
	}
}

void EntityRenderer::render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, LevelEditor& level)
{
	for (size_t i = 0; i < level.humans.size(); i++)
	{
		if (camera.isBoxInView(level.humans[i].getShape().destRect))
		{
			level.humans[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.zombies.size(); i++)
	{
		if (camera.isBoxInView(level.zombies[i].getShape().destRect))
		{
			level.zombies[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.coffers.size(); i++)
	{
		if (camera.isBoxInView(level.coffers[i].getShape().destRect))
		{
			level.coffers[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.keys.size(); i++)
	{
		if (camera.isBoxInView(level.keys[i].getShape().destRect))
		{
			level.keys[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.guns.size(); i++)
	{
		if (camera.isBoxInView(level.guns[i].getShape().destRect))
		{
			level.guns[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.bags.size(); i++)
	{
		if (camera.isBoxInView(level.bags[i].getShape().destRect))
		{
			level.bags[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.ammos.size(); i++)
	{
		if (camera.isBoxInView(level.ammos[i].getShape().destRect))
		{
			level.ammos[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.flashlights.size(); i++)
	{
		if (camera.isBoxInView(level.flashlights[i].getShape().destRect))
		{
			level.flashlights[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.doors.size(); i++)
	{
		if (camera.isBoxInView(level.doors[i].getShape().destRect))
		{
			level.doors[i].draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < level.medicines.size(); i++)
	{
		if (camera.isBoxInView(level.medicines[i].getShape().destRect))
		{
			level.medicines[i].draw(spriteBatch);
		}
	}
}

void EntityRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, LevelEditor& level)
{
	level.player.draw(debugRenderer, m_debugColor);

	for (size_t i = 0; i < level.humans.size(); i++)
	{
		if (camera.isBoxInView(level.humans[i].getShape().destRect))
		{
			level.humans[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.zombies.size(); i++)
	{
		if (camera.isBoxInView(level.zombies[i].getShape().destRect))
		{
			level.zombies[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.coffers.size(); i++)
	{
		if (camera.isBoxInView(level.coffers[i].getShape().destRect))
		{
			level.coffers[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.keys.size(); i++)
	{
		if (camera.isBoxInView(level.keys[i].getShape().destRect))
		{
			level.keys[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.guns.size(); i++)
	{
		if (camera.isBoxInView(level.guns[i].getShape().destRect))
		{
			level.guns[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.bags.size(); i++)
	{
		if (camera.isBoxInView(level.bags[i].getShape().destRect))
		{
			level.bags[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.ammos.size(); i++)
	{
		if (camera.isBoxInView(level.ammos[i].getShape().destRect))
		{
			level.ammos[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.flashlights.size(); i++)
	{
		if (camera.isBoxInView(level.flashlights[i].getShape().destRect))
		{
			level.flashlights[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.doors.size(); i++)
	{
		if (camera.isBoxInView(level.doors[i].getShape().destRect))
		{
			level.doors[i].draw(debugRenderer, m_debugColor);
		}
	}
	for (size_t i = 0; i < level.medicines.size(); i++)
	{
		if (camera.isBoxInView(level.medicines[i].getShape().destRect))
		{
			level.medicines[i].draw(debugRenderer, m_debugColor);
		}
	}
}

/*************************************Grid Renderer*********************************************************/
GridRenderer::GridRenderer()
{
	m_debugColor = NS2::ColorRGBA8(0, 0, 255, 128);
}

void GridRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, Grid<Entity>& grid)
{
	for (size_t i = 0; i < grid.getNbCellX(); i++)
	{
		for (size_t j = 0; j < grid.getNbCellY(); j++)
		{
			glm::vec4 desRect(i*grid.getCellSize(), j*grid.getCellSize(), grid.getCellSize(), grid.getCellSize());

			if (camera.isBoxInView(desRect))
			{
				debugRenderer.drawBox(desRect, m_debugColor);
			}
		}
	}
}

/*************************************Graph Renderer*********************************************************/
GraphRenderer::GraphRenderer()
{
	m_debugColor = NS2::ColorRGBA8(0, 0, 255, 128);
}

void GraphRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<GraphNode>& nodes,
	std::vector<GraphEdge>& edges)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (camera.isBoxInView(nodes[i].getShape().destRect))
		{
			nodes[i].getShape().draw(debugRenderer, m_debugColor);
			debugRenderer.drawCircle(nodes[i].getCenter(), m_debugColor, CENTER_WIDTH);
		}
	}

	for (size_t k = 0; k < edges.size(); k++)
	{
		edges[k].draw(debugRenderer, m_debugColor);
	}
}

/*************************************Light Renderer*********************************************************/
LightRenderer::LightRenderer()
{
	m_debugColor = NS2::ColorRGBA8(0, 0, 255, 128);
}

void LightRenderer::render(NS2::SpriteBatch& spriteBatch, NS2::Camera2D& camera, std::vector<Light>& lights)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (camera.isBoxInView(lights[i].destRect)) /// supress spot light
		{
			lights[i].draw(spriteBatch);
		}
	}
}

void LightRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Light>& lights)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (camera.isBoxInView(lights[i].destRect))
		{
			const float radius = lights[i].destRect.z / 2.0f;
			const glm::vec2 center(lights[i].destRect.x + radius, lights[i].destRect.y + radius);
			debugRenderer.drawCircle(center, m_debugColor, radius);
		}
	}
}

/*************************************AI Renderer*********************************************************/

AIRenderer::AIRenderer()
{
	m_debugColor = NS2::ColorRGBA8(0, 0, 255, 128);
}

void AIRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, std::vector<Entity*>& entities)
{
	for (auto& e : entities)
	{
		if ((e->getType() == EntityType::HUMAN || e->getType() == EntityType::ZOMBIE) &&
			camera.isBoxInView(e->getShape().destRect))
		{
			static_cast<Agent*>(e)->getAI()->getSightSensor().draw(debugRenderer, m_debugColor);
		}
	}
}

MapRenderer::MapRenderer()
{
	m_screenRatio = 1.0f / 20.0f;
	m_border = 10.0f;
	m_transparency = 64;
	m_position = glm::ivec2(1);
	m_showTiles = true;
	m_showZombies = true;
	m_showHumans = true;
	m_showPlayer = true;
	m_showObjects = true;
	m_debugColor.setColor(0, 0, 0, m_transparency);
	m_clTiles.setColor(0, 0, 255, m_transparency);
	m_clZombies.setColor(255, 0, 0, m_transparency);
	m_clHumans.setColor(0, 255, 0, m_transparency);
	m_clPlayer.setColor(255, 0, 255, m_transparency);
	m_clObjects.setColor(0, 255, 255, m_transparency);
}

void MapRenderer::render(NS2::DebugRenderer& debugRenderer, NS2::Camera2D& camera, Level& level)
{
	glm::vec4 desRect;
	camera.setScreenBox(desRect);
	float x = desRect.x + (desRect.z * m_position.x) - m_border - (level.width * TILESIZE * m_screenRatio);
	float y = desRect.y + (desRect.w * m_position.y) - m_border - (level.height * TILESIZE * m_screenRatio);

	for (auto& t : level.tiles)
	{
		if (!t.isCrossable()) {
			desRect.x = x + (t.getDestRect().x * m_screenRatio);
			desRect.y = y + (t.getDestRect().y * m_screenRatio);
			desRect.z = t.getDestRect().z * m_screenRatio;
			desRect.w = t.getDestRect().w * m_screenRatio;
			debugRenderer.drawBox(desRect, m_clTiles);
		}
	}

	for (auto& e : level.entities)
	{
		desRect.x = x + (e->getShape().destRect.x * m_screenRatio);
		desRect.y = y + (e->getShape().destRect.y * m_screenRatio);
		desRect.z = e->getShape().destRect.z * m_screenRatio;
		desRect.w = e->getShape().destRect.w * m_screenRatio;

		if (e->getType() == EntityType::HUMAN && m_showHumans) {
			debugRenderer.drawBox(desRect, m_clHumans);
		}
		else if (e->getType() == EntityType::ZOMBIE  && m_showZombies) {
			debugRenderer.drawBox(desRect, m_clZombies);
		}
		else if (e->getType() == EntityType::OBJECT && m_showObjects) {
			debugRenderer.drawBox(desRect, m_clObjects);
		}
	}

	if (m_showPlayer) {
		desRect.x = x + (level.player.getShape().destRect.x * m_screenRatio);
		desRect.y = y + (level.player.getShape().destRect.y * m_screenRatio);
		desRect.z = level.player.getShape().destRect.z * m_screenRatio;
		desRect.w = level.player.getShape().destRect.w * m_screenRatio;
		debugRenderer.drawBox(desRect, m_clPlayer);
	}
}