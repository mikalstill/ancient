#ifdef __GNUG__
#pragma implementation "ui.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <stdio.h>

#include "wx/image.h"
#include "wx/notebook.h"
#include "ui.h"
#include "x86info.h"

MyFrame   *frame = (MyFrame *) NULL;

wxString gWarnings = "";
wxString gTabSplash = "";
wxString gTabCPU = "";
wxString gTabFeature = "";
wxString gTabAdvanced = "";
wxString gTabCaching = "";
wxString gTabParity = "";
wxString gTabURLs = "";
wxString gTabUnknown = "";

wxString
toString (int number)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", number);
  return wxString (buffer);
}

void x86callback(int level, char *message)
{
  switch(level){
  case msg_author:
    gTabSplash += message + wxString("\n");
    break;

  case msg_begincpu:
    gTabCPU += wxString("\n");
  case msg_cpuconn:
  case msg_cpuinfo:
  case msg_cpuname:
  case msg_cpuspeed:
  case msg_vendor:
  case msg_numcpu:
  case msg_smpup:
  case msg_notsc:
    gTabCPU += message + wxString("\n");
    break;

  case msg_feature:
  case msg_featureamd:
  case msg_featurecentaur:
  case msg_featurecyrix:
  case msg_featureintel:
  case msg_featuretransmeta:
  case msg_longhaul:
  case msg_powernow:
  case msg_syscallextn:
    gTabFeature += message + wxString("\n");
    break;

  case msg_dumpregs:
    gTabAdvanced += message + wxString("\n");
    break;

  case msg_datacache:
  case msg_dataprefetch:
  case msg_datatlb:
  case msg_instcache:
  case msg_insttlb:
  case msg_insttracecache:
  case msg_l1cache:
  case msg_l1datacache:
  case msg_l1instcache:
  case msg_l2cache:
  case msg_l2cacheerrata:
  case msg_l3cache:
  case msg_tlb:
    gTabCaching += message + wxString("\n");
    break;

  case msg_dcparity:
  case msg_eccparity:
  case msg_extl2parity:
  case msg_icparity:
  case msg_instparity:
  case msg_precodeparity:
  case msg_targselparity:
  case msg_tlbl1parity:
  case msg_tlbl2parity:
  case msg_tlbparity:
    gTabParity += message + wxString("\n");
    break;

  case msg_url:
    gTabURLs += message + wxString("\n");
    break;

  case msg_usage:
    wxMessageBox(wxString(message), "Usage", wxOK);
    break;

  case msg_warning:
    gWarnings += message + wxString("\n");
    break;

  case msg_error:
    // todo mikal: Error icon?
    wxMessageBox(wxString(message), "Error", wxOK);
    break;

  default:
    gTabUnknown += message + wxString("\n");
    break;
  }
}

IMPLEMENT_APP(MyApp)
MyApp::MyApp()
{
}

bool MyApp::OnInit(void)
{
  wxImage::AddHandler(new wxPNGHandler);

  // Go off and find out about this machine
  setEmit(x86callback);
  int retval = x86info(argc, argv);

  // Display warnings, if any
  if(gWarnings != ""){
    wxMessageBox(wxString("The following warnings were accumulated whilst\n"
			  "performing system detection:\n\n") + gWarnings,
		 "Detection warnings", wxOK);
  }

  // Now build the UI
  frame = new MyFrame((wxFrame *) NULL, "x86info", wxPoint(0, 0), 
		      wxSize(600, 600));
  frame->CreateStatusBar(2);

  // Make a menubar
  wxMenu *file_menu = new wxMenu;
  wxMenu *help_menu = new wxMenu;

  file_menu->Append(UI_QUIT, "E&xit",                "Quit program");
  help_menu->Append(UI_ABOUT, "&About",              "About");

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");

  // Associate the menu bar with the frame
  frame->SetMenuBar(menu_bar);

  // Create a drawing canvas
  frame->Show(TRUE);
  frame->SetStatusText("x86info 1.11");

  if(retval != 0)
    return FALSE;
  return TRUE;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(UI_QUIT,      MyFrame::OnQuit)
    EVT_MENU(UI_ABOUT,     MyFrame::OnAbout)
END_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size):
  wxFrame(frame, -1, title, pos, size)
{
  m_panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER);
  m_notebook = new MyNotebook((wxWindow *) m_panel, UI_NOTEBOOK,
			      wxPoint(0, 0), wxSize(800, 500));
  m_sizerFrame = new wxBoxSizer(wxVERTICAL);
  m_panel->SetSizer(m_sizerFrame);
  m_panel->SetAutoLayout(TRUE);
  m_sizerFrame->SetMinSize(810, 510);
  m_sizerFrame->Fit(this);
  Centre(wxBOTH);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxMessageBox(gTabSplash, "About x86info", wxOK);
}

MyNotebook::MyNotebook(wxWindow *parent, wxWindowID id, const wxPoint& pos,
		       const wxSize& size, long style)
  : wxNotebook(parent, id, pos, size, style)
{
  if(gTabCPU != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabCPU, wxPoint(5, 5));
      AddPage(panel, "CPU");
    }

  if(gTabFeature != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabFeature, wxPoint(5, 5));
      AddPage(panel, "Features");
    }


  if(gTabCaching != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabCaching, wxPoint(5, 5));
      AddPage(panel, "Caching");
    }

  if(gTabParity != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabParity, wxPoint(5, 5));
      AddPage(panel, "Parity");
    }

  if(gTabAdvanced != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabAdvanced, wxPoint(5, 5));
      AddPage(panel, "Advanced");
    }

  if(gTabURLs != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabURLs, wxPoint(5, 5));
      AddPage(panel, "URLs");
    }

  if(gTabUnknown != "")
    {
      wxPanel *panel = new wxPanel(this);
      (void) new wxStaticText(panel, -1, gTabUnknown, wxPoint(5, 5));
      AddPage(panel, "Unknown");
    }
}
