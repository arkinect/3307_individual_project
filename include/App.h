/*
 * Author: Mathew Lane
 * Description: Declares the MyApp class to manage the application's lifecycle and global initialization.
 * Date: 2026-02-02
 */

#ifndef APP_H
#define APP_H

#include <wx/wx.h>

// MyApp inherits from wxApp to handle the application lifecycle
class MyApp : public wxApp {
public:
    // OnInit is the virtual function where the program starts
    virtual bool OnInit();
    virtual ~MyApp();
};

// Tells wxWidgets to create an instance of this class
wxDECLARE_APP(MyApp);

#endif
