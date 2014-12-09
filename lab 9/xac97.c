/*The following functions handle the low level control of the ac97 codec.
  Use these functions to implement higher level control and data movement
  in your Linux kernel modules. 
  Note: These functions are for opb_ac97_controller hardware versions 1.00.a
  and 1.01.a and are based on chapter 6 of the ML40x EDK Processor Reference 
  Design from the Xilinx website and example code provided by Xilinx.*/

#include "xac97.h"

/*Write to an AC97 control register using peripheral keyhole registers.
  -baseaddr is the virtual address of the audio controller. 
  -reg_addr is the register address defined in the associated header file.
  -value is the value that you want written to the corresponding codec
   register. Values are defined in the header file as well
  -please refer to the AC '97 codec datasheet for additional help*/
void XAC97_WriteReg(u32 baseaddr, u32 reg_addr, u32 value) {
    /*write value to out data register*/
    iowrite32(value,(baseaddr+AC97_REG_WRITE_OFFSET)); 
    /*write reg_addr to control address register*/
    iowrite32(reg_addr,(baseaddr+AC97_REG_CONTROL_OFFSET));
    /*wait for value to be written prior to exiting*/
    udelay(100);//might be extravagant
}

/*Read from an AC97 status register using peripheral keyhole registers*/
u32 XAC97_ReadReg(u32 baseaddr, u32 reg_addr) {
    /*write reg_addr to control address register*/
    /*set LSB to 1 to signify a read*/
    iowrite32((reg_addr | 0x1),(baseaddr+AC97_REG_CONTROL_OFFSET));
    /*wait for value to arrive from codec*/
    udelay(100);//might be extravagant
    return ioread32(baseaddr+AC97_REG_READ_OFFSET);
}

/*busy wait until codec is ready
  -use this function after reseting the codec*/
void XAC97_AwaitCodecReady(u32 baseaddr) {
    //busy wait  is there a better way??
    while(!XAC97_isCodecReady(baseaddr));
        //udelay(3);//wait for atleast 3 microseconds before checking again
}


/*FIFO Data Handling routines*/

/*do not use with interrupt interrupts*/
u32 XAC97_ReadFifo(u32 baseaddr) {
  /*busy wait until data arrives*/
  while(XAC97_isOutFIFOEmpty(baseaddr));
  return ioread32(baseaddr+AC97_OUT_FIFO_OFFSET);
}

/*do not use with interrupt interrupts*/
void XAC97_WriteFifo(u32 BaseAddress, u32 sample) {
  /*busy wait until fifo has space*/
  while(XAC97_isInFIFOFull(BaseAddress));
  iowrite32(sample,BaseAddress+AC97_IN_FIFO_OFFSET);
}


/*clear the data in the FIFOs*/
void XAC97_ClearFifos(u32 baseaddr) {
  u32 i;
  iowrite32(AC97_CLEAR_FIFOS,(baseaddr+AC97_CONTROL_OFFSET));
}


/*Sends a reset command to the codec and sets
  volume settings to default*/
void XAC97_SoftReset(u32 BaseAddress) {
  /*tell codec to reset*/
  //printk(KERN_ALERT "RESET 1\r\n");
  XAC97_WriteReg(BaseAddress, AC97_Reset, 0x0000);
  //printk(KERN_ALERT "RESET 2\r\n");
  /** Set default output volumes **/
  XAC97_WriteReg(BaseAddress, AC97_MasterVol, AC97_VOL_MAX);
  //printk(KERN_ALERT "RESET 3\r\n");
  XAC97_WriteReg(BaseAddress, AC97_AuxOutVol, AC97_VOL_MID);
  //printk(KERN_ALERT "RESET 4\r\n");
  XAC97_WriteReg(BaseAddress, AC97_MasterVolMono, AC97_VOL_MAX);
  //printk(KERN_ALERT "RESET 5\r\n");
  XAC97_WriteReg(BaseAddress, AC97_PCMOutVol, AC97_VOL_MAX);
  //printk(KERN_ALERT "RESET 6\r\n");

  /** Clear the fifos **/
  XAC97_ClearFifos(BaseAddress);
}


