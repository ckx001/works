#pragma once

typedef struct
{
	D3DXVECTOR3 pos;		// 埵抲
	D3DXVECTOR3 rot;		// 夞揮
	D3DXVECTOR3 scale;		// 僗働乕儖
	D3DXVECTOR3 move;		// 堏摦検
	D3DXCOLOR col;			// 怓
	float fSizeX;			// 暆
	float fSizeY;			// 崅偝
	int nTimer;				// 僞僀儅乕
	float nDecAlpha;		// 尭悐抣
	bool bUse;				// 巊梡偟偰偄傞偐偳偆偐
} EFFECT;

HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
EFFECT *RetEffect(void);
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);


