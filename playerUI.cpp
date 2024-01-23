//�v���C���[UI�\����cpp
//=============================================================================

#include "main.h"
#include "camera.h"
#include "playerUI.h"

#include "Player.h"
#include "bullet.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//****************************************************************************
#define TEXTURE_MAX			(10)								// �e�N�X�`���̐�
#define PLAYER_UI_MAX		(1)								// �v���C���[��UI�̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
playerUI g_Hit;

static const char* g_TexturName[TEXTURE_MAX] = {
	"Data/texture/bar_white.png",
	"Data/texture/ball_white.png",
	"Data/texture/capture.png",
	"Data/texture/attach.png",
	"Data/texture/collect.png",
	"Data/texture/meter_base.png",
	"Data/texture/reticle.png",
	"Data/texture/meter_base.png",
	"Data/texture/reroad.png",
	"Data/texture/hit.png",
};

HRESULT InitPlayerUI(void)
{
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

	return S_OK;
}

//=============================================================================
// // �A�^�b�`�\������
//=============================================================================
void DrawAttach(void)
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

	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = 1000.0f;						// �Q�[�W�̕\���ʒuX
	float py = 150.0f;						// �Q�[�W�̕\���ʒuY
	float pw = 400.0f;						// �Q�[�W�̕\����
	float ph = 200.0f;						// �Q�[�W�̕\������

	float tw = 1.0f;						// �e�N�X�`���̕�
	float th = 1.0f;						// �e�N�X�`���̍���
	float tx = 0.0f;						// �e�N�X�`���̍���X���W
	float ty = 0.0f;						// �e�N�X�`���̍���Y���W

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

	if (player->Aalpha >= 0.0f)
	{
		player->Aalpha -= 0.01;
	}


	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Aalpha));


	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// // �L���v�`���[�\������
//=============================================================================
void DrawCapture(void)
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

	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = 1000.0f;						// �Q�[�W�̕\���ʒuX
	float py = 150.0f;						// �Q�[�W�̕\���ʒuY
	float pw = 400.0f;						// �Q�[�W�̕\����
	float ph = 200.0f;						// �Q�[�W�̕\������

	float tw = 1.0f;						// �e�N�X�`���̕�
	float th = 1.0f;						// �e�N�X�`���̍���
	float tx = 0.0f;						// �e�N�X�`���̍���X���W
	float ty = 0.0f;						// �e�N�X�`���̍���Y���W

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	if (player->Calpha >= 0.0f)
	{
		player->Calpha -= 0.01;
	}


	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Calpha));


	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// �R���N�g�\������
//=============================================================================
void DrawCollect(void)
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

	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = 670.0f;						// �Q�[�W�̕\���ʒuX
	float py = 150.0f;						// �Q�[�W�̕\���ʒuY
	float pw = 600.0f;						// �Q�[�W�̕\����
	float ph = 200.0f;						// �Q�[�W�̕\������

	float tw = 1.0f;						// �e�N�X�`���̕�
	float th = 1.0f;						// �e�N�X�`���̍���
	float tx = 0.0f;						// �e�N�X�`���̍���X���W
	float ty = 0.0f;						// �e�N�X�`���̍���Y���W

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

	if (player->C2alpha >= 0.0f)
	{
		player->C2alpha -= 0.01;
	}


	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->C2alpha));


	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// �q�b�g�\������
//=============================================================================
void DrawHit(void)
{
	PLAYER* player = GetPlayer();
	BULLET* bullet = GetBullet();
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (bullet[i].flag_Halpha)
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

			//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = 670.0f;					// �Q�[�W�̕\���ʒuX
			float py = 150.0f;						// �Q�[�W�̕\���ʒuY
			float pw = 600.0f;						// �Q�[�W�̕\����
			float ph = 200.0f;						// �Q�[�W�̕\������

			float tw = 1.0f;						// �e�N�X�`���̕�
			float th = 1.0f;						// �e�N�X�`���̍���
			float tx = 0.0f;						// �e�N�X�`���̍���X���W
			float ty = 0.0f;						// �e�N�X�`���̍���Y���W

			if (bullet[i].flag_Halpha)
			{
				bullet[i].Halpha -= 0.01f;
			}

			else
			{
				bullet[i].Halpha = 1.0f;
			}

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

			SetSpriteLTColor(g_VertexBuffer,
				px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, bullet[i].Halpha));


			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// �����[�h�\������
