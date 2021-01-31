
# Qui andranno varie opzioni per compilare i file
PFLAGS = -O3 -Wall
SFLAGS = -Wall 

# Qui andranno i file .c da compilare con il relativo nome di output
FUNC = functions.c

MAINSEQ = falsi_seq.c
OUTSEQ = falsi_seq

MAINP = falsi_par.c
OUTP = falsi_par


# Qui vanno inseriti i flag per il run 
# Ad esempio se si vogliono usare più processori di quelli disponibili --oversubscribe -n numero_dei_processi
RUNFLAGS = --oversubscribe -n 

list:
	./comand_list.sh

install:
	sudo apt install mpich

compile: 
	mpicc $(PFLAGS) $(MAINP) $(FUNC) -o $(MAINO) -lm

compile-seq:
	gcc $(SFLAGS) $(MAINSEQ) $(FUNC) -o $(OUTSEQ) -lm -ggdb

run-seq-pres:
	./$(OUTSEQ) -x0 -0 -x1 3 -e 0.0003 -f function.txt
	
run: 
	mpiexec $(RUNFLAGS) $(N) $(MAINO) $(PARAMS)

run-pres:
	mpiexec $(RUNFLAGS) 5 $(MAINO)

clean:
	rm $(OUTSEQ) $(OUTP) 2>/dev/null

