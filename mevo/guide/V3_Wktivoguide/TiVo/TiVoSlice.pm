#
# Package of support modules for generating TiVo slice files.
#
=pod - Package of support modules for generating TiVo slice files


Description
-----------

This package contains a collection of modules that can be used by Perl scripts
to generate TiVo slice files.  Essentially, to generate a slice file, you
open it with SliceOpen, write all of the records with SliceWriteRecord and
then close it with SliceClose.

To construct TiVo slice records, you simply create an abstract object (a hash)
with named entries for each of the fields in the record.  Upon passing the
object to SliceWriteRecord, it will convert all of the fields within to the
proper TiVo slice format and emit the record.

You are responsible for assigning indexes to each of the records, although
SliceWriteRecord may monkey with them to ensure uniqueness (see "Server ID
Generation" below).

The rest of this section describes general principals applied to the creation
of slice files.  The individual routines are described with their actual code.


Output
------

This package will generate a TiVo slice file, as output, that contains all of
the records passed to it.  Program guide data consists of many records, as
follows:

     One Series and one or more Program records for each program.  If episodes
     are involved, there will likely be multiple Program records, one per
     episode, otherwise, Series and Program records will be paired.

     One Station record for each station for which data is supplied.

     One Station Day record for each station for each day for which program
     guide data is generated.

The various records in the slice file are connected together thusly (a little
ASCII line art):

           0x0106 (Station Day)                        0x0105 (Station)
          +--------------------+                    +--------------------+
          |                    |   +--------------->|                    |
          |                    |   |  +------------>|                    |
          +--------------------+   |  |             |                    |
          |Ix 1 -> Station Rec |---+  |             |                    |
          +--------------------+      |             |                    |
          |                    |      |             +--------------------+
          |    Showing List    |      |
          +--------------------+      |
     +----|Showing 0x070A      |      |
     |    +--------------------+      |
     | +--|Showing 0x070B      |      |                0x0103 (Program)
     | |  +--------------------+      |             +--------------------+
     | |  |         .          |   +--|------------>|                    |
     | |  |         .          |   |  |      +----->|                    |
     | |  |         .          |   |  |      |      |                    |
     | |  |   Showing Detail   |   |  |      |      |                    |
     | |  +--------------------+   |  |      |      +--------------------+
     | +->|0x070B (reversed)   |   |  |      |      |Ix -> Series Rec    |--+
     |    +--------------------+   |  |      |      +--------------------+  |
     |    |Ix 1 -> Program Rec |---+  |      |      |                    |  |
     |    +--------------------+      |      |      |                    |  |
     |    |Ix 2 -> Station Rec |------+      |      |                    |  |
     |    +--------------------+             |      |                    |  |
     |    |         .          |        Links from  +--------------------+  |
     |    |         .          |           other                            |
     |    |         .          |        0x0106 recs                         |
     |    |   Showing Detail   |         possible                           |
     |    +--------------------+                                            |
     +--->|0x070A              |              +-----------------------------+
          +--------------------+              |
          |Ix 1 -> Program Rec |              |
          +--------------------+              |        0x0104  (Series)
          |Ix 2 -> Station Rec |              |     +--------------------+
          +--------------------+              +---->|                    |
          |         .          |             Single |                    |
          |         .          |             link   |                    |
          |         .          |             only   |                    |
          +--------------------+                    +--------------------+


Numbers Format
--------------

All TiVo numerics have a special expanding format that is designed to save
space.  They work by reserving either two or four bits in the first byte of
any number for the number length code.  The first two or four bits encode the
length as follows:

     00   - A single byte.
     10   - Two bytes.
     11   - Three or more bytes (determined by the next two bits).
     1100 - Three bytes.
     1110 - Four bytes.
     1111 - Five bytes.

Additionally (according to TiVo's rules), the largest number a single byte
can encode is 0x3F.  Two bytes can encode 0x0FFF as their largest number
(which would appear as 0x8FFF).  Three bytes can encode 0x0FFFFF (which
would appear as 0xCFFFFF), while four and five bytes can encode 0x0FFFFFFF
(appearing as 0xEFFFFFFF) and 0xFFFFFFFF (appearing as 0xF0FFFFFFFF),
respectively.

The routines herein will take care of converting any numerics, where
appropriate, to TiVo numeric format before commiting them to the slice file.


Date Format
-----------

All TiVo dates are epoch based.  That is, they are the number of days from the
epoch date, which is 1970 Jan 1.  Externally to this module, times may be
calculated using the standard epoch seconds method (equivalent to Unix
time_t).

The routines herein will take care of dividing times by 86400 to get epoch
days before commiting any dates to the slice file.  Once the epoch days offset
is calculated, the number is represented as a TiVo numeric in the usual way.


Overall Format
--------------

 +0    1   First byte of file (don't know its significance but I've seen
           claims it has something to do with spanning or partial files.
           Everybody I know uses 0x03).
 +1    4   Length of record (excluding the length iself).  \
 +5    2   Record type.                                     +-- Repeats
 +7    x   Record data (variable length).                  /
 +y        More records follow until the end.

 Notes: 1) All data is in network byte order (i.e. hi-byte first, etc.).
        2) There are many record types (enumerated in types.list with the
           position in the list +0x0100 giving the record number).  Only
           those that we care about are shown below.


Program Type Record - 0x0103
----------------------------

 +0    4   Length of record (a regular 4-byte number, excludes itself).
 +1    2   0x0103 - program record.
 +6    a   Numeric server ID (this is really an index).
 +6+a  b   Numeric server version (increments when the data is updated).
           Tridge uses the same version for every field in every record.
 ...   1   Field type (see below).
       c   Variable information follows, depending on field.
           More fields follow until the end (0x00).

     Program Record Fields
     ---------------------

      ID   Type  Len  Description
  
     0x00   Fix   0   End of list marker.
     0x41   Str   x   TmsID (Times/Mirror ID).
     0x45   Str   x   Movie/show title.  In the case of episodic shows,
                      this will be the show's name (e.g. "Dark Angel").
     0x4A   Idx   z   Index tuple which points to the associated title type
                      record.  The tuple consists of an object type,
                      attribute and link (server ID).
     0x4D   Str   x   Description (seems like the maximum length is 0xC8 or,
                      at least, Tridge seems to think so, but then maybe not.
                      I've seen 0xF8 on occasion).
     0x51   Str   x   Language string (e.g. "English").
     0x54   Num   y   Show type (from ShowType.enum):
                      1 - Serial.                   5 - Series.
                      2 - Short film.               6 - Mini series.
                      3 - Special.                  7 - Paid programming.
                      4 - Limited series.
     0x58   Num   y   Release date (the year number).
     0x5C   Num   y   MPAA rating (from Mpaa.enum):
                      1 - Network.                  3 - Local.
                      1 - G.                        5 - X.
                      2 - PG.                       6 - NC-17 or NC17.
                      3 - PG-13 or PG13.            7 - AO.
                      4 - R.                        8 - NR.
     0x60   Num   y   Star rating (from Star.enum):
                      1 - One or *.
                      2 - One point five or *+.
                      3 - Two or **.
                      4 - Two point five or **+.
                      5 - Three or ***.
                      6 - Three point five or ***+.
                      7 - Four or ****.
     0x64   Num   y   Movie run time (in minutes).
     0x69   Str   x   Country (e.g. "USA").
     0x6D   Str   x   Network source.
     0x70   Num   y   Source type (from SourceType.enum):
                      1 - Network.                  3 - Local.
                      2 - Syndicated.               4 - Block.
     0x75   Str   y   Episode title for shows that are episodic (e.g. "Max
                      Gets a Gene Transplant").
     0x78   Num   y   Advisory (multiple fields allowed, from Advisory.enum):
                      1 - Language.                 6 - Violence.
                      2 - Graphic Language.         7 - Mild Violence.
                      3 - Nudity.                   8 - Strong Sexual Content.
                      4 - Brief Nudity.             9 - Rape.
                      5 - Graphic Violence.         10 - Adult Situations.
     0x7D   Str   x   Actor's name (last name first, separated from first
                      name, middle initial by "|".  E.g. "Blow|Joe M.".
                      Multiple fields are permitted).
     0x81   Str   x   Guest star's name (same).
     0x85   Str   x   Director's name (same).
     0x89   Str   x   Executive producer's name (same).
     0x8D   Str   x   Producer's name (same).
     0x91   Str   x   Writer's name (same).
     0x95   Str   x   Show host's name (same).
     0x9C   Num   y   Genre (multiple fields allowed, from Genre.enum).
                      There's way too many to enumerate here.
     0xA0   Num   y   Color code (from ColorCode.enum):
                      1 - B & W or B&W or BW.       3 - Colorized.
                      2 - Color and B & W.          4 - Color.
     0xA4   Num   y   Episode number.
     0xA8   Num   y   Original episode number.
     0xB4   Num   y   Is episode (0x00 = no, 0x01 = yes).  Should be set to
                      yes for shows that repeat and may or may not use the
                      same description over and over.  We'll have to see.
     0xC4   Num   y   Original air date (days since the epoch).

     Notes: 1) Fields with length "x" have a length following the
               field type.  These lengths are numerics (see the numbers
               story, above), just like everywhere else.
            2) Numbers, where used (shown with a length of "y"), are
               expandable, according to the numbers story (see above).
            3) Index tuples, where used (shown with a length of "z"), are
               composed of three expandable numbers, one after the other.
               Each of these numbers expands according to the numbers story
               (see above).


