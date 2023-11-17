//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum
{
	SOUND_LABEL_BGM_sample000,	// BGM0
	SOUND_LABEL_BGM_title,
	SOUND_LABEL_SE_ZIPPO,
	SOUND_LABEL_SE_BULLET,
	SOUND_LABEL_SE_ABSORB,
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);
bool SetVolume(float volume);
void UpdateVolume(float bgmVolume, bool bgmMute, float sfxVolume, bool sfxMute);