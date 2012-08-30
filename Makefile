ifndef COMSPEC
	CLEAN =	(cd alib; $(MAKE) clean) && (cd csvfix; $(MAKE) clean) 
else
	CLEAN = cmd.exe /c "clean.cmd"
endif

default:
	@echo "use 'make win' or 'make lin'"

win:
	cmd.exe /c "mkdirs.cmd"
	cd alib && $(MAKE) win
	cd csvfix && $(MAKE) win

lin:
	mkdir -p alib/obj alib/lib csvfix/obj csvfix/bin
	cd alib; $(MAKE) lin
	cd csvfix; $(MAKE) lin

clean:
	$(CLEAN)
