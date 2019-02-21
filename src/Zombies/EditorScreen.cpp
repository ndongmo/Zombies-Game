#include "EditorScreen.h"
#include "ScreenIndices.h"
#include "XmlExtractor.h"

#include <iostream>
#include <ResourceManager.h>
#include <IOManager.h>
#include <ImageLoader.h>

const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 1;
const float LIGHT_SELECT_RADIUS = 0.5f;
const float MOVE_SPEED = 4.5f;
const float CAM_SCALE_SPEED = 0.1f;
const float WHEEL_SPEED = 5.0f;

EditorScreen::EditorScreen() : m_guiEditor(this) {
	m_screenIndex = SCREEN_INDEX_EDITOR;
}

EditorScreen::~EditorScreen() {
}

int EditorScreen::getNextSCreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int EditorScreen::getPreviousSCreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void EditorScreen::build() {

}

void EditorScreen::destroy() {

}

void EditorScreen::onEntry() {
	m_mouseButtons[MOUSE_LEFT] = false;
	m_mouseButtons[MOUSE_RIGHT] = false;
	m_isDragging = false;
	m_isOnMap = false;
	m_isOnUi = false;

	// Init camera
	m_camera.init(m_game->getWindow().getScreenWidth(), m_game->getWindow().getScreenHeight());

	m_renderer.init(m_game->getWindow().getScreenWidth(), m_game->getWindow().getScreenHeight());

	initGui();

	m_level.init();
}

void EditorScreen::onExit() {
	clearLevel();
	m_game->getGui().clear();
	m_renderer.dispose();
}

void EditorScreen::update() {
	m_camera.update();

	checkInput();

	if (!m_isOnUi)
	{
		if (m_guiEditor.getWindow()->isVisible())
			m_guiEditor.getWindow()->hide();
	}
	else
	{
		if (!m_guiEditor.getWindow()->isVisible()){
			m_guiEditor.getWindow()->show();
			m_guiEditor.update();
		}
	}
}

void EditorScreen::draw() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_renderer.render(m_level, m_camera, m_isOnMap);
}

void EditorScreen::clearLevel() {
	m_level.clear();
}

bool EditorScreen::saveLevel(std::string& error)
{
	if (m_level.width * m_level.height != m_level.tiles.size()){
		error = "Size map not fits the tiles size !";
		return false;
	}
	std::string filePath = "Levels/" + m_level.name + ".xml";
	return XmlExtractor::saveLevel(m_level, filePath, error);
}

bool EditorScreen::loadLevel(std::string& fileName, std::string& error)
{
	fileName = "Levels/" + fileName;
	return XmlExtractor::loadLevel(m_level, fileName, error);
}

void EditorScreen::initGui() {
	m_guiEditor.initWithGui(m_game->getGui());
}

void EditorScreen::checkInput() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		m_game->onSDLEvent(ev);
		switch (ev.type) {
		case SDL_QUIT:
			m_guiEditor.onQuitClick(CEGUI::EventArgs());
			break;
		case SDL_KEYDOWN:
			if (!m_isOnUi)
			{
				switch (ev.key.keysym.sym)
				{
				case SDLK_e:
					m_camera.offsetPosition(glm::vec2(0, 1 * m_camera.getAspectRatio()) * MOVE_SPEED);
					break;
				case SDLK_d:
					m_camera.offsetPosition(glm::vec2(0, -1 * m_camera.getAspectRatio()) * MOVE_SPEED);
					break;
				case SDLK_s:
					m_camera.offsetPosition(glm::vec2(-1 * m_camera.getAspectRatio(), 0) * MOVE_SPEED);
					break;
				case SDLK_f:
					m_camera.offsetPosition(glm::vec2(1 * m_camera.getAspectRatio(), 0) * MOVE_SPEED);
					break;
				case SDLK_PLUS:
					m_camera.offsetScale(m_camera.getScale() * CAM_SCALE_SPEED);
					break;
				case SDLK_MINUS:
					m_camera.offsetScale(m_camera.getScale() * -CAM_SCALE_SPEED);
					break;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			updateMouseDown(ev);
			break;
		case SDL_MOUSEBUTTONUP:
			updateMouseUp(ev);
			break;
		case SDL_MOUSEMOTION:
			updateMouseMotion(ev);
			break;
		case SDL_MOUSEWHEEL:
			updateMouseWeel(ev);
			break;
		}
	}
}

void EditorScreen::updateMouseWeel(const SDL_Event& evnt)
{
	float offset = evnt.wheel.y * WHEEL_SPEED;

	if (m_level.objectMode == ObjectMode::LEVEL)
	{
		m_level.width += offset;
		m_level.height += offset;
	}
	else if (m_level.objectMode == ObjectMode::LIGHT)
	{
		lightMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::PLAYER)
	{
		playerMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::HUMAN)
	{
		humanMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::ZOMBIE)
	{
		zombieMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::COFFER)
	{
		cofferMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::KEY)
	{
		keyMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::GUN)
	{
		gunMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::DOOR)
	{
		doorMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::MEDICINE)
	{
		medocMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::AMMO)
	{
		ammoMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::BAG)
	{
		bagMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::FLASHLIGHT)
	{
		flashlightMouseWheel(offset);
	}
	else if (m_level.objectMode == ObjectMode::NODE)
	{
		nodeMouseWheel(offset);
	}
}

