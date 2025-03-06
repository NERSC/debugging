!  ===============================================================================
!  Copyright (C) March 2023 - Linaro Limited (or its affiliates). All rights reserved.
!  Copyright (C) Arm Limited, 2019-2023 All rights reserved.
!  The example code is provided to you as an aid to learning when working
!  with Linaro Forge, including but not limited to programming tutorials.
!  Linaro hereby grants to you, subject to the terms and conditions of this Licence,
!  a non-exclusive, non-transferable, non-sub-licensable, free-of-charge licence,
!  to use and copy the Software solely for the purpose of demonstration and
!  evaluation.
!  You accept that the Software has not been tested by Linaro therefore the Software
!  is provided “as is”, without warranty of any kind, express or implied. In no
!  event shall the authors or copyright holders be liable for any claim, damages
!  or other liability, whether in action or contract, tort or otherwise, arising
!  from, out of or in connection with the Software or the use of Software.
!  ===============================================================================


! File mmult.F90
  subroutine mmult(sz, nslices, A, B, C)
    integer, intent(in)     :: sz, nslices
    real(8), intent(in)     :: A(sz,sz), B(sz,sz)
    real(8), intent(inout)  :: C(sz,sz)
    integer                 :: i,j,k

    do i=1,sz/nslices
      do k=1,sz
        do j=1,sz
          C(j,i)=A(k,i)*B(j,k)+C(j,i)
        end do
      end do
    end do

  end subroutine mmult
