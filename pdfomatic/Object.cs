// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace OpenPdf
{
	public class Object
	{
		private int m_number;
		private int m_generation;
		private long m_parseStartedAt = -1;
		
		private Dictionary m_dictionary = new Dictionary();
		private ByteBuilder m_stream = new ByteBuilder();
		private string m_directValue;
		
		// It has already been verified that the next thing in the stream is an object
		public Object(PdfStream st, string header)
		{
			Utility.TraceLine("Object start position is " + st.Position + " - " + header.Length);
			m_parseStartedAt = st.Position - header.Length;
			
			Regex re = new Regex("^([0-9]+) ([0-9]+)");
			Match mtch = re.Match(header);
			if(!mtch.Success)
			{
				throw new ParseException("Object header was not valid");
			}
			
			m_number = Int32.Parse(mtch.Groups[1].Value);
			m_generation = Int32.Parse(mtch.Groups[2].Value);
			st.ConsumeWhitespace();
			Parse(st);
		}
		
		public Object(int number, int generation)
		{
			m_number = number;
			m_generation = generation;
		}
		
		public Object()
		{
			m_number = -1;
			m_generation = -1;
			m_parseStartedAt = -1;
		}
		
#region Parsing methods
		public void Parse(PdfStream st)
		{
			Utility.TraceLine("Read object parse pass for object " + Number + " " + Generation);
		
			try
			{
				m_dictionary.ReadDictionary(st);
				ReadStream(st);
				ReadDirectValue(st);
				if(st.PeekLine(true, true) != "endobj")
				{
					throw new ParseException("Object " + Number + " " + Generation + " ended early: \"" + 
						st.PeekLine(true, true) + "\"");
				}
				st.ReadLine();
			}
			catch(ParseException ex)
			{
				Utility.CrashDump(m_parseStartedAt, st.Position, st);
				throw new RuntimeException("Error processing object: " + ex.Message + " for object " + Number + " " + Generation);
			}
		}
		
		private void ReadStream(PdfStream st)
		{
			if(st.PeekLine() == "stream")
			{
				Utility.TraceLine("Read stream: ");
				st.ReadLine();
				while(!st.PeekLine(true, true).EndsWith("endstream") && !st.Eof)
				{
					Utility.Trace("-");
					m_stream.Append(st.ReadLineAsBytes());
				}
				Utility.TraceLine(" Done");
				st.ReadLine();
				
				// Sometimes there is a blank line after the endstream
				st.ConsumeWhitespace();
			}
		}
		
		private void ReadDirectValue(PdfStream st)
		{
			if(m_dictionary.Count == 0)
			{
				while(st.PeekLine() != "endobj")
				{
					m_directValue += st.ReadLine();
				}
			}
			else
			{
				Utility.TraceLine("Not eligible for direct value");
			}
		}
#endregion
		
#region Utility methods
		public bool Valid
		{
			get
			{
				if((m_number != -1) && (m_generation != -1))
					return true;
				else
					return false;
			}
		}
		
		public string Type
		{
			get
			{
				DictionaryItem di = m_dictionary.Get("Type");
				if(di.Valid)
				{
					return di.ValueAsString();
				}
				
				return "";
			}
		}
		
		public int Number
		{
			get
			{
				if(Valid)
					return m_number;
				else
					throw new RuntimeException("No number as object is invalid");
			}
		}
		
		public int Generation
		{
			get
			{
				if(Valid)
					return m_generation;
				else
					throw new RuntimeException("No generation, as object is invalid");
			}
		}
		
		public Dictionary Dictionary
		{
			get
			{
				return m_dictionary;
			}
		}
		
		public byte[] Stream
		{
			get
			{
				return m_stream.ToBytes();
			}
		}
		
		public long StartedAt
		{
			get
			{
				return m_parseStartedAt;
			}
		}
		
		public string DirectValue
		{
			get
			{
				return m_directValue;
			}
		}
		
		public ObjectCollection DirectValueAsObjects(ObjectCollection objs)
		{
			DictionaryItem di = new DictionaryItem("Direct", DirectValue);
			return di.ValueAsObjects(objs);
		}
		
		public override string ToString()
		{
			return Number + " " + Generation + " R";
		}
#endregion
	}
}
