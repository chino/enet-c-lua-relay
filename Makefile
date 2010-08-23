all:
	cd c; make; cd ..
	cd lua; make; cd ..

clean:
	cd c; make clean; cd ..
	cd lua; make clean; cd ..

.PHONY: all clean
