//=============================================================================
//
//  タイマー[Time_c.cpp]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#include "main.h"
#include "Time_c.h"

static DWORD begin_time[MAX_TIME_INDEX];
static DWORD time_difference[MAX_TIME_INDEX];
static bool time_start[MAX_TIME_INDEX];
static bool time_stop[MAX_TIME_INDEX];
static bool time_use[MAX_TIME_INDEX];
//1000 = 1s


void Init_Time_Set()
{
	for (int i = 0; i < MAX_TIME_INDEX; i++)
	{
		time_use[i] = false;
		time_start[i] = false;
		time_stop[i] = true;
	}
}


DWORD Time_Count(int index)
{
	if (time_stop[index] == false)
	{
	    if (time_start[index] == false)
	    {
	    	begin_time[index] = RetCurrentTime();
	    	time_start[index] = true;
	    	time_difference[index] = 0;
	    }
	    else
	    {
	    	time_difference[index] = RetCurrentTime() - begin_time[index];
	    }
	    return time_difference[index];
	}
	else
		return 0.0f;
}


void Time_Reset(int index)
{
	time_start[index] = false;
	time_stop[index] = false;
}


void Time_Start(int index)
{
	time_stop[index] = false;
}


void Time_Stop(int index)
{
	time_start[index] = false;
	time_stop[index] = true;
}


int CreateTimeCollider(void)
{
	int nIdxTime = -1;

	for (int i = 0; i < MAX_TIME_INDEX; i++)
	{
		if (time_use[i] == false)
		{
			time_use[i] = true;

			nIdxTime = i;
			break;
		}
	}
	return nIdxTime;
}