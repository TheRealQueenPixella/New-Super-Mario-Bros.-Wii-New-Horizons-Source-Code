#include <asm_setup.S>
.text

.align 4
.extern GetBMG__Fv
.extern WriteBMGToTextBox__FPQ34nw4r3lyt7TextBoxPQ27dScript5Res_ciiie
.extern returnFromTitleScreenVersion

.global TitleScreenVersion
TitleScreenVersion:
    lwz r7, 0x248(r31) # Replaced insn

    bl GetBMG__Fv
    or r4, r3, r3

    or r3, r7, r7 # T_E3verCheck
    li r5, 0x3E9  # Category
    li r6, 0x1    # Msg ID
    li r7, 0x0    # Arg count
    crxor 4*cr1+eq, 4*cr1+eq, 4*cr1+eq
    bl WriteBMGToTextBox__FPQ34nw4r3lyt7TextBoxPQ27dScript5Res_ciiie

    b returnFromTitleScreenVersion
