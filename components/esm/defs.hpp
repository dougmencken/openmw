#ifndef OPENMW_ESM_DEFS_H
#define OPENMW_ESM_DEFS_H

#include <libs/platform/stdint.h>

#include "components/misc/endianess.hpp"

namespace ESM
{

// Pixel color value. Standard four-byte rr,gg,bb,aa format.
typedef int32_t Color;

enum VarType
{
    VT_Unknown,
    VT_None,
    VT_Short,
    VT_Int,
    VT_Long,
    VT_Float,
    VT_String,
    VT_Ignored
};

enum Specialization
{
    SPC_Combat = 0,
    SPC_Magic = 1,
    SPC_Stealth = 2
};

enum RangeType
{
    RT_Self = 0,
    RT_Touch = 1,
    RT_Target = 2
};

#pragma pack(push)
#pragma pack(1)

// Position and rotation
struct Position
{
    float pos[3];
    float rot[3];
};
#pragma pack(pop)

// Hexadecimal versions of record names (in big-endian order).
enum RecNames
{
    REC_ACTI = OPENMW_FOURCHAR('A','C','T','I'),
    REC_ALCH = OPENMW_FOURCHAR('A','L','C','H'),
    REC_APPA = OPENMW_FOURCHAR('A','P','P','A'),
    REC_ARMO = OPENMW_FOURCHAR('A','R','M','O'),
    REC_BODY = OPENMW_FOURCHAR('B','O','D','Y'),
    REC_BOOK = OPENMW_FOURCHAR('B','O','O','K'),
    REC_BSGN = OPENMW_FOURCHAR('B','S','G','N'),
    REC_CELL = OPENMW_FOURCHAR('C','E','L','L'),
    REC_CLAS = OPENMW_FOURCHAR('C','L','A','S'),
    REC_CLOT = OPENMW_FOURCHAR('C','L','O','T'),
    REC_CNTC = OPENMW_FOURCHAR('C','N','T','C'),
    REC_CONT = OPENMW_FOURCHAR('C','O','N','T'),
    REC_CREA = OPENMW_FOURCHAR('C','R','E','A'),
    REC_CREC = OPENMW_FOURCHAR('C','R','E','C'),
    REC_DIAL = OPENMW_FOURCHAR('D','I','A','L'),
    REC_DOOR = OPENMW_FOURCHAR('D','O','O','R'),
    REC_ENCH = OPENMW_FOURCHAR('E','N','C','H'),
    REC_FACT = OPENMW_FOURCHAR('F','A','C','T'),
    REC_GLOB = OPENMW_FOURCHAR('G','L','O','B'),
    REC_GMST = OPENMW_FOURCHAR('G','M','S','T'),
    REC_INFO = OPENMW_FOURCHAR('I','N','F','O'),
    REC_INGR = OPENMW_FOURCHAR('I','N','G','R'),
    REC_LAND = OPENMW_FOURCHAR('L','A','N','D'),
    REC_LEVC = OPENMW_FOURCHAR('L','E','V','C'),
    REC_LEVI = OPENMW_FOURCHAR('L','E','V','I'),
    REC_LIGH = OPENMW_FOURCHAR('L','I','G','H'),
    REC_LOCK = OPENMW_FOURCHAR('L','O','C','K'),
    REC_LTEX = OPENMW_FOURCHAR('L','T','E','X'),
    REC_MGEF = OPENMW_FOURCHAR('M','G','E','F'),
    REC_MISC = OPENMW_FOURCHAR('M','I','S','C'),
    REC_NPC_ = OPENMW_FOURCHAR('N','P','C','_'),
    REC_NPCC = OPENMW_FOURCHAR('N','P','C','C'),
    REC_PGRD = OPENMW_FOURCHAR('P','G','R','D'),
    REC_PROB = OPENMW_FOURCHAR('P','R','O','B'),
    REC_RACE = OPENMW_FOURCHAR('R','A','C','E'),
    REC_REGN = OPENMW_FOURCHAR('R','E','G','N'),
    REC_REPA = OPENMW_FOURCHAR('R','E','P','A'),
    REC_SCPT = OPENMW_FOURCHAR('S','C','P','T'),
    REC_SKIL = OPENMW_FOURCHAR('S','K','I','L'),
    REC_SNDG = OPENMW_FOURCHAR('S','N','D','G'),
    REC_SOUN = OPENMW_FOURCHAR('S','O','U','N'),
    REC_SPEL = OPENMW_FOURCHAR('S','P','E','L'),
    REC_SSCR = OPENMW_FOURCHAR('S','S','C','R'),
    REC_STAT = OPENMW_FOURCHAR('S','T','A','T'),
    REC_WEAP = OPENMW_FOURCHAR('W','E','A','P')
};

// Hexadecimal versions of the various subrecord names (in big-endian order).
enum SubNames
{
    REC_AI_W = OPENMW_FOURCHAR('A','I','_','W'),
    REC_AI_T = OPENMW_FOURCHAR('A','I','_','T'),
    REC_AI_F = OPENMW_FOURCHAR('A','I','_','F'),
    REC_AI_E = OPENMW_FOURCHAR('A','I','_','E'),
    REC_AI_A = OPENMW_FOURCHAR('A','I','_','A'),
    REC_ANAM = OPENMW_FOURCHAR('A','N','A','M'),
    REC_BNAM = OPENMW_FOURCHAR('B','N','A','M'),
    REC_CNAM = OPENMW_FOURCHAR('C','N','A','M'),
    REC_CNDT = OPENMW_FOURCHAR('C','N','D','T'),
    REC_DELE = OPENMW_FOURCHAR('D','E','L','E'),
    REC_DNAM = OPENMW_FOURCHAR('D','N','A','M'),
    REC_DODT = OPENMW_FOURCHAR('D','O','D','T'),
    REC_FLTV = OPENMW_FOURCHAR('F','L','T','V'),
    REC_FNAM = OPENMW_FOURCHAR('F','N','A','M'),
    REC_INTV = OPENMW_FOURCHAR('I','N','T','V'),
    REC_NAME = OPENMW_FOURCHAR('N','A','M','E'),
    REC_ONAM = OPENMW_FOURCHAR('O','N','A','M'),
    REC_QSTN = OPENMW_FOURCHAR('Q','S','T','N'),
    REC_QSTF = OPENMW_FOURCHAR('Q','S','T','F'),
    REC_QSTR = OPENMW_FOURCHAR('Q','S','T','R'),
    REC_RNAM = OPENMW_FOURCHAR('R','N','A','M'),
    REC_SCVR = OPENMW_FOURCHAR('S','C','V','R'),
    REC_SNAM = OPENMW_FOURCHAR('S','N','A','M')
};

}
#endif
