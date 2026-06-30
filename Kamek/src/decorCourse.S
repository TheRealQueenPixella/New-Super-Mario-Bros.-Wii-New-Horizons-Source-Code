.text
#include <asm_setup.S>

.align 4
.extern GetTexFilenameForR5

.global CobCourseAltModel
CobCourseAltModel:
    lwz r5, 4(r30)
	srwi r5, r5, 28
	andi. r5, r5, 0xF
	b GetTexFilenameForR5
