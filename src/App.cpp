/*
 * Author: Mathew Lane
 * Description: <short file description>
 * Date: 2026-02-02
 */

#include "App.h"
#include "MainFrame.h"

// Macro that generates the main() function for you
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Create the main window instance
    MainFrame* frame = new MainFrame("CS3307 File Manager");
    
    // Make the window visible on screen
    frame->Show(true);
    
    // Return true to start the event loop; false would exit immediately
    return true;
}

MyApp::~MyApp() {
    // wxWidgets handles cleanup of the MainFrame automatically
}
