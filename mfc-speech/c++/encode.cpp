// Encode a MFC file using the codebook
// Arg1: name of codebook
// Arg2: name of file to encode

#include "world.h"

int
main (int argc, char *argv[])
{
  mfc codes, target;

  load (codes, argv[1]);
  load (target, argv[2]);
  codebook cb (codes);
  vector < int >encoded = cb.encode (target);

  fstream file;
  file.open (argv[3], ios::out);
  if (!file)
    {
      cerr << "Could not open output file:" << argv[3];
      exit (42);
    }

  for (size_t i = 0; i < encoded.size (); i++)
    {
      file << encoded[i] << " ";
    }
  file << endl;
}

void
load (mfc & data, string filename)
{
  string result;

  result = data.add (filename);
  if (result != "")
    {
      cout << "File load error: " << filename << ": " << result << endl;
      exit (-1);
    }
}
