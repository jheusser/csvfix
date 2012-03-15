default:
	@echo "use 'make win' or 'make lin'"

win:
	-mkdir alib\\obj alib\\lib csvfix\\obj csvfix\\bin
	cd alib && $(MAKE) win
	cd csvfix && $(MAKE) win

lin:
	mkdir -p alib/obj alib/lib csvfix/obj csvfix/bin
	cd alib; $(MAKE) lin
	cd csvfix; $(MAKE) lin

clean:
	cd alib; $(MAKE) clean
	cd csvfix; $(MAKE) clean
