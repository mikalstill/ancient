# Spec file for the Panda PDF generator
Summary: A GPL'ed PDF generation API
Name: panda
Version: 0.2
Release: 1
Copyright: GPL
Group: Console/Development
Source: http://www.stillhq.com/panda/source/panda_0_2.tgz
URL: http://www.stillhq.com/panda/
Vendor: Michael Still
Packager: Michael Still

%description
Panda is a GPL'ed PDF generation API. It may be used for building PDFs on the fly for the web, or for making PDFs for another purpose.

%prep
%setup

%build
./configure
make lib

%install
make install

%files
%doc CHANGELOG
%doc COPYING
%doc TODO
/usr/local/lib/libpanda.a
/usr/local/man/man3/checkinfo.3
/usr/local/man/man3/initpanda.3
/usr/local/man/man3/makedate.3
/usr/local/man/man3/nowdate.3
/usr/local/man/man3/panda-textmodes.3
/usr/local/man/man3/panda-xobjectmodes.3
/usr/local/man/man3/pdfclose.3
/usr/local/man/man3/pdfopen.3
/usr/local/man/man3/pdfpage.3
/usr/local/man/man3/setauthor.3
/usr/local/man/man3/setcharacterspacing.3
/usr/local/man/man3/setcreator.3
/usr/local/man/man3/setfont.3
/usr/local/man/man3/setfontmode.3
/usr/local/man/man3/setfontsize.3
/usr/local/man/man3/sethorizontalscaling.3
/usr/local/man/man3/setkeywords.3
/usr/local/man/man3/setleading.3
/usr/local/man/man3/setsubject.3
/usr/local/man/man3/settitle.3
/usr/local/man/man3/setwordspacing.3
