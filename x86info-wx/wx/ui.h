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

class MyCanvas;
class MyFrame: public wxFrame
{
  public:
    MyCanvas *canvas;
    MyFrame(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnActivate(bool) {}
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
DECLARE_EVENT_TABLE()
};

class MyCanvas: public wxScrolledWindow
{
  public:
    MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size);
    ~MyCanvas(void) ;

    void OnPaint(wxPaintEvent& event);
DECLARE_EVENT_TABLE()
};

#define UI_QUIT       100
#define UI_ABOUT      101

