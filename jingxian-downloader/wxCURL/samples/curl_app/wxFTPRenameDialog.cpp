/*******************************************
 *	base.cpp
 *	Created by Casey O'Donnell on Tue Jun 29 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *	Licence: wxWidgets Licence
 ******************************************/

#include "wxcurl/wxcurl_config.h"
#include <wx/xrc/xmlres.h>

#include <wxcurl/ftp.h>

#include "wxFTPRenameDialog.h"

//////////////////////////////////////////////////////////////////////
// Resources
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Event Tables and Other Macros for wxWindows
//////////////////////////////////////////////////////////////////////

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
IMPLEMENT_CLASS(wxFTPRenameDialog, wxDialog)
BEGIN_EVENT_TABLE(wxFTPRenameDialog, wxDialog)
	EVT_BUTTON(XRCID("rename_button"), wxFTPRenameDialog::OnRename)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxFTPRenameDialog::wxFTPRenameDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, "rename_ftp_dialog");

	SetSize(400, -1);

	m_pOrigCtrl		= XRCCTRL(*this, "orig_text_ctrl", wxTextCtrl);
	m_pNewCtrl		= XRCCTRL(*this, "new_text_ctrl", wxTextCtrl);
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pResponseCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pOrigCtrl && m_pNewCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultOrig		= m_pOrigCtrl->GetValue();
		m_szDefaultNew		= m_pNewCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}
}

wxFTPRenameDialog::~wxFTPRenameDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxFTPRenameDialog::OnRename(wxCommandEvent& WXUNUSED(event))
{
	if(m_pOrigCtrl && m_pNewCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szOrig		= m_pOrigCtrl->GetValue();
		wxString szNew		= m_pNewCtrl->GetValue();
		wxString szUser		= m_pUserCtrl->GetValue();
		wxString szPass		= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szOrig == m_szDefaultOrig) || (szNew == m_szDefaultNew))
		{
			wxMessageBox("Please change the ORIGINAL and NEW location.", "Error...", wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox("Please change the username or password.", "Error...", wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do it!
			wxCurlFTP ftp(szOrig, szUser, szPass);

			if(ftp.Rename(szNew))
			{
					szResponse = "SUCCESS!\n\n";
					szResponse += wxString::Format("\nResponse Code: %d\n\n", ftp.GetResponseCode());
					szResponse += ftp.GetResponseHeader();
					szResponse += "\n\n";
					szResponse += ftp.GetResponseBody();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
			else
			{
					szResponse = "FAILURE!\n\n";
					szResponse += wxString::Format("\nResponse Code: %d\n\n", ftp.GetResponseCode());
					szResponse += ftp.GetResponseHeader();
					szResponse += "\n\n";
					szResponse += ftp.GetResponseBody();
					szResponse += "\n\n";
					szResponse += ftp.GetErrorString();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
		}
	}
}
