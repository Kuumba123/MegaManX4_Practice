.set noreorder
    li $v0,1
    nop
    j ExitBranch
    sb $v0,0x23($s0)