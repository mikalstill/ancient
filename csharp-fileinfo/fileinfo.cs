using System;
using System.IO;

// This example displays simple information about a file, much like ls

namespace finfo
  {
  class fileinfo
    {
    static void Main(string[] args)
      {
      if(args.Length == 0)
	{
	Console.WriteLine("You need to specify a path to a file to list");
	return;
	}

      displayinfo(args[0], 0);
      }

    private static void displayinfo(string path, int lvl)
      {
      FileInfo fi;

      fi = new FileInfo(path);
      Console.WriteLine("Filename:  {0}", fi.Name);
      Console.WriteLine("Extension: {0}", fi.Extension);
      Console.WriteLine("Path to:   {0}", fi.DirectoryName);
      Console.WriteLine("Size:      {0}", fi.Length);
      Console.WriteLine("Hash:      {0}", fi.GetHashCode());
      }
    }
  }
