// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;

namespace OpenPdf
{
	public class Content
	{
		private ObjectCollection m_objects;
	
		public Content(Pdf Document, Object PageObject)
		{
			if(PageObject.Number == -1)
			{
				throw new RuntimeException("Page object is invalid");
			}
		
			DictionaryItem di = PageObject.Dictionary.Get("Contents");
			if(!di.Valid)
			{
				throw new RuntimeException("Page object does not refer to a content object");
			}
			
			m_objects = di.ValueAsObjects(Document.Objects);
			if(m_objects.Count == 0)
			{
				throw new RuntimeException("Page object " + PageObject.Number + " " + PageObject.Generation + 
					" does not refer to a content object which is valid");
			}
		}
		
		public FilterCollection Filters
		{
			get
			{
				FilterCollection fc = new FilterCollection();
				
				foreach(Object obj in m_objects)
				{
					DictionaryItem di = obj.Dictionary.Get("Filter");
					if(di.Valid)
					{
						if(di.Type == DictionaryItem.ValueType.String)
						{
							
							fc.Add(di.ValueAsString());
						}
						else if(di.Type == DictionaryItem.ValueType.Dictionary)
						{
							Dictionary subdict = di.ValueAsDictionary();
							
							foreach(DictionaryItem subdi in subdict)
							{
								fc.Add(subdi.ValueAsString());
							}
						}
					}
				}
				
				return fc;
			}
		}
		
		public ObjectCollection Objects
		{
			get
			{
				return m_objects;	
			}
		}
	}
}