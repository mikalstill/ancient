// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace OpenPdf
{
	public class Pdf
	{
		private	float m_version;
		private bool m_linearized = false;
		private ObjectCollection m_objects = new ObjectCollection();
		private Dictionary m_trailer = new Dictionary();

#region Parsing methods
		public Pdf(string filename)
		{
			PdfStream st = new PdfStream();
			st.FillFromFile(filename);
			
			long pos = st.Position;
			ReadHeader(st);
			
			// While we're still advancing through the stream, then all is good...
			bool eof = false;
			bool linflag = false;
			while(!eof && (st.Position != pos))
			{
				Console.WriteLine("Starting read pass");
				pos = st.Position;
				ReadComment(st);
				
				Object obj = ReadObject(st);
				if(obj.Valid)
				{
					m_objects.Add(obj);
					
					if(m_objects.Count == 1)
					{
						DictionaryItem di = obj.Dictionary.Get("Linearized");
						if(di.Valid)
						{
							if(di.Type == DictionaryItem.ValueType.Number)
							{
								if(di.ValueAsInteger() == 1)
								{
									linflag = true;
									m_linearized = true;
									Console.WriteLine("Linearized PDF document found");
								}
							}
							else
							{
								throw new ParseException("Linearized dictionary item is not a number");
							}
						}
					}
				}
				
				ReadXref(st);
				if(ReadTrailer(st))
				{
					if(linflag == true)
					{
						linflag = false;
						Console.WriteLine("Linearization trailer");
					}
					else
					{
						eof = true;
					}
				}
				ReadStartXref(st);
				
				if(!eof && (st.Position == pos) && (m_objects.Count < 2))
				{
					// There are some PDF documents which have a line of crap 
					// between their headers and the objects
					st.ReadLine();
				}
			}
		}
		
		public void ReadHeader(PdfStream st)
		{
			try
			{
				st.Expect("%PDF-", true);
				m_version = float.Parse(st.ReadBlock(3));
				st.ReadLine();
				Console.WriteLine("Read header");	
			}
			catch(Exception except)
			{
				throw new ParseException("No PDF header found: " + except.Message);
			}
		}
		
		public bool ReadComment(PdfStream st)
		{
			if(!st.Expect("%", false))
			{
				return false;
			}
			st.ReadLine();
			Console.WriteLine("Read comment");
			return true;
		}

		public Object ReadObject(PdfStream st)
		{
			string line = st.PeekLine();
			Regex re = new Regex("^([0-9]+) ([0-9]+) obj(.*)$");
			Match mtch = re.Match(line);

			if(mtch.Success)
			{
				Console.WriteLine("Read object");
				st.ReadLine();
				return new Object(st, mtch.Groups[1].Value, mtch.Groups[2].Value, mtch.Groups[3].Value);
			}
			return new Object();
		}
		
		public bool ReadXref(PdfStream st)
		{
			if(st.PeekLine(true, true) == "xref")
			{
				Console.WriteLine("Read xref");
				st.ReadLine();
				
				Regex re = new Regex("[0-9]+ [0-9]+");
				Match mtch = re.Match(st.PeekLine());
				if(!mtch.Success)
				{
					return true;
				}
				st.ReadLine();
				
				Regex xre = new Regex("[0-9]+ [0-9]+ [fn]");
				while(true)
				{
					Match xmtch = xre.Match(st.PeekLine());
					if(!xmtch.Success)
					{
						return true;
					}
					st.ReadLine();
				}				
			}
			
			return false;
		}
		
		public bool ReadStartXref(PdfStream st)
		{
			if(st.PeekLine(true, true) == "startxref")
			{
				Console.WriteLine("Read startxref");
				string line = st.ReadLine(true, true);
				while(line != "%%EOF")
				{
					Console.WriteLine("\t" + line);
					line = st.ReadLine(true, true);
				}
				Console.WriteLine("\t" + line);
				return true;
			}
			return false;
		}
		
		public bool ReadTrailer(PdfStream st)
		{
			if(st.PeekLine() == "trailer")
			{
				Console.WriteLine("Read trailer");
				st.ReadLine();
				m_trailer.ReadDictionary(st);
				return true;
			}
			
			return false;
		}
#endregion		
		
		public Object Catalog
		{
			get
			{
				// The trailer points to a root object, with a type of catalog, which contains a list of pages objects
				if(m_trailer.Count == 0)
				{
					throw new RuntimeException("Empty trailer");
				}
				
				DictionaryItem rootValue = m_trailer.Get("Root");
				if(!rootValue.Valid)
				{
					throw new RuntimeException("Root entry in trailer is empty");
				}
				
				if(rootValue.Type != DictionaryItem.ValueType.Objects)
				{
					throw new RuntimeException("Root entry in trailer does not point to an object: " + rootValue.ValueAsString());
				}
				
				return rootValue.ValueAsObject(m_objects);
			}
		}
		
		public ObjectCollection Pages
		{
			get
			{
				if(!Catalog.Valid)
				{
					throw new RuntimeException("Catalog is not valid");
				}
				
				DictionaryItem pages = Catalog.Dictionary.Get("Pages");
				return pages.ValueAsObjects(m_objects);
			}
		}
		
		public int PageCount
		{
			get
			{
				int count = 0;
				foreach(Object obj in Pages)
				{
					DictionaryItem kids = obj.Dictionary.Get("Kids");
					if(!kids.Valid)
					{
						throw new RuntimeException("Kids entry for a pages object was not valid");
					}
					ObjectCollection kidobjs = kids.ValueAsObjects(m_objects);
					count += kidobjs.Count;
				}
				
				return count;
			}
		}
	}
}
