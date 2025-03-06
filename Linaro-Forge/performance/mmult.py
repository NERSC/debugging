# ===============================================================================
# Copyright (C) March 2023 - Linaro Limited (or its affiliates). All rights reserved.
# Copyright (C) Arm Limited, 2019-2023 All rights reserved.
# The example code is provided to you as an aid to learning when working
# with Linaro Forge, including but not limited to programming tutorials.
# Linaro hereby grants to you, subject to the terms and conditions of this Licence,
# a non-exclusive, non-transferable, non-sub-licensable, free-of-charge licence,
# to use and copy the Software solely for the purpose of demonstration and
# evaluation.
# You accept that the Software has not been tested by Linaro therefore the Software
# is provided “as is”, without warranty of any kind, express or implied. In no
# event shall the authors or copyright holders be liable for any claim, damages
# or other liability, whether in action or contract, tort or otherwise, arising
# from, out of or in connection with the Software or the use of Software.
# ===============================================================================

#!/usr/bin/env python
from __future__ import division
from __future__ import print_function
from __future__ import with_statement



import argparse
import ctypes
import os
import sys
from ctypes import c_int

import numpy
import mpi4py

from mpi4py import MPI
from numpy.ctypeslib import ndpointer
from scipy.linalg import blas


# Enable MPI SINGLE thread
mpi4py.rc.threaded = False
mpi4py.rc.thread_level = "single"





# Check if C kernel has been compiled
try:
    open("libmmult_c.so", 'r')
except FileNotFoundError:
    print("C kernel not found. Please run 'make -f mmult_py.makefile' "
          "to compile it before running this script")
    sys.exit(1)

# Check and load F90 kernel
sys.path.insert(0, '.')
try:
    import libmmult_f
except ImportError:
    print("F90 kernel not found. Please run 'make -f mmult_py.makefile' "
          "to compile it before running this script")
    sys.exit(1)

# Load C kernel
C_MMULT_LIB = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "libmmult_c.so"))

# Declare ctype for ndarray pointer
arr_ptr_t_c = ndpointer(dtype=numpy.float64, ndim=1, flags='C')
C_MMULT_LIB.mmult.argtypes = [c_int, c_int, arr_ptr_t_c, arr_ptr_t_c, arr_ptr_t_c]
C_MMULT_LIB.mmult.restype = None

DEFAULT_SIZE = 64
DEFAULT_FN = "res_Py.mat"
DEFAULT_KERNEL = "C"
SOLVER_CHOICES = ["C", "F90", "Py"]


def minit(sz, fortran_style_array_order, A):
    for i in range(0, sz):
        for j in range(0, sz):
            if fortran_style_array_order:
                A[i, j] = i*(j+1)
            else:
                A[i*sz+j] = i*(j+1)


def mwrite(A, fn):
    f = open(fn, "w")
    A.tofile(f, sep="\t", format="%g")
    f.close()


