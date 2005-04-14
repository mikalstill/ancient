using System;
using System.Collections;

namespace mGnu
{
	/// <summary>
	/// A collection of strings
	/// </summary>
	public class StringCollection : CollectionBase
	{
		/// <summary>
		/// </summary>
		public void Add(string r)
		{
			List.Add(r);
		}

		/// <summary>
		/// </summary>
		public string this [int i] 
		{
			get                               
			{
				return (string) List[i];         
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
