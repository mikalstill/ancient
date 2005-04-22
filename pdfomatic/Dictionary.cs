// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

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
			Utility.TraceLine("Adding " + di.ToString());
			m_hash[di.Name] = di;
		}
		
		public DictionaryItem Get(string name)
		{
			if(m_hash[name] == null)
			{
				Utility.Trace("Dictionary name " + name + " missing. We have: ");
				foreach(string dikey in m_hash.Keys)
				{
					Utility.Trace(((DictionaryItem)m_hash[dikey]).Name + " ");
				}
				Utility.TraceLine("");
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
		
		public IEnumerator GetEnumerator()
		{
			return m_hash.GetEnumerator();
		}
		
#region Parsing code
		private enum QuoteType
		{
			None,
			Round,
			Square,
			Angle		
		}

		public void ReadDictionary(PdfStream st)
		{
			if(st.Expect("<<", false))
			{
				// Read all of the dictionary into one big string
				StringBuilder bloc = new StringBuilder();
				int count = 1;
				
				string thischar;
				string lastchar = " ";
				string thischarforlast;
				QuoteType quote = QuoteType.None;
				
				bool hasname = false;
				StringBuilder line = new StringBuilder();
				Regex isname = new Regex("^/[A-Za-z0-9]*$");
				Match mtch;
				
				Utility.TraceLine("Reading a dictionary: ");
				while((count > 0) && !st.Eof)
				{
					thischar = st.ReadBlock(1);
					thischarforlast = thischar;
					Utility.Trace(thischar);
					
					// If we're inside something quoted, then we ignore the end of dictionary markers
					// until we get to the end of the quoted section
					if(quote != QuoteType.None)
					{
						switch(quote)
						{
							case QuoteType.Angle:
								if((thischar == ">") && (lastchar != "\\"))
								{
									quote = QuoteType.None;
									thischarforlast = " ";
									Utility.Trace(" [ENDANQUOTE] ");
								}
								break;
								
							case QuoteType.Round:
								if((thischar == ")") && (lastchar != "\\"))
								{
									quote = QuoteType.None;
									Utility.Trace(" [ENDRDQUOTE] ");
								}
								break;
								
							case QuoteType.Square:
								if((thischar == "]") && (lastchar != "\\"))
								{
									quote = QuoteType.None;
									thischarforlast = " ";
									Utility.Trace(" [ENDSQQUOTE] ");
								}
								break;
						}
					}
					
					// Otherwise, it's possible that we're finishing the dictionary
					else
					{
						if((thischar == ">") && (lastchar == ">"))
						{
							count--;
							Utility.Trace(" [count = " + count + "] ");
						}
						
						else if((thischar == "<") && (lastchar == "<"))
						{
							count++;
							Utility.Trace(" [count = " + count + "] ");
						}
					}
					
					// If we have previously found a name, then it is possible that this is something
					// quoted
					if(hasname && (quote == QuoteType.None))
					{
						switch(thischar)
						{
							case "(":	quote = QuoteType.Round; Utility.Trace(" [STARTRDQUOTE] ");		break;
							case "[":	quote = QuoteType.Square; Utility.Trace(" [STARTSQQUOTE] ");	break;
							case "<":
								if((lastchar != "<") && (st.PeekBlock(1) != "<"))
								{
									quote = QuoteType.Angle; Utility.Trace(" [STARTANQUOTE] ");
								}
								break;
						}
					}
					
					// Check if we have found a name on this line yet
					if(!hasname)
					{
						mtch = isname.Match(line.ToString());
						if(mtch.Success)
						{
							hasname = true;
							Utility.Trace(" [NAME] ");
						}
					}
					
					// And we put the character into the line and bloc
					if((thischar == "\r") || (thischar == "\n"))
					{
						line.Append(" ");
						bloc.Append(" ");
						Utility.TraceLine(" [END]");
					}
					else
					{
						line.Append(thischar);
						bloc.Append(thischar);
					}
					
					lastchar = thischarforlast;
				}
				
				// Make sure the dictionary was balanced
				if(count > 0)
				{
					throw new ParseException("Dictionary didn't end: " + count);
				}
				Utility.TraceLine(" Done");
				
				ReadDictionary(bloc.ToString());
				
				string peeked = st.PeekLine(true, true);
				Utility.TraceLine("Data trailing after dictionary: \"" + peeked + "\"");
				if((peeked != "stream") && (peeked != "endobj"))
				{
					Utility.TraceLine("Wasted a line");
					st.ReadLine();
				}
			}
			else
			{
				Utility.TraceLine("Object has no dictionary: " + st.PeekLine());
			}
		}
				
		public void ReadDictionary(string processing)
		{
			Utility.TraceLine("Processing dictionary line: " + processing);
			Regex reObjRef =      new Regex("^/([^ \t/\\(\\[]+)[ \t]*([0-9]+ [0-9]+ R)[ \t]*(.*)$");
			Regex reObjRefs =     new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\[[ \t]*([0-9]+ [0-9]+ R[ \t]*)*[ \t]*\\])[ \t]*(.*)$");
			Regex reDictStart =   new Regex("^/([^ \t/\\(\\[]+)[ \t]*<<[ \t]*(.*)$");
			Regex reName =        new Regex("^/([^ \t/\\(\\[]+)[ \t]*/([^ \t/\\(\\[]+)[ \t]*(.*)$");
			Regex reStringOne =   new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\[([^\\]]*|\\(|\\))*\\])[ \t]*(.*)$");
			Regex reStringTwo =   new Regex("^/([^ \t/\\(\\[]+)[ \t]*([^ \t]*)[ \t]*(.*)$");
			Regex reStringThree = new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\(([^\\)]*|\\(|\\))*\\))[ \t]*(.*)$");
			Regex reStringFour  = new Regex("^/([^ \t/\\(\\[]+)[ \t]*(\\<([^\\>]*|\\<|\\>)*\\>)[ \t]*(.*)$");
			Regex reNumber =      new Regex("^/([^ \t/\\(\\[]+)[ \t]+([0-9]+)[ \t]*(.*)$");
			Regex reDecimal =     new Regex("^/([^ \t/\\(\\[]+)[ \t]+([0-9]+\\.[0-9]+)[ \t]*(.*)$");

			Match mtchObjRef;
			Match mtchObjRefs;
			Match mtchDictStart;
			Match mtchName;
			Match mtchStringOne;
			Match mtchStringTwo;
			Match mtchStringThree;
			Match mtchStringFour;
			Match mtchNumber;
			Match mtchDecimal;
			
			while(processing != "")
			{
				mtchObjRef = reObjRef.Match(processing);
				mtchObjRefs = reObjRefs.Match(processing);
				mtchDictStart = reDictStart.Match(processing);
				mtchName = reName.Match(processing);
				mtchStringOne = reStringOne.Match(processing);
				mtchStringTwo = reStringTwo.Match(processing);
				mtchStringThree = reStringThree.Match(processing);
				mtchStringFour = reStringFour.Match(processing);
				mtchNumber = reNumber.Match(processing);
				mtchDecimal = reDecimal.Match(processing);
				
				if(mtchObjRef.Success)
				{
					Utility.TraceLine("Dictionary: object reference");
					DictionaryItem di = new DictionaryItem(mtchObjRef.Groups[1].Value, mtchObjRef.Groups[2].Value);
					Add(di);
					processing = mtchObjRef.Groups[3].Value;
				}
				else if(mtchObjRefs.Success)
				{
					Utility.TraceLine("Dictionary: object references");
					DictionaryItem di = new DictionaryItem(mtchObjRefs.Groups[1].Value, mtchObjRefs.Groups[2].Value);
					Add(di);
					processing = mtchObjRefs.Groups[4].Value;
				}
				else if(mtchDictStart.Success)
				{
					// Determine how much of the remaining stream is to be used for this dictionary
					// This is very similar, but subtly different from the stream code above
					Utility.TraceLine("Dictionary: dictionary start: " + mtchDictStart.Groups[2].Value);
					StringBuilder bloc = new StringBuilder();
					int count = 1;
					
					char lastchar = ' ';
					char thischar;
					char thischarforlast;
					int i = 0;
					
					QuoteType quote = QuoteType.None;
				
					bool hasname = false;
					StringBuilder line = new StringBuilder();
					Regex isname = new Regex("^/[A-Za-z0-9]*$");
					Match mtch;
				
					while((count > 0) && (i < mtchDictStart.Groups[2].Value.Length))
					{
						thischar = mtchDictStart.Groups[2].Value[i];
						thischarforlast = thischar;
						Utility.Trace(thischar);
						
						// If we're inside something quoted, then we ignore the end of dictionary markers
						// until we get to the end of the quoted section
						// TODO: I really should try and find a wa\y to roll these two blocks of code
						// together
						if(quote != QuoteType.None)
						{
							switch(quote)
							{
								case QuoteType.Angle:
									if((thischar == '>') && (lastchar != '\\'))
									{
										quote = QuoteType.None;
										thischarforlast = ' ';
										Utility.Trace(" [ENDANQUOTE] ");
									}
									break;
									
								case QuoteType.Round:
									if((thischar == ')') && (lastchar != '\\'))
									{
										quote = QuoteType.None;
										Utility.Trace(" [ENDRDQUOTE] ");
									}
									break;
									
								case QuoteType.Square:
									if((thischar == ']') && (lastchar != '\\'))
									{
										quote = QuoteType.None;
										thischarforlast = ' ';
										Utility.Trace(" [ENDSQQUOTE] ");
									}
									break;
							}
						}
						
						// Otherwise, it's possible that we're finishing the dictionary
						else
						{
							if((thischar == '>') && (lastchar == '>'))
							{
								count--;
								Utility.Trace(" [count = " + count + "] ");
							}
							
							else if((thischar == '<') && (lastchar == '<'))
							{
								count++;
								Utility.Trace(" [count = " + count + "] ");
							}
						}
						
						// If we have previously found a name, then it is possible that this is something
						// quoted
						if(hasname && (quote == QuoteType.None))
						{
							switch(thischar)
							{
								case '(':	quote = QuoteType.Round; Utility.Trace(" [STARTRDQUOTE] ");		break;
								case '[':	quote = QuoteType.Square; Utility.Trace(" [STARTSQQUOTE] ");	break;
								case '<':
									if((lastchar != '<') && (mtchDictStart.Groups[2].Value[i + 1] != '<'))
									{
										quote = QuoteType.Angle; Utility.Trace(" [STARTANQUOTE] ");
									}
									break;
							}
						}
						
						// Check if we have found a name on this line yet
						if(!hasname)
						{
							mtch = isname.Match(line.ToString());
							if(mtch.Success)
							{
								hasname = true;
								Utility.Trace(" [NAME] ");
							}
						}
						
						// And we put the character into the line and bloc
						if((thischar == '\r') || (thischar == '\n'))
						{
							line.Append(" ");
							bloc.Append(" ");
							Utility.TraceLine(" [END]");
						}
						else
						{
							line.Append(thischar);
							bloc.Append(thischar);
						}
						
						lastchar = thischarforlast;
						i++;
					}
					Utility.TraceLine("");
					
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
					Utility.TraceLine("Dictionary: name");
					DictionaryItem di = new DictionaryItem(mtchName.Groups[1].Value, mtchName.Groups[2].Value);
					Add(di);
					processing = mtchName.Groups[3].Value;
				}
				else if(mtchDecimal.Success)
				{
					Utility.TraceLine("Dictionary: decimal");
					DictionaryItem di = new DictionaryItem(mtchDecimal.Groups[1].Value, mtchDecimal.Groups[2].Value);
					Add(di);
					processing = mtchDecimal.Groups[3].Value;
				}
				else if(mtchNumber.Success)
				{
					Utility.TraceLine("Dictionary: number");
					DictionaryItem di = new DictionaryItem(mtchNumber.Groups[1].Value, mtchNumber.Groups[2].Value);
					Add(di);
					processing = mtchNumber.Groups[3].Value;
				}
				else if(mtchStringOne.Success)
				{
					Utility.TraceLine("Dictionary: string one");
					DictionaryItem di = new DictionaryItem(mtchStringOne.Groups[1].Value, mtchStringOne.Groups[2].Value);
					Add(di);
					processing = mtchStringOne.Groups[4].Value;
				}
				else if(mtchStringThree.Success)
				{
					Utility.TraceLine("Dictionary: string three");
					DictionaryItem di = new DictionaryItem(mtchStringThree.Groups[1].Value, mtchStringThree.Groups[2].Value);
					Add(di);
					processing = mtchStringThree.Groups[3].Value;
				}
				else if(mtchStringFour.Success)
				{
					Utility.TraceLine("Dictionary: string four");
					DictionaryItem di = new DictionaryItem(mtchStringFour.Groups[1].Value, mtchStringFour.Groups[2].Value);
					Add(di);
					processing = mtchStringFour.Groups[3].Value;
				}
				else if(mtchStringTwo.Success)
				{
					Utility.TraceLine("Dictionary: string two");
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