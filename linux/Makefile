## Kleber Garcia (c) 2012

TARGETS = HotQ ColdSdk WarmLib

default : $(TARGETS) tests examples

clean :
	rm -rf build/*
	cd WarmLib/HotQ; make clean TARGET=HotQ;
	cd WarmLib/ColdSdk; make clean TARGET=ColdSdk;
	cd WarmLib/WarmLib; make clean TARGET=WarmLib;
	
HotQ : 
	cd WarmLib/$@; make $@ TARGET=$@;

ColdSdk :
	cd WarmLib/$@; make $@ TARGET=$@;

WarmLib : HotQ ColdSdk
	cd WarmLib/$@; make $@ TARGET=$@

tests : $(TARGETS)
	cd WarmLib/Tests; make Tests;

examples : $(TARGETS)
	cd Examples; make examples;
