//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : HOGE FUGA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <xaudio2.h>

class CSound
{
public:
	CSound();
	~CSound();
	//*****************************************************************************
	// �T�E���h�t�@�C��
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_SE_SHOT = 0,		// BGM0
		SOUND_LABEL_SE_EXPLOSION,		// �e���ˉ�
		SOUND_LABEL_SE_RECOVERY, //�񕜉�
		SOUND_LABEL_SE_SELECT,//���j���[�I����
		SOUND_LABEL_SE_ENTER,//���艹
		SOUND_LABEL_SE_WAON,//�킨�[��
		SOUND_LABEL_SE_SYAN,//syan
		SOUND_LABEL_SE_BOMB,//syan
		SOUND_LABEL_SE_PLAYERSHOT,//syan
		SOUND_LABEL_SE_BOSSDEATH,//syan
		SOUND_LABEL_SE_HIKKAKI,//syan
		SOUND_LABEL_BGM_BOSS1,//syan
		SOUND_LABEL_BGM_BOSS2,//syan
		SOUND_LABEL_BGM_DOCYU,//syan
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								    // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			    // �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

};

#endif
