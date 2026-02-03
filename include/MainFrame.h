/*
 * Author: Mathew Lane
 * Description: Defines the UI window class, interface component pointers, and private event handler declarations.
 * Date: 2026-02-02
 */

#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "FileManagerLogic.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    virtual ~MainFrame();

    // disallow multiple instances
    MainFrame(const MainFrame&) = delete;
    MainFrame& operator=(const MainFrame&) = delete;

private:
    enum {
        ID_RENAME = 1,
        ID_DELETE,
        ID_COPY,
        ID_CUT,
        ID_PASTE,
        ID_CREATE_FOLDER
    };

    void CreateControls();
    void UpdateList();
    void SetupMenuBar();
    
    // event handlers
    void OnExit(wxCommandEvent& event);
    void OnItemActivated(wxListEvent& event);
    void OnPathEnter(wxCommandEvent& event);
    void OnCreateFolder(wxCommandEvent& event);
    void OnRename(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);

    // UI components 
    wxListCtrl* m_fileList;
    wxTextCtrl* m_pathBar;
    FileManagerLogic m_logic;

    // class handles own events
    wxDECLARE_EVENT_TABLE();
};

#endif
