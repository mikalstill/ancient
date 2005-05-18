
// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.Configuration;
using System.Text;
using System.Text.RegularExpressions;

namespace OpenPdf
{
	internal class Utility
	{
		internal static bool IsWhite(string s)
		{
			if(s == " ") return true;
			if(s == "\t") return true;
			if(s == "\r") return true;
			if(s == "\n") return true;
			return false;		
		}
		
		internal static bool IsWhite(char s)
		{
			if(s == ' ') return true;
			if(s == '\t') return true;
			if(s == '\r') return true;
			if(s == '\n') return true;
			return false;		
		}
		
		internal static bool IsAllWhitespace(string s)
		{
			foreach(char c in s)
			{
				if(!IsWhite(c))
					return false;	
			}
			
			return true;
		}
		
		internal static bool IsNewLine(string s)
		{
			if(s == "\r") return true;
			if(s == "\n") return true;
			return false;		
		}
		
		internal static void CrashDump(long start, long end, PdfStream st)
		{
			if(start == -1)
				return;
				
			Console.WriteLine("------------------------------------------------------------------------");
			Console.WriteLine("OpenPdf Crash Dump.");
			Console.WriteLine("");
			Console.WriteLine("The following block caused a parsing error.");
			Console.WriteLine("");
			Console.WriteLine(st.ReadRange(start, end));
			Console.WriteLine("------------------------------------------------------------------------");
		}
		
		internal static void TraceLine(string message)
		{
			if(ConfigurationSettings.AppSettings["TraceMode"] == "true")
			{
				Console.WriteLine(message);	
			}
		}
		
		internal static void Trace(string message)
		{
			if(ConfigurationSettings.AppSettings["TraceMode"] == "true")
			{
				Console.Write(message);	
			}
		}

		internal static void Trace(char message)
		{
			Utility.Trace(message.ToString());
		}
		
		public static bool RegexCompare(string regex, string victim)
		{
			Regex re = new Regex(regex);
			Match m = re.Match(victim);
			return m.Success;
		}
		
		public static int min(int a, int b)
		{
			if(a < b) return a;
			return b;
		}
	}
}