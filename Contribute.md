CSVfix is an open source project, which means that you get the source code for it, but it also means that you can contribute effort to it. This article outlines how to do that, and gives some suggestions for ways you can contribute.

Firstly, you will need to [install Mercurial](http://mercurial.selenic.com/wiki/Download), and learn how to use it. A good, quick tutorial on the subject is [here](http://hginit.com/01.html). When you have done that, you can clone a copy of the CSVfix source code from here on Google Code or from the [Bitbucket](https://bitbucket.org/neilb/csvfix) copy of the code, and then pull it down onto your own machine.

If you intend to contribute C++ code, you will also need a C++ compiler. CSVfix is built on Windows and Linux using the GCC compiler suite, so you will need a copy of that. On Linux, you should already have it, on Windows instructions for installing it are [here](http://latedev.wordpress.com/2011/06/20/how-to-install-a-c-compiler-on-windows).

Once you have your development environment set up, you can start thinking about what to add or change. If you don't have an idea for a particular improvement yourself, here are a few suggestions:

  * Take a look at the Issue Tracker and the [support group](http://groups.google.com/group/csvfix) for suggestions that I for one reason or another didn't implement, and for other ideas.

  * CSVfix error messages could always be improved.

  * The csved\_strings.h header is a dependency for all CSVfix commands - it would be nice to split this up somehow.

  * More tests would always be welcome - you don't have to be a C++ programmer to write them - look in the csvfix/tests directory.

  * The help still has many spelling mistakes, and would benefit from more examples. Once again, you don't need to be a C++ programmer to contribute to the documentation, but you will need a copy of the (free) Windows program [HelpNDoc](http://www.helpndoc.com/) to edit the help source.

  * The makefiles need proper dependencies adding to them. And maybe it would be nice to use CMake.


There are lots of other things that need fixing. Once you have fixed, added or improved something, you need to push the code back to Google or Bitbucket and let me know that you have changes - the best way to to this is to post in the [support group](http://groups.google.com/group/csvfix) so that all interested parties can see it. If the changes look good, I'll then pull them into the official CSVfix source tree.

Any questions on how to contribute? Please post in the support group.