#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileutil.h"

void travdir(int, char *, long long, int, int);

int main(int argc, char *argv[]){
  int fd, count, bytespersec, secperclu, resseccnt, numfats, rootentcnt, totsec16, 
    fatsz16, rootdirsec, fatsz, totsec, totsec32, datasec, fattype, hidseccnt,
    fatcount, chainsize;
  long clucnt, lowclucnt, thisclu;
  char *file;
  long long filep;
  struct stat sb;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  // Map the input file into memory
  if ((fd = open (argv[1], O_RDONLY)) < 0)
    {
      fprintf (stderr, "Could not open the data file\n");
      exit (43);
    }
  
  if(fstat(fd, &sb) < 0){
    fprintf(stderr, "File stat error\n");
    exit(43);
  }

  if ((file =
       (char *) mmap (NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0)) == -1)
    {
      fprintf (stderr, "Could not mmap data file\n");
      exit (43);
    }
  filep = 0;
  
  /******************************
  Boot sector
  ******************************/
  printf("FAT boot sector\n");
  printf("Jump instruction: ");
  for(count = 0; count < 3; count++)
    printf("%02x ", (unsigned char) file[filep++]);
  printf("\n");

  printf("Formatting system (normally MSWIN4.1): \"");
  for(count = 0; count < 8; count++)
    printf("%c", file[filep++]);
  printf("\"\n");
  
  bytespersec = fileutil_displayshort(file, "Bytes per sector: ", &filep); printf("\n");
  secperclu = fileutil_displaybyte(file, "Sectors per allocation unit: ", &filep); printf("\n");
  resseccnt = fileutil_displayshort(file, "Number of sectors in the reserved region: ", &filep); printf("\n");
  numfats = fileutil_displaybyte(file, "Number of FATs: ", &filep); printf("\n");
  rootentcnt = fileutil_displayshort(file, "Number of root directory entries (FAT12 or FAT16 only): ", &filep); printf("\n");
  totsec16 = fileutil_displayshort(file, "16 bit total count of sectors on volume: ", &filep); printf("\n");
  fileutil_displaybyte(file, "Media type: ", &filep); printf("\n");
  fatsz16 = fileutil_displayshort(file, "16 bit count of the size of a single FAT: ", &filep); printf("\n");
  fileutil_displayshort(file, "Sectors per track (INT 13): ", &filep); printf("\n");
  fileutil_displayshort(file, "Number of heads (INT 13): ", &filep); printf("\n");
  hidseccnt = fileutil_displayinteger(file, "Hidden sectors proceeding this partition (INT 13): ", &filep); printf("\n");
  totsec32 = fileutil_displayinteger(file, "32 bit total count of sectors on volume: ", &filep); printf("\n");

  // Determine what type of fat this is...
  rootdirsec = ((rootentcnt * 32) + (bytespersec - 1)) / bytespersec;

  if(fatsz16 != 0)
    fatsz = 16;
  else
    fatsz = 32;

  if(totsec16 != 0)
    totsec = totsec16;
  else
    totsec = totsec32;
  
  datasec = totsec - (resseccnt + (numfats * fatsz) + rootdirsec);
  clucnt = datasec / secperclu;
  printf("Data sectors: %d\n", datasec);
  printf("Total cluster count: %d\n", clucnt);
  if(clucnt < 4085){
    printf("Fat type: 12\n");
    fattype = 12;
  }
  else if(clucnt < 65525){
    printf("Fat type: 16\n");
    fattype = 16;
  }
  else{
    printf("Fat type: 32\n");
    fattype = 32;
  }
  
  switch(fattype){
  case 32:
    fileutil_displayinteger(file, "32 bit count of the size of a single FAT: ", &filep); printf("\n");
    fileutil_displayshort(file, "Extended flags (TODO): ", &filep); printf("\n");
    fileutil_displayshort(file, "Filesystem version (TODO): ", &filep); printf("\n");
    fileutil_displayinteger(file, "Root cluster: ", &filep); printf("\n");
    fileutil_displayshort(file, "Filesystem information: ", &filep); printf("\n");
    fileutil_displayshort(file, "Backup boot sector: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Reserved: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Reserved: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Reserved: ", &filep); printf("\n");
    // Deliberate fall through

  case 12:
  case 16:
    fileutil_displaybyte(file, "Drive number: ", &filep); printf("\n");
    fileutil_displaybyte(file, "Reserved: ", &filep); printf("\n");
    fileutil_displaybyte(file, "Boot signature: ", &filep); printf("\n");
    fileutil_displayinteger(file, "Volume ID: ", &filep); printf("\n");

    printf("Volume label: \"");
    for(count = 0; count < 11; count++)
      printf("%c", file[filep++]);
    printf("\"\n");

    printf("File system type (descriptive): \"");
    for(count = 0; count < 8; count++)
      printf("%c", file[filep++]);
    printf("\"\n");
    break;

  default:
    printf("Unknown FAT type\n");
    exit(1);
  }

  filep = 510;
  printf("Boot sector end signature: ");
  for(count = 0; count < 2; count++)
    printf("%02x ", (unsigned char) file[filep++]);
  printf("\n\n");

  // Skip the FATs for now...
  filep = (resseccnt + numfats * fatsz16) * bytespersec;
  
  printf("Root directory starts at: %d\n", filep);
  travdir(0, file, filep, bytespersec, resseccnt);
  
  // It's polite to cleanup
  munmap(file, sb.st_size);
  close(fd);
  return 0;
}

void travdir(int indent, char *file, long long filep, int bytespersec, int resseccnt){
  int count, icount, isdir, newfilepl, newfileph;

  for(icount = 0; icount < indent; icount++) printf("  ");
  printf("[ Directory entries from %d ]\n\n", filep);

  while(1){
    if(file[filep] == 0)
      break;

    for(icount = 0; icount < indent; icount++) printf("  ");
    printf("Name: \"");
    for(count = 0; count < 11; count++)
      printf("%c", file[filep++]);
    printf("\"\n");
    
    isdir = 0;
    {
      char attrs;
      
      for(icount = 0; icount < indent; icount++) printf("  ");
      attrs = fileutil_displaybyte(file, "Attributes: ", &filep); printf("\n");

      for(icount = 0; icount < indent; icount++) printf("  ");
      printf("Decoded: ");
      if(attrs & 0x01) printf("ro ");
      if(attrs & 0x02) printf("hidden ");
      if(attrs & 0x04) printf("system ");
      if(attrs & 0x08) printf("volumeid ");
      if(attrs & 0x10){
	printf("dir ");
	isdir = 1;
      }
      if(attrs & 0x20) printf("archive ");
      if((attrs & 0x01) && (attrs & 0x02) && (attrs & 0x04) && (attrs & 0x08))
	printf("longfilename");
    }
    printf("\n");
    
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displaybyte(file, "NT resource: ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displaybyte(file, "File creation time (tenths of second portion): ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displayshort(file, "File creation time (seconds): ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displayshort(file, "File creation time (date): ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displayshort(file, "Last access date: ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    newfileph = fileutil_displayshort(file, "Starting cluster high word: ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displayshort(file, "Time of last access: ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displayshort(file, "Date of last access: ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    newfilepl = fileutil_displayshort(file, "Starting cluster low word: ", &filep); printf("\n");
    for(icount = 0; icount < indent; icount++) printf("  ");
    fileutil_displayinteger(file, "File size (bytes): ", &filep); printf("\n");
    printf("\n");

    if(isdir){
      travdir(indent + 1, file, 
	      ((newfileph << 16) + newfilepl + resseccnt) * bytespersec, 
	      bytespersec, resseccnt);
    }
  }
}
