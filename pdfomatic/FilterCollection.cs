// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System.Collections;
using System.Text;

namespace OpenPdf
{
	public class FilterCollection : CollectionBase
	{
		public void Add(string filt)
		{
			List.Add(filt);
		}
		
		public override string ToString()
		{
			StringBuilder retval = new StringBuilder();
			bool first = true;
			
			foreach(string s in List)
			{
				if(!first) retval.Append(" ");
				retval.Append(s);
				first = false;
			}
			
			return retval.ToString();
		}		
	}
}