void EditorScreen::updateMouseDown(const SDL_Event& evnt)
{
	if (!m_isOnMap) return;

	glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));

	if (evnt.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtons[MOUSE_LEFT] = true;

		if (m_level.objectMode == ObjectMode::LEVEL && m_level.onScreenShoot)
		{
			if (m_level.screenRect.z > 0 && m_level.screenRect.w > 0){
				std::string str = "Assets/" + m_level.name + ".png";
				glm::vec4 destRect = m_level.screenRect;

				if (destRect.z < 0.0f){
					destRect.x += destRect.z;
					destRect.z *= -1;
				}
				if (destRect.w < 0.0f){
					destRect.y += destRect.w;
					destRect.w *= -1;
				}
				destRect.x += m_game->getWindow().getScreenWidth() / 2;
				destRect.y += m_game->getWindow().getScreenHeight() / 2;
				NS2::ImageLoader::savePNG(destRect.x, destRect.y, destRect.z, destRect.w, str);
				m_level.screenRect.x = 0;
				m_level.screenRect.y = 0;
				m_level.screenRect.z = 0;
				m_level.screenRect.w = 0;
			}
			else
			{
				m_level.screenRect.x = pos.x;
				m_level.screenRect.y = pos.y;
			}
		}
		else if (m_level.objectMode == ObjectMode::TILE)
		{
			tileMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::LIGHT)
		{
			lightMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::PLAYER)
		{
			playerMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::HUMAN)
		{
			humanMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::ZOMBIE)
		{
			zombieMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::COFFER)
		{
			cofferMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::KEY)
		{
			keyMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::GUN)
		{
			gunMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::DOOR)
		{
			doorMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::MEDICINE)
		{
			medocMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::AMMO)
		{
			ammoMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::BAG)
		{
			bagMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::FLASHLIGHT)
		{
			flashlightMouseLeftDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::NODE)
		{
			nodeMouseLeftDown(pos);
		}
	}
	else
	{
		m_mouseButtons[MOUSE_RIGHT] = true;

		if (m_level.objectMode == ObjectMode::TILE)
		{
			tileMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::LIGHT)
		{
			lightMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::PLAYER)
		{
			playerMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::HUMAN)
		{
			humanMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::ZOMBIE)
		{
			zombieMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::COFFER)
		{
			cofferMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::KEY)
		{
			keyMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::GUN)
		{
			gunMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::DOOR)
		{
			doorMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::MEDICINE)
		{
			medocMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::AMMO)
		{
			ammoMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::BAG)
		{
			bagMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::FLASHLIGHT)
		{
			flashlightMouseRightDown(pos);
		}
		else if (m_level.objectMode == ObjectMode::NODE)
		{
			nodeMouseRightDown(pos);
		}
	}
}

void EditorScreen::updateMouseUp(const SDL_Event& evnt)
{
	switch (evnt.button.button) {
	case SDL_BUTTON_LEFT:
		m_mouseButtons[MOUSE_LEFT] = false;
		break;
	case SDL_BUTTON_RIGHT:
		m_mouseButtons[MOUSE_RIGHT] = false;
		break;
	}
	m_isDragging = false;
}

void EditorScreen::updateMouseMotion(const SDL_Event& evnt)
{
	m_isOnMap = false;
	m_isOnUi = false;

	if (isMouseInUI()) m_isOnUi = true;
	else if (isMouseInMap()) m_isOnMap = true;

	m_isDragging = m_isDragging && m_isOnMap;

	if (m_isOnMap)
	{
		glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));

		if (m_level.objectMode == ObjectMode::LEVEL && m_level.onScreenShoot && 
			m_level.screenRect.x > 0 && m_level.screenRect.y > 0)
		{
			m_level.screenRect.z = pos.x - m_level.screenRect.x;
			m_level.screenRect.w = pos.y - m_level.screenRect.y;
		}
		else if (m_level.objectMode == ObjectMode::TILE)
		{
			if (m_isDragging)
				tileMouseDragging(pos);
			else
				tileMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::LIGHT)
		{
			lightMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::PLAYER)
		{
			playerMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::HUMAN)
		{
			humanMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::ZOMBIE)
		{
			zombieMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::COFFER)
		{
			cofferMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::KEY)
		{
			keyMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::GUN)
		{
			gunMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::DOOR)
		{
			doorMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::MEDICINE)
		{
			medocMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::AMMO)
		{
			ammoMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::BAG)
		{
			bagMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::FLASHLIGHT)
		{
			flashlightMouseMotion(pos);
		}
		else if (m_level.objectMode == ObjectMode::NODE)
		{
			nodeMouseMotion(pos);
		}
	}
}

bool EditorScreen::isMouseInMap()
{
	glm::vec2 pos = m_camera.convertScreenToWorld(m_game->getEventHandler().getMouseCoords());
	return (pos.x >= 0.0f && pos.x <= m_level.width * TILESIZE && pos.y >= 0.0f && pos.y <= m_level.height * TILESIZE);
}

