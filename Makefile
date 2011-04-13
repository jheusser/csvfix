default:
	@echo "use 'make win' or 'make lin'"

win:
	cd alib; make win
	cd csvfix; make win

lin:
	cd alib; make lin
	cd csvfix; make lin

clean:
	cd alib; make clean
	cd csvfix; make clean
