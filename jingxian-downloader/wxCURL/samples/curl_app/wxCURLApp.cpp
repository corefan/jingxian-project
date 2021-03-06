/*******************************************
 *	base.cpp
 *	Created by Casey O'Donnell on Tue Jun 29 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *	Licence: wxWidgets Licence
 ******************************************/

#include "wxcurl/wxcurl_config.h"
#include <wx/xrc/xmlres.h>
#include <wxcurl/dav.h>
#include "wxCURLApp.h"
#include "wxCURLDlg.h"
#include "resource.h"

// Create a new application object
IMPLEMENT_APP(wxCURLApp)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCURLApp::wxCURLApp()
{
}

wxCURLApp::~wxCURLApp()
{
	// Shutdown CurlLib
	wxCurlDAV::Shutdown();
}

//////////////////////////////////////////////////////////////////////
// Overridden Virtual Methods
//////////////////////////////////////////////////////////////////////

bool wxCURLApp::OnInit()
{
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load("resource/main_dialog.xrc");
	wxXmlResource::Get()->Load("resource/get_dialog.xrc");
	wxXmlResource::Get()->Load("resource/get_file_dialog.xrc");
	wxXmlResource::Get()->Load("resource/put_dialog.xrc");
	wxXmlResource::Get()->Load("resource/put_text_dialog.xrc");
	wxXmlResource::Get()->Load("resource/mkcol_dialog.xrc");
	wxXmlResource::Get()->Load("resource/delete_dialog.xrc");
	wxXmlResource::Get()->Load("resource/propfind_dialog.xrc");
	wxXmlResource::Get()->Load("resource/getfs_dialog.xrc");
	wxXmlResource::Get()->Load("resource/options_dialog.xrc");
	wxXmlResource::Get()->Load("resource/head_dialog.xrc");
	wxXmlResource::Get()->Load("resource/trace_dialog.xrc");
	wxXmlResource::Get()->Load("resource/post_dialog.xrc");
	wxXmlResource::Get()->Load("resource/get_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/get_file_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/put_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/put_text_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/mkdir_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/rmdir_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/delete_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/rename_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/info_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/list_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/nlst_ftp_dialog.xrc");
	wxXmlResource::Get()->Load("resource/getfs_ftp_dialog.xrc");

	// create the main application window
	wxCURLDialog *dialog = new wxCURLDialog(_("wxCURL"),
								wxPoint(50, 50), wxSize(400, 300));

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	dialog->Show(TRUE);
	SetTopWindow(dialog);

	// Initialize LibCURL
	wxCurlDAV::Init();

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned FALSE here, the
	// application would exit immediately.
	return TRUE;
}

