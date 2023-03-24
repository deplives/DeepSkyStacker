#pragma once
#include "DSSProgress.h"

namespace Ui {
	class ProgressDlg;
}

namespace DSS
{
	class ProgressConsole : public ProgressBase
	{
	private:
		QTextStream m_out;
		TERMINAL_OUTPUT_MODE m_style;
		QString m_strLastSent[3];

	public:
		ProgressConsole(TERMINAL_OUTPUT_MODE mode) :
			ProgressBase(),
			m_out(stdout),
			m_style(mode)
		{
		}
		virtual ~ProgressConsole()
		{
			if(m_style == TERMINAL_OUTPUT_MODE::COLOURED)
				m_out << "\033[0m";
			Close();
		}

		virtual void initialise() override{};
		virtual void applyStart1Text(const QString& strText) override { PrintText(strText, OT_TEXT1); }
		virtual void applyStart2Text(const QString& strText) override 
		{
			if(m_total2>0)
				PrintText(strText, OT_TEXT2); 
		}
		virtual void applyProgress1(int lAchieved) override
		{
			PrintText(GetProgress1Text(), OT_PROGRESS1);
		}
		virtual void applyProgress2(int lAchieved) override
		{
			PrintText(GetProgress2Text(), OT_PROGRESS2);
		}
		virtual void applyTitleText(const QString& strText) override { PrintText(strText, OT_TITLE); }

		virtual void endProgress2() override {}
		virtual bool hasBeenCanceled() override { return false; }
		virtual void closeProgress() { }
		virtual bool doWarning(const QString& szText) override { return true; }
		virtual void applyProcessorsUsed(int nCount) override {};

	private:
		void PrintText(const QString& szText, eOutputType type)
		{
			QString singleLineText(szText);
			singleLineText.replace('\n', ' ');

			switch (m_style)
			{
			case TERMINAL_OUTPUT_MODE::BASIC:
				PrintBasic(singleLineText, type, false);
				break;
			case TERMINAL_OUTPUT_MODE::COLOURED:
				PrintBasic(singleLineText, type, true);
				break;
			}
		}
		void PrintBasic(const QString& szText, eOutputType type, bool bColour)
		{
			switch(type)
			{
			case OT_TITLE:
				if (m_strLastSent[0].compare(szText) != 0)
				{
					m_out << (bColour ? "\033[036m" : "") << szText << "\n";
					m_strLastSent[0] = szText;
				}
				break;
			case OT_TEXT1:
				if (m_strLastSent[1].compare(szText) != 0)
				{
					// Don't echo out if the same as the title text.
					if (m_strLastSent[1].compare(m_strLastSent[0]) != 0)
						m_out << (bColour ? "\033[1;33m" : "") << szText << "\n";
					m_strLastSent[1] = szText;
				}
				break;
			case OT_TEXT2:
				if (m_strLastSent[2].compare(szText) != 0)
				{
					// Don't echo out if the same as the detail text
					if(m_strLastSent[2].compare(m_strLastSent[1]) != 0)
						m_out << (bColour ? "\033[1;33m" : "") << szText << "\n";
					m_strLastSent[2] = szText;
				}
					
				break;
			case OT_PROGRESS1:
			case OT_PROGRESS2:
				m_out << (bColour ? "\033[32m" : "") << szText << "\r";
				break;
			}
			m_out.flush();
		}
	};
}
