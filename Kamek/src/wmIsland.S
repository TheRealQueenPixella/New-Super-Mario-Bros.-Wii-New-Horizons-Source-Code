.text
#include <asm_setup.S>

.align 4
.extern WM_ISLAND_anmSrtW2
.extern WM_ISLAND_anmClrW8

.global NewAnmW1
NewAnmW1:
    b WM_ISLAND_anmClrW8

.global NewAnmW8
NewAnmW8:
    b WM_ISLAND_anmSrtW2
