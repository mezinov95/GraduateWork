#ifndef TrgNvdH
#define TrgNvdH

struct TRGEXPOZITION { // ����������, ������
	unsigned short STATUS_L; // ��������� �������
	unsigned short STATUS_H;

	unsigned short TIME_L;	// ������� ������ ������� � ��� ������� ������
	unsigned short TIME_H;

	unsigned short AREG_L;   // �������� ������� ��������
	unsigned short AREG_H;
	unsigned short BREG_L;
	unsigned short BREG_H;
	unsigned short DREG_L;
	unsigned short DREG_H;
	unsigned short EREG_L;
	unsigned short EREG_H;


	unsigned short S_TM_L;	//�����, �������
	unsigned short S_TM_H;
	unsigned short NS_TM_L;  // �����, �����������
	unsigned short NS_TM_H;
	unsigned short NS_COR_L; // ���������, �����������
	unsigned short NS_COR_H;
	unsigned short S_SET_L;  // ��������� �������, �������
	unsigned short S_SET_H;


	unsigned short A_RS_FIFO[128];  // FIFO ��������� ������� ��������

	};

struct TRGNOISE {     // ��������� ������ ����
	unsigned short SC_FIFO[128];  // FIFO ��������� ������

	unsigned short TIME_L;	// ������� ������ ������� ������
	unsigned short TIME_H;

	unsigned short CONTROL_L;  // ����������� ������� ������/������
	unsigned short CONTROL_H;
};

struct TRGPORTS {
	unsigned short GCTRL;     // �������� ���������� ������� ������

	unsigned short DCTRL_L;
	unsigned short DCTRL_H;
	unsigned short DDATA_L;
	unsigned short DDATA_H;

	unsigned short ECTRL_L;
	unsigned short ECTRL_H;
	unsigned short EDATA_L;
	unsigned short EDATA_H;

	unsigned short FCTRL_L;
	unsigned short FCTRL_H;
	unsigned short FDATA_L;
	unsigned short FDATA_H;

	unsigned short DIDCODE; // �������� ����� �������������� ���� ���������� ������
	unsigned short EIDCODE;
	unsigned short FIDCODE;
};

struct TRGTIMERS {   // ��������� ��������
	unsigned short GATEWIDTH;   // ��������� ������ ����� ��������
	unsigned short MODE;      // ������ ������� ������

	unsigned short PDL_CTRL;   // ���������� ����������, ������ ������
	unsigned short PDL_DATA;
};

struct TRGTESTREG {  // �������� ��������
	unsigned short CCTRL_L;   // ������� ����� � ������
	unsigned short CCTRL_H;

	unsigned short SCRATCH;   // �������� ������� ������ ������

	unsigned short REVISION;  // ����������������� ����� �������� ������

	unsigned short TST_FIFO[128];	//�������� FIFO ������ ������
};

struct TRGMASKS {  // �������� �����, ������
	unsigned short AMASK_L;
	unsigned short AMASK_H;
	unsigned short BMASK_L;
	unsigned short BMASK_H;
	unsigned short DMASK_L;
	unsigned short DMASK_H;
	unsigned short EMASK_L;
	unsigned short EMASK_H;
};
#pragma pack(push,1)
typedef struct tagCONFIG_TRG
{
   unsigned short index; //���������� ����� � ������������
   unsigned long base; //������� ����� (���� 0, �� �� ������������)
   char name[8]; //��� ��������
   unsigned short idmaster;// 1- ��������� �������, �� ���� ����� �������
   unsigned short outmask; //����� ������� ���������
   unsigned short porsum;  //��������� �����
   unsigned short gatew;   //������ ����� // ��� ��� ������ ����� �������� �� ��������� ����������
   unsigned long timernoise; //����� ��������� ����� � �������� 100 ����
                             //1 ������� = 10 000 000
   unsigned short idmaska;// 0-�� �������, 1- �� ������������ ���, 2-�� �������
   unsigned short mode; //����� ������ �����
   unsigned short d1; //�������� 1 � �������� 10 ����
   unsigned short d2; //�������� 2 � �������� 10 ����
   unsigned short d3; //�������� 3 � �������� ����
   unsigned short d4; //�������� 4 � �������� ����
   unsigned short TrgMaska[8]; //128 ��� ����� ������� ��������
   unsigned short gateUD; //������ ����� �������� ������� ����� ����������� ���
   unsigned short porsumU; //��������� ����� ��� ������� ���������� ���
   unsigned short porsumD; //��������� ����� ��� ������ ���������� ���
}CONFIG_TRG;
#pragma pack(pop)

extern CONFIG_TRG CfgTrig[8]; //������������ ���������� ����
extern TRGEXPOZITION ExpTrigFull[8]; //������ ������� � ���������� ����

//���������� ������
extern void ExitTrgNvd();
//������������� ���������� �������
extern int InitTrigNvd();
//�������� ������� �������, ���������� ���������� ����� �������
extern int CheckTrgNvd(TRGEXPOZITION &exptrig);
//������ ����������� ���������� �� �������
extern int ReadEventTrgNvd();
//����� ��������, ����� ��������� �������
extern int ClearTrgNvd();
//����� � ��������� ��������, ����� �� �������
extern int ResetTrgNvd();
//��������� ��������, ����� �� �������, ��� �������� � ���������������
extern int SetTrgNvd();
//����� ��������� ����� ���������� �������� �� ����� timeout
extern int NoiseTrgNvd(int timeout);

#endif