/*
 * Author: Mathew Lane
 * Description: <short file description>
 * Date: 2026-02-02
 */

#include "MainFrame.h"

// Define the event table to map GUI events to class functions
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    // Application Controls
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    
    // File Navigation & Interaction
    EVT_TEXT_ENTER(wxID_ANY, MainFrame::OnPathEnter)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MainFrame::OnItemActivated)
    
    // File Operations
    EVT_MENU(ID_CREATE_FOLDER, MainFrame::OnCreateFolder)
    EVT_MENU(ID_RENAME, MainFrame::OnRename)
    EVT_MENU(ID_DELETE, MainFrame::OnDelete)
    
    // Virtual Clipboard Operations
    EVT_MENU(ID_COPY, MainFrame::OnCopy)
    EVT_MENU(ID_CUT, MainFrame::OnCut)
    EVT_MENU(ID_PASTE, MainFrame::OnPaste)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) 
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    CreateControls();
    SetupMenuBar();
    CreateStatusBar(2);
    SetStatusText("Ready", 0);
    
    // Initial load of the current directory
    UpdateList();
}

MainFrame::~MainFrame() {
    // Child UI elements (m_fileList, etc.) are deleted by the wxFrame parent
}

void MainFrame::CreateControls() {
    // Main panel to hold all UI elements
    wxPanel* panel = new wxPanel(this);
    
    // Vertical sizer to stack the path bar and the file list
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Path bar for address input/display
    m_pathBar = new wxTextCtrl(panel, wxID_ANY, m_logic.GetCurrentPath().string(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_pathBar, 0, wxEXPAND | wxALL, 5);

    // Initialize the list control in "Report" mode (grid view)
    m_fileList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    
    // Requirement: Add the four mandatory columns
    m_fileList->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 300);
    m_fileList->InsertColumn(1, "Type", wxLIST_FORMAT_LEFT, 100);
    m_fileList->InsertColumn(2, "Size", wxLIST_FORMAT_RIGHT, 100);
    m_fileList->InsertColumn(3, "Date Modified", wxLIST_FORMAT_LEFT, 150);

    mainSizer->Add(m_fileList, 1, wxEXPAND | wxALL, 5);
    panel->SetSizer(mainSizer);
}

void MainFrame::UpdateList() {
    m_fileList->DeleteAllItems();
    
    fs::path current = m_logic.GetCurrentPath();
    
    // Add ".." entry if a parent directory exists
    if (current.has_parent_path() && current != current.root_path()) {
        long index = m_fileList->InsertItem(0, "..");
        m_fileList->SetItem(index, 1, "Folder");
        m_fileList->SetItem(index, 2, "--");
        m_fileList->SetItem(index, 3, "--");
    }

    auto entries = m_logic.GetDirectoryContents(current);
    
    for (const auto& entry : entries) {
        // Use GetItemCount() to always append to the end of the list
        long index = m_fileList->InsertItem(m_fileList->GetItemCount(), entry.name);
        m_fileList->SetItem(index, 1, entry.type);
        m_fileList->SetItem(index, 2, entry.size);
        m_fileList->SetItem(index, 3, entry.modified);
    }
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnItemActivated(wxListEvent& event) {
    // Get the index of the double-clicked item
    long index = event.GetIndex();
    
    // Retrieve the name of the file/folder from the first column
    wxString itemName = m_fileList->GetItemText(index, 0);
    
    // Construct the potential new path
    fs::path newPath = (m_logic.GetCurrentPath() / itemName.ToStdString()).lexically_normal();

    if (fs::is_directory(newPath)) {
        m_logic.SetCurrentPath(newPath);
        m_pathBar->SetValue(newPath.string()); 
        UpdateList();
    }

    // Only navigate if it's actually a directory
    if (fs::is_directory(newPath)) {
        m_logic.SetCurrentPath(newPath);
        m_pathBar->SetValue(newPath.string()); // Update the UI address bar
        UpdateList(); // Refresh the file list view
    } else if (fs::exists(newPath)) {
        wxString pathString = wxString::FromUTF8(newPath.string().c_str());

        // 1. First attempt: Linux native default
        if (!wxLaunchDefaultApplication(pathString)) {
            
            // 2. Second attempt: Fallback to Windows via wslview
            wxString wslCommand = wxString::Format("wslview \"%s\"", pathString);
            
            // wxExecute returns -1 if the command itself fails to run
            if (wxExecute(wslCommand, wxEXEC_ASYNC) == -1) {
                
                // 3. Final Fallback: Display error message
                wxMessageBox("Could not open the file with Linux or Windows applications.", 
                            "Open Error", wxOK | wxICON_ERROR);
            }
        }
    }
}

void MainFrame::OnPathEnter(wxCommandEvent& event) {
    std::string typedPath = m_pathBar->GetValue().ToStdString();
    fs::path newPath(typedPath);

    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        m_logic.SetCurrentPath(newPath);
        UpdateList();
    } else {
        // Simple feedback if the path is invalid
        wxMessageBox("The directory does not exist.", "Navigation Error", wxOK | wxICON_ERROR);
        // Reset the bar to the actual current path
        m_pathBar->SetValue(m_logic.GetCurrentPath().string());
    }
}

