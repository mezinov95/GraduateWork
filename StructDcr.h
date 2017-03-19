#ifndef StructDCR_H
#define StructDCR_H

// ��������� ��� ������ � �������
#ifndef mydate
#pragma pack(push,1)
struct mydate {
  short da_year;     /* current year */
  char da_day;     /* day of the month */
  char da_mon;     /* month (1 = Jan) */
};
struct mytime {
  unsigned char  ti_min;   /* minutes */
  unsigned char  ti_hour;  /* hours */
  unsigned char  ti_hund;  /* hundredths of seconds */
  unsigned char  ti_sec;   /* seconds */
};
#pragma pack(pop)
#endif

#define MAXDCR 1  //������������ ���������� �����
#define MAXPM 8  //������������ ���������� ��
#define NCNTR 4  // ���-�� ������������ � ����� ���
#define NCHAN 4  // ���-�� ������� � ����� �����������
#define NGROUP8 76 // ����������� ��������� ���-�� ����� �� 8 ���
#define NGROUP16 38  // � ������ ������������ 38 ����� �� 16 ���
#define NGROUP64 10 // ����������� ��������� ���-�� ����� �� 64 ����

#pragma pack(push,1)
struct CONFIG_FROMPM
{
	char wrk_cntr; // NCNTR-����: 1-������ ����������, 0-���
	char nomer_cntr[NCNTR]; // ������ ������������ �� ����
};
// ��������� ������������ ������ ������
struct CONFIG_ONE
{
	 short include; // 0- �� � ������ , 1- � ������
	 short Nx; // ���-�� ��� � ������� X
	 short Ny; // ���-�� ��� � ������� Y
	 float Xx,Yx,Zx; // ���������� ������� ���� ������ � ������� X
	 float Xy,Yy,Zy; // ���������� ������� ���� ������ � ������� Y
	 float VXx,VYx,VZx; // ������������ ������ � ������� X
	 float VXy,VYy,VZy; // ������������ ������ � ������� Y
};
// ��������� ������������ ����� ������
struct CONFIG_DECOR
{
	  struct CONFIG_ONE cnf[128];
};
// ��������� ������ �� ���������� �����
struct TRGCOMMENT
{
  short tip; //����� ������
  char comment[66]; //�����������
};
struct TRGCNF
{
	unsigned short port; // ����� ���������� �����
	unsigned long interval; //�������� ������� ��� ����� �����
	short frequence; //�������� ������� ���������� � ���
	short stepinterval; //������� ��� ��������� ��������� ����� � ����
	//��������� � �����
	short vorota; //����� ���������� � ����
	short width1; // ������������1 � ����
	short delay1; // ��������1 � ����
	short width2; // ������������2 � ����
	short delay2; // ��������2 � ����
	short width3; // ������������3 � ����
	short delay3; // ��������3 � ����
	short reserv; // ������
	//-----------
	struct TRGCOMMENT Trgcomment[8]; //�������������� ����� ��������
	//��������� � �����
	unsigned char tabl[256]; //������� �������� ���������� �������
};
// ��������� ������ ���������� ��� ��������������� �� ���� PM
struct CMONITORALL
{
		 char id_cmonit; // ������� ��������� ������� ������ �� ��
		 CONFIG_FROMPM cnf[MAXPM];
		 short nbad[MAXPM][NCNTR][NCHAN]; // ���-�� ������� � ��������
};
// ��������� ������ �� ����� �� ����� PM
struct CNOISE
{
	  char maskacntr; // NCNTR-����: 0-��� ������ �����������, 1-����
	  short noisebuf[NCNTR*NCHAN]; // ������������ ������ �� NCHAN �����
											// �� ������ �� ������� ��� � maskacntr
};
// ��������� ������ ����� ������� �� ���� PM, ��� ����� ������������ �� ����.
struct CEVENTALL
{
// ��������������� ����� ������������ ���������� � ������������ ������
		char start[6];  // �������� ����� ������ ������
		short	Tip;	// ��� ������:0-Config,1-�������,2-Experement event,3-Noise
		unsigned long Nrun; // ����� �������� ����
		unsigned long Nevent; // ����� �������� �������
		unsigned char  hund;  // hundredths of seconds
		unsigned char  sec;   // seconds
		unsigned char  min;   // minutes
		unsigned char  hour;  // hours
		char day;     // day of the month
		char mon;     // month (1 = Jan)
		short year;     // current year
		unsigned long mcntr; //32 ���� : 0-�� ����������� , 1-�����������
		short len; // ����� ������� ������ � ������

