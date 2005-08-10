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
		private bool m_pdfa = true;
		private int m_xrefLocation = 0;
		private ObjectCollection m_objects = new ObjectCollection();
		private Dictionary m_trailer = new Dictionary();
		
		// Flags if we're currently like to see a linearization XREF
		bool m_linflag = false;

#region Parsing methods
		public Pdf(string filename)
		{
			PdfStream st = new PdfStream();
			st.FillFromFile(filename);
			
			long pos = st.Position;
			ReadHeader(st);
			
			// While we're still advancing through the stream, then all is good...
			bool eof = false;
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
									m_linflag = true;
									m_linearized = true;
									Utility.TraceLine("Linearized PDF document found");
									Utility.TraceLine("PDF/A: Linearization of document is ignored (section 5.10)");
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
					if(m_linflag == true)
					{
						m_linflag = false;
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
			
			if(st.Position != st.Length)
			{
				m_pdfa = false;
				Utility.TraceLine("PDF/A: Extraneous content after EOF marker breaches section 5.3 requirements");
			}
		}
		
		public void ReadHeader(PdfStream st)
		{
			try
			{
				st.Expect("%PDF-", true);
				m_version = float.Parse(st.ReadBlock(3));
				st.ReadLine();
				
				// PDF/A requires a four character comment using only characters with a value
				// greater than 127 (PDF/A ISO Specification 5.2)
				string comment = ReadComment(st);
				if(comment == "")
				{
					m_pdfa = false;
					Utility.TraceLine("PDF/A: No section 5.2 comment line");
				}
				else if(comment.Length != 5)
				{
					m_pdfa = false;
					Utility.TraceLine("PDF/A: Section 5.2 comment is wrong size");
				}
				else
				{
					for(int count = 1; count < 6; count++)
					{
						if(comment[count] < 128)
						{
							m_pdfa = false;
							Utility.TraceLine("PDF/A: Section 5.2 comment character " + count + " is not binary");
						}
					}
				}
				
				Utility.TraceLine("Read header");	
			}
			catch(Exception except)
			{
				throw new ParseException("No PDF header found: " + except.Message);
			}
		}
		
		public string ReadComment(PdfStream st)
		{
			if(!st.Expect("%", false))
			{
				return "";
			}
			
			return st.ReadLine();
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
				Utility.TraceLine("Reading a XREF");
				st.ReadLine();
				
				while(st.RegexMatch("^([0-9]+ [0-9]+)", false) != "")
				{
					Utility.TraceLine("Found XREF value block");
				
					// Each xref block starts with a line with the starting number of the objects in the
					// block, and then a count of the number of objects in the block PDF 1.5 p70
					int startat = 0;
					int count = 0;
					try
					{
						startat = Int32.Parse(st.RegexMatch("^([0-9]+)", true));
						count = Int32.Parse(st.RegexMatch("^[ \t]*([0-9]+)", true));
						st.ReadLine();
						
						// The you get one line per object in the block. They contain:
						// <byte offset> <generation> <n = inuse, f = free>
						Utility.TraceLine("XREF reading " + count + " lines");
						for(int i = 0; i < count; i++)
						{
							string line = st.ReadLine();
							long offset = Int64.Parse(line.Substring(0, 10));
							int generation = Int32.Parse(line.Substring(11, 5));
							string inuse = line.Substring(17, 1);
							
							Utility.TraceLine("XREF line: " + offset + " " + generation + " " + inuse);
							
							if((inuse == "n") && !m_linflag)
							{
								// PDF/A requires us to verify these offsets
								Object obj = m_objects.Get(startat + i, generation);
								if(offset != obj.StartedAt)
								{
									m_pdfa = false;
									Utility.TraceLine("PDF/A: Object offset is incorrect " + offset + " != " + 
										obj.StartedAt + " (required by section 5.4)");
								}
							}
						}
					}
					catch(Exception except)
					{
						throw new ParseException("Error reading xref block starter: " + except.Message);
					}
				}				
			}
			
			return false;
		}
		
		public bool ReadStartXref(PdfStream st)
		{
			if(st.PeekLine(true, true) == "startxref")
			{
				Utility.TraceLine("Read startxref");
				st.ReadLine();
				
				try
				{
					string sline = st.RegexMatch("^([0-9]+)");
					m_xrefLocation = Int32.Parse(sline);
				}
				catch(Exception except)
				{
					throw new ParseException("Error reading xref inset: " + except.Message);
				}
				
				string line = st.ReadLine(true, true);
				while(line != "%%EOF")
				{
					line = st.ReadLine(true, true);
				}
				return true;
			}
			return false;
		}
		
		public bool ReadTrailer(PdfStream st)
		{
			if(st.PeekLine() == "trailer")
			{
				st.ReadLine();
				m_trailer.ReadDictionary(st);
				
				// Enforce PDF/A requirements for the trailer (section 5.3 of draft standard)
				// I deliberately do all of these checks so that we can report _all_ errors
				// at once
				if(m_trailer.Get("Encrypt") != null)
				{
					m_pdfa = false;
					Utility.TraceLine("PDF/A: Forbidden encrypt dictionary item in section 5.3 document trailer");
				}
				if(m_trailer.Get("Info") != null)
				{
					m_pdfa = false;
					Utility.TraceLine("PDF/A: Forbidden info dictionary item in section 5.3 document trailer");
				}
				if(m_trailer.Get("Size") == null)
				{
					m_pdfa = false;
					Utility.TraceLine("PDF/A: Required size dictionary document trailer item is missing");
				}
				if(m_trailer.Get("ID") == null)
				{
					m_pdfa = false;
					Utility.TraceLine("PDF/A: Required ID dictionary document trailer item is missing");
				}
				
				Utility.TraceLine("Read trailer");
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
		
		public ObjectCollection Pages
		{
			get
			{
				ObjectCollection retval = new ObjectCollection();
				DictionaryItem immed = CatalogObject.Dictionary.Get("Pages");
				foreach(Object obj in immed.ValueAsObjects(m_objects))
				{
					retval.Add(TraversePagesTree(obj));
				}
				return retval;
			}
		}
		
		private ObjectCollection TraversePagesTree(Object top)
		{
			ObjectCollection retval = new ObjectCollection();
			
			if(top.Type == "Page")
			{
				Utility.TraceLine("Found a page: " + top.ToString());
				retval.Add(top);
				return retval;	
			}
			
			if(top.Type == "Pages")
			{
				DictionaryItem kids = top.Dictionary.Get("Kids");
				foreach(Object kid in kids.ValueAsObjects(m_objects))
				{
					retval.Add(TraversePagesTree(kid));
				}
				return retval;
			}
			
			if(top.Type == "")
			{
				foreach(Object kid in top.DirectValueAsObjects(m_objects))
				{
					retval.Add(TraversePagesTree(kid));
				}
				
				return retval;
			}
			
			throw new RuntimeException("Unknown object type \"" + top.Type + "\" in object " + top.Number + 
				" " + top.Generation + " pages tree");
		}
		
		public int XrefLocation
		{
			get
			{
				return m_xrefLocation;
			}
		}
		
		public bool PdfACompliant
		{
			get
			{
				return m_pdfa;
			}
		}
	}
}
