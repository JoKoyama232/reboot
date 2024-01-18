//=============================================================================
//
// ���U���g�e�N�X�`����ʏ��� [result.cpp]
// Author : ��c�@�P
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "result_tex.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�
#define BUTTON_MAX					(2)


#define TEXTURE_WIDTH_LOGO			(360)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(60)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number.png",
	"data/TEXTURE/replay_mission.png",
	"data/TEXTURE/quit_game.png",
	"data/TEXTURE/mission_complete_result.png",
	"data/TEXTURE/result.png",
	"data/TEXTURE/mission_fail_result.png",


};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�
static int selector = 0;
static BOOL						g_Load = FALSE;
static BUTTON					g_Button[BUTTON_MAX];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultTex(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use = true;
	g_w = SCREEN_WIDTH;
	g_h = SCREEN_HEIGHT;
	g_Pos = { g_w * 0.5f, 50.0f, 0.0f };
	g_TexNo = 0;

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i].texNo = 1 + i;
		g_Button[i].h = SCREEN_HEIGHT;
		g_Button[i].w = SCREEN_WIDTH;
		g_Button[i].pos = XMFLOAT3(1600, g_Button[i].h * 0.5f + 150.0f * (i + 1), 0.0f);

		g_Button[i].alpha = 1.0f;
		g_Button[i].flag_alpha = true;
		g_Button[i].flag_sound = false;
	}

	//// BGM�Đ�
	//PlaySound(SOUND_LABEL_BGM_sample002);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResultTex(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultTex(void)
{
	int x = GetMousePosX();
	int y = GetMousePosY();

	HWND windowHandle = GetForegroundWindow(); // �E�B���h�E�̃n���h�����擾
	RECT windowRect;
	GetWindowRect(windowHandle, &windowRect);// �E�B���h�E�̈ʒu�����擾
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		// �}�E���̈ʒu���摜�ɓ������Ă��邩�ǂ����̔���
		if ((x > g_Button[i].pos.x - TEXTURE_WIDTH_LOGO * 0.5f) &&
			(x < g_Button[i].pos.x + TEXTURE_WIDTH_LOGO * 0.5f) &&
			(y + 15.0f > g_Button[i].pos.y - TEXTURE_HEIGHT_LOGO * 0.5f) &&
			(y + 15.0f < g_Button[i].pos.y + TEXTURE_HEIGHT_LOGO * 0.5f))
		{

			//�_�ł�����
			if (g_Button[i].flag_alpha == true)
			{
				g_Button[i].alpha -= 0.02f;
				if (g_Button[i].alpha <= 0.0f)
				{
					g_Button[i].alpha = 0.0f;
					g_Button[i].flag_alpha = false;
				}
			}
			else
			{
				g_Button[i].alpha += 0.02f;
				if (g_Button[i].alpha >= 1.0f)
				{
					g_Button[i].alpha = 1.0f;
					g_Button[i].flag_alpha = true;
				}
			}

			if (g_Button[i].flag_sound == true)
			{
				PlaySound(SOUND_LABEL_SE_ZIPPO);
				g_Button[i].flag_sound = false;
			}

			//�}�E�X�̍��{�^���������ꂽ��
			if (GetKeyState(VK_LBUTTON) & 0x80)
			{
				switch (i) {
				case 0:
					SetFade(FADE_OUT, MODE_GAME);
					//�}�E�X��\��
					ShowCursor(false);
					//PlaySound();
					break;
				case 1:
				{
					int id = MessageBox(NULL, "�Q�[�����I�����܂����H", "", MB_YESNO | MB_ICONQUESTION);
					switch (id)
					{
					case IDYES:		// �Q�[�����I��
						exit(-1);
						break;
					case IDNO:		// ���������Ƀ^�C�g���ɖ߂�

						break;
					}
				}
				break;
				}
			}

		}
		else //�}�E�X���摜�͈̔͊O�Ȃ�_�ł����ɕ\��
		{
			g_Button[i].alpha = 1.0f;
			g_Button[i].flag_sound = true;
		}

	}
	

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		SetFade(FADE_OUT, MODE_TITLE);
	}

	// �Q�[���p�b�h�œ��͏���
	if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}


#ifdef _DEBUG	// �f�o�b�O����\������

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultTex(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// result�\�̕\��
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, 950.0f, 500.0f, 1059*0.8, 1074*0.8, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		GetDeviceContext()->Draw(4, 0);
	}

	// �X�R�A�\��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// ��������������
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float pw = 16 * 4;			// �X�R�A�̕\����
			float ph = 32 * 4;			// �X�R�A�̕\������
			float px = 1200.0f - i * pw;	// �X�R�A�̕\���ʒuX
			float py = 260.0f;			// �X�R�A�̕\���ʒuY

			float tw = 1.0f / 10;		// �e�N�X�`���̕�
			float th = 1.0f / 1;		// �e�N�X�`���̍���
			float tx = x * tw;			// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}
	
	// congratulations�̕\��
	{
		int score = GetScore();
		
		if (score < 1000)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, 950.0f, 850.0f, 889 * 0.5, 80 * 0.5, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}

		if (score >= 1000)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, 950.0f, 850.0f, 1279 * 0.5, 80 * 0.5, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Button[i].texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, g_Button[i].pos.x, g_Button[i].pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO,
			0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, g_Button[i].alpha));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}



}




