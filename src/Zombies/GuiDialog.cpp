#include "GuiDialog.h"

void GuiDialog::init(NS2::Gui& gui, CEGUI::Window* parent)
{
	m_gui = &gui;
	m_window = gui.createWindow("NS_dialog.layout");
	m_parent = parent;

	m_btOk = static_cast<CEGUI::PushButton*> (m_window->getChild("btn_ok"));
	m_btOk->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GuiDialog::onOkClick, this));

	m_btCancel = static_cast<CEGUI::PushButton*> (m_window->getChild("btn_cancel"));
	m_btCancel->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GuiDialog::onCancelClick, this));

	m_lblMsg = static_cast<CEGUI::Window*> (m_window->getChild("txt_msg"));
	m_window->setVisible(false);
}

void GuiDialog::initAll(unsigned int id, const std::string& title, const std::string& msg, DialogType type)
{
	m_id = id;
	m_type = type;
	m_ok = false;
	m_window->setVisible(true);
	m_window->setModalState(m_parent != nullptr);
	m_window->setText(title);
	m_lblMsg->setText(msg);
	m_btOk->setVisible(true);
	m_btCancel->setVisible(true);
	if (m_parent != nullptr)
	{
		m_parent->setEnabled(false);
		CEGUI::UDim x(m_parent->getXPosition().d_scale + ((m_parent->getWidth().d_scale - m_window->getWidth().d_scale) / 2.0f),
			m_parent->getXPosition().d_offset);
		CEGUI::UDim y(m_parent->getYPosition().d_scale + ((m_parent->getHeight().d_scale - m_window->getHeight().d_scale) / 2.0f),
			m_parent->getXPosition().d_offset);
		m_window->setPosition(CEGUI::UVector2(x, y));
	}
	else
	{
		CEGUI::UDim x(0.5f - (m_window->getWidth().d_scale / 2.0f), 5.0f);
		CEGUI::UDim y(0.5f - (m_window->getHeight().d_scale / 2.0f), 5.0f);
		m_window->setPosition(CEGUI::UVector2(x, y));
	}
	if (m_body != nullptr)
	{
		m_window->removeChild(m_body);
		m_body->destroy();
		m_body = nullptr;
	}
}
void GuiDialog::initConfirm(unsigned int id, const std::string& title, const std::string& msg)
{
	initAll(id, title, msg, DialogType::CONFIRM);
	m_btCancel->setVisible(false);
}
void GuiDialog::initQuestion(unsigned int id, const std::string& title, const std::string& msg)
{
	initAll(id, title, msg, DialogType::QUESTION);
}
void GuiDialog::initSelect(unsigned int id, std::vector<CEGUI::ListboxTextItem*>& list, 
	const std::string& title, const std::string& msg)
{
	initAll(id, title, msg, DialogType::SELECT);
	m_body = static_cast<CEGUI::Combobox*>(m_gui->createWindow(m_window, "TaharezLook/Combobox", 
		glm::vec4(0.0f, 0.3f, 1.0f, 0.5f), glm::vec4(5.0f, 0.0f, -5.0f, 0.0f), "SaveCombobox"));
	if (m_body != nullptr)
	{
		for (CEGUI::ListboxTextItem* item : list)
		{
			static_cast<CEGUI::Combobox*>(m_body)->addItem(item);
			static_cast<CEGUI::Combobox*>(m_body)->setReadOnly(true);
		}
		m_window->addChild(m_body);
	}
}

bool GuiDialog::onOkClick(const CEGUI::EventArgs& e)
{
	m_ok = true;

	if (m_parent != nullptr)
	{
		m_parent->setEnabled(true);
	}
	m_window->setModalState(false);
	m_window->setVisible(false);
	return true;
}
bool GuiDialog::onCancelClick(const CEGUI::EventArgs& e)
{
	m_ok = false;
	
	if (m_parent != nullptr)
	{
		m_parent->setEnabled(true);
	}
	m_window->setModalState(false);
	m_window->setVisible(false);
	return true;
}

void GuiDialog::setTitle(const std::string& title)
{
	m_window->setText(title);
}
void GuiDialog::setMsg(const std::string& msg)
{
	m_lblMsg->setText(msg);
}
void GuiDialog::setOk(const std::string& ok)
{
	m_btOk->setText(ok);
}
void GuiDialog::setCancel(const std::string& cancel)
{
	m_btCancel->setText(cancel);
}

std::string GuiDialog::getTextValue()
{
	switch (m_type)
	{
	case DialogType::QUESTION:
		break;
	case DialogType::CONFIRM:
		break;
	case DialogType::SELECT:
		CEGUI::Combobox* c = static_cast<CEGUI::Combobox*>(m_body);
		if (c->getSelectedItem() != nullptr)
			return c->getSelectedItem()->getText().c_str();
		break;
	}
	return "";
}
float GuiDialog::getNumericValue()
{
	return 0.0f;
}