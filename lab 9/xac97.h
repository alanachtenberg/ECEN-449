/*modified to support version 1.00.a of
  ac97 codec peripheral*/

#ifndef XAC97_H
#define XAC97_H

#include <asm/io.h>
#include <linux/delay.h>

#define BASE_ADDRESS 0x74400000

// AC97 core register offsets
#define AC97_IN_FIFO_OFFSET             0x0
#define AC97_OUT_FIFO_OFFSET            0x4
#define AC97_STATUS_OFFSET              0x8
#define AC97_CONTROL_OFFSET             0xC
#define AC97_REG_CONTROL_OFFSET         0x10
#define AC97_REG_READ_OFFSET            0x14
#define AC97_REG_WRITE_OFFSET           0x18

// Status register bitmask constants
#define AC97_IN_FIFO_FULL         0x01
#define AC97_IN_FIFO_EMPTY        0x02
#define AC97_OUT_FIFO_EMPTY       0x04
#define AC97_OUT_FIFO_DATA        0x08
#define AC97_REG_ACCESS_BUSY      0x10
#define AC97_CODEC_RDY            0x20
#define AC97_IN_FIFO_UNDERRUN     0x40
#define AC97_OUT_FIFO_OVERRUN     0x80

/*not available in this version
#define AC97_REG_ACCESS_ERROR     0x100
#define AC97_IN_FIFO_LEVEL        0x003ff000 // 21 downto 12
#define AC97_IN_FIFO_LEVEL_RSHFT  12
#define AC97_OUT_FIFO_LEVEL       0xffc00000 // 31 downto 22
#define AC97_OUT_FIFO_LEVEL_RSHFT 22*/

// FIFO Control Offsets
#define AC97_CLEAR_IN_FIFO              0x1
#define AC97_CLEAR_OUT_FIFO             0x2
#define AC97_CLEAR_FIFOS AC97_CLEAR_IN_FIFO | AC97_CLEAR_OUT_FIFO

/*not available in this version
#define AC97_ENABLE_IN_FIFO_INTERRUPT   0x4
#define AC97_ENABLE_OUT_FIFO_INTERRUPT  0x8
#define AC97_ENABLE_RESET_AC97          0x10
#define AC97_DISABLE_RESET_AC97         0x0*/

/* Macros for reading the status registers in the audio codec peripheral*/

/*returns true if codec register access is complete*/
#define XAC97_isRegisterAccessFinished(BaseAddress) \
            ((ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_REG_ACCESS_BUSY) == 0)
/*returns true if the audio codec is ready*/
#define XAC97_isCodecReady(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_CODEC_RDY)
/*returns true if the codec playback buffer is full*/
#define XAC97_isInFIFOFull(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_IN_FIFO_FULL)
/*return true if the codec playback buffer is empty*/
#define XAC97_isInFIFOEmpty(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_IN_FIFO_EMPTY)
/*returns true if there is no data in the codec record buffer*/
#define XAC97_isOutFIFOEmpty(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_OUT_FIFO_EMPTY)
/*returns true if the codec record buffer is full*/
#define XAC97_isOutFIFOFull(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_OUT_FIFO_FULL)
/*returns true if playback buffer is under run i.e. starving the codec for data*/
#define XAC97_isInFIFOUnderrun(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_IN_FIFO_UNDERRUN)
/*returns a true if codec record buffer is over run i.e. data in buffer overwritten*/
#define XAC97_isOutFIFOOverrun(BaseAddress) \
            (ioread32(BaseAddress+AC97_STATUS_OFFSET) & AC97_OUT_FIFO_UNDERRUN)


