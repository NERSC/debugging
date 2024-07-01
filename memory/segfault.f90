      program segfault
!...  A buggy code prepared for a debugger tutorial by NERSC
      use mpi
      integer, parameter :: n = 10
      real, pointer :: a(:) => null()
      real, pointer :: b(:) => null()
      real, pointer :: c(:) => null()
      integer me, i, ierr
      call mpi_init(ierr)
      call mpi_comm_rank(mpi_comm_world,me,ierr)
!     allocate (a(n), b(n), c(n))  ! Oops, forgot to allocate...
      call sub(a,b,c,n)
      print *, sum(c)
      deallocate (a, b, c)
      call mpi_finalize(ierr)
      end

      subroutine sub(a,b,c,n)
      integer n
      real a(n), b(n), c(n)
      call random_number(a)
      call random_number(b)
      do i=1,n
         c(i) = cos(a(i)) * sin(b(i))
      end do
      end
