/*
 * Author: Mathew Lane
 * Description: Implements the OnInit function to instantiate and display the main application frame.
 * Date: 2026-02-02
 */

#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MyApp);

/*
 * Function: OnInit
 * Description: initializes the main application frame and displays it
 * Parameters: none
 * Returns: bool indicating success or failure of initialization
 */
bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame("CS3307 File Manager");
    frame->Show(true);
    return true;
}

/*
 * Function: ~MyApp
 * Description: destructor for MyApp class
 * Parameters: none
 * Returns: void
 */
MyApp::~MyApp() {}