/*Initialize the codec with or without loopback*/
void XAC97_InitAudio(u32 BaseAddress, u8 Loopback) {
  //printk(KERN_ALERT "INIT 1\r\n");
  /** Reset audio codec **/
  XAC97_SoftReset(BaseAddress);
  //printk(KERN_ALERT "INIT 2\r\n");
  /** Wait until we receive the ready signal **/
  XAC97_AwaitCodecReady(BaseAddress);
  //printk(KERN_ALERT "INIT 3\r\n");
  /*if analog loopback*/
  if( Loopback == AC97_ANALOG_LOOPBACK ) {
    /*set microphone volume to max*/
    XAC97_WriteReg(BaseAddress, AC97_MicVol, AC97_VOL_MAX);
    /*set line in volume to max*/
    XAC97_WriteReg(BaseAddress, AC97_LineInVol, AC97_VOL_MAX);
  }
  /*if caller wants digital loop back, send command to codec*/
  else if( Loopback == AC97_DIGITAL_LOOPBACK )
    XAC97_WriteReg(BaseAddress, AC97_GeneralPurpose, AC97_GP_ADC_DAC_LOOPBACK);


} // end XAC97_InitAudio()


/*turn audio input on*/
void XAC97_EnableInput(u32 BaseAddress, u8 InputType) {
  XAC97_WriteReg(BaseAddress, AC97_RecordGain, AC97_VOL_MAX);  

  if( InputType == AC97_MIC_INPUT ) 
    XAC97_WriteReg(BaseAddress, AC97_RecordSelect, AC97_RECORD_MIC_IN);
  else if( InputType == AC97_LINE_INPUT ) 
    XAC97_WriteReg(BaseAddress, AC97_RecordSelect, AC97_RECORD_LINE_IN);
}

/*turn audio input off*/
void XAC97_DisableInput(u32 BaseAddress, u8 InputType) {
  XAC97_WriteReg(BaseAddress, AC97_RecordGain, AC97_VOL_MUTE);  
  
  if( InputType == AC97_MIC_INPUT ) 
    XAC97_WriteReg(BaseAddress, AC97_MicVol, AC97_VOL_MUTE);
  else if( InputType == AC97_LINE_INPUT ) 
    XAC97_WriteReg(BaseAddress, AC97_LineInVol, AC97_VOL_MUTE);
}


/*example record function that does not use interrupts*/
void XAC97_RecAudio(u32 BaseAddress, u32 StartAddress, 
                    u32 EndAddress) {
  u32 sample;
  volatile u32 *sound_ptr = (u32*)StartAddress;
  
  /** Enable VRA Mode **/
  XAC97_WriteReg(BaseAddress, AC97_ExtendedAudioStat, 1);

  /** Clear out the FIFOs **/
  XAC97_ClearFifos(BaseAddress);

  /** Wait until we receive the ready signal **/
  XAC97_AwaitCodecReady(BaseAddress);

  /** Volume settings **/
  XAC97_WriteReg(BaseAddress, AC97_MasterVol, AC97_VOL_MUTE);
  XAC97_WriteReg(BaseAddress, AC97_AuxOutVol, AC97_VOL_MUTE);
  XAC97_WriteReg(BaseAddress, AC97_MasterVolMono, AC97_VOL_MUTE);
  XAC97_WriteReg(BaseAddress, AC97_PCMOutVol, AC97_VOL_MUTE);
  
  /** Record the incoming audio **/
  while( sound_ptr < (u32*)EndAddress ) {
    sample = XAC97_ReadFifo(BaseAddress);
    *sound_ptr = sample;
    sound_ptr++;
  }

} // end XAC97_RecAudio()


/*example play back function that does not use interrupts*/
void XAC97_PlayAudio(u32 BaseAddress, u8* sound_ptr, 
                     u32 length){
  u32 sample;
  u32 i;

  /** Wait for the ready signal **/
  XAC97_AwaitCodecReady(BaseAddress);
  
/** Play Volume Settings **/
  XAC97_WriteReg(BaseAddress, AC97_MasterVol, AC97_VOL_MAX); 
  XAC97_WriteReg(BaseAddress, AC97_AuxOutVol, AC97_VOL_MAX);
  XAC97_WriteReg(BaseAddress, AC97_MasterVolMono, AC97_VOL_MAX);
  XAC97_WriteReg(BaseAddress, AC97_PCMOutVol, AC97_VOL_MAX);
  XAC97_WriteReg(BaseAddress, AC97_LineInVol, AC97_VOL_MAX);
  XAC97_WriteReg(BaseAddress, AC97_MicVol, AC97_VOL_MAX);

  /** Clear FIFOs **/
  XAC97_ClearFifos(BaseAddress);
 
  for(i=0; i<length;i++) {
    sample = sound_ptr[i]; 

    XAC97_WriteFifo(BaseAddress, sample);
  
}
  XAC97_ClearFifos(BaseAddress);
  
} // end XAC97_PlayAudio()


