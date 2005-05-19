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
		TestCase("PDF 16 fails", "<< 
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