/** AC97 CODEC Registers **/
#define AC97_Reset              0x00
#define AC97_MasterVol          0x02
#define AC97_AuxOutVol          0x04
#define AC97_MasterVolMono      0x06
//not listed in datasheet so not sure if available
#define AC97_Reserved0x08       0x08
//#define AC97_PCBeepVol          0x0A
#define AC97_PhoneInVol         0x0C
#define AC97_MicVol             0x0E
#define AC97_LineInVol          0x10
#define AC97_CDVol              0x12
//not listed in datasheet so not sure if available
//#define AC97_VideoVol           0x14
#define AC97_AuxInVol           0x16
#define AC97_PCMOutVol          0x18
#define AC97_RecordSelect       0x1A
#define AC97_RecordGain         0x1C
#define AC97_Reserved0x1E       0x1E
#define AC97_GeneralPurpose     0x20
//#define AC97_3DControl          0x22
#define AC97_PowerDown          0x26
#define AC97_ExtendedAudioID    0x28
#define AC97_ExtendedAudioStat  0x2A
#define AC97_PCM_DAC_Rate       0x2C
#define AC97_PCM_ADC_Rate       0x32
#define AC97_PCM_DAC_Rate0      0x78
#define AC97_PCM_DAC_Rate1      0x7A
#define AC97_Reserved0x34       0x34
#define AC97_JackSense          0x72
#define AC97_SerialConfig       0x74
#define AC97_MiscControlBits    0x76
#define AC97_VendorID1          0x7C
#define AC97_VendorID2          0x7E

// Volume Constants for registers:
//  AC97_MasterVol
//  AC97_HeadphoneVol
//  AC97_MasterVolMono
#define AC97_RIGHT_VOL_ATTN_0_DB       0x0
#define AC97_RIGHT_VOL_ATTN_1_5_DB     0x1
#define AC97_RIGHT_VOL_ATTN_3_0_DB     0x2
#define AC97_RIGHT_VOL_ATTN_4_5_DB     0x3
#define AC97_RIGHT_VOL_ATTN_6_0_DB     0x4
#define AC97_RIGHT_VOL_ATTN_7_5_DB     0x5
#define AC97_RIGHT_VOL_ATTN_9_0_DB     0x6
#define AC97_RIGHT_VOL_ATTN_10_0_DB     0x7
#define AC97_RIGHT_VOL_ATTN_11_5_DB     0x8
#define AC97_RIGHT_VOL_ATTN_13_0_DB     0x9
#define AC97_RIGHT_VOL_ATTN_14_5_DB     0xa
#define AC97_RIGHT_VOL_ATTN_16_0_DB     0xb
#define AC97_RIGHT_VOL_ATTN_17_5_DB     0xc
#define AC97_RIGHT_VOL_ATTN_19_0_DB     0xd
#define AC97_RIGHT_VOL_ATTN_20_5_DB     0xe
#define AC97_RIGHT_VOL_ATTN_22_0_DB     0xf
#define AC97_RIGHT_VOL_ATTN_23_5_DB     0x10
#define AC97_RIGHT_VOL_ATTN_25_0_DB     0x11
#define AC97_RIGHT_VOL_ATTN_26_5_DB     0x12
#define AC97_RIGHT_VOL_ATTN_28_0_DB     0x13
#define AC97_RIGHT_VOL_ATTN_29_5_DB     0x14
#define AC97_RIGHT_VOL_ATTN_31_0_DB     0x15
#define AC97_RIGHT_VOL_ATTN_32_5_DB     0x16
#define AC97_RIGHT_VOL_ATTN_34_0_DB     0x17
#define AC97_RIGHT_VOL_ATTN_35_5_DB     0x18
#define AC97_RIGHT_VOL_ATTN_37_0_DB     0x19
#define AC97_RIGHT_VOL_ATTN_38_5_DB     0x1a
#define AC97_RIGHT_VOL_ATTN_40_0_DB     0x1b
#define AC97_RIGHT_VOL_ATTN_41_5_DB     0x1c
#define AC97_RIGHT_VOL_ATTN_43_0_DB     0x1d
#define AC97_RIGHT_VOL_ATTN_44_5_DB     0x1e
#define AC97_RIGHT_VOL_ATTN_46_0_DB     0x1f

