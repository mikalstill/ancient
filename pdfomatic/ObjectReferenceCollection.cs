// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System.Collections;

namespace OpenPdf
{
	public class ObjectReferenceCollection : CollectionBase
	{
		public void Add(ObjectReference obj)
		{
			List.Add(obj);
		}
		
		public ObjectReference this[int i]
		{
			get
			{
				return (ObjectReference) List[i];
			}
		}
	}
}