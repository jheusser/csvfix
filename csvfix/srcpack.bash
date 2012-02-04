#!bash

# srcpack.bash
#
# packge alib & csvfix sources for upload
# Copyright (C) 2008 Neil Butterworth

# report error & exit
error()
{
	echo $*
	exit 1
}


# create output root directory
# root must not exist to ensure we don't overwrite anything important
make_outdir()
{
	if [ "$OUTDIR" = "" ]
	then
		error "No output directory specified"
	fi

	if [ -f "$OUTDIR" ]
	then
		error "A file named $OUTDIR already exists"
	fi

	if [ -d "$OUTDIR" ]
	then
		error "A directory named $OUTDIR already exists"
	fi

	if mkdir $1 
	then
		echo "Created output directory $1"
	else
		error "Could not create directory $1"
	fi
}

# create the csvfix dir structire & copy files into it
make_csvfix()
{
	echo "creating csvfix..."
	mkdir $1/csvfix
	mkdir $1/csvfix/inc
	mkdir $1/csvfix/src
	mkdir $1/csvfix/obj
	mkdir $1/csvfix/bin


	cp inc/*h $1/csvfix/inc
	cp src/*cpp $1/csvfix/src
	cp Makefile $1/csvfix
	cp -R test $1/csvfix
}

# create directories & files for alib library
make_alib()
{
	echo "creating alib..."
	mkdir $1/alib
	mkdir $1/alib/inc
	mkdir $1/alib/src
	mkdir $1/alib/obj
	mkdir $1/alib/expat
	mkdir $1/alib/lib

	cp ../alib/inc/*h $1/alib/inc
	cp ../alib/src/*cpp $1/alib/src
	cp ../alib/expat/*h $1/alib/expat
	cp ../alib/expat/*cpp $1/alib/expat
	cp ../alib/Makefile $1/alib
}

# check no SVN files
if [ -d '.svn' ]
then
	error "Need to run this command from SVN exported directory"
	exit 1
fi

# script parameter is root directory to create
OUTDIR="$1"

# this is the name of the makefile that will build alib & csvfix
MAKEALL="Makefile.srcpack"

# new root must not exist to avoid overwriting sources etc.

# create directories & contents
make_outdir "$OUTDIR"	
make_alib "$OUTDIR"
make_csvfix "$OUTDIR"

# copy misc files
cp "$MAKEALL" "$OUTDIR/Makefile"
cp LICENSE "$OUTDIR"

# that's it
echo "Completed output to $OUTDIR"
exit 0

