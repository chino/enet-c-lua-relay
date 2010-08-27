DIRS=`find */* -name 'Makefile' | xargs -n 1 dirname`

all:
	for dir in $(DIRS); do cd $$dir; make; cd -; done

clean:
	for dir in $(DIRS); do cd $$dir; make clean; cd -; done

.PHONY: all clean
