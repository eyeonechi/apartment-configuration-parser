all: compile run

compile:
	gcc src/apartment.c -o bin/apartment -Wall

run:
	for i in $$(seq 0 6); \
	do \
		./bin/apartment < data/test$$i.txt; \
	done

clean:
	rm bin/*
