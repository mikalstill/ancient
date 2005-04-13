// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;

namespace OpenPdf
{
	// Kinda like a StringBuilder, but for binary bytes
	class ByteBuilder
	{
		byte[] m_data = new byte[8];
		int m_grow = 8;
		int m_inset = 0;
		
		public void Append(byte[] data)
		{
			if(m_inset + data.Length > m_data.Length)
			{
				byte[] newdata = new byte[m_data.Length + data.Length + m_grow];
				m_grow *= 2;
				if(m_grow > 1024)
					m_grow = 1024;
					
				for(int i = 0; i < m_inset; i++)
				{
					newdata[i] = m_data[i];
				}
				
				m_data = newdata;
			}
			
			for(int i = 0; i < data.Length; i++)
			{
				m_data[m_inset++] = data[i];
			}
		}
		
		public void Append(byte data)
		{
			if(m_inset + 1 > m_data.Length)
			{
				byte[] newdata = new byte[m_data.Length + m_grow];
				m_grow *= 2;
				if(m_grow > 1024)
					m_grow = 1024;
					
				for(int i = 0; i < m_inset; i++)
				{
					newdata[i] = m_data[i];
				}
				
				m_data = newdata;
			}
			
			m_data[m_inset++] = data;
		}
		
		public byte[] ToBytes()
		{
			byte[] retdata = new byte[m_inset];
			
			for(int i = 0; i < m_inset; i++)
			{
				retdata[i] = m_data[i];
			}
			
			return retdata;
		}
	}
}