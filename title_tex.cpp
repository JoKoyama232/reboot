//=============================================================================
//
// �^�C�g���e�N�X�`������ [title_tex.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
//#include "collision.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title_tex.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�
#define BUTTON_MAX					(3)


#define TEXTURE_WIDTH_LOGO			(240)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(40)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����


static const char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title_logo.png",
	"data/TEXTURE/game_start.png",
	"data/TEXTURE/exit_2.png",
	"data/TEXTURE/option.png",
};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

float	alpha;
bool	flag_alpha;

static BOOL						g_Load = FALSE;
static BUTTON					g_Button[BUTTON_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitleTex(void)
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
	g_h = TEXTURE_HEIGHT;
	g_w = TEXTURE_WIDTH;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = true;

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i].use = false;
		g_Button[i].texNo = 1 + i;
		g_Button[i].h = TEXTURE_HEIGHT;
		g_Button[i].w = TEXTURE_WIDTH;
		g_Button[i].pos = XMFLOAT3(g_Button[i].w / 2, g_Button[i].h / 2 + 100 + (50.0f * i), 0.0f);

		g_Button[i].alpha = 1.0f;
		g_Button[i].flag_alpha = true;
		g_Button[i].flag_sound = true;

	}


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitleTex(void)
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
void UpdateTitleTex(void)
{
	int x = GetMousePosX();
	int y = GetMousePosY();

	for (int i{}; i < BUTTON_MAX; i++)
	{
		HWND windowHandle = GetForegroundWindow(); // �E�B���h�E�̃n���h�����擾
		if (windowHandle != NULL)
		{
			RECT windowRect;
			if (GetWindowRect(windowHandle, &windowRect)) // �E�B���h�E�̈ʒu�����擾
			{
				// �}�E���̈ʒu���摜�ɓ������Ă��邩�ǂ����̔���
				if ((x > g_Button[i].pos.x - TEXTURE_WIDTH_LOGO / 2) &&
					(x < g_Button[i].pos.x + TEXTURE_WIDTH_LOGO / 2) &&
					(y > g_Button[i].pos.y - TEXTURE_HEIGHT_LOGO / 2) &&
					(y < g_Button[i].pos.y + TEXTURE_HEIGHT_LOGO / 2))
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

					//�}�E�X�̍��{�^���������ꂽ��
					if (GetKeyState(VK_LBUTTON) & 0x80)
					{
						if (i == 0)
						{
							SetFade(FADE_OUT, MODE_RESULT);
							//PlaySound();
						}
						else if (i == 1)
						{
							exit(-1);
						}
						else if (i == 2)
						{

						}
					}




				}
				else //�}�E�X���摜�͈̔͊O�Ȃ�_�ł����ɕ\��
				{
					g_Button[i].alpha = 1.0f;
					g_Button[i].flag_sound = true;
				}

				
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitleTex(void)
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
		//�^�C�g���̃��S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
			SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y - 50.0f,
				TEXTURE_WIDTH_LOGO * 2.5f, TEXTURE_HEIGHT_LOGO * 2.5f,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		//�{�^����`��
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