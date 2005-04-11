using System;

namespace OpenPdf
{
	public class ParseException : Exception
	{
		public ParseException(string message) : base(message)
		{
		}
	}
}