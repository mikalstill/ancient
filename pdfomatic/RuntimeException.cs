// Copyright (C) Michael Still (mikal@stillhq.com) 2005
// Released under the terms of the GNU LGPL. See COPYING for more details...

using System;

namespace OpenPdf
{
	public class RuntimeException : Exception
	{
		public RuntimeException(string message) : base(message)
		{
		}
	}
}