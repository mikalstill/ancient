using System;
using System.Collections;
using System.Text;
using System.Text.RegularExpressions;

namespace OpenPdf
{
	public class Dictionary
	{
		Hashtable m_hash = new Hashtable();
	
		public void Add(DictionaryItem di)
		{
			m_hash[di.Name] = di;
		}
		
		public DictionaryItem Get(string name)
		{
			if(m_hash[name] == null)
			{
				Console.Write("Dictionary name " + name + " missing. We have: ");
				foreach(string dikey in m_hash.Keys)
				{
					Console.Write(((DictionaryItem)m_hash[dikey]).Name + " ");
				}
				Console.WriteLine("");
				return new DictionaryItem();
			}
			return (DictionaryItem) m_hash[name];
		}
		
		public int Count
		{
			get
			{
				return m_hash.Count;
			}
		}
		
#region Parsing code
		public void ReadDictionary(PdfStream st)
		{
			if(st.Expect("<<", false))
			{
				// Read all of the dictionary into one big string
				StringBuilder bloc = new StringBuilder();
				int count = 1;
				
				string lastchar = "";
				string thischar;
				string thischarforlast;
				Console.WriteLine("Reading a dictionary: ");
				while((count > 0) && !st.Eof)
				{
					thischar = st.ReadBlock(1);
					thischarforlast = thischar;
					if((thischar == ">") && (lastchar == ">"))
					{
						count--;
						thischarforlast = " ";
					}
					else if((thischar == "<") && (lastchar == "<"))
					{
						count++;
					}
					
					if((thischar != "\r") && (thischar != "\n"))
					{
						bloc.Append(thischar);
					}
					else if(!Utility.IsWhite(lastchar))
					{
						bloc.Append(" ");
					}
					
					lastchar = thischarforlast;
					Console.Write(".");
				}
				if(count > 0)
				{
					throw new ParseException("Dictionary didn't end");
				}
				Console.WriteLine(" Done");
				
				ReadDictionary(bloc.ToString());
				
				string peeked = st.PeekLine(true, true);
				Console.WriteLine("Data trailing after dictionary: \"" + peeked + "\"");
				if((peeked != "stream") && (peeked != "endobj"))
				{
					Console.WriteLine("Wasted a line");
					st.ReadLine();
				}
			}
			else
			{
				Console.WriteLine("Object has no dictionary: " + st.PeekLine());
			}
		}
				
