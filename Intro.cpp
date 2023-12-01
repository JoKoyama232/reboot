//=============================================================================
// ������ʏ��� [Intro.cpp]
// Author : ���R�@��
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "input.h"
#include "fade.h"

// ���g�̃C���N���[�h	
#include "Intro.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
enum IntroTexture
{
	background = 0,
	introBackground,
	logo,
	credit,
	IntroTextureMAX
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[IntroTextureMAX] = { NULL };	// �e�N�X�`�����

static char* g_TextureName[IntroTextureMAX] = { // �e�N�X�`���t�@�C���A�h���X�z��
	(char*) "Data/texture/sunrise.jpg",
	(char*) "Data/texture/fade_black.png",
	(char*) "Data/texture/Logo.png",
	(char*) "Data/texture/myName.png",

};

static BOOL		g_Use;					// TRUE:�g���Ă���  FALSE:���g�p
static float	g_w, g_h;				// ���ƍ���
static XMFLOAT3	g_Pos;					// �|���S���̍��W
static int		g_TexNo;				// �e�N�X�`���ԍ�
static SPRITE	sprite[IntroTextureMAX];	// �e�N�X�`���z��
static DWORD	timer;					// �^�C�}�[�i���Ԑ���̂��߁j
static bool		g_Load = false;			// ���[�h�t���O

//=============================================================================
// ����������
//=============================================================================
HRESULT InitIntro(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < IntroTextureMAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);

		sprite[i].pos = XMFLOAT2(0.0f, 0.0f);
		sprite[i].size = XMFLOAT2(0.0f, 0.0f);
		sprite[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		sprite[i].use = true;
		sprite[i].useColor = false;
	}

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_w = SCREEN_WIDTH;
	g_h = SCREEN_HEIGHT;

	sprite[introBackground].pos = { g_w * 0.5f, g_h * 0.5f };
	sprite[introBackground].size = { g_w, g_h };
	sprite[introBackground].useColor = true;
	sprite[introBackground].use = true;

	sprite[logo].pos = { g_w * 0.55f, g_h * 0.5f };
	sprite[logo].size = { g_w * 0.5f,  g_h * 0.31f };
	sprite[logo].useColor = true;
	sprite[logo].color.w = 0.0f;
	sprite[logo].use = true;

	sprite[credit].pos = { g_w * 0.5f,  g_h * 0.96f };
	sprite[credit].size = { g_w * 0.21f,  g_h * 0.019f };
	sprite[credit].useColor = true;
	sprite[credit].color.w = 0.0f;
	sprite[credit].use = true;

	sprite[background].pos = { g_w * 0.5f, g_h * 0.5f };
	sprite[background].size = { g_w, g_h };

	timer = timeGetTime();
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitIntro(void)
{
	if (g_Load == false) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < IntroTextureMAX; i++)
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
void UpdateIntro(void)
{
	DWORD time = timeGetTime() - timer;
	if (GetKeyboardTrigger(DIK_RETURN)) { time = 8001; }
	PrintDebugProc((char*)"Timer = %d\n", time);

	if (time > 8000)
	{
		sprite[logo].use = false;
		sprite[credit].use = false;
		sprite[introBackground].use = false;
		setLoad(false);
		
		// �C���g�����I���΃^�C�g���ֈڍs
		SetFade(FADE_OUT, MODE_TITLE);
	}
	else if (time > 7000)
	{
		sprite[logo].color.w = 1 - (time - 7000) * 0.001f;
		sprite[credit].color.w = 1 - (time - 7000) * 0.001f;
		sprite[introBackground].color.w = 1 - (time - 7000) * 0.001f;
		return;
	}
	else if (time > 3000)
	{
		sprite[credit].color.w = (time - 3000) * 0.001f;
	}
	else if (time > 1000)
	{
		sprite[logo].color.w = (time - 1000) * 0.0005f;
	}

	if (GetKeyState(VK_LBUTTON) & 0x80)
	{	
		setLoad(false);
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawIntro(void)
{
	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(false);

	// ���C�e�B���O�𖳌�
	SetLightEnable(false);

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

	for (int i = 0; i < IntroTextureMAX; i++)
	{
		if (!sprite[i].use)
			continue;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);

		if (sprite[i].useColor) {
			SetSpriteColor(g_VertexBuffer, sprite[i].pos.x, sprite[i].pos.y, sprite[i].size.x, sprite[i].size.y, 0.0f, 0.0f, 1.0f, 1.0f, sprite[i].color);
		}
		else {
			SetSprite(g_VertexBuffer, sprite[i].pos.x, sprite[i].pos.y, sprite[i].size.x, sprite[i].size.y, 0.0f, 0.0f, 1.0f, 1.0f);
		}
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// ���C�e�B���O��L����
	SetLightEnable(true);

	// Z��r����
	SetDepthEnable(true);
}










