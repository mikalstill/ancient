using System;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;

namespace pdfdb
  {
  class Class1
    {
    [STAThread]
    static void Main(string[] args)
      {
      // We use regular expressions to match URLs to the names of the Producers
      Hashtable urls = new Hashtable();
      Hashtable urlregexps = new Hashtable();
    
      Console.WriteLine("Load in the producer regexps");
      StreamReader re = new StreamReader(args[0] + "/producers.txt");
      try
	{
	while( re.Peek() != -1 )
	  {
	  string line = re.ReadLine();
	  string[] tokens = Regex.Split(line, ":[ \t]+");
	  urlregexps[tokens[0]] = tokens[1];
	  }
	}
      catch(Exception e)
	{
	Console.WriteLine("Could not read producer regexps");
	}
    
      // We store the great big list of things we've found here
      string all = "";
      SortedList producer = new SortedList();
      SortedList tagged = new SortedList();
      SortedList encrypted = new SortedList();
      SortedList linearized = new SortedList();
      SortedList version = new SortedList();
      SortedList pdfpage = new SortedList();
      SortedList pdfmeta = new SortedList();
      SortedList pagecount = new SortedList();
      SortedList plparse = new SortedList();
    
      int docs = 0;
      int exceptions = 0;
    
      Console.WriteLine("Find all the PDFs available in {0}", args[0]);
      string[] dirs = Directory.GetDirectories(args[0]);
    
      int i;
      for(i = 0; i < dirs.Length; i++)
	{
	// NB. This would also match a string of the form: 000000/foo/bar, 
	// which isn't really intended
	Match m = Regex.Match(dirs[i], "[0-9][0-9][0-9][0-9][0-9][0-9]");
	if( m.Success )
	  {
	  Console.Write(".");
	  StreamReader input = new StreamReader(args[0] + "/" + m.ToString() + "/data.info");
	  docs++;
	  all += ";" + m.ToString();
	  try	    {
	    while( input.Peek() != -1 )
	      {
	      string line = input.ReadLine();
	      string[] tokens = Regex.Split(line, ":[ \t]+");
	      if( tokens.Length > 1 )
		{
		switch(tokens[0])
		  {
		  case "Producer":
		    producer[tokens[1]] += ";" + m.ToString();
		    break;
		  
		  case "Tagged":
		    tagged[tokens[1]] += ";" + m.ToString();
		    break;
		  
		  case "Encrypted":
		    encrypted[tokens[1]] += ";" + m.ToString();
		    break;
		  
		  
		  case "Optimized":
		    linearized[tokens[1]] += ";" + m.ToString();
		    break;
		  
		  case "PDF version":
		    version[tokens[1]] += ";" + m.ToString();
		    break;
		  
		  case "Pages":
		    // Page counts are broken into classes of hundreds, with the 0-100 class
		    // further broken up into classes of size 10
		    int pc = Int32.Parse(tokens[1]);
		    int lower = 0;
		    if( pc < 100 ) lower = ((pc - pc % 10) / 10) * 10;
		    else lower = ((pc - pc % 100) / 100) * 100;
		    pagecount[lower] += ";" + m.ToString() + "," + tokens[1];
		    break;
		  
		  default:
		    pdfmeta[m.ToString()] += "<b>" + tokens[0] + ":</b> " + tokens[1] + "<br>";
		    break;
		  }
		}
	      else
		{
		Console.WriteLine("{1}: Unknown line: {0}", line, m.ToString());
		}
	      }
	    }
	  catch(Exception e)
	    {
	    Console.WriteLine("{0}: Caught exception processing this document", m.ToString());
	    exceptions++;
	    }

	  // We also need to check the status of the latest PandaLex parsing regression test
	  input = new StreamReader(args[0] + "/" + m.ToString() + "/data.pandalex");
	  string mostRecentStatus = "";
	  string previousStatus = "";
	  bool statusNext = true;
	  try
	    {
	    while( input.Peek() != -1 )
	      {
	      string line = input.ReadLine();
	      if( statusNext )
		{
		previousStatus = mostRecentStatus;
		mostRecentStatus = line;
		}

	      Match plm = Regex.Match(line, "--------------------------");
	      statusNext = plm.Success;
	      }
	    }
	  catch(Exception e)
	    {
	    Console.WriteLine("{0}: Caught exception processing pandalex regression: {1}", m.ToString(), e.ToString());
	    exceptions++;
	    }

	  // Now do something with the status we found
	  string[] results = Regex.Split(mostRecentStatus, "\t");
	  string change = results[1];
	  string oldvalue = change;
	  try
	    {
	    string[] oldresults = Regex.Split(previousStatus, "\t");
	    oldvalue  = oldresults[1];
	    }
	  catch(Exception e)
	    {
	    // Do nothing
	    }

	  if(change != oldvalue)
	    change += " (new)";
	  plparse[change] += ";" + m.ToString();

	  // End of PDF file processing
	  }
	}
      Console.WriteLine("");
    
      // Dump out the indices list
      Console.WriteLine("Writing the main index page");
      StreamWriter output = new StreamWriter(args[0] + "/db.html");
      output.WriteLine("<html><head><title>Mikal's PDF database</title></head><body>");
      output.WriteLine("<b><a href=\"http://www.stillhq.com\">Stillhq.com</a> PDF Database: Index</b><BR><BR>");
      output.WriteLine("<div align=center> <table border=2 width=\"80%\"><tr><td>");
      output.WriteLine("All the PDF documents in this database are sorted in a variety of ways. ");
      output.WriteLine("Click on a number to see a page about the PDF with that id number.");
      output.WriteLine("There are currently {0} PDF documents in the database. ", docs);
      output.WriteLine("<BR><BR><i>Update: Please note that the name of each producer is now a link back to ");
      output.WriteLine("their respective websites. I have also introduced a <a href=\"faq.html\">frequently asked questions page</a>. ");
      output.WriteLine("This FAQ page also discusses how to mount the PDF database as a drive on your windows machine, which is a very convenient way of downloading the entire database.</i>");
      output.WriteLine("<BR><BR>Please send email to <a href=\"mailto:mikal@stillhq.com\">mikal@stillhq.com</a> ");
      output.WriteLine("if you would like to donate sample PDFs. ");
      output.WriteLine("</td></tr></table></div><br><br>");
      output.WriteLine("Navigation: <a href=\"#producer\">Producer</a> ");
      output.WriteLine("<a href=\"#tagging\">Tagging</a> ");
      output.WriteLine("<a href=\"#encryption\">Encryption</a> ");
      output.WriteLine("<a href=\"#linearization\">Linearization</a> ");
      output.WriteLine("<a href=\"#version\">Version</a> ");
      output.WriteLine("<a href=\"#pagecount\">Length</a> ");
      output.WriteLine("<a href=\"#plparse\">PandaLex</a> ");
      output.WriteLine("<a href=\"#all\">All</a> ");
      output.WriteLine("<BR><BR>");
    
      int ht;
      for(ht = 0; ht < 7; ht++)
	{
	SortedList target = producer;
	string catname = "";
      
	switch(ht)
	  {
	  case 0:
	    target = producer;
	    catname = "Producer";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=producer>By producer</b></td></tr></table>");
	    break;
	  
	  case 1:
	    target = tagged;
	    catname = "Tagged";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=tagging>By tagging</b></td></tr></table>");
	    break;
	  
	  case 2:
	    target = encrypted;
	    catname = "Encrypted";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=encryption>By encryption</b></td></tr></table>");
	    break;

	  case 3:
	    target = linearized;
	    catname = "Linearized";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=linearization>By linearization</b></td></tr></table>");
	    break;

	  case 4:
	    target = version;
	    catname = "Specification verion";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=version>By PDF specification version</b></td></tr></table>");
	    break;

	  case 5:
	    target = pagecount;
	    catname = "Document length";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=pagecount>By document length</b></td></tr></table>");
	    break;

	  case 6:
	    target = plparse;
	    catname = "PandaLex parse results";
	    output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=plparse>By PandaLex parse result</b></td></tr></table>");
	    break;
	  }

	Console.Write("{0}: ", catname);
	output.WriteLine("<ul>");
	foreach(object key in target.Keys)
	  {
	  if( key.ToString() == "" )
	    {
	    output.Write("<p><b>None specified:</b> ");
	    }
	  else
	    {
	    // Producers now get URLs (only in the main index page for now)
	    if( target == producer )
	      {
	      // Lookup the URL for the producer
	      string url = "";
	      if( urls.Contains(key) )
		{
		Console.WriteLine("Found");
		}
	      else
		{
		foreach(object ukey in urlregexps.Keys)
		  {
		  Match umatch = Regex.Match(key.ToString(), ukey.ToString());
		  if( umatch.Success )
		    {
		    url = urlregexps[ukey].ToString();
		    urls[key] = url;
		    break;
		    }
		  }
		}

	      if( url != "" ) output.Write("<p><b><a href=\"{1}\">{0}</a>:</b> ", key.ToString(), url);
	      else output.Write("<p><b>{0}:</b> ", key.ToString());

	      }
	    else if( target == pagecount )
	      {
	      int upper = Int32.Parse(key.ToString());
	      if( upper < 100 ) upper += 10;
	      else upper += 100;
	      output.Write("<p><b>{0} to {1}:</b> ", key.ToString(), upper);
	      }
	    else
	      output.Write("<p><b>{0}:</b> ", key.ToString());
	    }

	  string[] docids = Regex.Split(target[key].ToString(), ";");
	  for(i = 0; i < docids.Length; i++)
	    {
	    if( docids[i].Length > 0 )
	      {
	      Console.Write(".");
	      string[] reallen = Regex.Split(docids[i], ",");
	      string doclen = "";
	      if( reallen.Length > 1 )
		{
		docids[i] = reallen[0];
		doclen = reallen[1];
		}

	      if( catname == "" )
		{
		catname = "None specified";
		}

	      pdfpage[docids[i]] += "<b>" + catname + ":</b> ";
	      if( doclen != "" )
		pdfpage[docids[i]] += doclen;
	      else
		pdfpage[docids[i]] += key.ToString();

	      pdfpage[docids[i]] += " [";
	      if( (i > 0) && (docids[i - 1].Length > 0) )
		pdfpage[docids[i]] += "<a href=\"../" + docids[i - 1] + "/info.html\">Previous</a>";
	      else
		pdfpage[docids[i]] += "No previous document";
	      pdfpage[docids[i]] += "] [";

	      // The next link needs the same fixup for the docid
	      if( (i < docids.Length - 1) && (docids[i + 1].Length > 0) )
		{
		string[] nextdi = Regex.Split(docids[i + 1], ",");
		pdfpage[docids[i]] += "<a href=\"../" + nextdi[0] + "/info.html\">Next</a>";
		}
	      else
		pdfpage[docids[i]] += "No next document";
	      pdfpage[docids[i]] += "] (of " + docids.Length + ")<br>";

	      output.Write("<a href=\"{0}/info.html\">{0}</a> ", docids[i]);
	      }
	    }
	  output.WriteLine("</p>");				
	  }
	output.WriteLine("</ul>");
	Console.WriteLine("");
	}

      // And finally list all documents
      output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b><a name=all>All documents</b></td></tr></table>");
      output.WriteLine("<ul>");
      string[] alldocids = Regex.Split(all, ";");
      for(i = 0; i < alldocids.Length; i++)
	{
	if( alldocids[i].Length > 0 )
	  {
	  pdfpage[alldocids[i]] += "<b>All documents:</b>";
	  pdfpage[alldocids[i]] += " [";
	  if( (i > 0) && (alldocids[i - 1].Length > 0) )
	    pdfpage[alldocids[i]] += "<a href=\"../" + alldocids[i - 1] + "/info.html\">Previous</a>";
	  else
	    pdfpage[alldocids[i]] += "No previous document";
	  pdfpage[alldocids[i]] += "] [";
	  if( (i < alldocids.Length - 1) && (alldocids[i + 1].Length > 0) )
	    pdfpage[alldocids[i]] += "<a href=\"../" + alldocids[i + 1] + "/info.html\">Next</a>";
	  else
	    pdfpage[alldocids[i]] += "No next document";
	  pdfpage[alldocids[i]] += "]<br>";
	  output.Write("<a href=\"{0}/info.html\">{0}</a> ", alldocids[i]);
	  }
	}
      output.WriteLine("</p>");				
      output.WriteLine("</ul>");
		
      output.WriteLine("<BR><BR><HR><BR><BR>PDF database administered by ");
      output.WriteLine("<a href=\"mailto:mikal@stillhq.com\">mikal@stillhq.com</a><br>");
      output.WriteLine("Database Copyright (c) Michael Still 2003. PDFs Copyright their various authors.");
      output.Close();

      // Now write out each of the individual pages
      Console.WriteLine("");
      Console.Write("Generating PDF pages: ");
      foreach(object key in pdfpage.Keys)
	{
	Console.Write(".");
	output = new StreamWriter(args[0] + "/" + key.ToString() + "/info.html");
	output.WriteLine("<html><title>Mikal's PDF database: PDF Number {0}</title></head><body>", key.ToString());
	output.WriteLine("<b><a href=\"http://www.stillhq.com\">Stillhq.com</a> ");
	output.WriteLine("<a href=\"../db.html\">PDF Database</a> : {0}</b><BR><BR>", key.ToString());
				
	// The header has been pregenerated
	output.WriteLine("<table><tr><td bgcolor=\"#CCCCCC\"><b>Database indices</b></td></tr></table><ul>");
	output.WriteLine(pdfpage[key]);

	// Other meta data
	output.WriteLine("</ul><table><tr><td bgcolor=\"#CCCCCC\"><b>Metadata</b></td></tr></table><ul>");
	output.WriteLine(pdfmeta[key]);

	// Did we see ghostscript errors?
	string gserrs = "";
	try
	  {
	  StreamReader gse = new StreamReader(args[0] + "/" + key.ToString() + "/gs.errors");
	  while( gse.Peek() != -1 )
	    gserrs += gse.ReadLine() + "\n";
	  }
	catch(Exception e)
	  {
	  gserrs = "";
	  }
	if( gserrs != "" )
	  {
	  output.WriteLine("</ul><table><tr><td bgcolor=\"#CCCCCC\"><b>Ghostscript errors</b></td></tr></table><ul>");
	  output.WriteLine("<pre>");
	  output.WriteLine(gserrs);
	  output.WriteLine("</pre>");
	  }

	// Do we have a notes field?
	string notes = "";
	try
	  {
	  StreamReader notesreader = new StreamReader(args[0] + "/" + key.ToString() + "/data.notes");
	  while( notesreader.Peek() != -1 )
	    notes += notesreader.ReadLine() + "\n";
	  }
	catch(Exception e)
	  {
	  notes = "";
	  }
	if( notes != "" )
	  {
	  notes = Regex.Replace(notes, "<", "&lt;");
	  notes = Regex.Replace(notes, ">", "&gt;");
	  notes = Regex.Replace(notes, "@", " [at] ");
	  output.WriteLine("</ul><table><tr><td bgcolor=\"#CCCCCC\"><b>Notes</b></td></tr></table><ul>");
	  output.WriteLine("<pre>");
	  output.WriteLine(notes);
	  output.WriteLine("</pre>");
	  }

	// The user can download the pdf
	output.WriteLine("</ul><table><tr><td bgcolor=\"#CCCCCC\"><b>Download</b></td></tr></table><ul>");
	output.WriteLine("Click <a href=\"data.pdf\">here</a> to download this PDF.");

	// And then some thumbnails
	output.WriteLine("</ul><table><tr><td bgcolor=\"#CCCCCC\"><b>Thumbnails</b></td></tr></table><ul>");
				
	string[] file = Directory.GetFiles(args[0] + "/" + key.ToString());
	int j;
	int tcount = 0;
	bool tfound = false;
	for(j = 0; j < file.Length; j++)
	  {
	  Match n = Regex.Match(file[j], "cgspage-(.*).png");
	  if( n.Success )
	    {
	    output.WriteLine("<img src=\"{0}\" alt=\"Thumbnail page {0}\" border=1>", n.ToString(), tcount + 1);
	    tcount++;
	    tfound = true;
	    }
	  }
	if( !tfound )
	  {
	  output.WriteLine("<table border=1><tr><td>Ghostscript failed to extract thumbnail images</td></tr></table>");
	  }

	output.WriteLine("</ul>");
	output.WriteLine("<HR><BR><BR>PDF database administered by ");
	output.WriteLine("<a href=\"mailto:mikal@stillhq.com\">mikal@stillhq.com</a><br>");
	output.WriteLine("Database Copyright (c) Michael Still 2003. PDFs Copyright their various authors.");
	output.Close();
	output.Close();
	}
      Console.WriteLine("");
      Console.WriteLine("Exceptions caught: {0}", exceptions);
      }
    }
  }
