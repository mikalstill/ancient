// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

using OpenPdf;

namespace OpenPdf
{
	public class PdfStream
	{
		private Stream m_st;
		
		public void FillFromFile(string filename)
		{
			m_st = new FileStream(filename, FileMode.Open);
			if(m_st == null)
			{
				throw new ParseException("Failed to open file: \"" + filename + "\"");
			}
		}
		
		public void FillFromString(string data)
		{
			UTF8Encoding ue = new UTF8Encoding();
			m_st = new MemoryStream();
			m_st.Write(ue.GetBytes(data), 0, data.Length);
			Position = 0;
		}
		
		public override string ToString()
		{
			long pos = Position;
			Position = 0;
			string retval = ReadBlock((int) Length);
			Position = pos;
			return retval;
		}
		
		public bool Eof
		{
			get
			{
				return Position >= Length;
			}
		}
		
		public long Position
		{
			get
			{
				return m_st.Position;
			}
			set
			{
				m_st.Position = value;
			}
		}
		
		public long Length
		{
			get
			{
				return m_st.Length;
			}
		}
		
		// We expect to see some text here...
		public bool Expect(string expected, bool throwOnFail)
		{
			return Expect(expected, throwOnFail, false);
		}
		
		public bool Expect(string expected, bool throwOnFail, bool stripTrailingWhitespace)
		{
			string read = PeekBlock(expected.Length);
			
			if(read != expected)
			{
				if(throwOnFail)
				{
					throw new Exception("found \"" + read + "\" at " + Position + " instead of \"" + expected + "\"");
				}
				return false;
			}
			
			ReadBlock(expected.Length);
			while(Utility.IsWhite(PeekBlock(1)))
			{
				ReadBlock(1);
			}
			return true;
		}
		
		public void TrimFromEnd(string expected, bool leadingWhiteSpace)
		{
			long pos = Position;
			Position = Length - expected.Length;
			if(PeekBlock(expected.Length) != expected)
			{
				Position = pos;
				return;
			}
				
			while(Utility.IsWhite(PeekBlock(1)))
			{
				Position--;
			}
			
			// TODO: This is really expensive, but better ways are too hard to code for now
			long len = Position;
			Position = 0;
			MemoryStream newst = new MemoryStream();
			byte[] buf = new byte[len + 1];
			m_st.Read(buf, 0, (int) (len));
			newst.Write(buf, 0, (int) (len));
			m_st = newst;
			Position = pos;
		}
		
		// Return a given number of characters without changing the state of the stream
		public string PeekBlock(int count)
		{
			long pos = Position;
			string line = ReadBlock(count);
			Position = pos;
			return line;
		}
		
		// Read a given number of characters
		public string ReadBlock(int count)
		{
			// TODO: This can be nicer if we use an encoder as we do for FillFromString
			StringBuilder sb = new StringBuilder();
			byte[] data = new byte[count];
			
			count = m_st.Read(data, 0, count);
			//Utility.TraceLine("Read " + count + " bytes from stream at " + Position + " (EOF = " + (Length == Position) + ")");
			for(int i = 0; i < count; i++)
			{
				sb.Append((char) data[i]);
			}
			
			return sb.ToString();
		}
		
		// Return a given number of characters without changing the state of the stream
		public string PeekBlockWithoutNewlines(int count)
		{
			long pos = Position;
			string line = ReadBlockWithoutNewlines(count);
			Position = pos;
			return line;
		}
		
		// Read a given number of characters
		public string ReadBlockWithoutNewlines(int count)
		{
			// TODO: This can be nicer if we use an encoder as we do for FillFromString
			StringBuilder sb = new StringBuilder();
			byte[] data = new byte[count];
			
			count = m_st.Read(data, 0, count);
			//Utility.TraceLine("Read " + count + " bytes from stream at " + Position + " (EOF = " + (Length == Position) + ")");
			for(int i = 0; i < count; i++)
			{
				if(((char) data[i] != '\n') && ((char) data[i] != '\r'))
				{
					sb.Append((char) data[i]);
				}
				else
				{
					sb.Append(' ');
				}
			}
			
			return sb.ToString();
		}
		
