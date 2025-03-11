    .set noreorder

    .extern game
    .extern ShowPoint

    lui $v0 , %hi(game)
    lb $a1 , %lo(game + 0x1D) ($v0)
    lb $a0 , %lo(game + 0x1D) ($v0)
    nop
    jal ShowPoint
    nop