void MainFrame::SetupMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar();

    // File Menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_CREATE_FOLDER, "New &Folder\tCtrl+N");
    fileMenu->Append(ID_RENAME, "&Rename\tCtrl+R");
    fileMenu->Append(ID_DELETE, "&Delete\tDel");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt+X");

    // Edit Menu
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(ID_COPY, "&Copy\tCtrl+C");
    editMenu->Append(ID_CUT, "Cu&t\tCtrl+X");
    editMenu->Append(ID_PASTE, "&Paste\tCtrl+V");

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(editMenu, "&Edit");

    SetMenuBar(menuBar); // This attaches it to the very top of the window
}

void MainFrame::OnCreateFolder(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter folder name:", "New Folder");
    
    if (dialog.ShowModal() == wxID_OK) {
        if (m_logic.CreateFolder(dialog.GetValue().ToStdString())) {
            UpdateList();
        } else {
            wxMessageBox(m_logic.GetLastError(), "Folder Creation Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnRename(wxCommandEvent& event) {
    long index = m_fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index != -1) {
        wxString oldName = m_fileList->GetItemText(index);
        wxTextEntryDialog dialog(this, "Enter new name:", "Rename", oldName);
        
        if (dialog.ShowModal() == wxID_OK) {
            fs::path oldPath = m_logic.GetCurrentPath() / oldName.ToStdString();
            if (m_logic.RenameItem(oldPath, dialog.GetValue().ToStdString())) {
                UpdateList();
            } else {
                wxMessageBox(m_logic.GetLastError(), "Rename Error", wxOK | wxICON_ERROR);
            }
        }
    }
}

void MainFrame::OnDelete(wxCommandEvent& event) {
    long index = m_fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index != -1) {
        wxString name = m_fileList->GetItemText(index);
        int answer = wxMessageBox("Are you sure you want to delete '" + name + "'?", 
                                  "Confirm Delete", wxYES_NO | wxICON_WARNING);
        
        if (answer == wxYES) {
            fs::path toDelete = m_logic.GetCurrentPath() / name.ToStdString();
            if (m_logic.DeleteItem(toDelete)) {
                UpdateList();
            }
        }
    }
}

void MainFrame::OnCopy(wxCommandEvent& event) {
    long index = m_fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index != -1) {
        fs::path selectedPath = m_logic.GetCurrentPath() / m_fileList->GetItemText(index).ToStdString();
        m_logic.Copy(selectedPath);
        SetStatusText("Copied: " + m_fileList->GetItemText(index), 0);
    }
}

void MainFrame::OnCut(wxCommandEvent& event) {
    long index = m_fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (index != -1) {
        fs::path selectedPath = m_logic.GetCurrentPath() / m_fileList->GetItemText(index).ToStdString();
        m_logic.Cut(selectedPath);
        SetStatusText("Cut: " + m_fileList->GetItemText(index), 0);
    }
}

void MainFrame::OnPaste(wxCommandEvent& event) {
    fs::path source = m_logic.GetClipboardPath();
    if (source.empty()) return;

    fs::path target = m_logic.GetCurrentPath() / source.filename();
    bool overwrite = false;

    // Check if the file already exists at the destination
    if (fs::exists(target)) {
        int answer = wxMessageBox("File already exists. Would you like to overwrite it?", 
                                  "Confirm Overwrite", wxYES_NO | wxICON_QUESTION);
        if (answer != wxYES) return; // User cancelled the operation
        overwrite = true;
    }

    if (m_logic.Paste(m_logic.GetCurrentPath(), overwrite)) {
        UpdateList();
        SetStatusText("Clipboard is now empty", 0);
    } else {
        wxMessageBox(m_logic.GetLastError(), "Paste Error", wxOK | wxICON_ERROR);
    }
}
