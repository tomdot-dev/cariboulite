/*
 * This file was derived from code written by HackRF Team:
 * 	1. Michael Ossmann
 * and was modified by David Michaeli (cariboulabs.co@gmail.com) to
 * adapt if for the CaribouLite project running on Linux OS (RPI).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __RFFC507X_REGS_DEF
#define __RFFC507X_REGS_DEF

#include <stdio.h>
#include <stdint.h>

#define RFFC507X_READBACK_REG 31

/* Generate static inline accessors that operate on the global
 * regs. Done this way to (1) allow defs to be scraped out and used
 * elsewhere, e.g. in scripts, (2) to avoid dealing with endian
 * (structs). This may be used in firmware, or on host predefined
 * register loads. */

/* On set_, register is always set dirty, even if nothing
 * changed. This makes sure that writes that have side effects,
 * e.g. frequency setting, are not skipped. */

/* n=name, r=regnum, o=offset (bits from LSB) of LSB of field,
 * l=length (bits) */
#define __MREG__(n,r,o,l) \
static inline uint16_t get_##n(rffc507x_st* dev) { \
        return (dev->rffc507x_regs[(r)] >> (o)) & ((1L<<(l))-1); \
} \
static inline void set_##n(rffc507x_st* dev, uint16_t v) {      \
	dev->rffc507x_regs[(r)] &= (uint16_t)(~(((1L<<(l))-1)<<(o)));	\
	dev->rffc507x_regs[(r)] |= (uint16_t)((((v)&((1L<<(l))-1))<<(o)));	\
	RFFC507X_REG_SET_DIRTY((dev),(r));				\
}

/* REG 00 (0): LF */
__MREG__(RFFC507X_PLLCPL,0,0,3)
__MREG__(RFFC507X_P1CPDEF,0,3,6)
__MREG__(RFFC507X_P2CPDEF,0,9,6)
__MREG__(RFFC507X_IFACT,0,15,1)
#define RFFC507X_

/* REG 01 (1): XO */
__MREG__(RFFC507X_SUWAIT,1,0,10)
__MREG__(RFFC507X_XOCF,1,10,1)
__MREG__(RFFC507X_XOC,1,11,4)
__MREG__(RFFC507X_XOCH,1,15,1)

/* REG 02 (2): CAL_TIME */
__MREG__(RFFC507X_TKV2,2,0,4)
__MREG__(RFFC507X_TKV1,2,4,4)
__MREG__(RFFC507X_TCT,2,10,5)
__MREG__(RFFC507X_WAIT,2,15,1)

/* REG 03 (3): VCO_CTRL */
__MREG__(RFFC507X_ICPUP,3,1,2)
__MREG__(RFFC507X_REFST,3,3,1)
__MREG__(RFFC507X_XOI3,3,4,1)
__MREG__(RFFC507X_XOI2,3,5,1)
__MREG__(RFFC507X_XOI1,3,6,1)
__MREG__(RFFC507X_KVPOL,3,7,1)
__MREG__(RFFC507X_KVRNG,3,8,1)
__MREG__(RFFC507X_KVAVG,3,9,2)
__MREG__(RFFC507X_CLKPL,3,1,1)
__MREG__(RFFC507X_CTPOL,3,12,1)
__MREG__(RFFC507X_CTAVG,3,13,2)
__MREG__(RFFC507X_XTVCO,3,15,1)

/* REG 04 (4): CT_CAL1 */
__MREG__(RFFC507X_P1CTDEF,4,0,7)
__MREG__(RFFC507X_P1CT,4,7,1)
__MREG__(RFFC507X_P1CTV,4,8,5)
__MREG__(RFFC507X_P1CTGAIN,4,13,3)

/* REG 05 (5): CT_CAL2 */
__MREG__(RFFC507X_P2CTDEF,5,0,7)
__MREG__(RFFC507X_P2CT,5,7,1)
__MREG__(RFFC507X_P2CTV,5,8,5)
__MREG__(RFFC507X_P2CTGAIN,5,13,3)

/* REG 06 (6): PLL_CAL1 */
__MREG__(RFFC507X_P1SGN,6,2,1)
__MREG__(RFFC507X_P1KVGAIN,6,3,3)
__MREG__(RFFC507X_P1DN,6,6,9)
__MREG__(RFFC507X_P1KV,6,15,1)

