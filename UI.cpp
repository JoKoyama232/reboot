//=============================================================================
// UI�\������ [UI.h]
// Author : ��c�P
//=============================================================================
#include "main.h"
#include "sprite.h"
#include "renderer.h"


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
			ph = ph * (g_Player.str / g_Player.str_max);
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
