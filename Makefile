default:
	@echo "use 'make win' or 'make lin'"

win:
	mkdir -p alib/obj alib/lib csvfix/obj csvfix/bin
	cd alib; make win
	cd csvfix; make win

lin:
	mkdir -p alib/obj alib/lib csvfix/obj csvfix/bin
	cd alib; make lin
	cd csvfix; make lin

clean:
	cd alib; make clean
	cd csvfix; make clean
