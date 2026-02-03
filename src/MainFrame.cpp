#include "MainFrame.h"

// Define the event table to map GUI events to class functions
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MainFrame::OnItemActivated)
    EVT_TEXT_ENTER(wxID_ANY, MainFrame::OnPathEnter)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) 
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    CreateControls();
    SetupMenuBar();
    
    // Initial load of the current directory
    UpdateList();
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
    m_fileList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    
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
        // Requirement: Open file using system's default application
        wxString pathString = wxString::FromUTF8(newPath.string().c_str());
        
        if (!wxLaunchDefaultApplication(pathString)) {
            wxMessageBox("Could not open the file with the default application.", 
                         "Error", wxOK | wxICON_ERROR);
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
void MainFrame::SetupMenuBar() { /* File/Edit menu logic here */ }