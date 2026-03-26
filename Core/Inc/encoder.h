#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Encoder_Init(void);
void Encoder_UpdateIRQHandler(void);
int32_t Encoder_GetCount(void);
void Encoder_ResetCount(void);
void Encoder_UpdateSpeed(void);
float Encoder_GetRPM(void);


#ifdef __cplusplus
}
#endif

#endif /* __ENCODER_H */

