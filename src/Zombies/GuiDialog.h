#pragma once

#include <IGuiWindow.h>

enum class DialogType {
	QUESTION, CONFIRM, SELECT
};

class GuiDialog : public NS2::IGuiWindow
{
public:
	void init(NS2::Gui& gui, CEGUI::Window* parent);
	void initAll(unsigned int id, const std::string& title, const std::string& msg, DialogType type);
	void initConfirm(unsigned int id, const std::string& title, const std::string& msg);
	void initQuestion(unsigned int id, const std::string& title, const std::string& msg);
	void initSelect(unsigned int id, std::vector<CEGUI::ListboxTextItem*>& list, const std::string& title, const std::string& msg);

	/* SETTERS */
	void setTitle(const std::string& title);
	void setMsg(const std::string& msg);
	void setOk(const std::string& ok);
	void setCancel(const std::string& cancel);

	/* GETTERS */
	DialogType getType(){ return m_type; }
	unsigned int getId(){ return m_id; }
	bool isOk(){ return m_ok; }
	std::string getTextValue();
	float getNumericValue();

private:
	bool onOkClick(const CEGUI::EventArgs& e);
	bool onCancelClick(const CEGUI::EventArgs& e);

	bool m_ok;
	unsigned int m_id;
	DialogType m_type;
	NS2::Gui* m_gui;
	CEGUI::Window* m_parent = nullptr;
	CEGUI::Window* m_body = nullptr;
	CEGUI::Window* m_lblMsg = nullptr;
	CEGUI::PushButton* m_btOk = nullptr;
	CEGUI::PushButton* m_btCancel = nullptr;
};

