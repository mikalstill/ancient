// This header contains everything we need to make something in memory which looks a lot like a tiff.
// It is only used by fax.cpp, but is placed here for clarity

// todo_mikal: I assume a short is 2 bytes and a long 4

typedef union
{
  long l;
  short s;
} fakeTiffValueOrOffset;

typedef struct
{
  short tag;
  short type;
  long count;
  fakeTiffValueOrOffset valueOrOffset;
}
fakeTiffIFD;

// This is the actual TIFF
typedef struct
{
  // Header (8 bytes)
  char endian[2];
  short version;
  long ifdOffset;
  short pad;

  // IFD -- this array must be big enough to fit all the entries...
  short ifdEntries;
  fakeTiffIFD entries[11]; // (12 bytes each)
}
fakeTiff;

typedef struct
{
  long n;
  long d;
}
fakeTiffRemoteRational;
