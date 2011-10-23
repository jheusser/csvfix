# winpack.bash
# create source package for windows
#
# must be run from csved root

pause()
{
	echo $*
	echo "Hit [enter] to continue"
	read x
}

error()
{
	pause $*
	exit 1
}

MYNAME=csvfix
OUTDIR=c:/tmp/win_$MYNAME
if [ -f $OUTDIR ]
then
	error "Output directory $OUTDIR is a file!"
fi

if [ -d $OUTDIR ]
then
	echo "About to remove $OUTDIR"
	echo "Hit Ctrl-C to abort, [enter] to continue"
	read stuff
	rm -rf $OUTDIR
fi

mkdir $OUTDIR
mkdir $OUTDIR/doc

cp bin/*.exe $OUTDIR
cp doc/readme/windows.txt $OUTDIR/readme.txt
cp doc/hm/html/*.* $OUTDIR/doc

pause "Completed - package is in $OUTDIR"

