#include <asm_setup.S>

.text
.extern DateFile_dispCurrentWorldNum

.global dispCurrentWorldNum
dispCurrentWorldNum:
	or r3, r30, r30
	or r5, r31, r31
	b DateFile_dispCurrentWorldNum