Series Type Record - 0x0104
---------------------------

 +0    4   Length of record (excluding the length iself).
 +1    2   0x0104 - series record.
 +6    a   Numeric server ID (this is really an index).
 +6+a  b   Numeric server version (increments when the data is updated).
           Tridge uses the same version for every field in every record.
 ...   1   Field type (see below).
       c   Variable information follows, depending on field.
           More fields follow until the end (0x00).

     Series Record Fields
     --------------------

      ID   Type  Len  Description
  
     0x00   Fix   0   End of list marker.
     0x41   Str   x   TmsID (Times/Mirror ID).
     0x45   Str   x   Movie/show title (duplicates 0x45 in the Program record).
     0x50   Num   y   Genre (multiple fields allowed, from Genre.enum).
                      There's way too many to enumerate here.  And, besides,
                      it duplicates 0x9C in the Program record.
     0x58   Num   y   Flag indicating whether show is episodic (i.e. has a
                      regular title and possibly an episode title -- does not
                      duplicate 0xB4 in the Program record).

     Notes: 1) Fields with length "x" have a length following the
               field type.  These lengths are numerics (see the numbers
               story, above), just like everywhere else.
            2) Numbers, where used (shown with a length of "y"), are
               expandable, according to the numbers story (see above).


Station Type Record - 0x0105
----------------------------

 +0    4   Length of record (excluding the length iself).
 +1    2   0x0105 - station record.
 +6    a   Numeric server ID (this is really an index).
 +6+a  b   Numeric server version (increments when the data is updated).
           Tridge uses the same version for every field in every record.
 ...   1   Field type (see below).
       c   Variable information follows, depending on field.
           More fields follow until the end (0x00).

     Station Record Fields
     ---------------------

      ID   Type  Len  Description
  
     0x00   Fix   0   End of list marker.
     0x41   Str   x   TmsID (Times/Mirror ID).
     0x45   Str   x   Full name of station.
     0x49   Str   x   Call sign of station.
     0x4D   Str   x   City where station is located.
     0x51   Str   x   State where station is located.
     0x55   Str   x   Zipcode of station.
     0x59   Str   x   Country where station is located.
     0x5D   Str   x   Affilitation (e.g. satellite).
     0x61   Str   x   DMA name (undoubtedly having to do with the Direct
                      Marketing Association and how we can sell you more
                      shit rather than Direct Memory Access).
     0x64   Num   y   DMA number (whatever that is).
     0x68   Num   y   FCC broadcast channel number.
     0x6C   Num   y   Logo index (0x10000 == TiVo space).
     0x70   Num   y   Affiliation index.
     0x74   Num   y   Pay per view flag.

     Notes: 1) Fields with length "x" have a length following the
               field type.  These lengths are numerics (see the numbers
               story, above), just like everywhere else.
            2) Numbers, where used (shown with a length of "y"), are
               expandable, according to the numbers story (see above).


Station Day Type Record - 0x0106
--------------------------------

 +0    4   Length of record (excluding the length iself).
 +1    2   0x0106 - station day record.
 +6    a   Numeric server ID (this is really an index).
 +6+a  b   Numeric server version (increments when the data is updated).
           Tridge uses the same version for every field in every record.
 ...   1   Field type (see below).
       c   Variable information follows, depending on field.
           More fields follow until the end (0x00).

     Station Day Record Fields
     -------------------------

      ID   Type  Len  Description
  
     0x00   Fix   0   End of list marker.
     0x42   Idx   z   Index tuple (Ix 1) which points to the associated
                      station and program records.  The tuple consists of an
                      object type (which indicates which type of record it
                      points to), attribute and link (server ID).
     0x44   Num   y   Day number at start of Station Day record (number of
                      days from the epoch).
     0x46   Idx   z   Another index tuple (Ix 2) which points to the
                      associated station record within each showing detail
                      slot.  The tuple consists of an object type (which
                      indicates which type of record it points to), attribute
                      and link (server ID).
     0x48   Num   y   Date in showing detail (number of days from the epoch).
     0x4A   Slt   w   Slot tag for program showing in this time slot.
     0x4C   Num   y   Start time of slot (in seconds from midnight).
     0x50   Num   y   Duration of slot (in seconds).
     0x54   Num   y   Part index (who knows).
     0x58   Num   y   Part count (likewise).
     0x5C   Num   y   Premiere number (from Premiere.enum):
                      1 - Season Finale.            4 - Season Premiere.
                      2 - Series Finale.            5 - Series Premiere.
                      3 - Premiere.
     0x60   Num   y   Live program number (from Live.enum):
                      1 - Delay.                    3 - Live.
                      2 - Tape.
     0x64   Num   y   Bits (don't know what this is).
     0x6C   Num   y   Don't index flag.
     0x70   Num   y   TV rating (from TvRating.enum):
                      1 - Y7 or TVY7.               4 - PG or TVPG.
                      2 - Y or TVY.                 5 - 14 or TV14.
                      3 - G or TVG.                 6 - M or TVM.
     0x7C   Num   y   Dolby program material flag.

     Notes: 1) Numbers, where used (shown with a length of "y"), are
               expandable, according to the numbers story (see above).
            2) Index tuples, where used (shown with a length of "z"), are
               composed of three expandable numbers, one after the other.
               Each of these numbers expands according to the numbers story
               (see above).
            3) Slot numbers, where used (shown with a length of "w"), are
               composed of two expandable numbers, one after the other.
               Each of these numbers expands according to the numbers story
               (see above).

After the list of slot tags (0x4A) for all of the programs in the station's
day, an end field occurs (0x00).  Following this field is a two-number slot
ID which is then followed by the slot detail data for that slot.  The detail
data uses field tags just like those shown above.  At the end of the slot
detail is an end field (0x00).  The two-number slot ID and slot detail data
repeats once for each slot listed by the slot tags (0x4A).  This whole
series of slots and slot details looks like this:

     0x4A,m,0x0A       Slot tag 1 (seems to start at 10 or 12).
     0x4A,m,0x0B       Slot tag 2.
     0x4A,m,0x0C       Slot tag 3.
               .
               .
               .
     0x4A,m,n          Slot tag n.
     0x00              End of slot list.
     m,n,slot-info     Slot n showing detail.
     0x00              End of slot n detail.
     m,0x0C,slot-info  Slot 3 showing detail.
     0x00              End of slot 3 detail.
     m,0x0B,slot-info  Slot 2 showing detail.
     0x00              End of slot 2 detail.
               .
               .
               .
     m,0x0A,slot-info  Slot 1 showing detail.
     0x00              End of slot 1 detail.  The end of it all when the
                       first slot is passed.


