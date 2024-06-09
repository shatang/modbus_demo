#ifndef _W5500_DEVICE_H
#define _W5500_DEVICE_H

#include "socket.h"
#include <stdint.h>

void Load_Net_Parameters(void);
bool Detect_Gateway(void);
void Socket_Init(SOCKET s);

void W5500_SystemInit(void);


//º”‘ÿ≤Œ ˝
void Modify_Gateway_IP(void); 
void Modify_Sub_Mask(void);
void Modify_Sub_MacAddr(void);
void Modify_LocalIP(void);

#endif
