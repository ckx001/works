//=============================================================================
//
// サウンド処理 [sound.h]
// Author : GP-12B-191 22 程凱旋
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_


// 僒僂儞僪僼傽僀儖
typedef enum
{
	SOUND_LABEL_BGM_TITLE,
	SOUND_LABEL_BGM_GAME,
	SOUND_LABEL_BGM_BOSS,
	SOUND_LABEL_SE_ACK,
	SOUND_LABEL_SE_ACKHIT,
	SOUND_LABEL_SE_ENEMY_WOLF_ACK,
	SOUND_LABEL_SE_USE_RED,
	SOUND_LABEL_SE_WALK,
	SOUND_LABEL_SE_HURT,
	SOUND_LABEL_SE_ROLL,
	SOUND_LABEL_SE_DIE,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// 僾儘僩僞僀僾愰尵
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);
void SetVolume(float value);

#endif
