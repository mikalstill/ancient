// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using OpenPdf;

class pdfdescribe
{
	public static void Main(string[] args)
	{
		Console.WriteLine("Reading PDF file...");
		Pdf doc = new Pdf(args[0]);
		
		Console.WriteLine("Number of pages: " + doc.PageCount);
	}
}