bool EditorScreen::isMouseInUI()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	const float SW = (float)m_game->getWindow().getScreenWidth();
	const float SH = (float)m_game->getWindow().getScreenHeight();
	CEGUI::Window* w = m_guiEditor.getWindow();

	return (x >= w->getXPosition().d_scale * SW && x <= w->getXPosition().d_scale * SW + w->getWidth().d_scale  * SW &&
		y >= w->getYPosition().d_scale * SH && y <= w->getYPosition().d_scale * SH + w->getHeight().d_scale * SH);
}

void EditorScreen::tileMouseLeftDown(glm::vec2& pos)
{
	int x = (int)(pos.x / TILESIZE);
	int y = (int)(pos.y / TILESIZE);
	Tile* tile = m_level.getTile(x, y);
	if (tile != nullptr)
	{
		if (m_level.selectionMode == SelectionMode::PLACE)
		{
			tile->init(m_level.tile, x, y);
			m_isDragging = true;
		}
		else if (m_level.selectionMode == SelectionMode::SELECT)
		{
			if (m_level.tileSelected) {
				tile->init(m_level.tile, x, y);
				m_level.tileSelected = false;
			}
			else {
				m_level.tile.init(*tile, x, y);
				tile->reset();
				m_level.tileSelected = true;
			}
		}
	}
}
void EditorScreen::tileMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.tileSelected) {
			m_level.tileSelected = false;
		}
		else {
			int x = (int)(pos.x / TILESIZE);
			int y = (int)(pos.y / TILESIZE);
			Tile* tile = m_level.getTile(x, y);
			if (tile != nullptr)
			{
				tile->reset();
				m_isDragging = true;
			}
		}
	}
}
void EditorScreen::tileMouseMotion(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE || m_level.tileSelected)
	{
		pos.x -= TILESIZE / 2.0f;
		pos.y -= TILESIZE / 2.0f;
		m_level.tile.updatePosition(pos);
	}
}
void EditorScreen::tileMouseDragging(glm::vec2& pos)
{
	if (m_mouseButtons[MOUSE_LEFT])
	{
		if (m_level.selectionMode == SelectionMode::PLACE)
		{
			if (m_level.objectMode == ObjectMode::TILE)
			{
				int x = (int)(pos.x / TILESIZE);
				int y = (int)(pos.y / TILESIZE);
				Tile* tile = m_level.getTile(x, y);
				if (tile != nullptr)
				{
					tile->init(m_level.tile, x, y);
				}
			}
		}
	}
	else
	{
		if (m_level.selectionMode == SelectionMode::SELECT)
		{
			if (m_level.objectMode == ObjectMode::TILE)
			{
				int x = (int)(pos.x / TILESIZE);
				int y = (int)(pos.y / TILESIZE);
				Tile* tile = m_level.getTile(x, y);
				if (tile != nullptr)
				{
					tile->reset();
				}
			}
		}
	}
}

void EditorScreen::lightMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.light.destRect.x + m_level.light.destRect.z / 2.0f;
		float y = m_level.light.destRect.y + m_level.light.destRect.z / 2.0f;

		m_level.lights.push_back(Light(m_level.light));
	}
	else
	{
		m_isDragging = m_level.selectLight(pos);
	}
}
void EditorScreen::lightMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectLight(pos))
		{
			if (m_level.indexLight == m_level.playerSrc)
				m_level.playerSrc = NO_SELECT;
			else if (m_level.indexLight == m_level.playerAround)
				m_level.playerAround = NO_SELECT;
			else if (m_level.indexLight == m_level.playerDiff)
				m_level.playerDiff = NO_SELECT;

			m_level.lights[m_level.indexLight] = m_level.lights.back();
			m_level.lights.pop_back();
			m_level.indexLight = NO_SELECT;
		}
	}
}
void EditorScreen::lightMouseMotion(glm::vec2& pos)
{
	Light* l = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		l = &m_level.light;
	else if (m_level.indexLight != NO_SELECT && m_isDragging)
		l = &m_level.lights[m_level.indexLight];

	if (l != nullptr)
	{
		l->destRect.x = pos.x - (l->destRect.z / 2.0f);
		l->destRect.y = pos.y - (l->destRect.z / 2.0f);
	}
}
void EditorScreen::lightMouseWheel(float offset)
{
	Light* l = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		l = &m_level.light;
	else if (m_level.indexLight != NO_SELECT)
		l = &m_level.lights[m_level.indexLight];

	if (l != nullptr)
	{
		l->destRect.z += offset;
		l->destRect.w = l->destRect.z;
	}
}

