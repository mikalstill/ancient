// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System.Collections;
using System.Text;

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
		
		public override string ToString()
		{
			StringBuilder retval = new StringBuilder();
			
			foreach(ObjectReference oref in List)
			{
				retval.Append(oref.Number + " " + oref.Generation + " R ");
			}
			
			return retval.ToString();
		}
	}
}