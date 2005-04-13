// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU GPL. See COPYING for more details...

// With a very big nod to Edd Dumhill
using System;
using Gdk;
using Gtk;
using GLib;
using Glade;
using Gnome;
using OpenPdf;

using System.Drawing;
using GtkSharp;

namespace OpenPdfUi
{
	public class OpenPdfUi
	{
		[Widget] Gnome.App pdfview;
		[Widget] Gnome.AppBar appbar1;
		[Widget] Gtk.TreeView pages;
		[Widget] Gtk.HPaned hpaned1;
		
		private static string [] authors = { "Michael Still" };
		private static string progversion = "0.1";
		private static string progname = "OpenPdfUi";
		private static string progcopyright = "(C) Michael Still 2005";
		
		private Pdf m_doc = null;
		
		public static void Main(string[] args)
		{
			new OpenPdfUi(args);
		}
	
		public OpenPdfUi(string[] args)
		{
			Program program = new Program(progname, progversion, Modules.UI, args);
			
			Glade.XML gxml = new Glade.XML(null, "openpdfui.glade", "pdfview", null);
			gxml.Autoconnect(this);
            
            // TODO: I need to work out how to set the minimum width of this splitter
            TreeViewColumn DemoCol = new TreeViewColumn ();
            CellRenderer DemoRenderer = new CellRendererText();
            DemoCol.Title = "Demo";
            DemoCol.PackStart (DemoRenderer, true);
            DemoCol.AddAttribute (DemoRenderer, "text", 0);
            pages.AppendColumn (DemoCol);
            pages.WidthRequest = 40;
            
			appbar1.Push("Ready");
			program.Run();
		}
		
		public void on_pdfview_delete_event(object o, EventArgs args)
		{
			Application.Quit();
		}
		
		public void on_quit1_activate(object o, EventArgs args)
		{
			Application.Quit();
		}
		
		public void on_properties1_activate(object o, EventArgs args)
		{
		}
		
		public void on_preferences1_activate(object o, EventArgs args)
		{
		}
	
		/////////////////////////////////////////////////////
		// File menu	
		public void on_save1_activate(object o, EventArgs args)
		{
		}
		
		public void on_save_as1_activate(object o, EventArgs args)
		{
		}
		
		public void on_open1_activate(object o, EventArgs args)
		{
			FileSelection fs = new FileSelection("Select a PDF file...");
			// TODO: This doesn't filter the file list...
			fs.Complete("*.pdf");
			fs.Run();
			fs.Hide();
			
			appbar1.Push("Opening document...");
			m_doc = new Pdf(fs.Filename);
			appbar1.Push(m_doc.PageCount + " page document");
			
			// Populate the pages list
			Gtk.ListStore model = new Gtk.ListStore(typeof(string));
			Gtk.TreeIter iter = new Gtk.TreeIter();
			Console.WriteLine("List has " + model.NColumns + " columns");
			for(int i = 0; i < m_doc.PageCount; i++)
			{
				iter = model.Append();
				GLib.Value v = new GLib.Value((i + 1).ToString());
				model.SetValue(iter, 0, v);
			}
			pages.Model = model;            
			pages.Show();
		}
		
		public void on_new1_activate(object o, EventArgs args)
		{
		}
		
		/////////////////////////////////////////////////////
		// Edit menu
		public void on_copy1_activate(object o, EventArgs args)
		{
		}
		
		public void on_cut1_activate(object o, EventArgs args)
		{
		}
		
		public void on_paste1_activate(object o, EventArgs args)
		{
		}
		
		public void on_clear1_activate(object o, EventArgs args)
		{
		}
		
		public void on_about1_activate(object o, EventArgs args)
		{
			new Gnome.About(progname, progversion, progcopyright, "An open source PDF viewer", authors, null, null, null).Run();
		}
	}
}