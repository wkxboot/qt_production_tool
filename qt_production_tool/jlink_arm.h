#ifndef JLINK_JLINKARM_H
#define JLINK_JLINKARM_H
/*********************************************************************
*              SEGGER MICROCONTROLLER SYSTEME GmbH                   *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996-2004 SEGGER Microcontroller Systeme GmbH           *
*                                                                    *
* Internet: www.segger.com Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : ARM.h
Purpose : Interface of the J-Link ARM module.
---------------------------END-OF-HEADER------------------------------


*/


#ifndef JLINKARM_H           /* Guard against multiple inclusion */
#define JLINKARM_H

#include "stdint.h"     /* Required for data types: uint8_t, uint16_t, uint32_t */

#if defined(__cplusplus)
  extern "C" {          /* Make sure we have C-declarations in C++ programs */
#endif


/*********************************************************************
*
*       Enums required for API
*/
enum ARM_DEVICE {
  ARM_DEVICE_UNKNOWN,
  ARM_DEVICE_ARM7,
  ARM_DEVICE_ARM9
};

enum {
   ARM_NO_DELAY
  ,ARM_WRITE_DELAYED
};

enum ARM_ENDIAN {
  ARM_ENDIAN_LITTLE, ARM_ENDIAN_BIG
};

/*********************************************************************
*
*       ARM core registers
*/
typedef enum {
  ARM_REG_R0,
  ARM_REG_R1,
  ARM_REG_R2,
  ARM_REG_R3,
  ARM_REG_R4,
  ARM_REG_R5,
  ARM_REG_R6,
  ARM_REG_R7,
  ARM_REG_CPSR,
  ARM_REG_R15,
  ARM_REG_R8_USR,   ARM_REG_R9_USR,  ARM_REG_R10_USR, ARM_REG_R11_USR,
                    ARM_REG_R12_USR, ARM_REG_R13_USR, ARM_REG_R14_USR,
  ARM_REG_SPSR_FIQ, ARM_REG_R8_FIQ,  ARM_REG_R9_FIQ,  ARM_REG_R10_FIQ,
                    ARM_REG_R11_FIQ, ARM_REG_R12_FIQ, ARM_REG_R13_FIQ, ARM_REG_R14_FIQ,
  ARM_REG_SPSR_SVC, ARM_REG_R13_SVC, ARM_REG_R14_SVC,
  ARM_REG_SPSR_ABT, ARM_REG_R13_ABT, ARM_REG_R14_ABT,
  ARM_REG_SPSR_IRQ, ARM_REG_R13_IRQ, ARM_REG_R14_IRQ,
  ARM_REG_SPSR_UND, ARM_REG_R13_UND, ARM_REG_R14_UND,
  ARM_REG_SPSR_SYS, ARM_REG_R13_SYS, ARM_REG_R14_SYS,
  ARM_NUM_REGS
} ARM_REG;


/*********************************************************************
*
*       ICEBreaker Module registers
*/
#define ARM_ICE_DBG_CTRL		    0x00
#define ARM_ICE_DBG_STS			    0x01
#define ARM_ICE_VECTOR_CATCH_CTRL	0x02     // ARM 9 only
#define ARM_ICE_DBG_COM_CTRL	    0x04
#define ARM_ICE_DBG_COM_DATA	    0x05

#define ARM_ICE_DCC_STAT 0x04       // Debug comms status / control
#define ARM_ICE_DCC_DATA 0x05       // Debug comms data
#define ARM_ICE_WP0_AV	 0x08
#define ARM_ICE_WP0_AM	 0x09
#define ARM_ICE_WP0_DV	 0x0A
#define ARM_ICE_WP0_DM	 0x0B
#define ARM_ICE_WP0_CV	 0x0C
#define ARM_ICE_WP0_CM	 0x0D
#define ARM_ICE_WP1_AV	 0x10
#define ARM_ICE_WP1_AM	 0x11
#define ARM_ICE_WP1_DV	 0x12
#define ARM_ICE_WP1_DM	 0x13
#define ARM_ICE_WP1_CV	 0x14
#define ARM_ICE_WP1_CM	 0x15


/*********************************************************************
*
*       API functions
*/
int          JLINK_TIF_Select(int);
void         JLINKARM_Close(void);
void         JLINKARM_ClrBP(unsigned BPIndex);
void         JLINKARM_ClrError(void);
void         JLINKARM_EnableLog2File(void);
const char * JLINKARM_GetCompileDateTime(void);
uint16_t     JLINKARM_GetEmbeddedFWVersion(void);
//void       JLINKARM_GetHWStatus(JTAG_HW_STATUS * pStat);
uint32_t     JLINKARM_GetId(void);
//void       JLINKARM_GetIdData(JTAG_ID_DATA * pIdData);
uint16_t     JLINKARM_GetSelDevice(void);
int          JLINKARM_GetVoltage(void);
uint16_t     JLINKARM_GetSpeed(void);
void         JLINKARM_Go(void);
void         JLINKARM_GoIntDis(void);
char         JLINKARM_Halt(void);
char         JLINKARM_HaltNoSave(void);
char         JLINKARM_IsConnected(void);
char         JLINKARM_IsHalted(void);
int          JLINKARM_IsOpen(void);
void         JLINKARM_ClrError(void);
uint32_t     JLINKARM_GetSN(void);
const char * JLINKARM_Open(void);
void         JLINKARM_Unlock(void);
int          JLINKARM_ExecCommand(char *,int,int);
int          JLINKARM_ReadDCC(uint32_t * pData, uint32_t NumItems, int TimeOut);
void         JLINKARM_ReadDCCFast(uint32_t * pData, uint32_t NumItems);
uint32_t     JLINKARM_ReadICEReg(int RegIndex);
int          JLINKARM_ReadMem (uint32_t addr, uint32_t count, void * p);
void         JLINK_ReadMemU8 (uint32_t Addr, uint32_t NumItems, uint8_t * pData, uint8_t* pStatus);
void         JLINK_ReadMemU16(uint32_t Addr, uint32_t NumItems, uint16_t* pData, uint8_t* pStatus);
void         JLINK_ReadMemU32(uint32_t Addr, uint32_t NumItems, uint32_t* pData, uint8_t* pStatus);
uint32_t     JLINKARM_ReadReg(ARM_REG RegIndex);
void         JLINKARM_Reset(void);
void         JLINKARM_ResetPullsTRST (uint8_t OnOff);
void         JLINKARM_ResetPullsRESET(uint8_t OnOff);
void         JLINKARM_SelDevice(uint16_t DeviceIndex);
void         JLINKARM_SetBP(unsigned BPIndex, uint32_t Addr);
int          JLINKARM_SetEndian(int v);
int          JLINKARM_SetInitRegsOnReset(int v);
void         JLINKARM_SetMaxSpeed(void);
void         JLINKARM_SetResetDelay(int ms);
int          JLINKARM_SetResetPara(int Value);
void         JLINKARM_SetSpeed(int Speed);
char         JLINKARM_Step(void);
int          JLINKARM_Test(void);
uint16_t     JLINKARM_UpdateFirmware(void);
uint32_t     JLINKARM_UpdateFirmwareIfNewer(void);
int          JLINKARM_WaitDCCRead(int TimeOut);
int          JLINKARM_WriteDCC(const uint32_t * pData, uint32_t NumItems, int TimeOut);
void         JLINKARM_WriteDCCFast(const uint32_t * pData, uint32_t NumItems);
void         JLINKARM_WriteICEReg(int RegIndex, uint32_t Value, int AllowDelay);
char         JLINKARM_WriteReg(ARM_REG RegIndex, uint32_t Data);
void         JLINKARM_WriteMem(uint32_t addr, uint32_t count, const void * p);
void         JLINKARM_WriteMemDelayed(uint32_t Addr, uint32_t Count, const void * p);
void         JLINKARM_WriteU8 (uint32_t addr, uint8_t Data);
void         JLINKARM_WriteU16(uint32_t addr, uint16_t Data);
void         JLINKARM_WriteU32(uint32_t addr, uint32_t Data);

/*********************************************************************
*
*       external functions
*
* These functions need to be provided by the application.
* Dummys will do for a start (or printf or MessageBox)
*/
void         JLINKARM_X_ErrorOut(const char * s);
void         JLINKARM_X_Log     (const char * s);
void         JLINKARM_X_Log2File(const char * s);
void         JLINKARM_X_Warn    (const char * s);
int          JLINKARM_X_GetTickCount(void);

#if defined(__cplusplus)
}     /* Make sure we have C-declarations in C++ programs */
#endif

#endif    /* Guard against multiple inclusion */

/*************************** end of file ****************************/



#endif // JLINK_JLINKARM_H
