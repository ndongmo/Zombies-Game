#include "Gui.h"
#include "Logger.h"
#include "EventHandler.h"

#include <SDL\SDL_timer.h>

namespace NS2
{
	CEGUI::OpenGL3Renderer* Gui::m_renderer = nullptr;

	bool Gui::init(const std::string& resourceDirectory)
	{
		// Check if the renderer and system were not already initialized
		if (m_renderer == nullptr)
		{
			try
			{
				m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

				CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
					(CEGUI::System::getSingleton().getResourceProvider());

				rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
				rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
				rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
				rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
				rp->setResourceGroupDirectory("looknfeels", resourceDirectory + "/looknfeel/");
				rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

				CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
				CEGUI::Scheme::setDefaultResourceGroup("schemes");
				CEGUI::Font::setDefaultResourceGroup("fonts");
				CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
				CEGUI::WindowManager::setDefaultResourceGroup("layouts");
				CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
			}
			catch (CEGUI::Exception ce){
				logE << ce.getMessage() << std::endl;
				return false;
			}
			catch (...){
				logE << "Erreur lors de l'initialisation de cegui !" << std::endl;
				return false;
			}
		}

		m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
		m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
		m_context->setRootWindow(m_root);
		return true;
	}

	void Gui::clear()
	{
		while (m_root->getChildCount() > 0)
		{
			if (m_root->isChild((size_t)0))
			{
				CEGUI::Window* w = m_root->getChildAtIdx(0);
				m_root->destroyChild(w);
			}
		}
	}

	void Gui::destroy()
	{
		CEGUI::System::getSingleton().destroyGUIContext(*m_context);
	}

	void Gui::update()
	{
		unsigned int elapsed;
		if (!m_lastTime) {
			elapsed = 0;
			m_lastTime = SDL_GetTicks();
		}
		else {
			unsigned int nextTime = SDL_GetTicks();;
			elapsed = nextTime - m_lastTime;
			m_lastTime = nextTime;
		}

		m_context->injectTimePulse((float)elapsed / 1000.0f);
	}

	void Gui::draw()
	{
		m_renderer->beginRendering();
		m_context->draw();
		m_renderer->endRendering();

		glDisable(GL_SCISSOR_TEST);
	}

	void Gui::loadScheme(const std::string& schemeFile){
		CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
	}

	void Gui::loadImageset(const std::string& imagesetName)
	{
		CEGUI::ImageManager::getSingleton().loadImageset(imagesetName);
	}

	CEGUI::Image& Gui::creatImage(const std::string& type, const std::string& name)
	{
		return CEGUI::ImageManager::getSingleton().create(type, name);
	}

	void Gui::setMouseCursor(const std::string& cursorFile){
		m_context->getMouseCursor().setDefaultImage(cursorFile);
	}

	void Gui::showMouseCursor(){
		m_context->getMouseCursor().show();
	}

	void Gui::hideMouseCursor(){
		m_context->getMouseCursor().hide();
	}

	void Gui::setFont(const std::string& fontFile){
		CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
		m_context->setDefaultFont(fontFile);
	}

	CEGUI::Window* Gui::createWindow(const std::string& scheme, const glm::vec4& destRectPerc, const glm::vec4& destRectPix,
		const std::string& name){
		CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(scheme, name);
		m_root->addChild(newWindow);
		setWindowDestRect(newWindow, destRectPerc, destRectPix);
		return newWindow;
	}

	CEGUI::Window* Gui::createWindow(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name /*= ""*/) {
		CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
		parent->addChild(newWindow);
		setWindowDestRect(newWindow, destRectPerc, destRectPix);
		return newWindow;
	}

	CEGUI::Window* Gui::createWindow(CEGUI::Window* parent, const std::string& layout)
	{
		CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(layout);
		parent->addChild(newWindow);
		return newWindow;
	}

	CEGUI::Window* Gui::createWindow(const std::string& layout)
	{
		CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(layout);
		m_root->addChild(newWindow);
		return newWindow;
	}

	void Gui::setWindowDestRect(CEGUI::Window* window, const glm::vec4& destRectPerc, const glm::vec4& destRectPix){
		window->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
		window->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
	}

