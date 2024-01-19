//=============================================================================
//
// スコア処理 [score.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX			(99999)		// スコアの最大値
#define BONUS_MAX			(99999)		// ボーナスの最大値
#define SCORE_DIGIT			(5)			// 桁数




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore(int add);
void AddBonus(int plus);
int GetScore(void);
int GetBonus(void);
void SetScore(int score);


