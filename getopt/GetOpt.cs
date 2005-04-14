using System;

namespace mGnu
{
	/// <summary>
	/// An implementation of the getopt standard, as used by Gnu GetOpt
	/// </summary>
	public class GetOpt
	{
		StringCollection m_params = new StringCollection();
		StringCollection m_extras = new StringCollection();
		ArgCollection m_args = new ArgCollection();

		public GetOpt(string[] CommandLineArgs, string ParametersDescription)
		{
			// Import the string array into the collection
			foreach(string s in CommandLineArgs)
			{
				m_params.Add(s);
			}

			// Parse the params description
			for(int i = 0; i < ParametersDescription.Length; i++)
			{
				Arg a = new Arg();
				a.Flag = "-" + ParametersDescription[i];
				if(ParametersDescription[i + 1] == ':')
				{
					a.TakesParameter = true;
					i++;
				}

				m_args.Add(a);
			}
		}

		public Arg NextArg()
		{
			if(m_args.Count == 0)
				return null;

			SnarfExtras();

			foreach(Arg a in m_args)
			{
				if(a.Flag == m_params[0])
				{
					Arg matched = a;
					try
					{
						if(a.TakesParameter)
						{
							matched.Parameter = m_params[1];
							m_params.RemoveAt(1);
						}
					}
					catch(Exception)
					{
					}

					m_params.RemoveAt(0);
					return matched;
				}
			}

			return null;
		}

		public string[] Extras()
		{
			SnarfExtras();

			string[] extras = new string[m_extras.Count];
			int i = 0;
			foreach(string ext in m_extras)
			{
				extras[i] = ext;
			}
			return extras;
		}

		private void SnarfExtras()
		{
			// Parameters must start with a hyphen
			while(m_params[0][0] != '-')
			{
				m_extras.Add(m_params[0]);
				m_params.RemoveAt(0);
			}
		}
	}
}