Server ID Generation
--------------------

There are four sets of server IDs (a.k.a. keys) that must be generated to
exactly simulate slice files.  These are for the 0x0103, 0x0104, 0x0105 and
0x0106 records.  Here is the numbering scheme used by these modules:

     0x0103  1xxxxxxx      Use any number in the range 1-9999999 for xxxxxxx.

     0x0104  2yyyyyyy      Use any number in the range 1-9999999 for yyyyyyy.
                           You can even use the same number as you used for
                           the corresponding 0x0103 record (that's the plan,
                           Stan).

     0x0105  nnn           Usually, these appear to be low numbers but some of
                           the more esoteric stations (like EDAC029) have
                           numbers like 1234567.  If you want to make your
                           slice files work with the existing channel lineup,
                           you'll need to reuse the same server IDs that are
                           already loaded into the database for the channel
                           lineup (see chanlineup.tcl for this).  Otherwise,
                           use the station's FCC or DirectTV number or any
                           other number that doesn't collide.

     0x0106  4zzzzzzz      For convenience sake, a 7-digit number is supported.
                           This allows you to use the station number plus the
                           5-digit day number as the ID.

Note that the numbering scheme above is forced by these modules.  When you
pass in index numbers to these routines, they are adjusted according to the
rules above (in GenServerID) before they are written out as server IDs.  Thus,
all index numbers for 0x0103 records have 10,000,000 added to them.  The index
numbers for 0x0104 records have 20,000,000 added to them and the 0x0104
records have 40,000,000 added to them.  Index numbers for 0x0105 records are
left as is.

The reason for forcing this numbering scheme is to avoid collisions with
existing TiVo data.  If you wish to dump all of the existing indexes from
the target TiVo and maintain a database of those used, you could generate
IDs that are lower.  However, there must be no collisions and this seems as
good a scheme as any.  Tridge uses 100,000,000, 200,000,000 and 400,000,000
respectively but this seemed excessive.

The version numbers seem to increment every time data is changed.  Tridge
just appears to increment one version number and then use it for every record
in the slice file.  Is there any percentage in keeping individual versions
for each piece of data or is this good enough?  You must decide.


Genre Numbers
-------------

Genre numbers are employed by the TiVo to group programs into groups of all
shows with similar content and to make recommendations for shows to record,
based on historical preferences exhibited by the user.

The lower numbered genres (1-999) are used to make recommendations for shows
to record.  If a trend is detected in the shows watched by a user which
indicates their interest in a particular genre, other shows of the same genre
will be automatically recorded.  This method is applied elsewhere too (e.g.
the same sort of recommended show recording is done for actors).

The genres numbered above 1000 are used to group programs into broad groups
of shows with similar content.  These 16 genres appear on the TiVo selection
lists for choose by name, browse by channel and browse by time.  Their use
there is to reduce the list of programs to choose from before the user's
further selection criteria is applied.  Thus, selecting "movies" on the list
for choose by name will only allow selection from a list of programs that
have the "movie group" genre as one of their properties.

=cut

use strict;
use FileHandle;

package TiVo::TiVoSlice;
#
# Turn this on for debugging.  Can be done by passing an optional third
# parameter to SliceOpen.
#
my $Debugging = 0;
#
# Manifest constants applied to slice file generation.
#
my $LOW_PROG_SLOT = 10;				# Lowest program slot generated
#
# Input file information.
#
my $OpenFileDone;					# Set true if open file done
my $SliceHand;						# Slice file handle
my $SliceVer;						# Version of slice file generated
#
# Accumulated record length for Station Day records.
#
my $AccumRecLen;					# Accumulated record length
my $HeadRecord;					# Header record
my $SlotCount;						# Count of slots
my $SlotsUsed;						# Number of slots used
my @SlotRecords;					# Slot records
#
# Tables of record object fields and how to process them.
#
# Each field has a specific format:
#
#      0 (0x00) - End of data marker.
#      A (0x41) - Name of actor or other person.
#      D (0x44) - Date (time_t) numeric field.
#      E (0x45) - Enumerated numeric field.
#      I (0x49) - Index tuple numeric field.
#      L (0x4C) - Logo index numeric field where the TiVo space is 0x10000.
#      N (0x4E) - Numeric field.
#      S (0x53) - String, length follows.
#      T (0x54) - Program showing time slot list count.
#
# The field format numbers are orred together with the field ID number in the
# tables below, format number first, field number second.
#
my %ProgramTable = (				# 0x0103
	"End", 0x0000,					# End of data marker
	"tmsid", 0x5341,				# TmsID (Times/Mirror ID)
	"title", 0x5345,				# Movie/episode title
	"index", 0x494A,				# Index tuple
	"description", 0x534D,			# Description
	"language", 0x5351,				# Language string
	"showtype", 0x4554,				# Show type
	"date", 0x4E58,				# Release date (year)
	"mpaarating", 0x455C,			# MPAA rating
	"starrating", 0x4560,			# Star rating
	"time", 0x4E64,				# Movie run time
	"country", 0x5369,				# Country
	"networksource", 0x536D,			# Network source
	"sourcetype", 0x4570,			# Source type
	"episodetitle", 0x5375,			# Episode title
	"advisory", 0x4578,				# Advisory
	"actor", 0x417D,				# Actor's name
	"gueststar", 0x4181,			# Guest star's name
	"director", 0x4185,				# Director's name
	"execproducer", 0x4189,			# Executive Producer's name
	"producer", 0x418D,				# Producer's name
	"writer", 0x4191,				# Writer's name.
	"host", 0x4195,				# Show host's name
	"genre", 0x459C,				# Genre number
	"color", 0x45A0,				# Color code
	"episodenum", 0x4EA4,			# Episode number
	"origepisode", 0x4EA8,			# Original episode number
	"isepisode", 0x4EB4,			# Is episode
	"origairdate", 0x44C4,			# Original air date
	"Junk", 0x00FF);				# Placeholder

my %SeriesTable = (					# 0x0104
	"End", 0x0000,					# End of data marker
	"tmsid", 0x5341,				# TmsID (Times/Mirror ID)
	"title", 0x5345,				# Movie/episode title
	"genre", 0x4550,				# Genre number
	"isepisodic", 0x4E58,			# Flag indicating show is episodic
	"Junk", 0x00FF);				# Placeholder

my %StationTable = (				# 0x0105
	"End", 0x0000,					# End of data marker
	"tmsid", 0x5341,				# TmsID (Times/Mirror ID)
	"name", 0x5345,				# Station name
	"callsign", 0x5349,				# Call sign
	"city", 0x534D,				# City where station is located
	"state", 0x5351,				# State where station is located
	"zipcode", 0x5355,				# Zipcode of station
	"country", 0x5359,				# Contry where station is located
	"affiliation", 0x535D,			# Affiliation (e.g. satellite)
	"dmaname", 0x5361,				# DMA name
	"dmanum", 0x4E64,				# DMA number
	"fccchnum", 0x4E68,				# FCC broadcast channel number
	"logoindex", 0x4C6C,			# Logo index
	"affiliationindex", 0x4E70,		# Affiliation index
	"payperview", 0x4E74,			# Pay per view flag
	"Junk", 0x00FF);				# Placeholder

