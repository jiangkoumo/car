#ifndef __TRACK_H__
#define __TRACK_H__

void Track_Init(void);
void Track_SetDefaultSpeed(uint16_t speed);
uint8_t Track_GetSensorState(void);
void Track_Run(void);

#endif // __TRACK_H__