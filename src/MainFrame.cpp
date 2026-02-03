#include "MainFrame.h"

// Define the event table to map GUI events to class functions
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MainFrame::OnItemActivated)
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
    m_pathBar = new wxTextCtrl(panel, wxID_ANY, m_logic.GetCurrentPath().string());
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
    
    // Get real filesystem data from our logic class
    auto entries = m_logic.GetDirectoryContents(m_logic.GetCurrentPath());
    
    for (size_t i = 0; i < entries.size(); ++i) {
        // Insert name into the first column
        long index = m_fileList->InsertItem(i, entries[i].name);
        
        // Populate the remaining columns
        m_fileList->SetItem(index, 1, entries[i].type);
        m_fileList->SetItem(index, 2, entries[i].size);
        m_fileList->SetItem(index, 3, entries[i].modified);
    }
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

// Stubs for next steps
void MainFrame::OnItemActivated(wxListEvent& event) { /* Navigation logic here */ }
void MainFrame::SetupMenuBar() { /* File/Edit menu logic here */ }