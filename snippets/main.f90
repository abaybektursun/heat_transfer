program simple
! Learning fortran

    implicit none
    
    
    integer   :: intVal
    real      :: realVal
    complex   :: complexVal
    logical   :: logicalVal
    
    character (len=100) :: charVar
    
    ! Spcify bytes
    integer(kind=2)  :: intVal2
    integer(kind=4)  :: intVal4
    integer(kind=8)  :: intVal8
    
    ! Simple arithmetic
    real :: x,y,z
    x = 11.5
    y = 45.3
    z = (x + y)*x
    print *, "z is: ", z
    
    ! Char array
    charVar = "I'm Rick Harrison and This Is My Pawn Shop"
    
    ! Largest sizes
    print *,"Largest Integer std. : ", huge(intVal )
    print *,"Largest Real: ",          huge(realVal)
    
    print *, "Largest 2 Byte Integer: ",  huge(intVal2 )
    print *, "Largest 4 Byte Integer: ",  huge(intVal4 )
    print *, "Largest 8 Byte Integer: ",  huge(intVal8 )
    
    print *, "Char array: ", charVar

end program simple
