// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;
using System.Text;
using System.Text.RegularExpressions;

namespace OpenPdf
{
	public class DictionaryItem
	{
		private string m_name;
		private ValueType m_type;
		
		private string m_stringval;
		private int m_intval;
		private ObjectReferenceCollection m_objects = new ObjectReferenceCollection();
		private Dictionary m_dictionary;
		
		public enum ValueType
		{
			String = 0,
			Objects,
			Dictionary,
			Number
		}
		
		public DictionaryItem(string name, Dictionary subdict)
		{
			if(name.StartsWith("/"))
			{
				m_name = name.Substring(1, name.Length - 1);
			}
			else
			{
				m_name = name;
			}
			
			m_dictionary = subdict;
			m_type = ValueType.Dictionary;
		}
		
		public DictionaryItem(string name, string val)
		{
			if(name.StartsWith("/"))
			{
				m_name = name.Substring(1, name.Length - 1);
			}
			else
			{
				m_name = name;
			}
			Utility.TraceLine("Dictionary item named: " + name + " added");
			
			if(val.StartsWith("["))
			{
				val = val.Substring(1, val.Length - 2);
			}
			else if(val.StartsWith("/"))
			{
				val = val.Substring(1, val.Length - 1);
			}
			
			Regex reObjects = new Regex("^[ \t]*([0-9]+) ([0-9]+) R[ \t]*(.*)$");
			Match mtchObjects = reObjects.Match(val);
			Regex reNumber = new Regex("^[0-9]$");
			Match mtchNumber = reNumber.Match(val);
			
			if(mtchObjects.Success)
			{
				// Object array
				string tempval = val;
				while(tempval != "")
				{
					mtchObjects = reObjects.Match(tempval);
					ObjectReference or = new ObjectReference();
					
					try
					{
						or.Number = Int32.Parse(mtchObjects.Groups[1].Value);
						or.Generation = Int32.Parse(mtchObjects.Groups[2].Value);
					}
					catch(Exception)
					{
						Utility.TraceLine("Gave up on object reference parsing for this dictionary item: " + val);
						m_objects.Clear();
						m_stringval = val;
						m_type = ValueType.String;
						return;
					}
					m_objects.Add(or);
					
					try
					{
						tempval = mtchObjects.Groups[3].Value;
					}
					catch(Exception)
					{
						tempval = "";
					}
				}
				
				m_type = ValueType.Objects;
			}
			else if(mtchNumber.Success)
			{
				m_intval = Int32.Parse(val);
				m_type = ValueType.Number;
			}
			else
			{
				m_stringval = val;
				m_type = ValueType.String;
			}
		}
		
		public DictionaryItem()
		{
			m_name = null;
		}
		
		public string Name
		{
			get
			{
				return m_name;
			}
		}
		
		public string ValueAsString()
		{
			switch(m_type)
			{
				case ValueType.String:
					return m_stringval;
					
				case ValueType.Objects:
					// TODO
					return "Object(TODO)";
					
				default:
					throw new RuntimeException("Unknown type to convert to string");
			}
		}
		
		public Object ValueAsObject(ObjectCollection objs)
		{
			if(!Valid)
				return new Object();
			
			if(m_objects.Count == 0)
				throw new RuntimeException("There are no objects to return");
				
			if(m_objects.Count != 1)
				return new Object();
			return objs.Get(m_objects[0]);
		}
		
		public ObjectCollection ValueAsObjects(ObjectCollection objs)
		{
			if(!Valid)
				return new ObjectCollection();
			
			if(m_objects.Count == 0)
				throw new RuntimeException("There are no objects to return");
			
			ObjectCollection oc = new ObjectCollection();
			foreach(ObjectReference or in m_objects)
			{
				oc.Add(objs.Get(or));
			}
			
			return oc;
		}
		
		public int ValueAsInteger()
		{
			if(m_type != ValueType.Number)
				throw new RuntimeException("Value cannot be represented as an integer");
			return m_intval;
		}
		
		public Dictionary ValueAsDictionary()
		{
			if(m_type != ValueType.Dictionary)
				throw new RuntimeException("Value cannot be represented as a dictionary");
			return m_dictionary;
		}
		
		public bool Valid
		{
			get
			{
				if(m_name != null)
					return true;
				else
					return false;
			}
		}
		
		public ValueType Type
		{
			get
			{
				return m_type;
			}
		}
		
		public override string ToString()
		{
			StringBuilder retval = new StringBuilder();
			retval.Append(Name + " = ");
			
			switch(m_type)
			{
				case ValueType.String:
					retval.Append(m_stringval);
					break;
				
				case ValueType.Objects:
					retval.Append(m_objects.ToString());
					break;
			
				case ValueType.Dictionary:
					retval.Append("TODO: Not yet supported");
					break;
			
				case ValueType.Number:
					retval.Append(m_intval);
					break;
					
				default:
					throw new RuntimeException("There is do ToString() formatter for this DictionaryItem type");
			}
			
			return retval.ToString();
		}
	}
}