
# Qui andranno varie opzioni per compilare i file
PFLAGS = -O3 -Wall
SFLAGS = -Wall 

# Qui andranno i file .c da compilare con il relativo nome di output
FUNC = functions.c read_input.c

MAINSEQ = falsi_seq.c
OUTSEQ = ./falsi_seq

MAINP = falsi_par.c
OUTP = ./falsi_par


# Qui vanno inseriti i flag per il run 
# Ad esempio se si vogliono usare più processori di quelli disponibili -n numero_dei_processi
RUNFLAGS = -n

list:
	./comand_list.sh

install:
	sudo apt install mpich

compile: 
	mpicc $(PFLAGS) $(FUNC) $(MAINP) -o $(OUTP) -lm

compile-seq:
	gcc $(SFLAGS) $(MAINSEQ) $(FUNC) -o $(OUTSEQ) -lm -ggdb

run-seq-pres:
	./$(OUTSEQ) -x0 -1 -x1 3 -ie 0.1 -fe 0.000001 -f exp
	
run: 
	mpiexec $(RUNFLAGS) $(N) $(OUTP) $(PARAMS)

run-pres:
	mpiexec $(RUNFLAGS) 4 $(OUTP) -a -fe 0.0001 -f exp

clean:
	rm $(OUTSEQ) $(OUTP) 2>/dev/null

