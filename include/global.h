/**************************************************************************/
/*!
    @file     global.h
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ensure this library description is only included once
#ifndef global_h
#define global_h

#define SERSPEED 9600

#define ENABLED 0x1
#define DISABLE 0x0

#define EVENT_INIT 0x0

#define MAX_SEQ_COUNTER 0xffffff

#define ACCEL_TRIG_POS 19.0
#define ACCEL_TRIG_NEG -19.0

#define STATE_NORMAL   0x01
#define STATE_TRIGGER  0x02
#define STATE_DISABLE  0x04
#define STATE_MKEXIT   0x10

#define UPD_NOSTATE_EV 0x00000
#define UPD_MATRIX_EV  0x00001
#define UPD_GACCEL_EV  0x00002
#define UPD_SOUND_EV   0x00004
#define UPD_SENDMSG_EV 0x00008
#define UPD_RECONN_EV  0x00010
#define UPD_USRMODE_EV 0x00020
#define UPD_WALLCLK_EV 0x00040
#define UPD_PWRCLK_EV  0x00080
#define UPD_SENDTMP_EV 0x00100

#define ALM_TIME 0xff
#define ALM_VOLUME 0xff

#define WALLCLOCK_INFF 0x3000 //dec:+9999
#define WALLCLOCK_INIT 0x270f //dec: 9999 => 0x270f
#define WALLCLOCK_SHORT 0x00ff
#define WALLCLOCK_LONG 0x2600


#endif