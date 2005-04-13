// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

namespace OpenPdf
{
	public class ObjectReference
	{
		private int m_number;
		public int Number
		{
			get
			{
				return m_number;
			}
			set
			{
				m_number = value;
			}
		}
		
		private int m_generation;
		public int Generation
		{
			get
			{
				return m_generation;
			}
			set
			{
				m_generation = value;
			}
		}
	}
}