		public string ReadRange(long start, long end)
		{
			// TODO: This can be nicer if we use an encoder as we do for FillFromString
			long pos = Position;
			int count = (int) (end - start);
			Position = start;
			
			StringBuilder sb = new StringBuilder();
			byte[] data = new byte[count];
			
			count = m_st.Read(data, 0, count);
			for(int i = 0; i < count; i++)
			{
				sb.Append((char) data[i]);
			}
			
			Position = pos;
			return sb.ToString();
		}

		// Read a line whilst not affecting the state of the stream
		public string PeekLine()
		{
			long pos = Position;
			string line = ReadLine();
			Position = pos;
			return line;
		}
		
		public string PeekLine(bool stripLeadingWhitespace, bool stripTrailingWhitespace)
		{
			long pos = Position;
			string line = ReadLine(true, true);
			Position = pos;
			return line;
		}			
									
		// Read to the end of a line...
		public string ReadLine()
		{
			return ReadLine(false, false);
		}
			
		public string ReadLine(bool stripLeadingWhitespace, bool stripTrailingWhitespace)
		{
			StringBuilder line = new StringBuilder();
			byte[] data = new byte[1];
		
			data[0] = (byte) ' ';
			if(stripLeadingWhitespace)
			{
				while((data[0] == ' ') || (data[0] == '\t'))
				{
					m_st.Read(data, 0, 1);
					if(Eof)
					{
						return line.ToString();
					}
				}
				line.Append((char) data[0]); 
			}
			
			while((data[0] != '\r') && (data[0] != '\n') && !Eof)
			{
				m_st.Read(data, 0, 1);
				if((data[0] != '\r') && (data[0] != '\n'))
				{
					line.Append((char) data[0]);
				}
			}
			
			if(data[0] == '\r')
			{
				long pos = Position;
				m_st.Read(data, 0, 1);
				if(Eof)
				{
					return line.ToString();
				}
				
				if(data[0] != '\n')
				{
					Position = pos;
				}
			}
			
			if(stripTrailingWhitespace)
			{
				while(line.ToString().EndsWith(" ") || line.ToString().EndsWith(" \t"))
				{
					line.Remove(line.Length - 1, 1);
				}
			}
			
			return line.ToString();
		}
		
		public byte[] ReadLineAsBytes()
		{
			ByteBuilder line = new ByteBuilder();
			byte[] data = new byte[1];
		
			data[0] = (byte) ' ';
			
			while((data[0] != '\r') && (data[0] != '\n') && !Eof)
			{
				m_st.Read(data, 0, 1);
				if((data[0] != '\r') && (data[0] != '\n'))
				{
					line.Append(data[0]);
				}
			}
			
			if(data[0] == '\r')
			{
				long pos = Position;
				m_st.Read(data, 0, 1);
				
				if(data[0] != '\n')
				{
					Position = pos;
					line.Append((byte) '\n');
				}
			}
			
			return line.ToBytes();
		}
		
		public string RegexMatch(string reg)
		{
			return RegexMatch(reg, true);
		}
		
		public string RegexMatch(string reg, bool consumeonmatch)
		{
			Regex re = new Regex(reg);
			string line = PeekBlockWithoutNewlines(4096);
			
			Match mtch = re.Match(line);
			if(!mtch.Success)
			{
				Utility.TraceLine("Line " + line.Substring(0, 20) + " (first 20) does not match " + reg);
				return "";
			}
			
			Utility.TraceLine("Line  " + line.Substring(0, 20) + " (first 20) matches " + reg);
			
			if(consumeonmatch)
			{
				ReadBlock(mtch.Groups[1].Value.Length);
			}
			return mtch.Groups[1].Value;
		}
		
		public void ConsumeWhitespace()
		{
			while(Utility.IsWhite(PeekBlock(1)))
				ReadBlock(1);
		}
	}
}