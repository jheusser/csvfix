  * **What is CSVfix?**

> CSVfix is a utility to mess with CSV files. Think of it as a  combination of sed, awk, grep and a few other tools that knows about the CSV format. This means that you can work naturally with CSV records and  fields without using horribly complex regular expressions.

  * **Which download do I need?**

> For Windows, you need the binary installer (unless you are planning on hacking on the source code), which gives you the Windows executable and the help in CHM format. For Linux and other Unix-like operating systems, you need the source code, which you will have to compile with the GCC compiler, plus the HTML version of the help files. You can get the source code as a zip from the CSVfix clone at https://bitbucket.org/neilb/csvfix/downloads#tag-downloads - simply choose your favourite zip format from the "Tags" tab.

  * **Where has the source zip gone?**

> Prior to version 1.3, I provided a zipped snapshot of the source for the version. I have decided to stop doing this, as it quickly got out of date. Instead, you can always get a zip of the latest and greatest code (or of  a specific version) from https://bitbucket.org/neilb/csvfix/downloads (click the "Tags" tab) - you don't need Mercurial installed to do this.

  * **Is CSVfix 32 or 64 bit software?**

> CSVfix is compiled and tested by me using only the 32-bit version of the GCC compiler on Windows and Linux. If you build your own version of CSVfix (which you have to do on Linux or a Mac) you may experience problems if you use a 64-bit compiler. An example of this is http://code.google.com/p/csvfix/issues/detail?id=47&can=1 in the issue tracker.

> This does not mean that you can't use CSVfix when compiled with 64-bit compilers, but only that you may encounter the odd problem. Please be aware of this and when reporting problems state if you are using a 64-bit compiler. As I fix such issues, they get updated in the project's Mercurial archive, from which you can check them out and try another compilation.

  * **Where is the on-line documentation?**

> Google Code doesn't really support this. As a convenience for people wanting to get an idea of what CSVfix can do, I have uploaded the HTML version of the manual to  a free hosting provider at http://csvfix.byethost5.com/csvfix15/csvfix.html. Bandwidth restrictions mean that you shouldn't use this as your main manual source - download the HTML version from the Downloads tab, or read the CHM version included with the Windows binary.

  * **What is CSV? Is there a standard for it?**

> CSV is shorthand for "comma-separated values". It is an industry-standard for data exchange that unhappily is not at all standardised. The nearest thing to a standard is the RFC at http://www.ietf.org/rfc/rfc4180.txt, and you may also want to take a look at a useful article at http://creativyst.com/Doc/Articles/CSV/CSV01.htm. CSVfix can cope with most things that label themselves as CSV, with judicious use of the -sep flag, which allows you to specify the inter-field separator.

  * **I have a problem with  CSVfix. Where do I get help?**

> In the support forum at http://groups.google.com/group/csvfix. Please post there **before** raising an issue in the issue tracker.

  * **How do I get the source code?**

> Either clone the Mercurial repository (see below) or get the code as  a zip from the clone at https://bitbucket.org/neilb/csvfix/downloads - simply choose the zip format you want from the "Tags" tab.

  * **How do I compile CSVfix?**

> Get the zipped source from https://bitbucket.org/neilb/csvfix, unzip it somewhere, then in  a command line shell (i.e. bash or cmd.exe) change into the directory you unzipped it into and type "make win" (for Windows) or "make lin" (for Linux and other Unix-like systems). See [this article](WindowsBuild.md) for full details of how to build on Windows.

> There are currently (September 2012) some issues compiling CSVfix 1.4 on the Mac - please see this discussion http://groups.google.com/group/csvfix/browse_thread/thread/33ec3e5f157c16dd in the support groups. Until this is resolved Mac users will need to stick with CSVfix 1.3.

  * **How do I compile the code in the Mercurial archive?**

> You will need to have Mercurial, make and GCC installed. To get the code, issue the command

> `hg clone https://csvfix.googlecode.com/hg/ mycsvfix`

> This will pull down the code and create  a directory called "mycsvfix". Change into this directory which contains two subdirectories called "alib" and "csvfix". Now issue the command "make win" or "make lin" depending on which platform you are building on - this will build the alib and the CSVfix executable, which will be placed in the bin directory of the csvfix subdirectory.

  * **Which compiler do I need?**

> CSVfix is developed on Windows using the GCC compiler provided by the excellent Twilight Dragon Media at http://tdm-gcc.tdragon.net. But you should be able to compile CSVfix  with any relatively recent version of GCC on either Windows, Linux or other Unix-like operating systems - the current release of CSVfix will compile with GCC 4.6.1. CSVfix does not use any external libraries such as Boost, except that the Windows version uses the built-in Windows support for ODBC. I have not tested compiling CSVfix with Microsoft C++, but don't see any reason why it wouldn't work.