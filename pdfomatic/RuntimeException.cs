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