#ifdef __GNUG__
#pragma interface
#endif

#include "wx/wx.h"

class MyApp: public wxApp
{
  public:
    MyApp(void) ;
    bool OnInit(void);
};

class MyNotebook: public wxNotebook
{
 public:
  MyNotebook(wxWindow *parent, wxWindowID id = -1,
	     const wxPoint& pos = wxDefaultPosition,
	     const wxSize& size = wxDefaultSize,
	     long style = 0);
};

class MyFrame: public wxFrame
{
  public:
    MyFrame(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnActivate(bool) {}
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

  private: 
    MyNotebook *m_notebook; 
    wxPanel *m_panel; 
    wxBoxSizer *m_sizerFrame;

    DECLARE_EVENT_TABLE()
};

#define UI_QUIT       100
#define UI_ABOUT      101
#define UI_NOTEBOOK   102

