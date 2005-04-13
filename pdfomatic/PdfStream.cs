// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.IO;
using System.Text;
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
			long pos = Position;
			string read = ReadBlock(expected.Length);
			
			if(read != expected)
			{
				Position = pos;

				if(throwOnFail)
				{
					throw new Exception("failed to find expected text \"" + expected + "\"");
				}
				return false;
			}
			
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
			for(int i = 0; i < count; i++)
			{
				sb.Append((char) data[i]);
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
	}
}