my %StationDayTable = (				# 0x0106
	"End", 0x0000,					# End of data marker
	"index1", 0x4942,				# Index tuple
	"day", 0x4444,					# Day in question
	"index2", 0x4946,				# Index tuple
	"date", 0x4448,				# Date
	"showlist", 0x544A,				# Count of program showing time slots
	"time", 0x4E4C,				# Start time of slot (in seconds)
	"duration", 0x4E50,				# Duration of slot (in seconds)
	"partindex", 0x4E54,			# Part index
	"partcount", 0x4E58,			# Part count
	"premiere", 0x455C,				# Premiere number
	"live", 0x4560,				# Live program number
	"bits", 0x4E64,				# Huh?
	"noindex", 0x4E6C,				# Don't index flag
	"tvrating", 0x4570,				# Rating
	"dolby", 0x4E7C,				# Dolby program material
	"Junk", 0x00FF);				# Placeholder
#
# Tables of enumerations for various numeric values.
#
# Note that the second item is a regular expression used to search a candidate
# string for a match for this value.  Case-insensitive searching is used.  Be
# careful with single words that could match multiple word strings because the
# order of matching is not determined by us, since the patterns are in a hash
# (e.g. "romantic comedy" might get matched by "comedy" before
# "romantic\s+comedy" ever comes up).  Best to use "^" and "$" in any single
# word patterns that are likely to match other strings to prevent this.
#
# Also note to double escape the backslash characters wherever you intend for
# them to be realy there in the regular expression.  Since they are within a
# quoted string, single backslahes won't have the effect you wanted.  For
# example: use "aaa\\s+bbb" instead of "aaa\s+bbb".
#
my %AdvisoryEnum = (				# Advisory
	1, "^language\$",
	2, "graphic\\s*language",
	3, "^nudity\$",
	4, "brief\\s+nudity",
	5, "graphic\\s*violence",
	6, "^violence\$",
	7, "mild\\s*violence",
	8, "strong\\s*sexual\\s*content",
	9, "rape",
	10, "adult\\s*situations");

my %ColorCodeEnum = (				# Color code
	1, "b\\s*\\&?\\s*w",
	2, "Color\\s*((and|\&)\\s*)?b\\s*\\&?\\s*w",
	3, "colorized",
	4, "^color\$");

my %GenreEnum = (					# Genre
	1, "^action|adventure|action\\s+((and|\&)\\s+)?adventure\$",
	2, "adult",
	3, "animals?",
	4, "animated",
	5, "anthology",
	6, "^art\$",
	7, "^auto\$",
	8, "auto\\s+maintenance",
	9, "award\\s+show",
	10, "ballet",
	11, "baseball",
	12, "basketball",
	13, "beauty",
	14, "bicycle",
	15, "billiards?",
	16, "biography",
	17, "boating",
	18, "body\\s*building",
	19, "bowling",
	20, "boxing",
	21, "^business|financial|business\\s+((and|\&)\\s+)?financial\$",
	22, "^childrens?\$",
	23, "classic",
	24, "collectibles?",
	25, "^comedy\$",
	26, "computers?",
	27, "cooking",
	28, "courtroom",
	29, "^crime\$",
	30, "crime\\s*drama",
	31, "curling",
	32, "dance",
	33, "docu\\s*drama",
	34, "^documentary\$",
	35, "^drama\$",
	36, "^educational\$",
	37, "electronics?",
	38, "family",
	39, "^fantasy\$",
	40, "fashion",
	41, "fishing",
	42, "football",
	43, "french",
	44, "fund\\s*raiser",
	45, "game\\s*show",
	46, "golf",
	47, "gymnastics?",
	48, "^health|fitness|health\\s+((and|\&)\\s+)?fitness\$",
	49, "^historical\$",
	50, "historical\\s*drama",
	51, "hockey",
	52, "^holiday\$",
	53, "holiday\\s*special",
	54, "^home|garden|home\\s+((and|\&)\\s+)?garden\$",
	55, "horror",
	56, "horse",
	57, "housewares?",
	58, "how\\s*to",
	59, "international",
	60, "interview",
	61, "jewelry",
	62, "lacrosse",
	63, "magazine",
	64, "martial\\s*arts?",
	65, "medical",
	66, "motor\\s*sports?",
	67, "motor\\s*cycles?",
	68, "^movies?\$",
	69, "^music\$",
	70, "musical",
	71, "^mystery|mystery\\s+((and|\&)\\s+)?suspense\$",
	72, "^nature\$",
	73, "^news\$",
	74, "olympics",
	75, "^opera\$",
	76, "outdoors?",
	77, "public\\s*affairs",
	78, "racquetball",
	79, "real\\s*life",
	80, "religion",
	81, "rodeo",
	82, "romance",
	83, "romantic\\s+comedy",
	84, "rugby",
	85, "running",
	86, "satire",
	87, "^science\$",
	88, "^sci(ence)?\\s*fi(ction)?\$",
	89, "self\\s*help",
	90, "shopping",
	91, "situation",
	92, "skating",
	93, "skiing",
	94, "sled\\s*dogs?",
	95, "snow\\s*sports?",
	96, "soap\\s*opera",
	97, "soccer",
	98, "softball",
	99, "spanish",
	100, "^specials?\$",
	101, "^sports?\$",
	102, "sports\\s*news",
	103, "sports\\s*talk",
	104, "^suspense\$",
	105, "^swimming|diving|swimming\\s+((and|\&)\\s+)?diving\$",
	106, "^talk\$",
	107, "talk\\s*show",
	108, "tennis",
	109, "theatre",
	110, "thriller",
	111, "^track|field|track\\s+((and|\&)\\s+)?field\$",
	112, "travel",
	113, "variety",
	114, "volleyball",
	115, "^war\$",
	116, "water\\s*sports?",
	117, "^weather\$",
	118, "^westerns?\$",
	119, "wrestling",
	1000, "^arts?|living|arts,?\\s+music\\s+((and|\&)\\s+)?living\\s*group\$",
	1001, "^childrens\\s*group\$",
	1002, "^comedy\\s*group\$",
	1003, "^daytime\\s*group\$",
	1004, "^documentary\\s*group\$",
	1005, "^drama\\s*group\$",
	1006, "^movie\\s*group\$",
	1007, "^news,?\\s+business\\s+((and|\&)\\s+)?weather\\s*group\$",
	1008, "^science,?\\s+nature\\s+((and|\&)\\s+)?technology\\s*group\$",
	1009, "^sports\\s*group\$",
	1010, "^talk\\s*show\\s*group\$",
	1011, "^action\\s+((and|\&)\\s+)?adventure\\s*group\$",
	1012, "^educational\\s*group\$",
	1013, "^mystery\\s+((and|\&)\\s+)?thriller\\s*group\$",
	1014, "^sci(ence)?\\s*fi(ction)?\\s+((and|\&)\\s+)?fantasy\\s*group\$",
	1015, "^western\\s*group\$");

my %LiveEnum = (					# Live
	1, "delay",
	2, "tape",
	3, "live");

my %MPAAEnum = (					# MPAA rating
	1, "^g\$",
	2, "^pg\$",
	3, "pg13|pg-13",
	4, "^r\$",
	5, "^x\$",
	6, "nc17|nc-17",
	7, "ao",
	8, "nr");

my %PremiereEnum = (				# Premiere
	1, "season\\s+finale",
	2, "series\\s+finale",
	3, "^premiere\$",
	4, "season\\s+premiere",
	5, "series\\s+premiere");

my %ShowTypeEnum = (				# Show type
	1, "serial",
	2, "short\\s+film",
	3, "special",
	4, "limited\\s+series",
	5, "^series\$",
	6, "mini\\s+series",
	7, "paid(\\s+programming)?");

my %SourceTypeEnum = (				# Source type
	1, "network",
	2, "local|syndicated",
	3, "block");

