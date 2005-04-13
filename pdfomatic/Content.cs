// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;

namespace OpenPdf
{
	public class Content
	{
		private Object m_object;
	
		///<summary>
		///Get the content object for this page
		///</summary>
		public Content(Pdf Document, Object PageObject)
		{
			DictionaryItem di = PageObject.Dictionary.Get("Content");
			if(!di.Valid)
			{
				throw new RuntimeException("Page object does not refer to a content object");
			}
			
			m_object = di.ValueAsObject(Document.Objects);
			if(!m_object.Valid)
			{
				throw new RuntimeException("Page object does not refer to a content object which is valid");
			}
		}
		
		///<summary>
		///Get a list of the filters on the stream. Returns null for no filters.
		///</summary>
		public string[] Filters
		{
			get
			{
				DictionaryItem di = m_object.Dictionary.Get("Filter");
				if(!di.Valid)
				{
					// No filters
					return null;
				}
				
				if(di.Type == DictionaryItem.ValueType.String)
				{
					return new string[] { di.ValueAsString() };
				}
				
				if(di.Type == DictionaryItem.ValueType.Dictionary)
				{
					Dictionary subdict = di.ValueAsDictionary();
					string[] results = new string[subdict.Count];
					
					int i = 0;
					foreach(DictionaryItem subdi in subdict)
					{
						results[i] = subdi.ValueAsString();
					}
					
					return results;
				}
				
				throw new RuntimeException("Unknown filters format");
			}
		}
		
		///<summary>
		///The content object
		///</summary>
		public Object Object
		{
			get
			{
				return m_object;	
			}
		}
	}
}