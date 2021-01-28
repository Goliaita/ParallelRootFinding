
# Qui andranno varie opzioni per compilare i file
CFLAGS = -O3

# Qui andranno i file .c da compilare con il relativo nome di output
MAINC = main.c
MAINO = main


# Qui vanno inseriti i flag per il run 
# Ad esempio se si vogliono usare più processori di quelli disponibili --oversubscribe -n numero_dei_processi
RUNFLAGS = --oversubscribe -n 


install:
	sudo apt install openmpi-bin openmpi-common openssh-client openssh-server  libopenmpi-dev libomp-10-dev libomp5-10 libomp-dev

compile: 
	mpicc $(FLAGS) $(MAINC) -o $(MAINO)

compile-seq:
	gcc -Wall my_falsi.c -o my_falsi -lm -ggdb

run-seq:
	./my_falsi -x0 -1.5 -x1 2.3 -e 0.0003
	
run: 
	mpiexec $(RUNFLAGS) $(N) $(MAINO) $(PARAMS)

run_pres:
	mpiexec $(RUNFLAGS) 5 $(MAINO)

clean:
	rm main test my_falsi newton falsi_method 2>/dev/null