my %StarEnum = (					# Star rating
	1, "^(one|\\*)\$",
	2, "^(one\\s*point\\s*five|\\*\\+)\$",
	3, "^(two|\\*\\*)\$",
	4, "^(two\\s*point\\s*five|\\*\\*\\+)\$",
	5, "^(three|\\*\\*\\*)\$",
	6, "^(three\\s*point\\s*five|\\*\\*\\*\\+)\$",
	7, "^(four|\\*\\*\\*\\*)\$");

my %TVRatingEnum = (				# TV rating
	1, "y7|tv\\s*y7",
	2, "^(y|tvy)\$",
	3, "^(g|tvg)\$",
	4, "^(pg|tvpg)\$",
	5, "^(14|ty14)\$",
	6, "^(m|tvm)\$");
#
# Table of all enumerations and which enumeration table to use each one.
#
# The record type is orred with the field number so that one table can be used
# to look up all enumerations for all records.
#
my %EnumTable = (
	0x0354, \%ShowTypeEnum,			# Show type
	0x035C, \%MPAAEnum,				# MPAA rating
	0x0360, \%StarEnum,				# Star rating
	0x0370, \%SourceTypeEnum,		# Source type
	0x0378, \%AdvisoryEnum,			# Advisory
	0x039C, \%GenreEnum,			# Genre
	0x03A0, \%ColorCodeEnum,			# Color code
	0x0450, \%GenreEnum,			# Genre
	0x065C, \%PremiereEnum,			# Premiere
	0x0660, \%LiveEnum,				# Live
	0x0770, \%TVRatingEnum,			# TV rating
	0x00FF, 0);					# Placeholder
###############################################################################
BEGIN
	{
	$OpenFileDone = 0; $SliceVer = "1.3";

	$AccumRecLen = $SlotCount = $SlotsUsed = 0;
	$HeadRecord = ""; @SlotRecords = ();
	}
###############################################################################

=pod - TiVo slice file open


Use TiVoSlice

TiVo::SliceOpen(SliceFileName, Version, [DebugFl])

SliceFileName                 The full path and file name of the slice file
                              that is to be generated.  This file will be
                              created, if necessary, and opened by this
                              routine.  Any previous data in this file will
                              be overwritten.

Version                       A version number string, to be used to generate
                              different flavors of the slice file.  Currently,
                              the following versions are supported:

                              1.3 - Early slice file version (the default).

DebugFl                       An optional flag which will enable debugging if
                              set to TRUE.  If omitted, debugging will not be
                              enabled.


Description
-----------

This routine will open up a new slice file or reuse an existing one.  It
locates the slice file by the path/file name given.  Once the file is opened,
it is set up for use by SliceWriteRecord and any initial header information
is generated.  The file must be closed by SliceClose when you're done.

=cut

sub TiVo::SliceOpen
{
my ($SliceFileName, $Version, $DebugFl) = @_;
#
# If we've already opened the slice file, just skip this.
#
return if ($OpenFileDone);
#
# Save the version number, in case we need to generate something different
# some day.
#
$SliceVer = $Version;
#
# Enable debugging if the debug flag is passed and it is set TRUE.
#
$Debugging = 1 if ((scalar(@_) > 2) && $DebugFl);
#
# Open up the slice file.
#
$SliceHand = new FileHandle;

print("Opening version $SliceVer slice file $SliceFileName\n") if ($Debugging);

$SliceHand->open("+>$SliceFileName")
	or die "Could not open Slice file $SliceFileName: $!";
binmode($SliceHand);

$OpenFileDone = 1;
#
# Start it out by writing the slice file header.
#
$SliceHand->print(pack("c", 0x03));
#
# We're all done with open.
#
return;
}
###############################################################################

=pod - TiVo slice file write record


Use TiVoSlice

TiVo::SliceWriteRecord(RecordType, Index, Version, ptRecord)

RecordType                    A record type name that indicates what type of
                              record is to be generated:

                              Program - A type 0x03 record with detailed
                                   program information in it.

                              Series - A type 0x04 record with series
                                   information in it.

                              Station - A type 0x05 record with station
                                   details in it.

                              StationDay - The header for a 0x06 record will
                                   be set up with station program information
                                   in it.  Nothing will be generated until all
                                   of the slot program details have been
                                   generated via "SlotDetail" records (see
                                   below).

                              SlotDetail - Detailed program information for a
                                   program slot is generated.  The correct
                                   number of slots must be generated following
                                   each "StationDay" record.

Index                         A number to be used for indexing purposes.  This
                              number will be converted into a server ID for
                              the record when it is written.  Server IDs are
                              how TiVo indexes records.  The Server ID is
                              created by permuting the index number according
                              to the rules determined in this code.  If you
                              really need to know, look at GenServerID.

                              For the "SlotDetail" record only, this index
                              must be a 1-based number that indicates the
                              sequence position of the slot detail record.
                              Thus, for these types of records, the index
                              number must take a value of 1 through n.

Version                       A version number, which is used by TiVo to keep
                              track of record versions in the TiVo database.

                              Try setting the version number to the same,
                              monotonically increasing (over subsequent slice
                              files) number.  This seems to work for Tridge.

ptRecord                      A pointer to a hash the describes the record to
                              be written.  There is one hash entry for each
                              field in the record.  These are described below.

                              Note that you should tie the hash passed in
                              ptRecord to IxHash to cause the fields in the
                              record to be generated in the same order that
                              you inserted them (if you care).  Presumably
                              TiVo doesn't care but you never know.  If you
                              can't find IxHash in your Perl libraries it is
                              available at www.cpan.org.


Description
-----------

This routine will generate one record in a slice file, based on the
information passed to it.  Its primary purpose is to maintain the integrity
of the slice file by generating properly composed records with properly
formatted data in their fields.  The caller is still responsible for the
overall cohesiveness of the slice file (i.e. index links must point to the
right places).  Also, the slice file is only as good as the data it is
created from.

The output of this routine is a single record written to the slice file that
was opened by SliceOpen.  The format of the output follows the rules described
in the overview of this module.  Note that slice files consist of many records
so you will be calling this routine repetitively to generate a whole slice
file.

Input to this routine is an object (hash) that describes the record to be
written.  There are five types of record that can be written.  For each of
these records, there are a specific set of fields that apply.  To generate a
field, include a hash key for the field in the record object and set its
value to the appropriate field value.  If you use IxHash (available from
www.cpan.org), the fields will be generated in the same order in which they
are inserted into the hash.  The permissable fields for each record are
described below.

Program Day records are treated as a special case.  Basically, a Program Day
record is composed of a header section which includes a list of time slots.
Following the time slot list is a number of sub-records (one corresponding
to each slot in the list), that gives time slot program detail information.
Since the entire Program Day record can be thought of conceptually as a
series of individual records, that's how this routine writes them.

To write a complete Station Day record, first call this routine with the
RecordType set to "StationDay".  This will set up the record and prepare the
slice file for the sub-records.  Note that you must include the "ShowList"
field in the record object and set its value to the number of showing time
slots that are needed to describe the day's showing programs.  That causes
the indicated number of time slots to be generated in the showing list.

