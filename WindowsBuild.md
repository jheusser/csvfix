## Introduction ##

CSVfix comes as a pre-built binary installer for Windows, but you may want to try building it yourself using code from the Mercurial repository. Why would you want to bother? Well, bug fixes and new features are added to the repository code long before I get round to creating a new release, complete with installer. also, you might want to add features or fix bugs yourself. This article details how to get together all the bits you need to build the Windows version of CSVfix.

## Get TDM GCC ##

CSVfix is built using the TDM distribution of the GNU Compiler Collection. I have a blog article describing exactly how to get it and install it [here](http://latedev.wordpress.com/2011/06/20/how-to-install-a-c-compiler-on-windows). Installing GCC gives you the C++ compiler, the make utility and various other programming tools.

## Get The Code ##

You can get the latest version of CSVfix by cloning  the Mercurial repository, or simply by downloading a zip from Bitbucket. The latter is the simplest course, particularly if you don't want to install Mercurial. Just go to the clone of CSVfix on Bitbucket at https://bitbucket.org/neilb/csvfix/downloads and choose "zip" from the "Tags" tab. This will download a zip containing the current source (or the revision you selected), which you will need to unzip somewhere in your Windows home directory tree to do the build - for example, I use C:\users\neilb\devel. To follow the rest of this tutorial, name the the root of the unzipped tree "mycsvfix".

## Get Mercurial (optional) ##

In order to retrieve the code from the Mercurial archive, you will need [download the Mercurial command line client](http://mercurial.selenic.com/wiki/Download). Make sure that the hg command has been added to your PATH variable by opening a new command line prompt, and typing the "hg" command.

## Get Code From Repository (optional) ##

In a Windows command line window, create a working directory somewhere that you have permissions to read and write, and cd into that directory. The directory should be somewhere in your home directory tree - for example, I use C:\users\neilb\devel. Then check out the code by issuing this command:

```
    hg clone https://nbutterworth1953@code.google.com/p/csvfix/  mycsvfix
```

This will clone a copy of the Mercurial repository in a directory called mycsvfix.

## Build the code ##

To build the code, cd into the mycsvfix directory you just unzipped (or cloned), and enter the command:

```
    mingw32-make win
```

This will start the build process. Note that the MinGW make utility is given that strange name to avoid clashes with other versions of make you may have hanging around.

The build goes through four stages:

  * First, the sources in the alib utility directory are compiled
  * Then the alib static library is created
  * Then the CSVfix sources are compiled
  * Lastly, everything is linked to create the CSVfix executable

The executable will be created in the mycsvfix\csvfix\bin directory, and will be called csvfix.exe. Congratulations! You have now build CSVfix.