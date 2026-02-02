#include <wx/wx.h>

class SimpleFrame : public wxFrame {
public:
    SimpleFrame() : wxFrame(nullptr, wxID_ANY, "CS3307 UI Test", wxDefaultPosition, wxSize(300, 200)) {
        // Create a simple button as requested for the project UI
        new wxButton(this, wxID_ANY, "Hello World Button", wxPoint(50, 60), wxSize(200, 40));
    }
};

class SimpleApp : public wxApp {
public:
    virtual bool OnInit() {
        SimpleFrame* frame = new SimpleFrame();
        frame->Show(true);
        return true;
    }
};

// This macro replaces the standard main() for wxWidgets applications
wxIMPLEMENT_APP(SimpleApp);