#define AC97_LEFT_VOL_ATTN_0_DB        0x0
#define AC97_LEFT_VOL_ATTN_1_5_DB      0x100
#define AC97_LEFT_VOL_ATTN_3_0_DB      0x200
#define AC97_LEFT_VOL_ATTN_4_5_DB      0x300
#define AC97_LEFT_VOL_ATTN_6_0_DB      0x400
#define AC97_LEFT_VOL_ATTN_7_5_DB      0x500
#define AC97_LEFT_VOL_ATTN_9_0_DB      0x600
#define AC97_LEFT_VOL_ATTN_10_0_DB     0x700
#define AC97_LEFT_VOL_ATTN_11_5_DB     0x800
#define AC97_LEFT_VOL_ATTN_13_0_DB     0x900
#define AC97_LEFT_VOL_ATTN_14_5_DB     0xa00
#define AC97_LEFT_VOL_ATTN_16_0_DB     0xb00
#define AC97_LEFT_VOL_ATTN_17_5_DB     0xc00
#define AC97_LEFT_VOL_ATTN_19_0_DB     0xd00
#define AC97_LEFT_VOL_ATTN_20_5_DB     0xe00
#define AC97_LEFT_VOL_ATTN_22_0_DB     0xf00
#define AC97_LEFT_VOL_ATTN_23_5_DB     0x1000
#define AC97_LEFT_VOL_ATTN_25_0_DB     0x1100
#define AC97_LEFT_VOL_ATTN_26_5_DB     0x1200
#define AC97_LEFT_VOL_ATTN_28_0_DB     0x1300
#define AC97_LEFT_VOL_ATTN_29_5_DB     0x1400
#define AC97_LEFT_VOL_ATTN_31_0_DB     0x1500
#define AC97_LEFT_VOL_ATTN_32_5_DB     0x1600
#define AC97_LEFT_VOL_ATTN_34_0_DB     0x1700
#define AC97_LEFT_VOL_ATTN_35_5_DB     0x1800
#define AC97_LEFT_VOL_ATTN_37_0_DB     0x1900
#define AC97_LEFT_VOL_ATTN_38_5_DB     0x1a00
#define AC97_LEFT_VOL_ATTN_40_0_DB     0x1b00
#define AC97_LEFT_VOL_ATTN_41_5_DB     0x1c00
#define AC97_LEFT_VOL_ATTN_43_0_DB     0x1d00
#define AC97_LEFT_VOL_ATTN_44_5_DB     0x1e00
#define AC97_LEFT_VOL_ATTN_46_0_DB     0x1f00

