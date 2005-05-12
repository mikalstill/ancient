// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.Collections;
using System.IO;

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
				// Until we get to the end of this dictionary
				while(st.PeekBlock(2) != ">>")
				{
					// Consume any leading whitespace
					while(Utility.IsWhite(st.PeekBlock(1)))
						st.ReadBlock(1);
				
					// Name
					Utility.TraceLine("Checking for a name");
					string name = st.RegexMatch("^(/[^ \t/\\[\\]\\(\\)\\<\\>]+)[ \t]*");
					if(name == "")
					{
						throw new ParseException("Dictionary items must have a name");
					}
					
					st.ConsumeWhitespace();
				
					// Value
					if(st.PeekBlock(2) == "<<")
					{
						Utility.TraceLine("Traversing subdictionary");
						Dictionary dict = new Dictionary();
						dict.ReadDictionary(st);
						DictionaryItem di = new DictionaryItem(name, dict);
						Add(di);
					}
					else
					{
						Utility.TraceLine("Finding the value");
						string nameval = st.RegexMatch("^(/[^ \t/\\[\\]\\(\\)\\<\\>]+)", false);
						string objrefval = st.RegexMatch("^([0-9]+ [0-9]+ R)", false);
						string numval = st.RegexMatch("^(-{0,1}[0-9]+)", false);
						string floatval = st.RegexMatch("^(-{0,1}[0-9]+\\.[0-9]+)", false);
						
						// The old version of these:
						//string rdbrackets = st.RegexMatch("^(\\([^\\)]*\\)+)[ \t]*", false);
						
						string sqbrackets = st.RegexMatch(@"^(\[.*?[^\\]+?\])", false);
						string rdbrackets = st.RegexMatch(@"^(\(.*?[^\\]+?\))", false);
						string anbrackets = st.RegexMatch(@"^(\<.*?[^\\]+?\>)", false);
						string singleword = st.RegexMatch("^([^ \t]+)", false);
						
						if(nameval != "")
						{
							DictionaryItem di = new DictionaryItem(name, nameval);
							Add(di);
							st.ReadBlock(nameval.Length);
						}
						else if(objrefval != "")
						{
							DictionaryItem di = new DictionaryItem(name, objrefval);
							Add(di);
							st.ReadBlock(objrefval.Length);
						}
						else if(floatval != "")
						{
							DictionaryItem di = new DictionaryItem(name, floatval);
							Add(di);
							st.ReadBlock(floatval.Length);
						}
						else if(numval != "")
						{
							DictionaryItem di = new DictionaryItem(name, numval);
							Add(di);
							st.ReadBlock(numval.Length);
						}
						else if(sqbrackets != "")
						{
							DictionaryItem di = new DictionaryItem(name, sqbrackets);
							Add(di);
							st.ReadBlock(sqbrackets.Length);
						}
						else if(rdbrackets != "")
						{
							DictionaryItem di = new DictionaryItem(name, rdbrackets);
							Add(di);
							st.ReadBlock(rdbrackets.Length);
						}
						else if(anbrackets != "")
						{
							DictionaryItem di = new DictionaryItem(name, anbrackets);
							Add(di);
							st.ReadBlock(anbrackets.Length);
						}
						else if(singleword != "")
						{
							DictionaryItem di = new DictionaryItem(name, singleword);
							Add(di);
							st.ReadBlock(singleword.Length);
						}
						else
						{
							throw new ParseException("Unknown value format: " + st.PeekLine());
						}
					}
					
					st.ConsumeWhitespace();
				}
				
				// Skip over the >> at the end of the dictionary
				st.ReadBlock(2);
				st.ConsumeWhitespace();
			}
		}
#endregion
	}
}