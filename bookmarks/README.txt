
 bkmrkconv - Convert Netscape's bookmarks.html to HTML
 ~~~~~~~~~   (c) Andrew Flegg 1999,2000            mailto:andrew@bleb.org
             v1.09 (20-Sep-2000)  http://www.bleb.org/software/bkmrkconv/


Licence
------- 
 This program and all its associated files have been released under
 the Open Source[TM] Artistic Licence, for further details and a full
 copy of the text please see the following URL. By using this product
 you are implicitly agreeing to the terms of this agreement.

    http://www.opensource.org/licenses/artistic-license.html


Introduction
------------
 This Perl program will convert a Netscape bookmarks.html file into
 a series of separate pages useful for placing on the external web.


Usage
-----
 Simply modify the first line of bkmrkconv.pl to wherever your Perl
 executable lives (usually either /usr/local/bin/perl or
 /usr/bin/perl - failing that try 'which perl' or refer to your User's
 Guide).

 The command line syntax is:

     bkmrkconv [OPTION]...

   -h, --help                  list the command line options
   -n, --nocontents            do not create a contents.html
   -s, --onlysections          contents page will list the sections
                               rather than each individual link
   -f, --file FILE             read input from FILE rather than stdin
   -d, --dir OUTPUTDIR         read templates from OUTPUTDIR/src and
                               output HTML files to here. [Default: .]

 All links are output (if the template has a
 <!--LINK-->...<!--/LINK--> section (see later). However, if they
 contain a '(P)' they will be skipped. Sections containing a '(P)' are
 skipped entirely.


Configuration
-------------
 Three files are used for configuration all in OUTPUTDIR/src/:

   rootpage.html               Template for OUTPUTDIR/index.html, the
                               frontpage for your site.

   linkspage.html              Generated for every folder in the
                               bookmarks file, this forms the basis of
                               most pages in the output.

   contentspage.html           OUTPUTDIR/contents.html contains every
                               link in alphabetical order, or just
                               every section if --onlysections is
                               specified.

 Two examples are included, src.example and src.aflegg. The latter
 contains the templates I use to generate my own bookmarks site at:

     http://www.eh.org/~jaffa/bookmarks/

 The former contains simpler templates which show that the pages don't
 necessarily have to look like Yahoo's. You need a 'src' directory in
 your output directory which can be based on one of these if you so
 wish. However, it is recommended to "roll your own" to develop a
 distinctive style for your site.


Macros
------
 The templates are normal HTML files but some special strings when the
 pages are created.

 All templates
 ~~~~~~~~~~~~~
    $DATE                 The current date and time in the form
                          returned by 'scalar localtime()'; eg. (on
                          this system): Wed Dec 15 20:05:55 1999.

    <!--EXEC "..."-->     Text between the double quotes is passed out
                          using the Perl backtick operator and the
                          results of the command inserted into the
                          file, eg. each page could contain a cookie
                          from the fortune command.

    <!--INCLUDE "..."-->  Include the specified file into this
                          template and continue. The current directory for
                          the file is the output directory. Macros are
                          expanded, and you can even use more INCLUDEs!
                          eg. <!--INCLUDE "src/foo.txt"-->

 Section templates (rootpage.html and linkspage.html)
 ~~~~~~~~~~~~~~~~~
    $TITLE                The name of the current section.

    $PATH                 A colon separated of the sections about this
                          one, eg. "Root : Misc."

    $PATHURL              As $PATH but each of the sections is a
                          hyperlink to that section's page.

    <!--IF cond THEN-->   "cond" is evaluated as a Perl expression and if
      ...                 it evaluates to true the first block is
    [<!--ELSE-->]         placed into the text; if not, the second, optional
      ...                 block is used instead. Two variables which may
    <!--ENDIF-->          be of use here are '$numSections' and
                          '$numLinks' which contain the number of
                          sections and the number of links on the
                          current page respectively.

    <!--SECTION-->        Repeat this section for every subsection in this
      ...                 page.
    <!--/SECTION-->

    <!--LINK-->           Repeat this block for every link in this section.
      ...
    <!--/LINK-->

    $URL                  The URL of whatever is currently being
                          replaced, either a relative URL for a new
                          section or an absolute URL from the
                          bookmarks file for a link.

    $NAME                 The text describing the above URL, either a
                          section name or link title.

    $NUMSECTIONS          Within a <!--SECTION--> block returns the
                          number of links contained within the
                          subsection currently being processed.

    $NEXT                 Within a <!--SECTION--> block this will
                          contain the next two links (and an ellipsis
                          if appropriate).

    $DESC                 Inside a link this contains the description
                          field for that link, if defined. This must
                          be used within a <!--DESC-->...<!--/DESC-->
                          block within the <!--LINK-->
                          section. Currently only the first line of
                          the description is used.

 Contents template (contentspage.html)
 ~~~~~~~~~~~~~~~~~
    <!--SECTION-->        A section exists for [A-Z] and a symbol category.
      ...                 A <!--LINK--> block is inserted for each link.
      <!--LINK-->
        ...
      <!--/LINK-->
      ...
    <!--/SECTION-->

    $PREFIX               The first letter of the section being
                          currently generated.

    $SECTION              Of the form <a href="...">...</a>, this
                          contains a link to the section this link
                          came from.


Tables
------
 Two column tables are produced if there is a <table ...> tag within
 16 characters of the <!--SECTION--> tag. This will insert <tr
 class="section"> and </tr> tags as appropriate. This also applies
 with LINKs as well.


Testing
-------
 The program has been successfully run on RedHat Linux 5.2 with
 Perl 5.00404, Windows 98 with Perl 5.00307 and Solaris 7 with
 Perl 5.00505.


Limitations
-----------
 Each section must have its own page - ie. you can't generate a whole
 page of all the links. This is a design decision rather than a
 limitation (IMHO)...


Thanks
------
 Thanks to David S. Cargo for suggesting the contents page, private links and
 INCLUDE and Paul Leclerc for the description line.


Data Structure
--------------
 The following Perl fragment demonstrates the internal format of the
 bookmarks - mainly documentation for documentation's sake ;-)...

%root = (
    name     => 'My bookmarks',
    url      => 'index.html',
    sections => [
      { name     => 'Misc.',
        url      => 'root_misc.html',
        links    => [
          { name     => 'Slashdot',
            desc     => 'News For Nerds. Stuff that Matters.'
            url      => 'http://slashdot.org/index.pl', },
        ],
      },
      { name     => 'Other...',
        url      => 'root_other.html',
        links    => [
          { name     => 'The Register',
            url      => 'http://www.theregister.co.uk/', },
        ],
      },
    ],
    links    => [
      { name     => 'Home',
        url      => 'http://www.eh.org/~jaffa/', },
      { name     => 'Jaffa Software',
        url      => 'http://www.jaffasoft.co.uk/', },
    ],
);
