// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using OpenPdf;
using mGnu;

class pdfdescribe
{
	public static void Main(string[] args)
	{
		bool verbose = false;
	
		if(args.Length == 0)
		{
			Console.WriteLine("Usage: openpdfdescribe -v");
			return;
		}
		
		GetOpt options = new GetOpt(args, "v");
		Arg a = options.NextArg();
		while(a != null)
		{
			switch(a.Flag)
			{
				case "-v":
					verbose = true;
					break;
					
				default:
					Console.WriteLine("Unknown command line option: " + a.Flag);
					Console.WriteLine("Usage: openpdfdescribe -v -i <input pdf>");
					return;
			}
			
			a = options.NextArg();
		}
		
		// Dump information for each of the remaining things on the command line, which should
		// all be PDF documents...
		foreach(string file in options.Extras())
		{
			Console.WriteLine("Information about " + file);
			Pdf doc = new Pdf(file);
			
			OpenPdf.Object info = doc.InfoObject;
			if(info.Valid && verbose)
			{
				Dictionary dict = info.Dictionary;
				if(dict.Count != 0)
				{
					OutputDictionaryValue(dict, "Title", "Title: ");
					OutputDictionaryValue(dict, "Author", "Author: ");
					OutputDictionaryValue(dict, "Subject", "Subject: ");
					OutputDictionaryValue(dict, "Keywords", "Keywords: ");
					OutputDictionaryValue(dict, "Creator", "Creator: ");
					OutputDictionaryValue(dict, "Producer", "Producer: ");
					OutputDictionaryValue(dict, "CreationDate", "Creation date: ");
					OutputDictionaryValue(dict, "ModDate", "Modification date: ");
					OutputDictionaryValue(dict, "Trapped", "Trapped: ");
				}
				else
				{
					Console.WriteLine("Document meta data malformed");
				}
			}
			else
			{
				Console.WriteLine("No document meta data available");			
			}
			
			Console.WriteLine("Number of pages: " + doc.Pages.Count);
			Console.WriteLine("Cross reference starts at: " + doc.XrefLocation + " bytes");
			Console.WriteLine("Number of objects: " + doc.Objects.Count);
			
			Console.WriteLine("Document is valid PDF/A: " + doc.PdfACompliant);
			
			if(verbose)
			{
				Console.WriteLine("Page objects are:");
				foreach(OpenPdf.Object obj in doc.Pages)
				{
					Content cnt = new Content(doc, obj);
					FilterCollection filterCollection = cnt.Filters;
					string filters = "no";
					if(filterCollection != null)
					{
						filters = filterCollection.ToString();
					}
					
					Console.WriteLine("\t" + obj.ToString() + " with content at " + cnt.Objects.ToString() + 
						" and " + filters + " filter");	
				}
			}
			
			Console.WriteLine("");
		}
	}
	
	private static void OutputDictionaryValue(Dictionary dict, string key, string label)
	{
		DictionaryItem di = dict.Get(key);
		if(di.Valid)
		{
			Console.WriteLine(label + di.ValueAsString());
		}
	}
}