#include "game.h"
#include "fileload.h"
#include "nsmbwVer.h"
#include <utils.h>

#define GAME_NAME "NSMBW: New Horizons"
// See utils.h for game version string
#define REPORT_TO "github.com/TheRealQueenPixella/NSMBW-New-Horizons"

const bool dsisrFun = false;
const bool gprFun = false;

u32 srr0;
const u32 dlcode = 0x80D26040;

struct OSContext
{
    u32 gpr[32];
    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;
    double fpr[32];
    u8 unknown0[8];
    u32 srr[2];
    u8 unknown1[2];
    u16 flags;
    u32 gqr[8];
    u8 unknown2[4];
    float fppr[32];
};

char *GetRegionAndVersion()
{
	NSMBWVer version = getNsmbwVer();
	switch(version)
	{
		case pal:
			return "PALv1";
		case pal2:
			return "PALv2";
		case ntsc:
			return "NTSCv1";
		case ntsc2:
			return "NTSCv2";
		case jpn:
			return "JPNv1";
		case jpn2:
			return "JPNv2";
		case kor:
			return "KOR";
		case twn:
			return "TWN";
		default:
			return "UNKNOWN";
	}
}
char *GetErrorDescription(u16 OSError)
{
    char *desc[] =
        {
            "SYSTEM RESET",
            "MACHINE CHECK",
            "DSI", "ISI",
            "EXTERNAL INTERRUPT",
            "ALIGNMENT",
            "PROGRAM",
            "FLOATING POINT",
            "DECREMENTER",
            "SYSTEM CALL",
            "TRACE",
            "PERFORMANCE MONITOR",
            "BREAKPOINT",
            "SYSTEM INTERRUPT",
            "THERMAL INTERRUPT",
            "PROTECTION",
            "FPE"};
    return desc[OSError];
}
void PrintContext(u16 OSError, void *_osContext, u32 _dsisr, u32 _dar)
{
    OSContext *osContext = (OSContext *)_osContext;
    nw4r::db::Exception_Printf_("" GAME_NAME " %s has crashed - %s\n\nPlease report an issue on GitHub at:\n" REPORT_TO "\n\n[%s]\n", GAME_VERSION_NUM, GetErrorDescription(OSError), GetRegionAndVersion());
    nw4r::db::Exception_Printf_("SRR0: %08X", osContext->srr[0]);
    if (osContext->srr[0] >= dlcode) {
        nw4r::db::Exception_Printf_(" | %08X NewerASM", osContext->srr[0] - dlcode);
    }
    nw4r::db::Exception_Printf_("\n");

    if (gprFun)
    {
        int i = 0;
        do
        {
            nw4r::db::Exception_Printf_("R%02d:%08X  R%02d:%08X  R%02d:%08X\n", i, osContext->gpr[i], i + 0xB, osContext->gpr[i + 0xB], i + 0x16, osContext->gpr[i + 0x16]);
            i++;
        } while (i < 10);
        nw4r::db::Exception_Printf_("R%02d:%08X  R%02d:%08X\n\n", 10, osContext->gpr[10], 0x15, osContext->gpr[0x15]);
    }

    // Stack trace
    int i = 0;
    u32 *stackPointer = (u32 *)((char *)nw4r::db::sException + 0x33C);
    do
    {
        if (stackPointer == 0 || (u32)stackPointer == 0xFFFFFFFF)
            break;

        nw4r::db::Exception_Printf_("%08X", stackPointer[1]);
        if (stackPointer[1] >= dlcode)
        {
            nw4r::db::Exception_Printf_(" - %08X NewerASM", stackPointer[1] - dlcode);
        }
        nw4r::db::Exception_Printf_("\n");

        i++;
        stackPointer = (u32 *)*stackPointer;
    } while (i < 0x10);
}