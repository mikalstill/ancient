Summary: GIF-manipulation library
Name: giflib
Version: 4.1.0
Release: 1
Copyright: MIT
Group: Libraries
Source0: ftp://prtr-13.ucsc.edu/pub/libungif/giflib-%{version}.tar.gz
URL: http://prtr-13.ucsc.edu/~badger/software/giflib.shtml
BuildRoot: /var/tmp/giflib-root
Prefix: /usr

%description
GIF loading and saving shared library.

IMPORTANT!  Please read /usr/doc/PATENT_PROBLEMS for information on
why using this library may put you at legal risk and what
alternatives you may have.

%package devel
Summary: GIF-manipulation library header files, static libraries, and documentation. 
Group: Development/Libraries
%description devel
Libraries and headers needed for developing programs that use giflib
to load and save gif image files.

IMPORTANT!  Please read /usr/doc/PATENT_PROBLEMS for information on
why using this library may put you at legal risk and what
alternatives you may have.

%package progs
Summary: Programs for converting and transforming gif images.
Group: Applications/Graphics
%description progs
This package contains various programs for manipulating gif image files.

IMPORTANT!  Please read /usr/doc/PATENT_PROBLEMS for information on
why using this library may put you at legal risk and what
alternatives you may have.

%prep
%setup

%build
libtoolize --copy --force
CFLAGS="${RPM_OPT_FLAGS}" ./configure --prefix=/usr
make all

%install
rm -rf ${RPM_BUILD_ROOT}
make DESTDIR="${RPM_BUILD_ROOT}" install-strip

{
    cd ${RPM_BUILD_ROOT}/usr/lib
    chmod +x libgif.so.%{version}
}

%clean
rm -rf ${RPM_BUILD_ROOT}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(- root root)
%doc COPYING README PATENT_PROBLEMS NEWS ONEWS
%doc ChangeLog TODO BUGS AUTHORS
/usr/lib/lib*.so.*

%files devel
%defattr(- root root)
%doc doc/*
%doc util/giffiltr.c
%doc util/gifspnge.c
/usr/lib/lib*.a
/usr/lib/lib*.so
/usr/lib/lib*.la
/usr/include/gif_lib.h

%files progs
%attr(0755 root root) /usr/bin/*

%changelog
* Tue Jan 19 1999 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Upgrade to version 4.1
  + Fix a few minor memory leaks in error conditions.
  + Add a new function EGifOpen( void *userData, OutputFunc writeFunc) that
    allows user specified gif writing function.
- Merge spec file from libungif-3.1-5

* Fri Dec 17 1998 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Add note to read PATENT_PROBLEMS.

* Mon Dec 14 1998 Toshio Kuratomi <badger@prtr-13.ucsc.edu>
- Upgrade to version 4.0 (Fixes rather nasty behaviour when dealing with
  Extensions.)
