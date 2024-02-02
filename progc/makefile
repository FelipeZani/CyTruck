all: compile

compile:
	gcc tflag.c -o tflag
	gcc -I. sflag.c sflag_tools.c -o sflag
run:
	./tflag ../temp/temp_tflag_1.csv ../temp/temp_tflag_2.csv

clean:
	rm tflag
	rm sflag
	