/* REG 07 (7): PLL_CAL2 */
__MREG__(RFFC507X_P2SGN,7,2,1)
__MREG__(RFFC507X_P2KVGAIN,7,3,3)
__MREG__(RFFC507X_P2DB,7,6,9)
__MREG__(RFFC507X_P2KV,7,15,1)

/* REG 08 (8): VCO_AUTO */
__MREG__(RFFC507X_CTMIN,8,1,7)
__MREG__(RFFC507X_CTMAX,8,8,7)
__MREG__(RFFC507X_AUTO,8,15,1)

/* REG 09 (9): PLL_CTRL */
__MREG__(RFFC507X_PLLDY,9,0,2)
__MREG__(RFFC507X_ALOI,9,2,1)
__MREG__(RFFC507X_RELOK,9,3,1)
__MREG__(RFFC507X_LDLEV,9,4,1)
__MREG__(RFFC507X_LDEN,9,5,1)
__MREG__(RFFC507X_TVCO,9,6,5)
__MREG__(RFFC507X_PLLST,9,11,1)
__MREG__(RFFC507X_CLKDIV,9,12,3)
__MREG__(RFFC507X_DIVBY,9,15,1)

/* REG 0A (10): PLL_BIAS */
__MREG__(RFFC507X_P2VCOI,10,0,3)
__MREG__(RFFC507X_P2LOI,10,3,4)
__MREG__(RFFC507X_P1VCOI,10,8,3)
__MREG__(RFFC507X_P1LOI,10,11,4)

/* REG 0B (11): MIX_CONT */
__MREG__(RFFC507X_P2MIXIDD,11,9,3)
__MREG__(RFFC507X_P1MIXIDD,11,12,3)
__MREG__(RFFC507X_FULLD,11,15,1)

/* REG 0C (12): P1_FREQ1 */
__MREG__(RFFC507X_P1VCOSEL,12,0,2)
__MREG__(RFFC507X_P1PRESC,12,2,2)
__MREG__(RFFC507X_P1LODIV,12,4,3)
__MREG__(RFFC507X_P1N,12,7,9)

/* REG 0D (13): P1_FREQ2 */ /* !!!!! CHECK FOR OVERFLOW !!!!! */
__MREG__(RFFC507X_P1NMSB,13,0,16)

/* REG 0E (14): P1_FREQ3 */
__MREG__(RFFC507X_P1NLSB,14,8,8)

/* REG 0F (15): P2_FREQ1 */
__MREG__(RFFC507X_P2VCOSEL,15,0,2)
__MREG__(RFFC507X_P2PRESC,15,2,2)
__MREG__(RFFC507X_P2LODIV,15,4,3)
__MREG__(RFFC507X_P2N,15,7,9)

/* REG 10 (16): P2_FREQ2 */ /* !!!!! CHECK FOR OVERFLOW !!!!! */
__MREG__(RFFC507X_P2NMSB,16,0,16)

/* REG 11 (17): P2_FREQ3 */
__MREG__(RFFC507X_P2NLSB,17,8,8)

/* REG 12 (18): FN_CTRL */
__MREG__(RFFC507X_TZPS,18,1,1)
__MREG__(RFFC507X_DMODE,18,2,1)
__MREG__(RFFC507X_FM,18,3,1)
__MREG__(RFFC507X_DITH,18,4,1)
__MREG__(RFFC507X_DSM_MODE,18,5,1)
__MREG__(RFFC507X_PHSALNDLY,18,6,2)
__MREG__(RFFC507X_PHSALNGAIN,18,8,3)
__MREG__(RFFC507X_PHALN,18,11,1)
__MREG__(RFFC507X_SDM,18,12,2)
__MREG__(RFFC507X_DITHR,18,14,1)
__MREG__(RFFC507X_FNZ,18,15,1)

/* REG 13 (19): EXT_MOD */
__MREG__(RFFC507X_MODSTEP,19,10,4)
__MREG__(RFFC507X_MODSETUP,19,14,2)

/* REG 14 (20): FMOD */ /* !!!!! CHECK FOR OVERFLOW !!!!! */
__MREG__(RFFC507X_MODULATION,20,0,16)