		public void ReadDictionary(string processing)
		{
			Console.WriteLine("Processing dictionary line: " + processing);
			Regex reObjRef =      new Regex("^/([^ \t/\\(\\[]+)[ \t]*([0-9]+ [0-9]+ R)[ \t]*(.*)$");
			Regex reObjRefs =     new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\[([0-9]+ [0-9]+ R[ \t]*)*\\])[ \t]*(.*)$");
			Regex reDictStart =   new Regex("^/([^ \t/\\(\\[]+)[ \t]*<<[ \t]*(.*)$");
			Regex reName =        new Regex("^/([^ \t/\\(\\[]+)[ \t]*/([^ \t/\\(\\[]+)[ \t]*(.*)$");
			Regex reStringOne =   new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\[([^\\]]*|\\(|\\))*\\])[ \t]*(.*)$");
			Regex reStringTwo =   new Regex("^/([^ \t/\\(\\[]+)[ \t]*([^ \t]*)[ \t]*(.*)$");
			Regex reStringThree = new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\(([^\\)]*|\\(|\\))*\\))[ \t]*(.*)$");
			Regex reStringFour  = new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\<([^\\>]*|\\<|\\>)*\\>)[ \t]*(.*)$");
			Regex reNumber =      new Regex("^/([^ \t/\\(\\[]+)[ \t]+([0-9]+)[ \t]*(.*)$");
			Regex reDecimal =     new Regex("^/([^ \t/\\(\\[]+)[ \t]+([0-9]+\\.[0-9]+)[ \t]*(.*)$");
			
			while(processing != "")
			{
				Match mtchObjRef = reObjRef.Match(processing);
				Match mtchObjRefs = reObjRefs.Match(processing);
				Match mtchDictStart = reDictStart.Match(processing);
				Match mtchName = reName.Match(processing);
				Match mtchStringOne = reStringOne.Match(processing);
				Match mtchStringTwo = reStringTwo.Match(processing);
				Match mtchStringThree = reStringThree.Match(processing);
				Match mtchStringFour = reStringFour.Match(processing);
				Match mtchNumber = reNumber.Match(processing);
				Match mtchDecimal = reDecimal.Match(processing);
				
				if(mtchObjRef.Success)
				{
					Console.WriteLine("Dictionary: object reference");
					DictionaryItem di = new DictionaryItem(mtchObjRef.Groups[1].Value, mtchObjRef.Groups[2].Value);
					Add(di);
					processing = mtchObjRef.Groups[3].Value;
				}
				else if(mtchObjRefs.Success)
				{
					Console.WriteLine("Dictionary: object references");
					DictionaryItem di = new DictionaryItem(mtchObjRefs.Groups[1].Value, mtchObjRefs.Groups[2].Value);
					Add(di);
					processing = mtchObjRefs.Groups[4].Value;
				}
				else if(mtchDictStart.Success)
				{
					// Determine how much of the remaining stream is to be used for this dictionary
					// This is very similar, but subtly different from the stream code above
					Console.WriteLine("Dictionary: dictionary start");
					StringBuilder bloc = new StringBuilder();
					int count = 1;
					
					char lastchar = 'm';
					char thischar;
					char thischarforlast;
					int i = 0;
					while((count > 0) && (i < mtchDictStart.Groups[2].Value.Length))
					{
						thischar = mtchDictStart.Groups[2].Value[i];
						thischarforlast = thischar;
						if((thischar == '>') && (lastchar == '>'))
						{
							count--;
							thischarforlast = ' ';
						}
						else if((thischar == '<') && (lastchar == '<'))
						{
							count++;
						}
						
						if((thischar != '\r') && (thischar != '\n'))
						{
							bloc.Append(thischar);
						}
						else if(!Utility.IsWhite(lastchar))
						{
							bloc.Append(" ");
						}
						
						lastchar = thischarforlast;
						i++;
					}
					if(count > 0)
					{
						throw new ParseException("Sub dictionary didn't end");
					}
					
					while((i < mtchDictStart.Groups[2].Value.Length) && Utility.IsWhite(mtchDictStart.Groups[2].Value[i]))
					{
						i++;
					}
					
					string dict = bloc.ToString().Substring(0, bloc.Length - 2);
					string left = mtchDictStart.Groups[2].Value.Substring(i, mtchDictStart.Groups[2].Value.Length - i);
					
					Dictionary subdict = new Dictionary();
					subdict.ReadDictionary(dict);
					DictionaryItem di = new DictionaryItem(mtchDictStart.Groups[1].Value, subdict);
					Add(di);
					
					processing = left;
				}
				else if(mtchName.Success)
				{
					Console.WriteLine("Dictionary: name");
					DictionaryItem di = new DictionaryItem(mtchName.Groups[1].Value, mtchName.Groups[2].Value);
					Add(di);
					processing = mtchName.Groups[3].Value;
				}
				else if(mtchDecimal.Success)
				{
					Console.WriteLine("Dictionary: decimal");
					DictionaryItem di = new DictionaryItem(mtchDecimal.Groups[1].Value, mtchDecimal.Groups[2].Value);
					Add(di);
					processing = mtchDecimal.Groups[3].Value;
				}
				else if(mtchNumber.Success)
				{
					Console.WriteLine("Dictionary: number");
					DictionaryItem di = new DictionaryItem(mtchNumber.Groups[1].Value, mtchNumber.Groups[2].Value);
					Add(di);
					processing = mtchNumber.Groups[3].Value;
				}
				else if(mtchStringOne.Success)
				{
					Console.WriteLine("Dictionary: string one");
					DictionaryItem di = new DictionaryItem(mtchStringOne.Groups[1].Value, mtchStringOne.Groups[2].Value);
					Add(di);
					processing = mtchStringOne.Groups[4].Value;
				}
				else if(mtchStringThree.Success)
				{
					Console.WriteLine("Dictionary: string three");
					DictionaryItem di = new DictionaryItem(mtchStringThree.Groups[1].Value, mtchStringThree.Groups[2].Value);
					Add(di);
					processing = mtchStringThree.Groups[3].Value;
				}
				else if(mtchStringFour.Success)
				{
					Console.WriteLine("Dictionary: string four");
					DictionaryItem di = new DictionaryItem(mtchStringFour.Groups[1].Value, mtchStringFour.Groups[2].Value);
					Add(di);
					processing = mtchStringFour.Groups[3].Value;
				}
				else if(mtchStringTwo.Success)
				{
					Console.WriteLine("Dictionary: string two");
					DictionaryItem di = new DictionaryItem(mtchStringTwo.Groups[1].Value, mtchStringTwo.Groups[2].Value);
					Add(di);
					processing = mtchStringTwo.Groups[3].Value;
				}
				else if(processing == ">>")
				{
					return;
				}
				else
				{
					throw new ParseException("Unknown dictionary value: \"" + processing + "\"");
				}
			}
		}
#endregion
	}
}