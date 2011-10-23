#!bash

MYTH_TESTS=1
export MYTH_TESTS
if bin/Debug/test.exe > /dev/null
then
	swine msgbox -caption 'ALib Smoke Test' -text 'PASSED  ' -icon info
else
	swine msgbox -caption 'ALib Smoke Test' -text 'FAILED' -icon stop
fi