	void Gui::onSDLEvent(SDL_Event& ev)
	{
		CEGUI::utf32 codePoint;

		switch (ev.type){
		case SDL_QUIT:
			break;
		case SDL_MOUSEMOTION:
			m_context->injectMousePosition((float)ev.motion.x, (float)ev.motion.y);
			break;
		case SDL_KEYDOWN:
			m_context->injectKeyDown(sdlkeyTOceguikey(ev.key.keysym.sym));
			break;
		case SDL_KEYUP:
			m_context->injectKeyUp(sdlkeyTOceguikey(ev.key.keysym.sym));
			break;
		case SDL_TEXTINPUT:
			// Wrong conversion, for good conversion look at http://utfcpp.sourceforge.net
			codePoint = 0;
			for (int i = 0; ev.text.text[i] != '\0'; i++){
				codePoint |= (((CEGUI::utf32)ev.text.text[i]) << (i * 8));
			}
			m_context->injectChar(codePoint);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_context->injectMouseButtonDown(sdlbuttonTOceguibutton(ev.button.button));
			break;
		case SDL_MOUSEBUTTONUP:
			m_context->injectMouseButtonUp(sdlbuttonTOceguibutton(ev.button.button));
			break;
		}

		if (SDL_NumJoysticks() > 0 && ev.jbutton.which == 0) {
			sdlJoystickTOcegui(ev);
		}
	}

	void Gui::sdlJoystickTOcegui(SDL_Event& ev)
	{
		switch (ev.type) {
		case SDL_JOYBUTTONDOWN:
			if (ev.jbutton.button == Joystick::DPAD_LEFT - MARGIN)
			{
				m_context->injectKeyDown(CEGUI::Key::NumpadEnter);
			}
			else if (ev.jbutton.button == Joystick::DPAD_DOWN - MARGIN)
			{
				m_context->injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
			}
			else if (ev.jbutton.button == Joystick::DPAD_RIGHT - MARGIN)
			{
				m_context->injectKeyDown(CEGUI::Key::Escape);
			}
			else if (ev.jbutton.button == Joystick::DPAD_UP - MARGIN)
			{
				m_context->injectMouseButtonDown(CEGUI::MouseButton::RightButton);
			}
			break;
		case SDL_JOYBUTTONUP:
			if (ev.jbutton.button == Joystick::DPAD_LEFT - MARGIN)
			{
				m_context->injectKeyUp(CEGUI::Key::NumpadEnter);
			}
			else if (ev.jbutton.button == Joystick::DPAD_DOWN - MARGIN)
			{
				m_context->injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
			}
			else if (ev.jbutton.button == Joystick::DPAD_RIGHT - MARGIN)
			{
				m_context->injectKeyUp(CEGUI::Key::Escape);
			}
			else if (ev.jbutton.button == Joystick::DPAD_UP - MARGIN)
			{
				m_context->injectMouseButtonUp(CEGUI::MouseButton::RightButton);
			}
			break;
		case SDL_JOYHATMOTION:
			if (ev.jhat.hat == 0)
			{
				switch (ev.jhat.value)
				{
				case SDL_HAT_UP:
					m_context->injectKeyDown(CEGUI::Key::ArrowUp);
					break;
				case SDL_HAT_DOWN:
					m_context->injectKeyDown(CEGUI::Key::ArrowDown);
					break;
				case SDL_HAT_RIGHT:
					m_context->injectKeyDown(CEGUI::Key::ArrowRight);
					break;
				case SDL_HAT_LEFT:
					m_context->injectKeyDown(CEGUI::Key::ArrowLeft);
					break;
				}
			}
			break;
		}
	}

	CEGUI::MouseButton Gui::sdlbuttonTOceguibutton(Uint8 sdlButton){
		using namespace CEGUI;
		switch (sdlButton)
		{
		case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
		case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
		case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
		case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
		case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
		}
		return CEGUI::MouseButton::NoButton;
	}

