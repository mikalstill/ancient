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

      m_files = 0;
      m_directories = 0;
      Console.WriteLine("Total size: {0} bytes", traverse(args[0]));
      Console.WriteLine("File count: {0}", m_files);
      Console.WriteLine("Directory count: {0}", m_directories);
      }

    private static long traverse(string path)
      {
      string[] dirs = Directory.GetDirectories(path);
      string[] files = Directory.GetFiles(path);
      long dirtotal = 0;

      m_directories++;
      for(int i = 0; i < dirs.Length; i++)
	{
	long subdir = traverse(dirs[i]);
	dirtotal += subdir;
	}

      for(int i = 0; i < files.Length; i++)
	{
	FileInfo fi = new FileInfo(files[i]);
	m_files++;
	dirtotal += fi.Length;
	}

      return dirtotal;
      }

    private static long m_directories;
    private static long m_files;
    }
  }