void EditorScreen::nodeMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.node.getShape().destRect.x + m_level.node.getShape().destRect.z / 2.0f;
		float y = m_level.node.getShape().destRect.y + m_level.node.getShape().destRect.w / 2.0f;

		m_level.nodes.push_back(GraphNode(m_level.node));
	}
	else
	{
		if (m_level.nodeMode == NodeMode::NODE)
		{
			m_isDragging = m_level.selectNode(pos) && m_level.nodes[m_level.indexNode].getNeighbors().empty();
		}
		else if (m_level.nodeMode == NodeMode::CENTER)
		{
			if (m_level.selectNode(pos) && m_level.nodes[m_level.indexNode].getNeighbors().empty())
				m_level.nodes[m_level.indexNode].setCenter(pos.x, pos.y);
		}
		else if (m_level.nodeMode == NodeMode::EDGE && m_level.indexNode != NO_SELECT)
		{
			if (m_level.edge.getNode1() != m_level.indexNode) {
				m_level.edge.startPath(m_level.indexNode, m_level.nodes[m_level.indexNode].getCenter());
			}

			int index = m_level.selectCenterNode(pos);
			if (m_level.edge.addPathPoint(pos) && index != NO_SELECT){
				m_level.edge.endPath(index);
				m_level.edges.emplace_back(m_level.edge);
				m_level.nodes[m_level.indexNode].addNeighbor(index, m_level.edges.size() - 1);
				m_level.nodes[index].addNeighbor(m_level.indexNode, m_level.edges.size() - 1);
				m_level.edge.reset();
			}
		}
	}
}
void EditorScreen::nodeMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.nodeMode == NodeMode::EDGE  && m_level.indexNode != NO_SELECT)
		{
			int index = m_level.indexNode;
			if (m_level.selectNode(pos) && m_level.indexNode != index)
			{
				m_level.deleteLastEdge(index, m_level.indexNode);
				m_level.indexNode = index; // back to initial selected node
			}
			else
				m_level.edge.removeLastPoint();
		}
		else if (m_level.nodeMode != NodeMode::EDGE && m_level.selectNode(pos))
		{
			m_level.deleteSelectedNode();
		}
		m_mouseButtons[MOUSE_RIGHT] = false;
	}
}
void EditorScreen::nodeMouseMotion(glm::vec2& pos)
{
	GraphNode* n = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		n = &m_level.node;
	else if (m_level.indexNode != NO_SELECT && m_isDragging)
		n = &m_level.nodes[m_level.indexNode];
	else if (m_level.nodeMode == NodeMode::EDGE)
		m_level.edge.updateLastPoint(pos);

	if (n != nullptr)
	{
		pos.x = pos.x - (n->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (n->getShape().destRect.w / 2.0f);
		n->setPosition(pos);
	}
}
void EditorScreen::nodeMouseWheel(float offset)
{
	GraphNode* n = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		n = &m_level.node;
	else if (m_level.indexNode != NO_SELECT && m_level.nodeMode == NodeMode::NODE)
		n = &m_level.nodes[m_level.indexNode];
	else if (m_level.nodeMode == NodeMode::EDGE)
		m_level.edge.setWidth(m_level.edge.getWidth() + offset);

	if (n != nullptr)
	{
		n->setSize(offset, offset);
	}
}

void EditorScreen::humanMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.human.getShape().destRect.x + m_level.human.getShape().destRect.z / 2.0f;
		float y = m_level.human.getShape().destRect.y + m_level.human.getShape().destRect.w / 2.0f;

		m_level.humans.push_back(Human(m_level.human));
	}
	else
	{
		m_isDragging = m_level.selectHuman(pos);
	}
}
void EditorScreen::humanMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectHuman(pos))
		{
			m_level.humans[m_level.indexHuman] = m_level.humans.back();
			m_level.humans.pop_back();
			m_level.indexHuman = NO_SELECT;
		}
	}
}
void EditorScreen::humanMouseMotion(glm::vec2& pos)
{
	Human* h = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		h = &m_level.human;
	else if (m_level.indexHuman != NO_SELECT && m_isDragging)
		h = &m_level.humans[m_level.indexHuman];

	if (h != nullptr)
	{
		pos.x = pos.x - (h->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (h->getShape().destRect.w / 2.0f);
		h->getShape().setPosition(pos);
	}
}
void EditorScreen::humanMouseWheel(float offset)
{
	Human* h = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		h = &m_level.human;
	else if (m_level.indexHuman != NO_SELECT)
		h = &m_level.humans[m_level.indexHuman];

	if (h != nullptr)
	{
		h->getShape().destRect.z += offset;
		h->getShape().destRect.w += offset;
	}
}

void EditorScreen::zombieMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.zombie.getShape().destRect.x + m_level.zombie.getShape().destRect.z / 2.0f;
		float y = m_level.zombie.getShape().destRect.y + m_level.zombie.getShape().destRect.w / 2.0f;

		m_level.zombies.push_back(Zombie(m_level.zombie));
	}
	else
	{
		m_isDragging = m_level.selectZombie(pos);
	}
}
void EditorScreen::zombieMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectZombie(pos))
		{
			/*m_level.updateCoffers(CofferMode::ZOMBIE);

			m_level.zombies[m_level.indexZombie] = m_level.zombies.back();
			m_level.zombies.pop_back();
			m_level.indexZombie = NO_SELECT;*/
		}
	}
}
void EditorScreen::zombieMouseMotion(glm::vec2& pos)
{
	Zombie* z = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		z = &m_level.zombie;
	else if (m_level.indexZombie != NO_SELECT && m_isDragging)
		z = &m_level.zombies[m_level.indexZombie];

	if (z != nullptr)
	{
		pos.x = pos.x - (z->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (z->getShape().destRect.w / 2.0f);
		z->getShape().setPosition(pos);
	}
}
void EditorScreen::zombieMouseWheel(float offset)
{
	Zombie* z = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		z = &m_level.zombie;
	else if (m_level.indexZombie != NO_SELECT)
		z = &m_level.zombies[m_level.indexZombie];

	if (z != nullptr)
	{
		z->getShape().destRect.z += offset;
		z->getShape().destRect.w += offset;
	}
}

void EditorScreen::playerMouseLeftDown(glm::vec2& pos)
{
	m_isDragging = m_level.playerSelected = m_level.selectPlayer(pos);
	if (!m_isDragging)
	{
		if ((m_level.playerMode == PlayerMode::GUN1 || m_level.playerMode == PlayerMode::GUN2)
			&& m_level.selectGun(pos))
		{
			if (m_level.playerMode == PlayerMode::GUN1)
				m_level.playerGun1 = m_level.indexGun;
			else
				m_level.playerGun2 = m_level.indexGun;
		}
		else if (m_level.playerMode != PlayerMode::PLAYER && m_level.selectLight(pos))
		{
			if (m_level.playerMode == PlayerMode::SRC_LIGHT)
				m_level.playerSrc = m_level.indexLight;
			else if (m_level.playerMode == PlayerMode::AROUND_LIGHT)
				m_level.playerAround = m_level.indexLight;
			else if (m_level.playerMode == PlayerMode::DIFF_LIGHT)
				m_level.playerDiff = m_level.indexLight;
		}
	}
}
void EditorScreen::playerMouseRightDown(glm::vec2& pos)
{
	if ((m_level.playerMode == PlayerMode::GUN1 || m_level.playerMode == PlayerMode::GUN2)
		&& m_level.selectGun(pos))
	{
		if (m_level.playerMode == PlayerMode::GUN1)
			m_level.playerGun1 = NO_SELECT;
		else
			m_level.playerGun2 = NO_SELECT;
	}
	else if (m_level.playerMode != PlayerMode::PLAYER && m_level.selectLight(pos))
	{
		if (m_level.playerMode == PlayerMode::SRC_LIGHT)
			m_level.playerSrc = NO_SELECT;
		else if (m_level.playerMode == PlayerMode::AROUND_LIGHT)
			m_level.playerAround = NO_SELECT;
		else if (m_level.playerMode == PlayerMode::DIFF_LIGHT)
			m_level.playerDiff = NO_SELECT;
	}
}
void EditorScreen::playerMouseMotion(glm::vec2& pos)
{
	if (m_level.playerSelected && m_isDragging)
	{
		pos.x = pos.x - (m_level.player.getShape().destRect.z / 2.0f);
		pos.y = pos.y - (m_level.player.getShape().destRect.w / 2.0f);
		m_level.player.getShape().setPosition(pos);
	}
}
void EditorScreen::playerMouseWheel(float offset)
{
	if (m_level.playerSelected)
	{
		m_level.player.getShape().destRect.z += offset;
		m_level.player.getShape().destRect.w += offset;
	}
}

void EditorScreen::cofferMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.coffer.getShape().destRect.x + m_level.coffer.getShape().destRect.z / 2.0f;
		float y = m_level.coffer.getShape().destRect.y + m_level.coffer.getShape().destRect.w / 2.0f;

		m_level.coffers.push_back(Coffer(m_level.coffer));
	}
	else
	{
		m_isDragging = m_level.selectCoffer(pos);
		if (!m_isDragging && m_level.indexCoffer != NO_SELECT)
		{
			if (m_level.cofferMode == CofferMode::AMMO && m_level.selectAmmo(pos)) {
				int index = m_level.cofferObjectIndex(CofferMode::AMMO);
				m_level.coffersObject[m_level.indexCoffer][index] = CofferMode::AMMO;
			}
			else if(m_level.cofferMode == CofferMode::BAG && m_level.selectBag(pos)) {
				int index = m_level.cofferObjectIndex(CofferMode::BAG);
				m_level.coffersObject[m_level.indexCoffer][index] = CofferMode::BAG;
			}
			else if(m_level.cofferMode == CofferMode::FLASHLIGHT && m_level.selectFlashlight(pos)) {
				int index = m_level.cofferObjectIndex(CofferMode::FLASHLIGHT);
				m_level.coffersObject[m_level.indexCoffer][index] = CofferMode::FLASHLIGHT;
			}
			else if (m_level.cofferMode == CofferMode::GUN && m_level.selectGun(pos)) {
				int index = m_level.cofferObjectIndex(CofferMode::GUN);
				m_level.coffersObject[m_level.indexCoffer][index] = CofferMode::GUN;
			}
			else if (m_level.cofferMode == CofferMode::KEY && m_level.selectKey(pos)) {
				int index = m_level.cofferObjectIndex(CofferMode::KEY);
				m_level.coffersObject[m_level.indexCoffer][index] = CofferMode::KEY;
			}
			else if (m_level.cofferMode == CofferMode::MEDICINE && m_level.selectMedoc(pos)) {
				int index = m_level.cofferObjectIndex(CofferMode::MEDICINE);
				m_level.coffersObject[m_level.indexCoffer][index] = CofferMode::MEDICINE;
			}
		}
	}
}
void EditorScreen::cofferMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectCoffer(pos))
		{
			m_level.coffersObject[m_level.indexCoffer] = m_level.coffersObject[m_level.coffers.size() - 1];
			m_level.coffersObject.erase(m_level.coffers.size() - 1);

			m_level.coffers[m_level.indexCoffer] = m_level.coffers.back();
			m_level.coffers.pop_back();
			m_level.indexCoffer = NO_SELECT;
		}
		else if (m_level.indexCoffer != NO_SELECT)
		{
			if (m_level.cofferMode == CofferMode::AMMO && m_level.selectAmmo(pos))
			{
				int zIndex = m_level.cofferObjectIndex(CofferMode::AMMO);
				if (m_level.coffersObject[m_level.indexCoffer].find(zIndex) !=
					m_level.coffersObject[m_level.indexCoffer].end())
					m_level.coffersObject[m_level.indexCoffer].erase(zIndex);
			}
			else if(m_level.cofferMode == CofferMode::BAG && m_level.selectBag(pos))
			{
				int zIndex = m_level.cofferObjectIndex(CofferMode::BAG);
				if (m_level.coffersObject[m_level.indexCoffer].find(zIndex) !=
					m_level.coffersObject[m_level.indexCoffer].end())
					m_level.coffersObject[m_level.indexCoffer].erase(zIndex);
			}
			else if(m_level.cofferMode == CofferMode::FLASHLIGHT && m_level.selectFlashlight(pos))
			{
				int zIndex = m_level.cofferObjectIndex(CofferMode::FLASHLIGHT);
				if (m_level.coffersObject[m_level.indexCoffer].find(zIndex) !=
					m_level.coffersObject[m_level.indexCoffer].end())
					m_level.coffersObject[m_level.indexCoffer].erase(zIndex);
			}
			else if (m_level.cofferMode == CofferMode::GUN && m_level.selectGun(pos))
			{
				int gIndex = m_level.cofferObjectIndex(CofferMode::GUN);
				if (m_level.coffersObject[m_level.indexCoffer].find(gIndex)
					!= m_level.coffersObject[m_level.indexCoffer].end())
					m_level.coffersObject[m_level.indexCoffer].erase(gIndex);
			}
			else if (m_level.cofferMode == CofferMode::KEY && m_level.selectKey(pos))
			{
				int kIndex = m_level.cofferObjectIndex(CofferMode::KEY);
				if (m_level.coffersObject[m_level.indexCoffer].find(kIndex)
					!= m_level.coffersObject[m_level.indexCoffer].end())
					m_level.coffersObject[m_level.indexCoffer].erase(kIndex);
			}
			else if (m_level.cofferMode == CofferMode::MEDICINE && m_level.selectMedoc(pos))
			{
				int mIndex = m_level.cofferObjectIndex(CofferMode::MEDICINE);
				if (m_level.coffersObject[m_level.indexCoffer].find(mIndex)
					!= m_level.coffersObject[m_level.indexCoffer].end())
					m_level.coffersObject[m_level.indexCoffer].erase(mIndex);
			}
		}
	}
}
void EditorScreen::cofferMouseMotion(glm::vec2& pos)
{
	Coffer* c = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		c = &m_level.coffer;
	else if (m_level.indexCoffer != NO_SELECT && m_isDragging)
		c = &m_level.coffers[m_level.indexCoffer];

	if (c != nullptr)
	{
		pos.x = pos.x - (c->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (c->getShape().destRect.w / 2.0f);
		c->getShape().setPosition(pos);
	}
}
void EditorScreen::cofferMouseWheel(float offset)
{
	Coffer* c = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		c = &m_level.coffer;
	else if (m_level.indexCoffer != NO_SELECT)
		c = &m_level.coffers[m_level.indexCoffer];

	if (c != nullptr)
	{
		c->getShape().destRect.z += offset;
		c->getShape().destRect.w += offset;
	}
}

void EditorScreen::keyMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.key.getShape().destRect.x + m_level.key.getShape().destRect.z / 2.0f;
		float y = m_level.key.getShape().destRect.y + m_level.key.getShape().destRect.w / 2.0f;

		m_level.keys.push_back(Key(m_level.key));
	}
	else
	{
		m_isDragging = m_level.selectKey(pos);
	}
}
void EditorScreen::keyMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectKey(pos))
		{
			m_level.updateCoffers(CofferMode::KEY);

			m_level.keys[m_level.indexKey] = m_level.keys.back();
			m_level.keys.pop_back();
			m_level.indexKey = NO_SELECT;
		}
	}
}
void EditorScreen::keyMouseMotion(glm::vec2& pos)
{
	Key* k = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		k = &m_level.key;
	else if (m_level.indexKey != NO_SELECT && m_isDragging)
		k = &m_level.keys[m_level.indexKey];

	if (k != nullptr)
	{
		pos.x = pos.x - (k->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (k->getShape().destRect.w / 2.0f);
		k->getShape().setPosition(pos);
	}
}
void EditorScreen::keyMouseWheel(float offset)
{
	Key* k = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		k = &m_level.key;
	else if (m_level.indexKey != NO_SELECT)
		k = &m_level.keys[m_level.indexKey];

	if (k != nullptr)
	{
		k->getShape().destRect.z += offset;
		k->getShape().destRect.w += offset;
	}
}

void EditorScreen::doorMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.door.getShape().destRect.x + m_level.door.getShape().destRect.z / 2.0f;
		float y = m_level.door.getShape().destRect.y + m_level.door.getShape().destRect.w / 2.0f;

		m_level.doors.push_back(Door(m_level.door));
	}
	else
	{
		m_isDragging = m_level.selectDoor(pos);
	}
}
void EditorScreen::doorMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectDoor(pos))
		{
			m_level.doors[m_level.indexDoor] = m_level.doors.back();
			m_level.doors.pop_back();
			m_level.indexDoor = NO_SELECT;
		}
	}
}
void EditorScreen::doorMouseMotion(glm::vec2& pos)
{
	Door* d = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		d = &m_level.door;
	else if (m_level.indexDoor != NO_SELECT && m_isDragging)
		d = &m_level.doors[m_level.indexDoor];

	if (d != nullptr)
	{
		pos.x = pos.x - (d->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (d->getShape().destRect.w / 2.0f);
		d->getShape().setPosition(pos);
	}
}
void EditorScreen::doorMouseWheel(float offset)
{
	Door* d = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		d = &m_level.door;
	else if (m_level.indexDoor != NO_SELECT)
		d = &m_level.doors[m_level.indexDoor];

	if (d != nullptr)
	{
		d->getShape().destRect.z += offset;
		d->getShape().destRect.w += offset;
	}
}

void EditorScreen::gunMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.gun.getShape().destRect.x + m_level.gun.getShape().destRect.z / 2.0f;
		float y = m_level.gun.getShape().destRect.y + m_level.gun.getShape().destRect.w / 2.0f;

		m_level.guns.push_back(Gun(m_level.gun));
	}
	else
	{
		m_isDragging = m_level.selectGun(pos);
	}
}
void EditorScreen::gunMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectGun(pos))
		{
			if (m_level.indexGun == m_level.playerGun1)
				m_level.playerGun1 = NO_SELECT;
			else if (m_level.indexGun == m_level.playerGun2)
				m_level.playerGun2 = NO_SELECT;

			m_level.updateCoffers(CofferMode::GUN);

			m_level.guns[m_level.indexGun] = m_level.guns.back();
			m_level.guns.pop_back();
			m_level.indexGun = NO_SELECT;
		}
	}
}
void EditorScreen::gunMouseMotion(glm::vec2& pos)
{
	Gun* g = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		g = &m_level.gun;
	else if (m_level.indexGun != NO_SELECT && m_isDragging)
		g = &m_level.guns[m_level.indexGun];

	if (g != nullptr)
	{
		pos.x = pos.x - (g->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (g->getShape().destRect.w / 2.0f);
		g->getShape().setPosition(pos);
	}
}
void EditorScreen::gunMouseWheel(float offset)
{
	Gun* g = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		g = &m_level.gun;
	else if (m_level.indexGun != NO_SELECT)
		g = &m_level.guns[m_level.indexGun];

	if (g != nullptr)
	{
		g->getShape().destRect.z += offset;
		g->getShape().destRect.w += offset;
	}
}

void EditorScreen::medocMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.medicine.getShape().destRect.x + m_level.medicine.getShape().destRect.z / 2.0f;
		float y = m_level.medicine.getShape().destRect.y + m_level.medicine.getShape().destRect.w / 2.0f;

		m_level.medicines.push_back(Medicine(m_level.medicine));
	}
	else
	{
		m_isDragging = m_level.selectMedoc(pos);
	}
}
void EditorScreen::medocMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectMedoc(pos))
		{
			m_level.updateCoffers(CofferMode::MEDICINE);

			m_level.medicines[m_level.indexMedoc] = m_level.medicines.back();
			m_level.medicines.pop_back();
			m_level.indexMedoc = NO_SELECT;
		}
	}
}
void EditorScreen::medocMouseMotion(glm::vec2& pos)
{
	Medicine* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.medicine;
	else if (m_level.indexMedoc != NO_SELECT && m_isDragging)
		m = &m_level.medicines[m_level.indexMedoc];

	if (m != nullptr)
	{
		pos.x = pos.x - (m->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (m->getShape().destRect.w / 2.0f);
		m->getShape().setPosition(pos);
	}
}
void EditorScreen::medocMouseWheel(float offset)
{
	Medicine* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.medicine;
	else if (m_level.indexMedoc != NO_SELECT)
		m = &m_level.medicines[m_level.indexMedoc];

	if (m != nullptr)
	{
		m->getShape().destRect.z += offset;
		m->getShape().destRect.w += offset;
	}
}

void EditorScreen::bagMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.bag.getShape().destRect.x + m_level.bag.getShape().destRect.z / 2.0f;
		float y = m_level.bag.getShape().destRect.y + m_level.bag.getShape().destRect.w / 2.0f;

		m_level.bags.push_back(Bag(m_level.bag));
	}
	else
	{
		m_isDragging = m_level.selectBag(pos);
	}
}
void EditorScreen::bagMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectBag(pos))
		{
			m_level.updateCoffers(CofferMode::BAG);

			m_level.bags[m_level.indexBag] = m_level.bags.back();
			m_level.bags.pop_back();
			m_level.indexBag = NO_SELECT;
		}
	}
}
void EditorScreen::bagMouseMotion(glm::vec2& pos)
{
	Bag* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.bag;
	else if (m_level.indexBag != NO_SELECT && m_isDragging)
		m = &m_level.bags[m_level.indexBag];

	if (m != nullptr)
	{
		pos.x = pos.x - (m->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (m->getShape().destRect.w / 2.0f);
		m->getShape().setPosition(pos);
	}
}
void EditorScreen::bagMouseWheel(float offset)
{
	Bag* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.bag;
	else if (m_level.indexBag != NO_SELECT)
		m = &m_level.bags[m_level.indexBag];

	if (m != nullptr)
	{
		m->getShape().destRect.z += offset;
		m->getShape().destRect.w += offset;
	}
}

void EditorScreen::ammoMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.ammo.getShape().destRect.x + m_level.ammo.getShape().destRect.z / 2.0f;
		float y = m_level.ammo.getShape().destRect.y + m_level.ammo.getShape().destRect.w / 2.0f;

		m_level.ammos.push_back(Ammo(m_level.ammo));
	}
	else
	{
		m_isDragging = m_level.selectAmmo(pos);
	}
}
void EditorScreen::ammoMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectAmmo(pos))
		{
			m_level.updateCoffers(CofferMode::AMMO);

			m_level.ammos[m_level.indexAmmo] = m_level.ammos.back();
			m_level.ammos.pop_back();
			m_level.indexAmmo = NO_SELECT;
		}
	}
}
void EditorScreen::ammoMouseMotion(glm::vec2& pos)
{
	Ammo* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.ammo;
	else if (m_level.indexAmmo != NO_SELECT && m_isDragging)
		m = &m_level.ammos[m_level.indexAmmo];

	if (m != nullptr)
	{
		pos.x = pos.x - (m->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (m->getShape().destRect.w / 2.0f);
		m->getShape().setPosition(pos);
	}
}
void EditorScreen::ammoMouseWheel(float offset)
{
	Ammo* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.ammo;
	else if (m_level.indexAmmo != NO_SELECT)
		m = &m_level.ammos[m_level.indexAmmo];

	if (m != nullptr)
	{
		m->getShape().destRect.z += offset;
		m->getShape().destRect.w += offset;
	}
}

void EditorScreen::flashlightMouseLeftDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::PLACE)
	{
		float x = m_level.flashlight.getShape().destRect.x + m_level.flashlight.getShape().destRect.z / 2.0f;
		float y = m_level.flashlight.getShape().destRect.y + m_level.flashlight.getShape().destRect.w / 2.0f;

		m_level.flashlights.push_back(Flashlight(m_level.flashlight));
	}
	else
	{
		m_isDragging = m_level.selectFlashlight(pos);
	}
}
void EditorScreen::flashlightMouseRightDown(glm::vec2& pos)
{
	if (m_level.selectionMode == SelectionMode::SELECT)
	{
		if (m_level.selectFlashlight(pos))
		{
			m_level.updateCoffers(CofferMode::FLASHLIGHT);

			m_level.flashlights[m_level.indexFlashlight] = m_level.flashlights.back();
			m_level.flashlights.pop_back();
			m_level.indexFlashlight = NO_SELECT;
		}
	}
}
void EditorScreen::flashlightMouseMotion(glm::vec2& pos)
{
	Flashlight* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.flashlight;
	else if (m_level.indexFlashlight != NO_SELECT && m_isDragging)
		m = &m_level.flashlights[m_level.indexFlashlight];

	if (m != nullptr)
	{
		pos.x = pos.x - (m->getShape().destRect.z / 2.0f);
		pos.y = pos.y - (m->getShape().destRect.w / 2.0f);
		m->getShape().setPosition(pos);
	}
}
void EditorScreen::flashlightMouseWheel(float offset)
{
	Flashlight* m = nullptr;

	if (m_level.selectionMode == SelectionMode::PLACE)
		m = &m_level.flashlight;
	else if (m_level.indexFlashlight != NO_SELECT)
		m = &m_level.flashlights[m_level.indexFlashlight];

	if (m != nullptr)
	{
		m->getShape().destRect.z += offset;
		m->getShape().destRect.w += offset;
	}
}