/* REG 15 (21): SDI_CTRL */
__MREG__(RFFC507X_RESET,21,1,1)
__MREG__(RFFC507X_ADDR,21,11,1)
__MREG__(RFFC507X_4WIRE,21,12,1)
__MREG__(RFFC507X_MODE,21,13,1)
__MREG__(RFFC507X_ENBL,21,14,1)
__MREG__(RFFC507X_SIPIN,21,15,1)

/* REG 16 (22): GPO */
__MREG__(RFFC507X_LOCK,22,0,1)
__MREG__(RFFC507X_GATE,22,1,1)
__MREG__(RFFC507X_P1GPO,22,2,7)
__MREG__(RFFC507X_P2GPO,22,9,7)

/* REG 17 (23): T_VCO */
__MREG__(RFFC507X_CURVE_VCO3,23,7,3)
__MREG__(RFFC507X_CURVE_VCO2,23,10,3)
__MREG__(RFFC507X_CURVE_VCO1,23,13,3)

/* REG 18 (24): IQMOD1 */
__MREG__(RFFC507X_BUFDC,24,0,2)
__MREG__(RFFC507X_DIVBIAS,24,2,1)
__MREG__(RFFC507X_CALBLK,24,3,1)
__MREG__(RFFC507X_CALNUL,24,4,1)
__MREG__(RFFC507X_CALON,24,5,1)
__MREG__(RFFC507X_LOBIAS,24,6,2)
__MREG__(RFFC507X_MODBIAS,24,8,3)
__MREG__(RFFC507X_CTRL,24,11,5)   /* shown as 5 fields in reg overview */

/* REG 19 (25): IQMOD2 */
__MREG__(RFFC507X_MODBUF,25,0,2)
__MREG__(RFFC507X_MOD,25,2,2)
__MREG__(RFFC507X_CALATTEN,25,4,2)
__MREG__(RFFC507X_RCTUNE,25,6,6)
__MREG__(RFFC507X_BBATTEN,25,12,4)

/* REG 1A (26): IQMOD3 */
__MREG__(RFFC507X_DACEN,26,3,1)
__MREG__(RFFC507X_BUFDACQ,26,4,6)
__MREG__(RFFC507X_BUFDACI,26,10,6)

/* REG 1B (27): IQMOD4 */
__MREG__(RFFC507X_BUFBIAS2,27,2,2)
__MREG__(RFFC507X_BUFBIAS1,27,4,2)
__MREG__(RFFC507X_MODDACQ,27,6,6)
__MREG__(RFFC507X_MODDACI,27,12,6)

/* REG 1C (28): T_CTRL */
__MREG__(RFFC507X_V_TEST,28,5,1)
__MREG__(RFFC507X_LDO_BY,28,6,1)
__MREG__(RFFC507X_EXT_FILT,28,7,1)
__MREG__(RFFC507X_REF_SEL,28,8,1)
__MREG__(RFFC507X_FILT_CTRL,28,9,2)
__MREG__(RFFC507X_FC_EN,28,11,1)
__MREG__(RFFC507X_TBL_SEL,28,12,2)
__MREG__(RFFC507X_TC_EN,28,14,2)

/* REG 1D (29): DEV_CTRL */
__MREG__(RFFC507X_BYPAS,29,1,1)
__MREG__(RFFC507X_CTCLK,29,2,1)
__MREG__(RFFC507X_DAC,29,3,1)
__MREG__(RFFC507X_CPD,29,4,1)
__MREG__(RFFC507X_CPU,29,5,1)
__MREG__(RFFC507X_RSMSTOPST,29,6,5)
__MREG__(RFFC507X_RSMST,29,11,1)
__MREG__(RFFC507X_READSEL,29,12,4)

/* REG 1E (30): TEST */
__MREG__(RFFC507X_LFSRD,30,0,1)     /* n/a in reg overview */
__MREG__(RFFC507X_RCBYP,30,1,1)
__MREG__(RFFC507X_RGBYP,30,2,1)
__MREG__(RFFC507X_LFSRT,30,3,1)
__MREG__(RFFC507X_LFSRGATET,30,4,4)
__MREG__(RFFC507X_LFSRP,30,8,1)
__MREG__(RFFC507X_LFSR,30,9,1)
__MREG__(RFFC507X_TSEL,30,10,2)
__MREG__(RFFC507X_TMUX,30,12,3)
__MREG__(RFFC507X_TEN,30,15,1)

#endif // __RFFC507X_REGS_DEF
