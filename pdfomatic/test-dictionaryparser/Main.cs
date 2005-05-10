using System;
using System.IO;

using OpenPdf;

class MainClass
{
	public static void Main(string[] args)
	{
		TestCase("Not a dictionary", "hdsgkjhdfjsghjkfdshgh", 0);
		TestCase("Empty dictionary 1", "<<>>", 0);
		TestCase("Empty dictionary 2", "<< >>", 0);
		TestCase("Name and int", "<</Name 1>>", 1);
		TestCase("Name and int * 3", "<</Name 1 /Foo 2 /Bar 3>>", 3);
		TestCase("Name and name", "<</Name/Foo>>", 1);
		TestCase("Name and name * 3", "<</Name/Name/Foo/Foo/Bar/Bar>>", 3);
		TestCase("Interesting quoting", "<</ID[<2ff783c9846ab546bd49f709cb7be307><2ff783c9846ab546bd49f709cb7be307>]>>", 1);
		TestCase("Sub dictionary", "<</Foo<</Bar 1>>>>", 1);
		TestCase("PDF 16 fails", "<< /Type /Page /MediaBox [ 0 0 592 840 ] /Parent 2 0 R /Rotate 0 /PieceInfo << /Illustrator 6 0 R >> /ArtBox [ -15.29199 -31 541 781.60938 ] /Thumb 140 0 R /Contents 142 0 R /Resources << /ExtGState << /R1 4 0 R /R2 78 0 R /R5 117 0 R /R9 137 0 R >> /Font << /F1 73 0 R >> /XObject << /Fm1 75 0 R /Fm2 114 0 R /Fm4 134 0 R >> /ProcSet [ /PDF /Text ] >> >>", 9);
	}
	
	public static void TestCase(string label, string input, int expectedCount)
	{
		Console.Write(label + ": ");
		int count = Test(input);
		if(count == expectedCount)
			Console.WriteLine("pass");
		else
			Console.WriteLine("fail (got " + count + ")");
	}
	
	public static int Test(string input)
	{
		PdfStream ps = new PdfStream();
		ps.FillFromString(input);
		
		Dictionary dict = new Dictionary();
		try
		{
			dict.ReadDictionary(ps);
		}
		catch(Exception except)
		{
			Console.Write("exception (" + except.Message + ") ");
			return -1;
		}
		
		return dict.Count;
	}
}