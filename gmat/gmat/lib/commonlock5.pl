# -*- Perl -*-
#
# File locking library
#
# $Id: commonlock5.pl,v 1.1.1.1 2002-07-15 21:33:31 root Exp $
#
#
######################################################################

package commonlock;

%LOCKS = ();

END {
    &main::unset_all_locks();
}

sub main::set_lock {
    my($lockfile, $extra) = @_;
    my($dev,$ino,$mode,$nlink,$rdev,$size,
       $atime,$mtime,$ctime,$blksize,$blocks);
    my($name,$passwd,$uid,$gid,
       $quota,$comment,$gcos,$dir,$shell);
    my($rc);
    local($_);

    warn "EXTRA ARGUMENTS TO set_lock: $extra\n" if defined($extra);

    if (-f $lockfile) {
	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat($lockfile);
	
	($name,$passwd,$uid,$gid,
	 $quota,$comment,$gcos,$dir,$shell) = getpwuid($uid);

	print STDERR "Obtaining lock...waiting for $name...";
    }

    $rc = system ("lockfile -l 86400 $lockfile");

    if (!$rc) {
	$LOCKS{$lockfile} = 1;
    }

    $rc / 256;
}

sub main::unset_lock {
    my($lockfile, $extra) = @_;
    my($rc);

    warn "EXTRA ARGUMENTS TO unset_lock: $extra\n" if defined($extra);

    $rc = system ("rm -f $lockfile");

    if (!$rc) {
	delete $LOCKS{$lockfile};
    }

    $rc / 256;
}

sub main::unset_delete_lock {
    &main::unset_lock(@_);
}

sub main::unset_all_locks {
    my($lock, @locks);

    @locks = keys %LOCKS;

    foreach $lock (@locks) {
	print STDERR "Releasing $lock ...\n" if $main::VERBOSE;
	&main::unset_lock($lock);
    }
}

1;