#define AC97_VOL_ATTN_0_DB    AC97_LEFT_VOL_ATTN_0_DB | AC97_RIGHT_VOL_ATTN_0_DB
#define AC97_VOL_ATTN_1_5_DB  AC97_LEFT_VOL_ATTN_1_5_DB | AC97_RIGHT_VOL_ATTN_1_5_DB
#define AC97_VOL_ATTN_3_0_DB  AC97_LEFT_VOL_ATTN_3_0_DB | AC97_RIGHT_VOL_ATTN_3_0_DB
#define AC97_VOL_ATTN_4_5_DB  AC97_LEFT_VOL_ATTN_4_5_DB | AC97_RIGHT_VOL_ATTN_4_5_DB
#define AC97_VOL_ATTN_6_0_DB  AC97_LEFT_VOL_ATTN_6_0_DB | AC97_RIGHT_VOL_ATTN_6_0_DB
#define AC97_VOL_ATTN_7_5_DB  AC97_LEFT_VOL_ATTN_7_5_DB | AC97_RIGHT_VOL_ATTN_7_5_DB
#define AC97_VOL_ATTN_9_0_DB  AC97_LEFT_VOL_ATTN_9_0_DB | AC97_RIGHT_VOL_ATTN_9_0_DB
#define AC97_VOL_ATTN_10_0_DB    AC97_LEFT_VOL_ATTN_10_0_DB | AC97_RIGHT_VOL_ATTN_10_0_DB
#define AC97_VOL_ATTN_11_5_DB    AC97_LEFT_VOL_ATTN_11_5_DB | AC97_RIGHT_VOL_ATTN_11_5_DB
#define AC97_VOL_ATTN_13_0_DB    AC97_LEFT_VOL_ATTN_13_0_DB | AC97_RIGHT_VOL_ATTN_13_0_DB
#define AC97_VOL_ATTN_14_5_DB    AC97_LEFT_VOL_ATTN_14_5_DB | AC97_RIGHT_VOL_ATTN_14_5_DB
#define AC97_VOL_ATTN_16_0_DB    AC97_LEFT_VOL_ATTN_16_0_DB | AC97_RIGHT_VOL_ATTN_16_0_DB
#define AC97_VOL_ATTN_17_5_DB    AC97_LEFT_VOL_ATTN_17_5_DB | AC97_RIGHT_VOL_ATTN_17_5_DB
#define AC97_VOL_ATTN_19_0_DB    AC97_LEFT_VOL_ATTN_19_0_DB | AC97_RIGHT_VOL_ATTN_19_0_DB
#define AC97_VOL_ATTN_20_5_DB    AC97_LEFT_VOL_ATTN_20_5_DB | AC97_RIGHT_VOL_ATTN_20_5_DB
#define AC97_VOL_ATTN_22_0_DB    AC97_LEFT_VOL_ATTN_22_0_DB | AC97_RIGHT_VOL_ATTN_22_0_DB
#define AC97_VOL_ATTN_23_5_DB    AC97_LEFT_VOL_ATTN_23_5_DB | AC97_RIGHT_VOL_ATTN_23_5_DB
#define AC97_VOL_ATTN_25_0_DB    AC97_LEFT_VOL_ATTN_25_0_DB | AC97_RIGHT_VOL_ATTN_25_0_DB
#define AC97_VOL_ATTN_26_5_DB    AC97_LEFT_VOL_ATTN_26_5_DB | AC97_RIGHT_VOL_ATTN_26_5_DB
#define AC97_VOL_ATTN_28_0_DB    AC97_LEFT_VOL_ATTN_28_0_DB | AC97_RIGHT_VOL_ATTN_28_0_DB
#define AC97_VOL_ATTN_29_5_DB    AC97_LEFT_VOL_ATTN_29_5_DB | AC97_RIGHT_VOL_ATTN_29_5_DB
#define AC97_VOL_ATTN_31_0_DB    AC97_LEFT_VOL_ATTN_31_0_DB | AC97_RIGHT_VOL_ATTN_31_0_DB
#define AC97_VOL_ATTN_32_5_DB    AC97_LEFT_VOL_ATTN_32_5_DB | AC97_RIGHT_VOL_ATTN_32_5_DB
#define AC97_VOL_ATTN_34_0_DB    AC97_LEFT_VOL_ATTN_34_0_DB | AC97_RIGHT_VOL_ATTN_34_0_DB
#define AC97_VOL_ATTN_35_5_DB    AC97_LEFT_VOL_ATTN_35_5_DB | AC97_RIGHT_VOL_ATTN_35_5_DB
#define AC97_VOL_ATTN_37_0_DB    AC97_LEFT_VOL_ATTN_37_0_DB | AC97_RIGHT_VOL_ATTN_37_0_DB
#define AC97_VOL_ATTN_38_5_DB    AC97_LEFT_VOL_ATTN_38_5_DB | AC97_RIGHT_VOL_ATTN_38_5_DB
#define AC97_VOL_ATTN_40_0_DB    AC97_LEFT_VOL_ATTN_40_0_DB | AC97_RIGHT_VOL_ATTN_40_0_DB
#define AC97_VOL_ATTN_41_5_DB    AC97_LEFT_VOL_ATTN_41_5_DB | AC97_RIGHT_VOL_ATTN_41_5_DB
#define AC97_VOL_ATTN_43_0_DB    AC97_LEFT_VOL_ATTN_43_0_DB | AC97_RIGHT_VOL_ATTN_43_0_DB
#define AC97_VOL_ATTN_44_5_DB    AC97_LEFT_VOL_ATTN_44_5_DB | AC97_RIGHT_VOL_ATTN_44_5_DB
#define AC97_VOL_ATTN_46_0_DB    AC97_LEFT_VOL_ATTN_46_0_DB | AC97_RIGHT_VOL_ATTN_46_0_DB