	unsigned short triger; // ���������� �� ��������
	unsigned long Weit_time; // ����� �������� ����� ������� � �������� step
	unsigned char history[2][16]; // ����������� �� 1/freq(� �������� �������)
	unsigned short counter[2][8]; // ����� ����� �� 8-�� ��-��� � �������� 8*step
	unsigned char maska_lam_chan[16]; // �� NCHAN-���� ����� LAM�� �� ��� PM
	unsigned long Maska_Len_maska_masok; // 32-���� : 0 maska_masok- char
													 //          1 maska_masok- short
	unsigned long maskacntr; //32 ���� : 0-��� ������ , 1-����
	short len_alldan; // ����� ������������ ����������� �������
	unsigned char buf[1514*8]; // ������������ ������
};

// ��������� ������ ������ � DECOR_MAIN
struct STR_DECOR {
	long err_oper; // ����� ������ ����������� ��� ���������� ��������
					  //���� err_oper&64 != 0, �� ������ ���������� �����
					  // � ���������� ���������� ���
	short maska_packet_err; //�� ���� ���� "ERROR"
	short maska_packet_bad; // ����� ����������� �������
	//������� ������������
	short pm_config; // ��������� ������������ ����
	short PM_work; // ������� ����� ������� �����
	short PM_exp; // ������� ����� ����������� � ������������ PM
	short id_cinit[MAXPM]; // ��������� ������� ������
	short len_cinit[MAXPM]; // ����� ������
	struct CONFIG_FROMPM CInit[MAXPM];
	//����� ����
	long Nrun; // ����� ������� ���������
	//���������������� ������
	unsigned long configcntr; // ����� ������������ �� ������������
	struct CONFIG_DECOR CNF; // ��������� ������������ ����� ������
	struct TRGCNF Trgcnf[2]; //������������ ��� ���������� �����
	//����������
	unsigned long configmonit; // ����� ������������ �� �����������
	struct CMONITORALL CMonitAll;
	//����
	unsigned short counter[2][8]; //���� ��������� �� 1 ���
	unsigned long confignoise; // ����� ������������ �� �����
	short id_cnoise[MAXPM]; // ��������� ������� ������
	short len_cnoise[MAXPM]; // ����� ������
	struct CNOISE CNoise[MAXPM];
	//�������
	unsigned long configevent; // ����� ������������ ������������ �� �������
	short len_ceventall; // ����� ������
	struct CEVENTALL CEventAll; // ��������� ������ ������� ������
					  };
#pragma pack(pop)

#pragma pack(push,1)

struct CNF_SET{
    //��� ������������ ����������� �������
    short id_gscreen; //��������� ���������� � ����������� ������
    short id_gscrbuf; //��������� ������������� ���������� ������� ������
    short id_gnoise; //��������� ������� ������ �� �����
    unsigned short noise_chan[NCNTR*MAXPM][NCHAN]; // ���� ��� ������ �� �����
    //��� ����������
    short id_SetN; // ������� ���������� N8,N16,N64,Nread;
    short N8[NCNTR*MAXPM]; // ���-�� ����� �� 8 ���
    short N16[NCNTR*MAXPM]; // ���-�� ����� �� 16 ���
    short N64[NCNTR*MAXPM]; // ���-�� ����� �� 64 ����
    short Nread[NCNTR*MAXPM][NCHAN]; // ���-�� ������ ���� ������ �� ������ ������
           };

#pragma pack(pop)

extern struct CNF_SET CNF_S;
extern struct STR_DECOR *DcrF;
extern short id_eventDCR;//������� ����, ��� ���� �������� ������ ������� �� �����

#endif