def main(sz, kernel, filename):
    intercomm = MPI.Comm.Get_parent()

    comm = MPI.COMM_WORLD
    nproc = comm.size
    mr = comm.rank

    if mr == 0:
        print("-------------------------------------------------------------------\n"
              "This program contains an intentional bug. See the 'Worked Examples'\n"
              "section of the Linaro Forge user guide for more information:\n"
              "https://docs.linaroforge.com/latest/html/forge/index.html or\n"
              "../doc/userguide-forge.pdf\n"
              "-------------------------------------------------------------------\n")

    remainder = sz%nproc

    if remainder > 0:
        if mr == 0:
            print("{}: Info: reducing SIZE {} to {} to be a multiple of number of "
                  "processes ({})".format(mr, sz, sz-remainder, nproc))
        sz = sz-remainder

    mslice = int(sz*sz/nproc)
    mslice_r = int(sz/nproc)

    fortran_style_array_order = kernel in ('F90', 'Py')

    if kernel == "F90":
        print("Skipping due to the Fortran function call ommiting some call arguments in Python")
        sys.exit(0)
    
    if mr == 0:
        print("{rank}: Size of the matrices: {size}x{size}".format(rank=mr, size=sz))
        print("{}: Kernel: {}".format(mr, kernel))

    if mr == 0:
        if fortran_style_array_order:
            mat_a = numpy.ndarray(shape=(sz, sz), dtype='d', order='F')
            mat_b = numpy.ndarray(shape=(sz, sz), dtype='d', order='F')
            mat_c = numpy.ndarray(shape=(sz, sz), dtype='d', order='F')
        else:
            mat_a = numpy.ndarray(shape=(sz*sz), dtype='d', order='C')
            mat_b = numpy.ndarray(shape=(sz*sz), dtype='d', order='C')
            mat_c = numpy.ndarray(shape=(sz*sz), dtype='d', order='C')

        print("{}: Initializing matrices...".format(mr))
        minit(sz, fortran_style_array_order, mat_a)
        minit(sz, fortran_style_array_order, mat_b)
        minit(sz, fortran_style_array_order, mat_c)

        print("{}: Sending matrices".format(mr))
        for i in range(1, nproc):
            # Get a slice from the mat_a and mat_c matrix
            if fortran_style_array_order:
                mat_a_slice = mat_c[:, i*mslice_r:(i+1)*mslice_r]
                mat_c_slice = mat_c[:, i*mslice_r:(i+1)*mslice_r]
            else:
                mat_a_slice = mat_a[i*mslice:(i+1)*mslice]
                mat_c_slice = mat_c[i*mslice:(i+1)*mslice]
            comm.send(mat_a_slice, dest=i, tag=i)
            comm.send(mat_b, dest=i, tag=100+i)
            comm.send(mat_c_slice, dest=i, tag=200+i)
    else:
        print("{}: Receiving matrices".format(mr))
        if fortran_style_array_order:
            mat_a = numpy.ndarray(shape=(sz, mslice_r), dtype='d', order='F')
            mat_b = numpy.ndarray(shape=(sz, sz), dtype='d', order='F')
            mat_c = numpy.ndarray(shape=(sz, mslice_r), dtype='d', order='F')
        else:
            mat_a = numpy.ndarray(shape=(mslice), dtype='d', order='C')
            mat_b = numpy.ndarray(shape=(sz*sz), dtype='d', order='C')
            mat_c = numpy.ndarray(shape=(mslice), dtype='d', order='C')

        mat_a = comm.recv(source=0, tag=mr)
        mat_b = comm.recv(source=0, tag=100+mr)
        mat_c = comm.recv(source=0, tag=200+mr)

    # Processing
    print("{}: Processing..".format(mr))
    if kernel == "F90":
        # f2py makes sz parameter optional
        libmmult_f.mmult(nproc, mat_a, mat_b, mat_c)

    elif kernel == "Py":
        mat_c = blas.dgemm(alpha=1.0, a=mat_b, b=mat_a, beta=1.0, c=mat_c,
                           overwrite_c=True, trans_b=False)
    else:
        C_MMULT_LIB.mmult(sz, nproc, mat_a, mat_b, mat_c)

    if mr == 0:
        print("{}: Receiving result matrix...".format(mr))
        for i in range(1, nproc):
            if fortran_style_array_order:
                mat_c[:, i*mslice_r:(i+1)*mslice_r] = comm.recv(source=i, tag=500+i)
            else:
                mat_c[i*mslice:(i+1)*mslice] = comm.recv(source=i, tag=500+i)
    else:
        print("{}: Sending result matrix...".format(mr))
        comm.send(mat_c, dest=0, tag=500+mr)

    # Writing result
    if mr == 0:
        mwrite(mat_c, filename)

    if mr == 0:
        print("{}: Done".format(mr))

    if intercomm != MPI.COMM_NULL:
        intercomm.Barrier()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Matrix product.")
    parser.add_argument("-k", dest="kernel", metavar="KERNEL", action="store", type=str,
                        help=("Solver. Options: [%s] (default is C)"
                              % "|".join(SOLVER_CHOICES)),
                        choices=SOLVER_CHOICES, default=DEFAULT_KERNEL)
    parser.add_argument("-s", dest="mat_size", metavar="SIZE", action="store", type=int,
                        help=("size of the matrix to compute (default is %d)"
                              % (DEFAULT_SIZE)),
                        default=DEFAULT_SIZE)
    parser.add_argument("-o", dest="fn", metavar="FILENAME", action="store", type=str,
                        help=("output matrix file name (default is %s)" % DEFAULT_FN),
                        default=DEFAULT_FN)

    args = parser.parse_args()

    main(args.mat_size, args.kernel, args.fn)