Next, you must call this routine the number of times indicated by the
"ShowList" field with the RecordType set to "SlotDetail".  Set up the detail
record with the appropriate information but remember to call this routine
with the index numbers of the slot detail records set to 1 through n.  The
order of the slot detail records is unimportant (because this routine will
generate them in TiVo's preferred reverse order) but the lowest one must be
numbered one and their index numbers must be monotonically increasing from
there by one.

After n slot detail records have been passed to this routine, it will actually
write the entire Station Day record which it has been saving up.  Note that
you must send this routine the proper number of program detail records or it
will not generate any Station Day record.


Record Type "Program"
---------------------

The following field types may be passed in records of this type:

     Field Name        Type   Description

     Actor             Name   Actor's name in normal order (i.e. first name,
                              initial, last name).  The names will have their
                              order reversed to put them in TiVo order.
                              Multiple names may be passed, separated by "|".
     Advisory          Enum   Advisory (to match one of the regular
                              expressions in AdvisoryEnum).  Multiple values
                              may be passed, separated by "|".
     Color             Enum   Color number (to match one of the regular
                              expressions in ColorEnum).
     Country           Str    Country (e.g. "USA").
     Date              Num    Release date (the year number).
     Description       Str    Extended description of show or movie.
     Director          Name   Director's name in normal order (i.e. first
                              name, initial, last name).  The names will have
                              their order reversed to put them in TiVo order.
                              Multiple names may be passed, separated by "|".
     EpisodeNum        Num    Episode number.
     EpisodeTitle      Str    Episode title for shows that are episodic (e.g.
                              "Max Gets a Gene Transplant").
     ExecProducer      Name   Executive Producer's name in normal order (i.e.
                              first name, initial, last name).  The names will
                              have their order reversed to put them in TiVo
                              order.  Multiple names may be passed, separated
                              by "|".
     Genre             Enum   Genre (to match one of the regular
                              expressions in GenreEnum).  Multiple values
                              may be passed, separated by "|".
     GuestStar         Name   Any guest star's name in normal order (i.e.
                              first name, initial, last name).  The names will
                              have their order reversed to put them in TiVo
                              order.  Multiple names may be passed, separated
                              by "|".
     Host              Name   Show host's name in normal order (i.e. first
                              name, initial, last name).  The names will have
                              their order reversed to put them in TiVo order.
                              Multiple names may be passed, separated by "|".
     Index             Index  Index tuple which points to the associated title
                              type record.  The tuple consists of a string
                              containing an object type, attribute and link
                              (server ID), separated by commas.
     IsEpisode         Num    Is episode (0x00 = no, 0x01 = yes).  Should be
                              set to yes for shows that repeat and may or may
                              not use the same description over and over.
     Language          Str    Language string (e.g. "English").
     MPAARating        Enum   MPAA rating (to match one of the regular
                              expressions in MPAAEnum).
     NetworkSource     Str    Network source name (e.g. "CBS").
     OrigAirDate       Date   Original air date (days since the epoch).  You
                              should set this field's value to the time_t
                              value that gives the Original Air Date.  The
                              time_t will be converted to the correct epoch
                              offset.
     OrigEpisode       Num    Original episode number.
     Producer          Name   Producer's name in normal order (i.e.  first
                              name, initial, last name).  The names will have
                              their order reversed to put them in TiVo order.
                              Multiple names may be passed, separated by "|".
     Showtype          Enum   Show type (to match one of the regular
                              expressions in ShowTypeEnum).
     StarRating        Enum   Star rating (to match one of the regular
                              expressions in StarEnum).
     SourceType        Enum   Source type (to match one of the regular
                              expressions in SourceTypeEnum).
     Time              Num    Movie run time (in minutes).
     Title             Str    Movie/show title.  In the case of episodic
                              shows, this should be the show's name (e.g.
                              "Dark Angel").
     TmsID             Str    TmsID (Times/Mirror ID).
     Writer            Name   Writer's name in normal order (i.e. first
                              name, initial, last name).  The names will have
                              their order reversed to put them in TiVo order.
                              Multiple names may be passed, separated by "|".


Record Type "Series"
--------------------

The following field types may be passed in records of this type:

     Field Name        Type   Description

     Genre             Enum   Genre (to match one of the regular
                              expressions in GenreEnum).  Multiple values
                              may be passed, separated by "|".
     IsEpisodic        Num    Flag indicating whether show is episodic (i.e.
                              has a regular title and possibly an episode
                              title -- does not duplicate IsEpisode in the
                              Program record).
     Title             Str    Movie/episode title.
     TmsID             Str    TmsID (Times/Mirror ID).


Record Type "Station"
---------------------

The following field types may be passed in records of this type:

     Field Name        Type   Description

     Affiliation       Str    Affiliation (e.g. satellite).
     AffiliationIndex  Num    Affiliation index.
     CallSign          Str    Call sign used by station (e.g. "WGBH").
     City              Str    City where station is located.
     Country           Str    Contry where station is located.
     DMAName"          Str    DMA name.
     DmaNum"           Num    DMA number.
     FCCChNum          Num    FCC broadcast channel number.
     Name              Str    Station name.
     PayPerView        Num    Pay per View flag (0x00 = no, 0x01 = yes).
                              Should be set to yes for PPV shows.
     State             Str    State where station is located.
     TmsID             Str    TmsID (Times/Mirror ID).
     Zipcode           Str    Zipcode of station.
	LogoIndex         Logo   Logo index (any numbers below 0x10000 are
                              adjusted by that amount to put them in the
                              TiVo space).


Record Type "Station Day"
-------------------------

The following field types may be passed in records of this type:

     Field Name        Type   Description

     Day               Date   Day (actually, days since the epoch) for which
                              this station day information applies.  You
                              should set this field's value to the time_t
                              value that gives the date you want.  The time_t
                              will be converted to the correct epoch offset.
     Index1            Index  An index tuple which points to the associated
                              station record.  The tuple consists of a string
                              containing an object type, attribute and link
                              (server ID), separated by commas.
     ShowList          Num    Count of the number of program showing slots
                              which should be generated and stored in the
                              "Station Day" field.


Record Type "SlotDetail"
------------------------

The index value passed with this record type must be a slot number between
1 and n, where n is the number of slots generated by the showlist field of
the "StationDay" record.  Note that slots must be generated in ascending
time sequence.  Slot 1 must begin at midnight and proceed for whatever time
it chooses (e.g. 1800).  Slot two must be that many seconds further along in
the day and begin in the time slot immediately following the first one.  And,
so it proceeds.  The "Time" of any slot must be equal to the sums of all
slot durations which preceed it.

The following field types may be passed in records of this type:

     Field Name        Type   Description

     Bits              Num    Don't know.
     Date              Date   Day (actually, days since the epoch) for which
                              this station day information applies.  You
                              should set this field's value to the time_t
                              value that gives the day you want.  The time_t
                              will be converted to the correct epoch offset.
     Dolby             Num    Dolby material flag (0x00 = no, 0x01 = yes).
                              Should be set to yes for Dolby shows.
     Duration          Num    The duration of the slot (in seconds).  This is
                              summed to the previous slot's time field to get
                              the next slot's time field.
     Index1            Index  An index tuple which points to the associated
                              program record.  The tuple consists of a string
                              containing an object type, attribute and link
                              (server ID), separated by commas.
     Index2            Index  An index tuple which points to the associated
                              station record.  The tuple consists of a string
                              containing an object type, attribute and link
                              (server ID), separated by commas.
     Live              Enum   Live program material (to match one of the
                              regular expressions in LiveEnum).
     Premiere          Enum   Premiere type (to match one of the regular
                              expressions in PremiereEnum).
     PartIndex         Num    Part index.
     PartCount         Num    Part count.
     Time              Num    Start time of slot, in seconds from midnight.
     TVRating          Enum   TV rating (to match one of the regular
                              expressions in TVRatingEnum).
     NoIndex           Num    Don't index flag (0x00 = no, 0x01 = yes).
                              Should be set to yes for those shows you don't
                              want idexed.

=cut

