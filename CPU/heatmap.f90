program heatmap

    implicit none
    
    INTEGER :: rows, cols, num_of_heaters, x, y, i
    
    character(len=10) :: fname
    
    INTEGER, allocatable, dimension(:,:) :: heaters

    REAL,    allocatable, dimension(:,:) :: heatMatrix
    REAL,    allocatable, dimension(:,:) :: heatMatrix_cache

    
    ! Manual Setup
    print *, "Enter number of rows for the heatmap: "
    read (*,*), rows
    print *, "Enter number of columns for the heatmap: "
    read (*,*), cols
    
    allocate(heatMatrix(rows,cols))
    allocate(heatMatrix_cache(rows,cols))
    
    print *, "Enter number of heat sources: "
    read (*,*), num_of_heaters
    
    allocate(heaters(num_of_heaters,2))
    
    
! Action
    ! Randomly generate heatmap (0 - 100)
    call random_seed() 
    do x=1, rows
        do y=1, cols
            call random_number(heatMatrix(x,y))
            heatMatrix(x,y) = heatMatrix(x,y) * 100
        end do
    end do
    
    ! Get the heaters
    do i = 1, num_of_heaters
        print *, "Enter x coord for the heat source #", i
        read (*,*), heaters(i,1)
        print *, "Enter y coord for the heat source #", i
        read (*,*), heaters(i,2)
        heatMatrix(heaters(i,1), heaters(i,2)) = 100
    end do
    
    do i=1,100
        ! Start the process
        do x=2, rows-1
            do y=2, cols-1
                !        Thermal conductivity of water is 0.0014
                heatMatrix_cache(x,y) = heatMatrix(x,y) + 0.0014 * &
                (heatMatrix(x-1,y) + heatMatrix(x+1,y) + & 
                 heatMatrix(x,y-1) + heatMatrix(x,y+1) -(4*heatMatrix(x,y)))
            end do
        end do
        
        write(unit=fname,fmt=*)i
        
        ! fname//'.ppm'
        
        
        
        heatMatrix = heatMatrix_cache
    end do

    
    deallocate (heaters)
    deallocate (heatMatrix)
    deallocate (heatMatrix_cache)
    
end program heatmap