	CEGUI::Key::Scan Gui::sdlkeyTOceguikey(SDL_Keycode key){
		using namespace CEGUI;
		switch (key)
		{
		case SDLK_BACKSPACE:    return Key::Backspace;
		case SDLK_TAB:          return Key::Tab;
		case SDLK_RETURN:       return Key::Return;
		case SDLK_PAUSE:        return Key::Pause;
		case SDLK_ESCAPE:       return Key::Escape;
		case SDLK_SPACE:        return Key::Space;
		case SDLK_COMMA:        return Key::Comma;
		case SDLK_MINUS:        return Key::Minus;
		case SDLK_PERIOD:       return Key::Period;
		case SDLK_SLASH:        return Key::Slash;
		case SDLK_0:            return Key::Zero;
		case SDLK_1:            return Key::One;
		case SDLK_2:            return Key::Two;
		case SDLK_3:            return Key::Three;
		case SDLK_4:            return Key::Four;
		case SDLK_5:            return Key::Five;
		case SDLK_6:            return Key::Six;
		case SDLK_7:            return Key::Seven;
		case SDLK_8:            return Key::Eight;
		case SDLK_9:            return Key::Nine;
		case SDLK_COLON:        return Key::Colon;
		case SDLK_SEMICOLON:    return Key::Semicolon;
		case SDLK_EQUALS:       return Key::Equals;
		case SDLK_LEFTBRACKET:  return Key::LeftBracket;
		case SDLK_BACKSLASH:    return Key::Backslash;
		case SDLK_RIGHTBRACKET: return Key::RightBracket;
		case SDLK_a:            return Key::A;
		case SDLK_b:            return Key::B;
		case SDLK_c:            return Key::C;
		case SDLK_d:            return Key::D;
		case SDLK_e:            return Key::E;
		case SDLK_f:            return Key::F;
		case SDLK_g:            return Key::G;
		case SDLK_h:            return Key::H;
		case SDLK_i:            return Key::I;
		case SDLK_j:            return Key::J;
		case SDLK_k:            return Key::K;
		case SDLK_l:            return Key::L;
		case SDLK_m:            return Key::M;
		case SDLK_n:            return Key::N;
		case SDLK_o:            return Key::O;
		case SDLK_p:            return Key::P;
		case SDLK_q:            return Key::Q;
		case SDLK_r:            return Key::R;
		case SDLK_s:            return Key::S;
		case SDLK_t:            return Key::T;
		case SDLK_u:            return Key::U;
		case SDLK_v:            return Key::V;
		case SDLK_w:            return Key::W;
		case SDLK_x:            return Key::X;
		case SDLK_y:            return Key::Y;
		case SDLK_z:            return Key::Z;
		case SDLK_DELETE:       return Key::Delete;
		case SDLK_KP_PERIOD:    return Key::Decimal;
		case SDLK_KP_DIVIDE:    return Key::Divide;
		case SDLK_KP_MULTIPLY:  return Key::Multiply;
		case SDLK_KP_MINUS:     return Key::Subtract;
		case SDLK_KP_PLUS:      return Key::Add;
		case SDLK_KP_ENTER:     return Key::NumpadEnter;
		case SDLK_KP_EQUALS:    return Key::NumpadEquals;
		case SDLK_UP:           return Key::ArrowUp;
		case SDLK_DOWN:         return Key::ArrowDown;
		case SDLK_RIGHT:        return Key::ArrowRight;
		case SDLK_LEFT:         return Key::ArrowLeft;
		case SDLK_INSERT:       return Key::Insert;
		case SDLK_HOME:         return Key::Home;
		case SDLK_END:          return Key::End;
		case SDLK_PAGEUP:       return Key::PageUp;
		case SDLK_PAGEDOWN:     return Key::PageDown;
		case SDLK_F1:           return Key::F1;
		case SDLK_F2:           return Key::F2;
		case SDLK_F3:           return Key::F3;
		case SDLK_F4:           return Key::F4;
		case SDLK_F5:           return Key::F5;
		case SDLK_F6:           return Key::F6;
		case SDLK_F7:           return Key::F7;
		case SDLK_F8:           return Key::F8;
		case SDLK_F9:           return Key::F9;
		case SDLK_F10:          return Key::F10;
		case SDLK_F11:          return Key::F11;
		case SDLK_F12:          return Key::F12;
		case SDLK_F13:          return Key::F13;
		case SDLK_F14:          return Key::F14;
		case SDLK_F15:          return Key::F15;
		case SDLK_RSHIFT:       return Key::RightShift;
		case SDLK_LSHIFT:       return Key::LeftShift;
		case SDLK_RCTRL:        return Key::RightControl;
		case SDLK_LCTRL:        return Key::LeftControl;
		case SDLK_RALT:         return Key::RightAlt;
		case SDLK_LALT:         return Key::LeftAlt;
		case SDLK_SYSREQ:       return Key::SysRq;
		case SDLK_MENU:         return Key::AppMenu;
		case SDLK_POWER:        return Key::Power;
		default:                return Key::Unknown;
		}
	}
}