//=============================================================================
void DrawReload(void)
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

	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
	float reroad_x = 975.0f * 0.2;
	float reroad_y = 250.0f * 0.2;

	float px = SCREEN_CENTER_X - reroad_x/2;						// �Q�[�W�̕\���ʒuX
	float py = SCREEN_CENTER_Y - reroad_y/2 - 200.0f;						// �Q�[�W�̕\���ʒuY
	float pw = reroad_x;						// �Q�[�W�̕\����
	float ph = reroad_y;						// �Q�[�W�̕\������

	float tw = 1.0f;						// �e�N�X�`���̕�
	float th = 1.0f;						// �e�N�X�`���̍���
	float tx = 0.0f;						// �e�N�X�`���̍���X���W
	float ty = 0.0f;						// �e�N�X�`���̍���Y���W

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, player->Ralpha));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// // �v���C���[��UI�\������
//=============================================================================
void DrawPlayerUI(void)
{
	for (int i = 0; i < PLAYER_UI_MAX; i++)
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

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = 40.0f;						// �Q�[�W�̕\���ʒuX
		float py = 125.0f + (20.0f * i);		// �Q�[�W�̕\���ʒuY
		float pw = 8.0f;						// �Q�[�W�̕\����
		float ph = 350.0f;						// �Q�[�W�̕\������

		float tw = 50.0f;						// �e�N�X�`���̕�
		float th = 1.0f;						// �e�N�X�`���̍���
		float tx = 0.0f;						// �e�N�X�`���̍���X���W
		float ty = 0.0f;						// �e�N�X�`���̍���Y���W

		PLAYER* player = GetPlayer();

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		SetSpriteLTColor(g_VertexBuffer,
			px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// �v���C���[��HP�ɏ]���ăQ�[�W�̒�����\��
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		{
			ph = ph * (player->str / player->str_max);
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		}
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}
//=============================================================================
// // �v���C���[�̎c�e�\������
//=============================================================================
void DrawPlayerRestBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		BULLET* bullet = GetBullet();
		if (bullet[i].object.draw)continue;
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

			//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = 70.0f;		// �Q�[�W�̕\���ʒuX
			float py = 125.0f + (37.5f * i);		// �Q�[�W�̕\���ʒuY
			float pw = 30.0f;		// �Q�[�W�̕\����
			float ph = 30.0f;		// �Q�[�W�̕\������

			float tw = 1.0f;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = 0.0f;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			// �v���C���[��HP�ɏ]���ăQ�[�W�̒�����\��
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			}
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// // �Z���^�[���e�B�N���\������
//=============================================================================
void DrawReticle(void)
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

	float reticle_size = 800.0f * 0.4;			//	���e�B�N���̃T�C�Y

	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = SCREEN_CENTER_X - reticle_size / 2;				// �Q�[�W�̕\���ʒuX
	float py = SCREEN_CENTER_Y - reticle_size / 2;				// �Q�[�W�̕\���ʒuY
	float pw = reticle_size;						// �Q�[�W�̕\����
	float ph = reticle_size;						// �Q�[�W�̕\������

	float tw = 1.0f;						// �e�N�X�`���̕�
	float th = 1.0f;						// �e�N�X�`���̍���
	float tx = 0.0f;						// �e�N�X�`���̍���X���W
	float ty = 0.0f;						// �e�N�X�`���̍���Y���W

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}

//=============================================================================
// // �p�����[�^�\������
//=============================================================================
void DrawMeterBase(void)
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

	float meter_size = 1275.0f * 0.4;			//	���e�B�N���̃T�C�Y

	//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = SCREEN_CENTER_X - meter_size / 2;				// �Q�[�W�̕\���ʒuX
	float py = SCREEN_CENTER_Y - meter_size / 2;				// �Q�[�W�̕\���ʒuY
	float pw = meter_size;						// �Q�[�W�̕\����
	float ph = meter_size;						// �Q�[�W�̕\������

	float tw = 1.0f;						// �e�N�X�`���̕�
	float th = 1.0f;						// �e�N�X�`���̍���
	float tx = 0.0f;						// �e�N�X�`���̍���X���W
	float ty = 0.0f;						// �e�N�X�`���̍���Y���W

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

	SetSpriteLTColor(g_VertexBuffer,
		px - 2.5f, py - 2.5f, pw + 5.0f, ph + 5.0f,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}


playerUI* GetHit(void)
{
	return &g_Hit;
}