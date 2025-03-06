include common.makefile

# The MPI compiler commands (typically mpicc and mpif90) are autodetected
# by common.makefile. You can override by uncommenting the following:
#MPICC=
#MPIF90=

CFLAGS = -O2

targets = libmmult_c.so libmmult_f

.PHONY: all
all: $(targets)

libmmult_c.so: mmultlib.c
	$(MPICC) -std=c99 -fPIC -shared $(CFLAGS) $^ -o $@

.PHONY: libmmult_f
libmmult_f: mmultlib.f90
	f2py --opt="$(CFLAGS)" -c $^ -m $@

.PHONY: clean
clean:
	$(RM) libmmult_c.so libmmult_f*.so res*.mat

