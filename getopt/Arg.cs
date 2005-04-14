using System;

namespace mGnu
{
	/// <summary>
	/// An argument for GetOpt
	/// </summary>
	public class Arg
	{
		public string Flag;
		public bool TakesParameter = false;

		public string Parameter = "";
	}
}
