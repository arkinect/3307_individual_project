/*
 * Author: Mathew Lane
 * Description: Declares the MyApp class to manage the application's lifecycle and global initialization.
 * Date: 2026-02-02
 */

#ifndef APP_H
#define APP_H

#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
    virtual ~MyApp();
};

wxDECLARE_APP(MyApp);

#endif
