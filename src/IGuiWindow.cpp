#include "IGuiWindow.h"

namespace NS2
{
	IGuiWindow::~IGuiWindow()
	{
		if (m_nextWindow != nullptr)
			destroyNext();
	}
	void IGuiWindow::destroy()
	{
		CEGUI::Window* parent = m_window->getParent();
		if (parent != nullptr)
		{
			parent->destroyChild(m_window);
		}
	}

	void IGuiWindow::destroyNext()
	{
		if (m_nextWindow != nullptr)
		{
			m_nextWindow->destroy();
			delete m_nextWindow;
			m_nextWindow = nullptr;
		}
	}

	void IGuiWindow::next()
	{
		if (m_nextWindow != nullptr)
		{
			m_window->hide();
			m_nextWindow->getWindow()->show();
		}
	}

	void IGuiWindow::previous()
	{
		if (m_previousWindow != nullptr)
		{
			m_window->hide();
			m_previousWindow->getWindow()->show();
			m_previousWindow->destroyNext();
		}
	}
}
