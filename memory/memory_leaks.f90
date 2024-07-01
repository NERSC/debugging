      program memory_leaks

!...  Buggy code prepared by NERSC User Service Group for a debugging tutorial
!...  February, 2012

      use mpi
      implicit none
      integer, parameter :: n = 1000000
      real val
      integer i, ierr
      call mpi_init(ierr)
      val = 0.
      do i=1,10
         call sub_ok(val,n)
      end do
      do i=1,10
         call sub_bad(val,n)
      end do
      do i=1,10
         call sub_badx2(val,n)
      end do
      print *, val
      call mpi_finalize(ierr)
      end

      subroutine sub_ok(val,n)      ! no memory leak
      integer n
      real val
      real, allocatable :: a(:)
      allocate (a(n))
      call random_number(a)
      val = val + sum(a)
!     deallocate(a)                 ! ok not to deallocate
      end

      subroutine sub_bad(val,n)     ! memory leak of 4*n bytes per call
      integer n
      real val
      real, pointer :: a(:)
      allocate (a(n))
      call random_number(a)
      val = val + sum(a)
!     deallocate(a)                 ! not ok not to deallocate
      end

      subroutine sub_badx2(val,n)   ! memory leak of 8*n bytes per call
      integer n
      real val
      real, pointer :: a(:)
      allocate (a(n))
      call random_number(a)
      val = val + sum(a)
      allocate (a(n))               ! not ok to allocate again
      call random_number(a)
      val = val + sum(a)
!     deallocate(a)                 ! not ok not to deallocate
      end