#define AC97_VOL_MUTE     0x8000
#define AC97_VOL_MIN      0x1f1f
#define AC97_VOL_MID      0x0a0a
#define AC97_VOL_MAX      0x0000

#define AC97_RECORD_MIC_IN  0x0000
#define AC97_RECORD_LINE_IN 0x0404 // both left and right

// Extended Audio Control
#define AC97_EXTENDED_AUDIO_CONTROL_VRA 0x1


// PCM Data rate constants
// AC97_PCM_DAC_Rate       0x2C
// AC97_PCM_ADC_Rate       0x32
#define AC97_PCM_RATE_8000_HZ  0x1F40
#define AC97_PCM_RATE_11025_HZ 0x2B11
#define AC97_PCM_RATE_16000_HZ 0x3E80
#define AC97_PCM_RATE_22050_HZ 0x5622
#define AC97_PCM_RATE_44100_HZ 0xAC44
#define AC97_PCM_RATE_48000_HZ 0xBB80


// General Purpose register constants (LM4549A)
// bits are zero by default
//#define AC97_GP_PCM_BYPASS_3D       0x8000  // POP bit (on)
//#define AC97_GP_NATIONAL_3D_ON      0x2000  // 3D bit (on)
#define AC97_GP_MONO_OUTPUT_MIX     0x0     // MIX bit (off)
#define AC97_GP_MONO_OUTPUT_MIC     0x200   // MIX bit (on)
#define AC97_GP_MIC_SELECT_MIC1     0x0     // MS bit (off)
#define AC97_GP_MIC_SELECT_MIC2     0x100   // MS bit (on)
#define AC97_GP_ADC_DAC_LOOPBACK    0x80    // LPBK bit

#define AC97_MIC_INPUT   1
#define AC97_LINE_INPUT  2

#define AC97_ANALOG_LOOPBACK  1
#define AC97_DIGITAL_LOOPBACK 2

/*defines for ioctl*/
#define ADJUST_AUX_VOL 0x01DEAD
#define ADJUST_MAST_VOL 0x02DEAD
#define ADJUST_PLAYBACK_RATE 0x03DEAD
#define ENABLE_DISABLE_MONO 0x04DEAD
#define CLEAR_PLAYBACK_FIFO 0x05DEAD
#define READ_AUX_VOL 0x81DEAD
#define READ_MAST_VOL 0x82DEAD
#define READ_PLAYBACK_RATE 0x83DEAD


/* AC97 driver function prototypes*/
void XAC97_WriteReg(u32 BaseAddress, u32 RegAddress, u32 Value);
u32 XAC97_ReadReg(u32 BaseAddress, u32 RegAddress);
void XAC97_AwaitCodecReady(u32 BaseAddress);

void XAC97_SoftReset(u32 BaseAddress);
//not supported
//void XAC97_HardReset(Xuint32 BaseAddress);

void XAC97_InitAudio(u32 BaseAddress, u8 Loopback);
void XAC97_EnableInput(u32 BaseAddress, u8 InputType);
void XAC97_DisableInput(u32 BaseAddress, u8 InputType);
void XAC97_RecAudio(u32 BaseAddress, u32 StartAddress,
                    u32 EndAddress);
void XAC97_PlayAudio(u32 BaseAddress, u8*, u32);
void XAC97_WriteFifo(u32 BaseAddress, u32 Sample);
u32 XAC97_ReadFifo(u32 BaseAddress);
void XAC97_ClearFifos(u32 BaseAddress);

#endif
