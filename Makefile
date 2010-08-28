dirs=`find */* -name 'Makefile' | xargs -n 1 dirname`
make=for dir in $(dirs); do cd $$dir; make $(1); cd -; done

all:
	$(call make,all)

clean:
	$(call make,clean)

.PHONY: all clean
