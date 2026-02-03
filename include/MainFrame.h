#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "FileManagerLogic.h"

class MainFrame : public wxFrame {
public:
    // Initializes the main window with a title and dimensions
    MainFrame(const wxString& title);

private:
    // UI Components as specified in the assignment requirements
    wxListCtrl* m_fileList;    // Displays file name, type, size, and date
    wxTextCtrl* m_pathBar;     // Allows user to type or view the current path
    FileManagerLogic m_logic;  // Object that handles actual disk operations

    // Setup methods to keep the constructor clean
    void CreateControls();
    void SetupMenuBar();
    void UpdateList();

    // Event handlers for user interactions
    void OnOpen(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnItemActivated(wxListEvent& event); // Triggered by double-clicking items
    void OnPathEnter(wxCommandEvent& event);

    // Tells wxWidgets this class handles its own events
    wxDECLARE_EVENT_TABLE();
};

#endif