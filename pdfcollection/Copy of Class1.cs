using System;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;

namespace pdfdb
{
	class Class1
	{
		[STAThread]
		static void Main(string[] args)
		{
			// We store the great big list of things we've found here
			ArrayList alldocs = new ArrayList();
			Hashtable producer = new Hashtable();
			Hashtable tagged = new Hashtable();
			Hashtable encrypted = new Hashtable();
			Hashtable linearized = new Hashtable();
			Hashtable version = new Hashtable();

			Console.WriteLine("Find all the PDFs available in {0}", args[0]);
			string[] dirs = Directory.GetDirectories(args[0]);
			
			int i;
			for(i = 0; i < dirs.Length; i++)
			{
				// NB. This would also match a string of the form: 000000/foo/bar, 
				// which isn't really intended
				Match m = Regex.Match(dirs[i], "[0-9][0-9][0-9][0-9][0-9][0-9]");
				if( m.Success )
				{
					Console.WriteLine("{0}", m.ToString());
					StreamReader input = new StreamReader(args[0] + "/" + m.ToString() + "/data.info");
					while( input.Peek() != -1 )
					{
						string line = input.ReadLine();
						string[] tokens = Regex.Split(line, ":[ \t]+");
						if( tokens.Length > 1 )
						{
							switch(tokens[0])
							{
								case "Producer":
									producer[tokens[1]] += ";" + m.ToString();
									break;

								case "Tagged":
									tagged[tokens[1]] += ";" + m.ToString();
									break;

								case "Encrypted":
									encrypted[tokens[1]] += ";" + m.ToString();
									break;


								case "Optimized":
									linearized[tokens[1]] += ";" + m.ToString();
									break;

								case "PDF version":
									version[tokens[1]] += ";" + m.ToString();
									break;								
								
								default:
									Console.WriteLine("Match {0}~{1}~{2}", tokens.Length, tokens[0], tokens[1]);
									break;
							}
						}
						else
						{
							Console.WriteLine("Unknown line: {0}", line);
						}
					}
				}
			}

			// Dump out the producers list
			foreach(object key in producer.Keys)
			{
				Console.Write("{0}: ", key.ToString());
				string[] docids = Regex.Split(producer[key].ToString(), ";");
				for(i = 0; i < docids.Length; i++)
				{
					if( docids[i].Length > 0 )
						Console.Write("[{0}]", docids[i]);
				}
				Console.WriteLine("");				
			}
		}
	}
}
