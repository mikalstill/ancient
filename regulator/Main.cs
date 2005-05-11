using System;
using System.Text.RegularExpressions;

class MainClass
{
	public static void Main(string[] args)
	{
		Console.WriteLine("Enter the line of text you would like to match on:");
		string line = Console.ReadLine();
		
		while(true)
		{
			Console.Write(">> ");
			string regex = Console.ReadLine();
			
			try
			{
				Regex re = new Regex(regex);
				Match mtch = re.Match(line);
				if(mtch.Success)
				{
					Console.WriteLine("Match: ");
					foreach(Group grp in mtch.Groups)
					{
						Console.WriteLine("\t" + grp.Value);
					}
				}
				else
				{
					Console.WriteLine("Didn't match");
				}
			}
			catch
			{
				Console.WriteLine("EXCEPTION");
			}
		}
	}
}