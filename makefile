all: compile run

compile:
	gcc tflag.c -o tflag

run:
	./tflag ../temp/temp_tflag_1.csv
