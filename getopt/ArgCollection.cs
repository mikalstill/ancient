using System;
using System.Collections;

namespace mGnu
{
	/// <summary>
	/// A collection of Args for getopt
	/// </summary>
	public class ArgCollection : CollectionBase
	{
		/// <summary>
		/// </summary>
		public void Add(Arg r)
		{
			List.Add(r);
		}

		/// <summary>
		/// </summary>
		public Arg this [int i] 
		{
			get                               
			{
				return (Arg) List[i];         
			}     
			set
			{
				if ( i >= Count )
				{
					Add(value);
				}
				else
				{
					List[i] = value;
				}
			}
		}
	}
}
