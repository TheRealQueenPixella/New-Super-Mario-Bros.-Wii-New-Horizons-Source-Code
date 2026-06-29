.text
.align 4

#include <asm_setup.S>

.extern drawTheWaves
.extern do2CastleCheck
.extern continueCheckWater
.extern returnFromCheckWater
.extern waterDrawReturn
.extern waterDrawContinue
.extern continueWaterSetup
.extern waterSetupDo2CastleCheck
.extern dontSetWaveThing
.extern setWaveThing
.extern setWaveEffect
.extern noSetWaveEffect

.global WaterWavesDraw
WaterWavesDraw:
    # 08-24 A4 check that we have to reimplement,
    # since Kamek can't make bne branch hooks...
    cmplwi r4, 0x7
    bne ww_not8Castle

    lbz r0, 0x120D(r5)
    cmplwi r0, 0x17
    bne ww_not8Castle

    lbz r0, 0x120E(r5)
    cmplwi r0, 0x3
    beq _drawTheWaves

ww_not8Castle:
    cmplwi r4, 0x8
    bne _do2CastleCheck

    lbz r0, 0x120D(r5)
    cmplwi r0, 0x7
    bne _do2CastleCheck

    lbz r0, 0x120E(r5)
    cmplwi r0, 0x3
    beq _drawTheWaves

_do2CastleCheck:
    b do2CastleCheck
_drawTheWaves:
    b drawTheWaves


.global CheckWater
CheckWater:
    cmplwi r0, 0x7
    bne cw_not8Castle

    lbz r0, 0x120D(r6)
    cmplwi r0, 0x17
    bne cw_not8Castle

    lbz r0, 0x120E(r6)
    cmplwi r0, 0x3
    beq _returnFromCheckWater

cw_not8Castle:
    lbz r0, 0x120C(r6)
    cmplwi r0, 0x8
    bne _continueCheckWater

    lbz r0, 0x120D(r6)
    cmplwi r0, 0x7
    bne _continueCheckWater

    lbz r0, 0x120E(r6)
    cmplwi r0, 0x3
    bne _continueCheckWater

_returnFromCheckWater:
    b returnFromCheckWater
_continueCheckWater:
    b continueCheckWater


.global WaterDraw
WaterDraw:
    lbz r0, 0x120C(r4)
    cmplwi r0, 0x8
    bne _waterDrawContinue

    lbz r0, 0x120D(r4)
    cmplwi r0, 0x7
    bne _waterDrawContinue

    lbz r0, 0x120E(r4)
    cmplwi r0, 0x3
    beq _waterDrawReturn

_waterDrawContinue:
    addi r3, r1, 0x34
    b waterDrawContinue
_waterDrawReturn:
    b waterDrawReturn


.global WaterSetup
WaterSetup:
    cmplwi r4, 0x8
    bne _waterSetupDo2CastleCheck

    lbz r0, 0x120D(r9)
    cmplwi r0, 0x7
    bne _waterSetupDo2CastleCheck

    lbz r0, 0x120E(r9)
    cmplwi r0, 0x3
    beq _continueWaterSetup

_waterSetupDo2CastleCheck:
    lbz r4, 0x120C(r9)
    cmplwi r4, 0x1
    b waterSetupDo2CastleCheck
_continueWaterSetup:
    b continueWaterSetup


.global SetWaveThing
SetWaveThing:
    cmplwi r0, 0x7
    bne setWaveThing_chkSpecial

    lbz r0, 0x120D(r9)
    cmplwi r0, 0x17
    bne setWaveThing_chkSpecial

    lbz r0, 0x120E(r9)
    cmplwi r0, 0x3
    beq _setWaveThing

setWaveThing_chkSpecial:
    lbz r0, 0x120C(r9)
    cmplwi r0, 0x8
    bne _dontSetWaveThing

    lbz r0, 0x120D(r9)
    cmplwi r0, 0x7
    bne _dontSetWaveThing

    lbz r0, 0x120E(r9)
    cmplwi r0, 0x3
    beq _setWaveThing

_dontSetWaveThing:
    b dontSetWaveThing
_setWaveThing:
    b setWaveThing


.global WaveEffectOverride
WaveEffectOverride:
    lbz r0, 0x120C(r31)
    cmplwi r0, 0x8
    bne _noSetWaveEffect

    lbz r0, 0x120D(r31)
    cmplwi r0, 0x7
    bne _noSetWaveEffect

    lbz r0, 0x120E(r31)
    cmplwi r0, 0x3
    beq _setWaveEffect

_noSetWaveEffect:
    lbz r3, 0x38D(r30)
    b noSetWaveEffect
_setWaveEffect:
    b setWaveEffect
