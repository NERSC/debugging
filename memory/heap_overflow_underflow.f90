      program heap_overflow_underflow
!...  A buggy code prepared for a debugger tutorial by NERSC
      use mpi
      integer, parameter :: n = 1024
      integer, parameter :: ouf = 8
      real, allocatable :: a(:), b(:)
      integer i, ierr
      call mpi_init(ierr)
      allocate (a(n), b(n))
      call random_number(a)
      b = cos(a)
      b(1)     = cos(a(1-ouf))  ! read underflow
      b(1-ouf) = cos(a(1))      ! write underflow
      b(n) =     cos(a(n+ouf))  ! read overflow
      b(n+ouf) = cos(a(n))      ! write overflow
      print *, sum(b)
      deallocate (a, b)
      call mpi_finalize(ierr)
      end
