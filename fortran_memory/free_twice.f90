      program free_twice
!...  A buggy code prepared for a debugger tutorial by NERSC
      use mpi
      integer, parameter :: n = 1024
      real, allocatable :: a(:), b(:)
      integer i, ierr
      call mpi_init(ierr)
      allocate (a(n), b(n))
      call random_number(a)
      b = cos(a)
      deallocate (a)
      print *, sum(b)
      deallocate (a,b)  ! Oops..., deallocating 'a' again
      call mpi_finalize(ierr)
      end
