#ifdef __GNUG__
#pragma implementation "ui.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/image.h"
#include "ui.h"

MyFrame   *frame = (MyFrame *) NULL;

IMPLEMENT_APP(MyApp)
MyApp::MyApp()
{
}

bool MyApp::OnInit(void)
{
  wxImage::AddHandler(new wxPNGHandler);

  frame = new MyFrame((wxFrame *) NULL, "wxPNGBitmap Demo", wxPoint(0, 0), wxSize(300, 300));
  frame->CreateStatusBar(2);

  // Make a menubar
  wxMenu *file_menu = new wxMenu;
  wxMenu *help_menu = new wxMenu;

  file_menu->Append(UI_QUIT, "E&xit",                "Quit program");
  help_menu->Append(UI_ABOUT, "&About",              "About PNG demo");

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");

  // Associate the menu bar with the frame
  frame->SetMenuBar(menu_bar);

  // Create a drawing canvas
  MyCanvas *canvas = new MyCanvas(frame, wxPoint(0, 0), wxSize(100, 100));
  frame->canvas = canvas;
  frame->Show(TRUE);
  frame->SetStatusText("Hello, wxWindows");
  return TRUE;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(UI_QUIT,      MyFrame::OnQuit)
    EVT_MENU(UI_ABOUT,     MyFrame::OnAbout)
END_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size):
  wxFrame(frame, -1, title, pos, size)
{
  canvas = (MyCanvas *) NULL;
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    (void)wxMessageBox("PNG demo\nJulian Smart (c) 1998",
            "About PNG Demo", wxOK);
}

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

MyCanvas::MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size):
 wxScrolledWindow(parent, -1, pos, size)
{
}

MyCanvas::~MyCanvas(void)
{
}

void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  dc.SetPen(* wxRED_PEN);

  int i;
  for ( i = 0; i < 500; i += 10)
  {
    dc.DrawLine(0, i, 800, i);
  }
}


