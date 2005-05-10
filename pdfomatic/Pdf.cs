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
				Utility.TraceLine("Starting read pass");
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
									Utility.TraceLine("Linearized PDF document found");
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
						Utility.TraceLine("Linearization trailer");
					}
					else
					{
						eof = true;
					}
				}
				ReadStartXref(st);
				
				if(!eof && (st.Position == pos))
				{
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
				Utility.TraceLine("Read header");	
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
			Utility.TraceLine("Read comment");
			return true;
		}

		public Object ReadObject(PdfStream st)
		{
			string objheader = st.RegexMatch("^([0-9]+ [0-9]+ obj)");
			if(objheader != "")
			{
				return new Object(st, objheader);
			}
			return new Object();
		}
		
		public bool ReadXref(PdfStream st)
		{
			if(st.PeekLine(true, true) == "xref")
			{
				Utility.TraceLine("Read xref");
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
				Utility.TraceLine("Read startxref");
				string line = st.ReadLine(true, true);
				while(line != "%%EOF")
				{
					Utility.TraceLine("\t" + line);
					line = st.ReadLine(true, true);
				}
				Utility.TraceLine("\t" + line);
				return true;
			}
			return false;
		}
		
		public bool ReadTrailer(PdfStream st)
		{
			if(st.PeekLine() == "trailer")
			{
				Utility.TraceLine("Read trailer");
				st.ReadLine();
				m_trailer.ReadDictionary(st);
				return true;
			}
			
			return false;
		}
#endregion		
		
		public ObjectCollection Objects
		{
			get
			{
				return m_objects;
			}
		}
		
		private Object GetTrailerObject(string name)
		{
			if(m_trailer.Count == 0)
			{
				throw new RuntimeException("Empty trailer");
			}
			
			DictionaryItem rootValue = m_trailer.Get(name);
			if(!rootValue.Valid)
			{
				return new Object();
			}
			
			if(rootValue.Type != DictionaryItem.ValueType.Objects)
			{
				throw new RuntimeException(name + " entry in trailer does not point to an object: " + rootValue.ValueAsString());
			}
			
			return rootValue.ValueAsObject(m_objects);
		}
		
		public Object CatalogObject
		{
			get
			{
				return GetTrailerObject("Root");
			}
		}

		public Object InfoObject
		{
			get
			{
				return GetTrailerObject("Info");
			}
		}

		public ObjectCollection PagesObjects
		{
			get
			{
				if(!CatalogObject.Valid)
				{
					throw new RuntimeException("Catalog is not valid");
				}
				
				DictionaryItem pages = CatalogObject.Dictionary.Get("Pages");
				return pages.ValueAsObjects(m_objects);
			}
		}
		
		public ObjectCollection Pages
		{
			get
			{
				ObjectCollection retval = new ObjectCollection();
				foreach(Object obj in PagesObjects)
				{
					DictionaryItem kids = obj.Dictionary.Get("Kids");
					if(!kids.Valid)
					{
						throw new RuntimeException("Kids entry for a pages object was not valid");
					}
					
					ObjectCollection kidobjs = kids.ValueAsObjects(m_objects);
					foreach(Object kid in kidobjs)
					{
						retval.Add(kid);
					}
				}
				
				return retval;
			}
		}
	}
}
