//=============================================================================
//
//  タイマー[Time_c.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once

#define MAX_TIME_INDEX 2000

DWORD Time_Count(int index);
void Time_Reset(int index);
void Init_Time_Set(void);
int CreateTimeCollider(void);
void Time_Stop(int index);
void Time_Start(int index);