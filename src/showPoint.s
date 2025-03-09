    .set noreorder
    .extern game

    lui $v0 , %hi(game)
    lb $a1 , %lo(game + 0x1D) ($v0)
    li $a0 , 1
    addiu $a1,$a1,0x3B