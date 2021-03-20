
# Parallel Flags and Sequential Flags for compiling
PFLAGS = -O3 -Wall
SFLAGS = -O3 -Wall 

# Compiling files
FUNC = functions.c read_input.c

# Sequential file names
MAINSEQ = falsi_seq.c
OUTSEQ = ./falsi_seq

# Parallel file names
MAINP = falsi_par.c
OUTP = ./falsi_par


# Flags for run
RUNFLAGS = -n

list:
	./comand_list.sh

install:
	sudo apt install mpich

compile: 
	mpicc $(PFLAGS) $(FUNC) $(MAINP) -o $(OUTP) -lm

compile-seq:
	gcc $(SFLAGS) $(MAINSEQ) $(FUNC) -o $(OUTSEQ) -lm

run:
	mpiexec $(RUNFLAGS) 2 $(OUTP) -x0 -15000 -x1 15000 -p 0.001 -f exp

run-seq:
	$(OUTSEQ) -x0 -15000 -x1 15000 -p 0.001 -f sin

clean:
	rm $(OUTSEQ) $(OUTP) 2>/dev/null

