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
    // Initializes the main window with a title and dimensions
    MainFrame(const wxString& title);
    virtual ~MainFrame();

    // Disable copy constructor and assignment operator
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

    // Setup methods to keep the constructor clean
    void CreateControls();
    void UpdateList();
    void SetupMenuBar();
    

    // Event handlers for user interactions
    void OnExit(wxCommandEvent& event);
    void OnItemActivated(wxListEvent& event); // Triggered by double-clicking items
    void OnPathEnter(wxCommandEvent& event);
    void OnCreateFolder(wxCommandEvent& event);
    void OnRename(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);

    // UI Components as specified in the assignment requirements
    wxListCtrl* m_fileList;    // Displays file name, type, size, and date
    wxTextCtrl* m_pathBar;     // Allows user to type or view the current path
    FileManagerLogic m_logic;  // Object that handles actual disk operations

    // Tells wxWidgets this class handles its own events
    wxDECLARE_EVENT_TABLE();
};

#endif
