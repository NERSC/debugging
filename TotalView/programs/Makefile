SRC=../src
CXX=g++
OMP_CC=gcc
OMP_F77=gfortran
OMP_OPT=-fopenmp
CC=gcc
F90=gfortran
MPICC=cc
MPICXX=CC
CFLAGS= -g
CCFLAGS= -g
F90FLAGS= -g
MPIFLAGS=-DUSEMPI -DMPICH_IGNORE_CXX_SEEK




PROGRAMS= simple combined demoMpi_v2 filterapp TVscript_demo ReplayEngine_demo simple_threaded  


all: ${PROGRAMS} 

simple: ${SRC}/simple.c ${SRC}/array.c ${SRC}/array.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC)/simple.c $(SRC)/array.c -o $@ -lm

simple_threaded: ${SRC}/simple_threaded.c 
	$(CXX) $(CCFLAGS) $(LDFLAGS) $(SRC)/simple_threaded.c -o $@ -lpthread

combined: ${SRC}/combined.cxx
	$(CXX) $(CCFLAGS) $(LDFLAGS) $(SRC)/combined.cxx -lpthread -o $@

demoMpi: $(SRC)/demoMpi.C
	$(MPICXX) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) $(SRC)/demoMpi.C -o $@

filterapp-mpi: $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx $(SRC)/myClassB.hxx $(SRC)/myClassA.hxx
	$(MPICXX) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@ $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx

memory-mpi: $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx $(SRC)/myClassB.hxx $(SRC)/myClassA.hxx
	$(MPICXX) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@ $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx

memory-comp: $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx $(SRC)/myClassB.hxx $(SRC)/myClassA.hxx
	$(MPICXX) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@ $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx

memory-redzone: $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx $(SRC)/myClassB.hxx $(SRC)/myClassA.hxx
	$(MPICXX) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@ $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx

demoMpi_v2: $(SRC)/demoMpi_v2.C
	$(MPICXX) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) $(SRC)/demoMpi_v2.C -o $@

MPI_Replay_Engine_demo: $(SRC)/MPI_Replay_Engine_demo.C $(SRC)/merge.h
	$(MPICXX) $(CCFLAGS) -I$(SRC) $(MPIFLAGS) $(LDFLAGS) $(SRC)/MPI_Replay_Engine_demo.C -o $@

filterapp: $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx $(SRC)/myClassB.hxx $(SRC)/myClassA.hxx
	$(CXX) $(CCFLAGS) $(LDFLAGS) -o $@ $(SRC)/main.cxx $(SRC)/myClassA.cxx $(SRC)/myClassB.cxx

f90_demo: $(SRC)/f90_demo.f

f90_demo: $(SRC)/f90_demo.f
	$(F90) $(F90FLAGS)  $(LDFLAGS) -o $@ $(SRC)/f90_demo.f

springs : $(SRC)/omp-springs.c
	${CC} ${CFLAGS} ${LD_FLAGS} $(SRC)/springs.c -o springs -lm

tx_omp_c_llnl3 : $(SRC)/tx_omp_c_llnl3.c
	${OMP_CC} ${CFLAGS} ${OMP_OPT} ${LD_FLAGS} $(SRC)/tx_omp_c_llnl3.c -o tx_omp_c_llnl3 -lm

omp-springs : $(SRC)/omp-springs.c
	${OMP_CC} ${CFLAGS} ${OMP_OPT} ${LD_FLAGS} $(SRC)/omp-springs.c -o omp-springs -lm

omp-springs-fort : $(SRC)/omp-springs.f
	${OMP_F77} ${F90FLAGS} ${OMP_OPT} ${LD_FLAGS} $(SRC)/omp-springs.f -o omp-springs-fort -lm

txdining: $(SRC)/txdining.cxx $(SRC)/txdining.hxx
	$(CXX) $(CCFLAGS) $(LDFLAGS) $(SRC)/txdining.cxx -o $@ -lpthread -lrt

ReplayEngine_demo: $(SRC)/ReplayEngine_demo.cxx
	$(CXX) $(CCFLAGS) $(LDFLAGS) $(SRC)/ReplayEngine_demo.cxx -o $@

RedZone_demo: $(SRC)/RedZone_demo.cxx
	$(CXX) $(CCFLAGS) $(LDFLAGS) $(SRC)/RedZone_demo.cxx -o $@

TVscript_demo: $(SRC)/TVscript_demo.c
	$(MPICC) $(CCFLAGS) $(MPIFLAGS) $(LDFLAGS) $(SRC)/TVscript_demo.c -o $@ -lm 

sudoku: ${SRC}/sudoku.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC)/sudoku.c -o $@ -lm

mem_example: ${SRC}/mem_example.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(SRC)/mem_example.cpp -o $@ -lm


cppview_demo:	${SRC}/cppview_demo.cxx ${SRC}/tv_data_display.c  ${SRC}/tv_data_display.h
	$(CXX) $(CCFLAGS) $(LDFLAGS) ${SRC}/cppview_demo.cxx ${SRC}/tv_data_display.c -I${SRC} -o $@

threads: ${SRC}/threads.cxx
	$(CXX) $(CCFLAGS) $(LDFLAGS) -lpthread $(SRC)/threads.cxx  -o $@



clean:
	rm -f $(PROGRAMS) *.o

cuda-clean:
	rm -f $(CUDA_PROGRAMS) *.o