sub TiVo::SliceWriteRecord
{
my ($RecordType, $Index, $Version, $ptRecord) = @_;
my $ObjType = 0x01;					# Object type for records
my $RecType = 0x00;					# Record type
my $ptFieldTable = 0;				# Field translate table
my $SlotID = 0;					# Time slot field ID to use
my $TimeSlots = 0;					# Number of time slots needed
my $RecLen = 0;					# Record length
my $SliceData = "";					# Slice data to be written
my ($FieldName, $FieldVal);
#
# Figure out what type of record we really have.
#
if (lc($RecordType) eq "program")
	{ $RecType = 0x03; $ptFieldTable = \%ProgramTable; }
elsif (lc($RecordType) eq "series")
	{ $RecType = 0x04; $ptFieldTable = \%SeriesTable; }
elsif (lc($RecordType) eq "station")
	{ $RecType = 0x05; $ptFieldTable = \%StationTable; }
elsif (lc($RecordType) eq "stationday")
	{
	$RecType = 0x06; $ptFieldTable = \%StationDayTable;

	$AccumRecLen = $SlotCount = $SlotsUsed = 0;
	$HeadRecord = ""; @SlotRecords = ();
	}
elsif (lc($RecordType) eq "slotdetail")
	{ $ObjType = 0x00; $RecType = 0x07; $ptFieldTable = \%StationDayTable; }
#
# Start out with the object type and record type.
#
$SliceData .= pack("c", $ObjType) if ($ObjType > 0x00);
$SliceData .= pack("c", $RecType);
#
# Add the server ID and version.  So far, every record has these except the
# special case for the Slot Detail record.
#
if ($ObjType > 0x00)
	{
	printf("\nRecord %s (0x%02x%02x)\n", $RecordType, $ObjType, $RecType)
		if ($Debugging);

	$SliceData .= GenNumeric(GenServerID($RecType, $Index));
	$SliceData .= GenNumeric($Version);

	print("  Server ID, Version = ".GenServerID($RecType, $Index).", $Version\n")
		if ($Debugging);
	}
#
# If this is a Station Day Slot Detail record, it only has one index number,
# not a server ID and version.
#
else
	{
	print("  Showing Detail = $RecType, $Index\n") if ($Debugging);

	$SliceData .= GenNumeric($Index+$LOW_PROG_SLOT);
	}
#
# Loop through all of the fields in the record and generate them.
#
while (($FieldName, $FieldVal) = each %$ptRecord)
	{
	#
	# Look up the field ID and type from the appropriate record table.
	#
	if (!exists($ptFieldTable->{lc($FieldName)}))
		{
		print("  Unknown field $FieldName = $FieldVal\n") if ($Debugging);

		next;
		}

	my $FieldID = $ptFieldTable->{lc($FieldName)};
	my $FieldType = ($FieldID & 0xFF00) >> 8;
	$FieldID &= 0xFF;
	#
	# Process the field data based on what type of field it is.
	#
	# The field is an actor (or other) name string field with a length.
	# The last name is found and its order is reversed so that the name
	# is stored as "Joe M. Blow" --> "Blow|Joe M."
	#
	# Multiple names are allowed with each one being separated from the
	# previous ones by a single "|".
	#
	if ($FieldType == 0x41)
		{
		my (@NameList, $NameStr, $StrLen);
		#
		# Split the multiple names in the field into an array.  Multiple
		# names are separated by "|".
		#
		@NameList = split(/\|/, $FieldVal);
		#
		# For each of the names found, flip the last name with the rest of
		# the data.
		#
		while (defined($NameStr = shift(@NameList)))
			{
			if ($NameStr =~ /^(.+)\s([^\s]+)$/)
				{ $NameStr = $2."|".$1; }
			#
			# Generate the name string field.
			#
			my $StrLen = length($NameStr);

			$SliceData .= pack("c", $FieldID);
			$SliceData .= GenNumeric($StrLen); $SliceData .= $NameStr;

			printf("  %s (0x%02x) = %s\n", $FieldName, $FieldID, $NameStr)
				if ($Debugging);
			}
		}
	#
	# The field is an epoch-based date (a Unix time_t that is adjusted to
	# give days from the epoch).
	#
	elsif ($FieldType == 0x44)
		{
		my $Days = int $FieldVal / 86400;

		$SliceData .= pack("c", $FieldID);
		$SliceData .= GenNumeric($Days);

		printf("  %s (0x%02x) = %d\n", $FieldName, $FieldID, $Days)
			if ($Debugging);
		}
	#
	# The field is an enumerated numeric field.
	#
	# Multiple values are allowed with each one being separated from the
	# previous ones by a single "|".
	#
	elsif ($FieldType == 0x45)
		{
		my ($ptEnumTable, @EnumList, $EnumVal, $EnumPat);
		#
		# Make up a key for looking up which enum table to use and then find
		# that table.
		#
		$ptEnumTable = $EnumTable{($RecType << 8) | $FieldID};
		#
		# Split the multiple values in the enumeration into an array.
		# Multiple values are separated by "|".
		#
		@EnumList = split(/\|/, $FieldVal);
		#
		# For all of the possible enumerated values, use each one's regular
		# expression to search our candidate strings and see if it gets a
		# match.
		#
		# Note that, due to a peculiarity (or is that stupidity) of Perl, we
		# must read through the entire hash table, even if we've processed
		# all of the candidate strings before we get to the end (pretty
		# likely).  This is due to the fact that starting through the list
		# again, later on, will be screwed up unless it is allowed to run
		# to the end always.  Nice, huh?
		#
		while (($EnumVal, $EnumPat) = each %$ptEnumTable)
			{
			my $EnumCnt = 0;
			#
			# Process each of the enumerations separately.  Emit one field
			# for each one that matches.
			#
			while ((@EnumList > 0) && ($EnumCnt < @EnumList))
				{
				#
				# Small hack by Warren.
				# If the value we are given is numeric,
				# then simply use this, and don't search
				# for a matching pattern
				#
				if ($EnumList[$EnumCnt] =~ /^\d+$/) {
				  $SliceData .= pack("c", $FieldID);
				  $SliceData .= GenNumeric($EnumList[$EnumCnt]);
				  splice(@EnumList, $EnumCnt, 1);
				  next;
				}

				if ((length($EnumList[$EnumCnt]) > 0)
					&& ($EnumList[$EnumCnt] =~ /$EnumPat/i))
					{
					$SliceData .= pack("c", $FieldID);
					$SliceData .= GenNumeric($EnumVal);

					printf("  %s (0x%02x) = %s (%d)\n", $FieldName,
						$FieldID,	$EnumList[$EnumCnt], $EnumVal)
						if ($Debugging);

					splice(@EnumList, $EnumCnt, 1);
					}
				else { $EnumCnt++; }
				}
			}
		}
	#
	# The field is a numeric index tuple field.
	#
	elsif ($FieldType == 0x49)
		{
		my ($IxType, $IxObj, $IxID);
		my $IxTypeStr = "unknown";
		#
		# Split out the three components of the index tuple.
		#
		$FieldVal =~ /^(\d+),\s*(\d+),\s*(\d+)$/;
		$IxType =$1; $IxObj = $2; $IxID = $3;
		#
		# Accumulate some dubugging information, if need be.
		#
		if ($Debugging)
			{
			if ($IxType == 0x03) { $IxTypeStr = "program"; }
			elsif ($IxType == 0x04) { $IxTypeStr = "series"; }
			elsif ($IxType == 0x05) { $IxTypeStr = "station"; }
			elsif ($IxType == 0x06) { $IxTypeStr = "station day"; }
			}
		#
		# Generate the index tuple.
		#
		$SliceData .= pack("c", $FieldID);
		$SliceData .= GenNumeric($IxType);
		$SliceData .= GenNumeric($IxObj);
		$SliceData .= GenNumeric(GenServerID($IxType, $IxID));

		printf("  %s (0x%02x) = %d, %d, %d --> %s record\n", $FieldName,
			$FieldID, $IxType, $IxObj, GenServerID($IxType, $IxID),
			$IxTypeStr) if ($Debugging);
		}
	#
	# The field is a logo index numeric field.  All of these are put in
	# the TiVo index space, if they fit there.
	#
	elsif ($FieldType == 0x4C)
		{
		$FieldVal |= 0x10000 if ($FieldVal < 0x10000);

		$SliceData .= pack("c", $FieldID);
		$SliceData .= GenNumeric($FieldVal);

		printf("  %s (0x%02x) = %d\n", $FieldName, $FieldID, $FieldVal)
			if ($Debugging);
		}
	#
	# The field is a numeric field.
	#
	elsif ($FieldType == 0x4E)
		{
		$SliceData .= pack("c", $FieldID);
		$SliceData .= GenNumeric($FieldVal);

		printf("  %s (0x%02x) = %d\n", $FieldName, $FieldID, $FieldVal)
			if ($Debugging);
		}
	#
	# The field is a string field with a length.
	#
	elsif ($FieldType == 0x53)
		{
		my $StrLen = length($FieldVal);

		$SliceData .= pack("c", $FieldID);
		$SliceData .= GenNumeric($StrLen); $SliceData .= $FieldVal;

		printf("  %s (0x%02x) = %s\n", $FieldName, $FieldID, $FieldVal)
			if ($Debugging);
		}
	#
	# The field is a program showing time slot list.  Just save it for the
	# end (where it belongs).
	#
	elsif ($FieldType == 0x54) { $SlotID = $FieldID; $TimeSlots = $FieldVal; }
	}
#
# If there are any time slots required, generate a showing list now.  It
# should always be last, right before the showing detail records.
#
if ($TimeSlots > 0)
	{
	my $SlotNum = 1;
	#
	# Bang out as many time slots as needed.
	#
	# Note that the fixed number used below should match that assigned to
	# $RecType for "slotdetail" records, above.
	#
	while ($SlotNum <= $TimeSlots)
		{
		$SliceData .= pack("c", $SlotID);
		$SliceData .= pack("c", 0x07);
		$SliceData .= GenNumeric($SlotNum+$LOW_PROG_SLOT);

		print("  Showing = 7, $SlotNum\n") if ($Debugging);

		$SlotNum++;
		}
	#
	# Save the slot count for use in program slot detail generation.
	#
	$SlotCount = $SlotsUsed = $TimeSlots;
	}
#
# Records always end with the end field.
#
$SliceData .= pack("c", $ptFieldTable->{"End"});
#
# Figure the length of the slice data.
#
$RecLen = length($SliceData);
#
# If this is a Program Day record or a slot detail, save it up for generation
# of the record at a later date.
#
if (($ObjType == 0x00) || ($RecType == 0x06))
	{
	$AccumRecLen += $RecLen;

	if ($ObjType == 0x00)
		{ $SlotRecords[$Index-1] = $SliceData; $SlotCount--; }
	else { $HeadRecord = $SliceData; }
	#
	# If we've got all of the slot detail records, we can now generate the
	# entire Program Day record.
	#
	if ($SlotCount <= 0)
		{
		#
		# Pump out the accumulated length and saved head of the Station Day
		# record.
		#
		print("  Ultimate record length = $AccumRecLen\n") if ($Debugging);

		$SliceHand->print(pack("N", $AccumRecLen));
		$SliceHand->print($HeadRecord);
		#
		# Pump out all of the slot detail records in reverse order (just
		# like the way TiVo does it).
		#
		while ($SlotsUsed-- > 0)
			{ $SliceHand->print($SlotRecords[$SlotsUsed]); }
		}
	}
#
# Figure the length of the slice data and write it out to the slice file.
# Then, write the slice data itself.
#
else
	{
	print("  Ultimate record length = $RecLen\n") if ($Debugging);

	$SliceHand->print(pack("N", $RecLen));
	$SliceHand->print($SliceData);
	}
#
# We're all done with this record.
#
return;
}
###############################################################################

