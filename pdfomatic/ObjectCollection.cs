// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System.Collections;
using System.Text;

namespace OpenPdf
{
	public class ObjectCollection : CollectionBase
	{
		public void Add(Object obj)
		{
			List.Add(obj);
		}
		
		public void Add(ObjectCollection oc)
		{
			foreach(Object obj in oc)
			{
				List.Add(obj);
			}
		}
		
		public Object Get(ObjectReference or)
		{
			return Get(or.Number, or.Generation);
		}
		
		public Object Get(int number, int generation)
		{
			Utility.TraceLine("Looking up object " + number + " " + generation);
			foreach(Object o in List)
			{
				if((o.Number == number) && (o.Generation == generation))
				{
					Utility.TraceLine("Object found");
					return o;
				}
			}
			
			Utility.TraceLine("Object not found");
			return new Object();
		}
		
		public ObjectCollection ByType(string type)
		{
			ObjectCollection oc = new ObjectCollection();
			foreach(Object o in List)
			{
				if((o.Valid) && (o.Type == type))
					oc.Add(o);
			}
			
			return oc;
		}
		
		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			
			foreach(Object obj in List)
			{
				sb.Append(obj.ToString());
				sb.Append(" ");
			}
			
			sb.Length--;
			return sb.ToString();
		}
	}
}