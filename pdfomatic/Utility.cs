using System;

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
	}
}