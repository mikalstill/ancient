using System;
using System.IO;
using System.Collections;

// This example traverses a directory, and lists the contents recursively

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

      traverse(args[0], 0);
      }

    private static void traverse(string path, int lvl)
      {
      string[] dirs = Directory.GetDirectories(path);
      string[] files = Directory.GetFiles(path);
      SortedList all = new SortedList();
      String indent = new String(' ', lvl * 2);

      // Add all the directories to the list
      for(int i = 0; i < dirs.Length; i++)
	{
	all[dirs[i]] = "d";
	}

      // Add all the files to the list
      for(int i = 0; i < files.Length; i++)
	{
	all[files[i]] = "f";
	}

      // For each item in the directory, display it and possibly recurse
      foreach(string key in all.Keys)
	{
	Console.WriteLine(indent + key + " (" + all[key] + ")");
	if(all[key] == "d")
	  {
	  traverse(key, lvl + 1);
	  }
	}
      }
    }
  }
