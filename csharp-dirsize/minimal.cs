using System;
using System.IO;

// This example traverses a path, and states the size of the contents
// in bytes
namespace traverse
  {
  class dirtrav
    {
    static void Main(string[] args)
      {
      if(args.Length == 0)
	{
	Console.WriteLine("You need to specify a path to a directory to list");
	return;
	}

      string[] dirs = Directory.GetDirectories(args[0]);
      Console.WriteLine("{0}", dirs.Length);
      }
    }
  }