=pod - TiVo slice file close


Use TiVoSlice

TiVo::SliceClose()


Description
-----------

This routine will close up a slice file that was previously opened by
SliceOpen.  It should be the last thing done to the slice file after you are
finished generating all of its records with SliceWriteRecord.

=cut

sub TiVo::SliceClose
{
#
# If we've already closed the slice file, just skip this.
#
return if (!$OpenFileDone);
#
# Close the slice file.
#
print("\nClosing slice file\n") if ($Debugging);

$SliceHand->close(); undef($SliceHand);

$OpenFileDone = 0;
#
# We're all done with close.
#
return;
}
###############################################################################
sub GenNumeric
#
# NumValue                              The numeric value which is to be
#                                       converted to an output string which
#                                       can be written to the slice file.
#
# returns                               The byte stream containing the numeric
#                                       value represented as a TiVo number.
#                                       The length function can be used to
#                                       determine its length for counting
#                                       purposes.
#
# This routine converts a numeric value into a variable length, expandable,
# byte stream representing in TiVo numeric format.  The return value is the
# byte stream itself, which may be appended to a slice record.
#
# All TiVo numerics have a special expanding format that is designed to save
# space.  They work by reserving either two or four bits in the first byte of
# any number for the number length code.  The first two or four bits encode
# the length as follows:
#
#      00   - A single byte.
#      10   - Two bytes.
#      11   - Three or more bytes (determined by the next two bits).
#      1100 - Three bytes.
#      1110 - Four bytes.
#      1111 - Five bytes.
#
# The largest number a single byte can encode is 0x3F.  By convention, two
# bytes can encode 0x0FFF (which would appear as 0x8FFF) as their largest
# number.  Three bytes can encode 0x0FFFFF (which would appear as 0xCFFFFF),
# while four and five bytes can encode 0x0FFFFFFF (appearing as 0xEFFFFFFF)
# and 0x0FFFFFFFFF (appearing as 0xFFFFFFFFFF) respectively.  Mind you, Perl
# can only deal with 32 bit integers so that the real largest number you can
# have is 0xFFFFFFFF (appearing as 0xF0FFFFFFFF).
#
{
my ($NumValue) = @_;
my ($ByteStream);
#
# Depending on the size of the number, generate the correct number of bytes.
#
if ($NumValue <= 0x3F) { $ByteStream = pack("c", $NumValue); }
elsif ($NumValue <= 0x0FFF) { $ByteStream = pack("n", ($NumValue | 0x8000)); }
elsif ($NumValue <= 0x0FFFFF)
	{
	my $HiByte = (($NumValue & 0x0F0000) >> 16) | 0xC0;
	$ByteStream = pack("c n", $HiByte, ($NumValue & 0xFFFF));
	}
elsif ($NumValue <= 0x8000000)
	{ $ByteStream = pack("N", ($NumValue | 0xE0000000)); }
else { $ByteStream = pack("c N", 0xF0, $NumValue); }
#
# Return the byte stream.
#
return ($ByteStream);
}
###############################################################################
sub GenServerID
#
# RecType                               The type of record for which the server
#                                       ID is to be generated:
#
#                                       0x03 - Program.
#                                       0x04 - Series.
#                                       0x05 - Station.
#                                       0x06 - Station Day.
#
# IxValue                               The index value that is to be
#                                       converted into a server ID.  This can
#                                       be any number from 1-9999999.
#
# returns                               The server ID that results from the
#                                       index value passed.
#
# This routine converts an index value into a server ID.  It does so to ensure
# that all server IDs used to generate records are unique.  To do this, it
# imposes a fixed range on the server IDs for each type of record.  By doing
# this, it allows identical indexes to be used for all different types of
# records without fear of any collisions.  This, for example allows a single
# Guide index to be used as the server IDs for both program and series records.
#
# If you dislike the numbering scheme, change it here only.  No other changes
# are necessary.
#
{
my ($RecType, $IxValue) = @_;
#
# If the number is too large for adjustment, leave it as is.
#
return ($IxValue) if ($IxValue > 9999999);
#
# Adjust the index number based on the type of record.
#
return ($IxValue + 10000000) if ($RecType == 0x03);
return ($IxValue + 20000000) if ($RecType == 0x04);
return ($IxValue + 40000000) if ($RecType == 0x06);

return ($IxValue);
}

1;
