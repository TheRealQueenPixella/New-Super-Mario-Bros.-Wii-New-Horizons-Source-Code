#include <asm_setup.S>

.text
.extern SCMWorldAndLevelNumbers__FPQ23m2d13EmbedLayout_cii
.extern handleFileters__FiiPQ23m2d13EmbedLayout_c
.extern returnSCMcrap
.global SCMWorldAndLevelNumber
SCMWorldAndLevelNumber:
	addi r3,r30,0x70 # layout
	lwz r4,0x27c(r30) # world number
	lwz r5,0x280(r30) # level number
    bl SCMWorldAndLevelNumbers__FPQ23m2d13EmbedLayout_cii

	lwz r3,0x27c(r30) # world number
	lwz r4,0x280(r30) # level number
	addi r5,r30,0x70  # layout
	bl handleFileters__FiiPQ23m2d13EmbedLayout_c

    b returnSCMcrap