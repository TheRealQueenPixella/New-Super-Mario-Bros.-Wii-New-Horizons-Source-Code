.text
.align 4

#include <asm_setup.S>

.extern CurrentWorld
.extern CurrentLevel

.global CircleWipeForNextGotoBlock
CircleWipeForNextGotoBlock:
    lis    r4, CurrentWorld@h
    ori    r4, r4, CurrentWorld@l
    lbz    r4, 0(r4)
    cmpwi  r4, 0 # World 1
    bne    isNotCredits

    lis    r4, CurrentLevel@h
    ori    r4, r4, CurrentLevel@l
    lbz    r4, 0(r4)
    cmpwi  r4, 41 # Stage 42 (credits)
    bne    isNotCredits

    li     r6, 1 # Circle wipe
    blr

isNotCredits:
    li     r6, 3 # Wavy wipe
    blr


.global CheckStopRequest1
CheckStopRequest1:
    lis    r5, CurrentWorld@h
    ori    r5, r5, CurrentWorld@l
    lbz    r5, 0(r5)
    cmpwi  r5, 0 # World 1
    bne    setFlag1

    lis    r5, CurrentLevel@h
    ori    r5, r5, CurrentLevel@l
    lbz    r5, 0(r5)
    cmpwi  r5, 41 # Stage 42 (credits)
    bne    setFlag1

    # No need to set flag
    blr

setFlag1:
    ori r4, r4, 0xF # Set stop request flag
    blr


.global CheckStopRequest2
CheckStopRequest2:
    lis    r3, CurrentWorld@h
    ori    r3, r3, CurrentWorld@l
    lbz    r3, 0(r3)
    cmpwi  r3, 0 # World 1
    bne    setFlag2

    lis    r3, CurrentLevel@h
    ori    r3, r3, CurrentLevel@l
    lbz    r3, 0(r3)
    cmpwi  r3, 41 # Stage 42 (credits)
    bne    setFlag2

    # No need to set flag
    blr

setFlag2:
    ori r0, r0, 0xF # Set stop request flag
    blr
