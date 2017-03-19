#ifndef StructNVD_H
#define StructNVD_H
#include "config.h"
#include "trignvd.h"
//��������� ��� ���������� ������ �� �����
#pragma pack(push,1)
//��������� ��� �������� ���� � ������� � ����� ������� ���
//��������� �� ��������� ������, �����
#ifndef dTDateTimeKadr
#define dTDateTimeKadr
struct TDateTimeKadr
{
 union{
  struct{
  unsigned char hsecond; //����� �������
  unsigned char second;  //�������
  unsigned char minute;  //������
  unsigned char hour; //���
  }tm;
  unsigned long time;
 };
 union{
  struct{
  unsigned char day; //����
  unsigned char month; //�����
  unsigned short year; //���
  }dt;
  unsigned long date;
 };
};
#endif
#pragma pack(pop)

//���� ���������� �������� � ��
struct SNoiseTrigNvd
{
  unsigned short NoiseA[128]; //�� ���������
  unsigned short NoiseB[128]; //�� ���������
  unsigned short NoiseC[128]; //�� ���������
  unsigned short NoiseSKT[128]; //�� ���������
};
#pragma pack(push,1)
//������ ����������� ������ ����
struct SMonADC
{
   unsigned short ToSave; //���� ������� ����� ������, 1 - ���� �� ���������.
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned short maskapmt[4];  //[ksm] ����� ��� ���������
   unsigned short nzap; //�o�������� �������� ���������
   unsigned short nsum[4][6][2]; //[ksm][pmt][dinod12, dinod9] ���-�� ��������� � ������� �����������
   float sred[4][6][2]; //[ksm][pmt][dinod12, dinod9] ����������
   float sigma[4][6][2]; //[ksm][pmt][dinod12, dinod9] ����� �����������
};
struct SMonADC_S //��� ��������
{
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned char maskapmt[4];  //[ksm] ����� ��� ���������
   unsigned short nzap; //�o�������� �������� ���������
   unsigned short nsum[4][6][2]; //[ksm][pmt][dinod12, dinod9] ���-�� ��������� � ������� �����������
   short sred[4][6][2]; //[ksm][pmt][dinod12, dinod9] ���������� *10
   short sigma[4][6][2]; //[ksm][pmt][dinod12, dinod9] ����� ����������� *10
};

struct SMonShumTV
{
   unsigned short ToSave; //���� ������� ����� ������, 1 - ���� �� ���������.
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned short maskapmt[4];  //[ksm] ����� ��� ���������
   float noisePMT[4][6]; //[ksm][pmt] ���� ��� � ���
   float tbek[4];//����������� � ���: Tout, T3,T2,T1
   float vbek[5]; //����������  � ���: V1,V2,Vcc,V3,V4
};
struct SMonShumTV_S //��� ��������
{
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned char maskapmt[4];  //[ksm] ����� ��� ���������
   unsigned short noisePMT[4][6]; //[ksm][pmt] ���� ��� � ��
   unsigned short tbek[4];//����������� � ���: Tout, T3,T2,T1  *10
   short vbek[5]; //����������  � ���: V1,V2,Vcc,V3,V4 *100
};

struct SMonBek
{
   unsigned short ToSave; //���� ������� ����� ������, 1 - ���� �� ���������.
   TDateTimeKadr dt; //����� ��������� ����� ���������(UTC)
   unsigned short maskaTrA; //����� ��� ��������� ����� �������� A
   unsigned short maskaTrB; //����� ��� ��������� ����� �������� B
   unsigned short maskaTrC; //����� ��� ��������� ����� �������� C
   unsigned short noiseTrA[4]; //[ksm] ���� ����������� ������� A
   unsigned short noiseTrB[4]; //[ksm] ���� ����������� ������� B
   unsigned short noiseTrC[4]; //[ksm] ���� ����������� ������� C
};
struct SMonBek_S //��� ��������
{
   TDateTimeKadr dt; //����� ��������� ����� ���������(UTC)
   unsigned short noiseTrA[4]; //[ksm] ���� ����������� ������� A
   unsigned short noiseTrB[4]; //[ksm] ���� ����������� ������� B
   unsigned short noiseTrC[4]; //[ksm] ���� ����������� ������� C
};

struct SEvtBek
{
  short id_bek[2]; //[0]-��������� ������� ������ ������� ������� ���, [1]- ���(0) ��� ���(1)
  short maskaksm; //����� ������������ KSM
  short maskahit[4]; //[ksm] ����� ����������� ���
  unsigned short acp[4][6][2]; //[ksm][pmt][12d,9d] ���� ���
  unsigned short fifoA[4];//[ksm]
  unsigned short fifoB[4];
  unsigned short fifoC[4];
  unsigned short maskatrig[4]; //[ksm] ����� ����� ���������, � ������� ��� ����������, ���:0-A,1-B,2-C
};
#pragma pack(pop)

#pragma pack(push,1)
//������ ����������� ������ ����
struct SMonADC_SCT
{
   unsigned short ToSave; //���� ������� ����� ������, 1 - ���� �� ���������.
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned char maskapmt[8];  //[ksm] ����� ��� ���������
   unsigned short nzap; //�o�������� �������� ���������
   unsigned short nsum[8][5]; //[ksm][pmt] ���-�� ��������� � ������� �����������
   float sred[8][5]; //[ksm][pmt] ����������
   float sigma[8][5]; //[ksm][pmt] ����� �����������
};
struct SMonADC_SCT_S //��� ��������
{
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned char maskapmt[8];  //[ksm] ����� ��� ���������
   unsigned short nzap; //�o�������� �������� ���������
   unsigned short nsum[8][5]; //[ksm][pmt] ���-�� ��������� � ������� �����������
   short sred[8][5]; //[ksm][pmt] ����������  *10
   short sigma[8][5]; //[ksm][pmt] ����� ����������� *10
};

struct SMonShumTV_SCT
{
   unsigned short ToSave; //���� ������� ����� ������, 1 - ���� �� ���������.
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned char maskapmt[8];  //[ksm] ����� ��� ���������
   float noisePMT[8][5]; //[ksm][pmt] ���� ��� � ���
   float tbek[4];//����������� � ���: Tout, T3,T2,T1
   float vbek[5]; //����������  � ���: V1,V2,Vcc,V3,V4
};
struct SMonShumTV_SCT_S //��� ��������
{
   TDateTimeKadr dt; //���� � ����� ��������� (UTC)
   unsigned char maskapmt[8];  //[ksm] ����� ��� ���������
   unsigned short noisePMT[8][5]; //[ksm][pmt] ���� ��� � ��
   unsigned short tbek[4];//����������� � ���: Tout, T3,T2,T1 *10
   short vbek[5]; //����������  � ���: V1,V2,Vcc,V3,V4 *100
};

struct SMonBep
{
   unsigned short ToSave; //���� ������� ����� ������, 1 - ���� �� ���������.
   TDateTimeKadr dt; //����� ��������� ����� ���������(UTC)
   unsigned short maskaTrSCT[8]; //[ksm] ����� ��� ��������� ����� �������� ���
   unsigned short noiseTrSCT[8][5]; //[ksm][pmt] ���� ����������� ������� ���
};
struct SMonBep_S //��� ��������
{
   TDateTimeKadr dt; //����� ��������� ����� ���������(UTC)
   unsigned short noiseTrSCT[8][5]; //[ksm][pmt] ���� ����������� ������� ���
};

struct SEvtBep
{
  short id_bek[2]; //[0]-��������� ������� ������ ������� ������� ���, [1]- ���(0) ��� ���(1)
  short maskaksm; //����� ������������ KSM
  unsigned char maskahit[8]; //[ksm] ����� ����������� ���
  unsigned short acp[8][5]; //[ksm][pmt] ���� ���
  unsigned short fifoSCT[8][5];//[ksm][pmt]
  unsigned char maskatrig[8]; //[ksm] ������� �����, ������������� � �������� ��� ���������
};
#pragma pack(pop)

//��������� ��� ������ ������� �� ����
#define HDR_W 0 //��� ������ ������������ � ����������� �����
  #define ID_CONFIG 0  //������������ �����
  #define ID_MONIT 1   //���������� �����
  #define ID_EVENT 2   // �� ������������
  #define ID_NOISE 3   // ���� �����
#define CFGNVD_W 1
#define MONPDS_W 2
#define MONPDSL_W 3
#define MONAMPL_W 4
#define MONSHUMTV_W 5
#define MONBEK_W 6
#define EVENT_W 7
#define MONPDS_SCT_W 8
#define MONSHUMTV_SCT_W 9
#define MONBEP_W 10

#pragma pack(push,1)
struct HEADER_REC
{
   char start[5]; // ����� "start" - ����� ������ ������.
   unsigned char tip_zap; //��� ������:
                          // =0,1 - ������������ ������.
                          // =2,3,4,5,6 - ������ ��������������� ������.
                          // =7 - ������ �������.
   TDateTimeKadr datetime; //���� � ����� (UTC)
   unsigned long lendata; //����� ��������� �� ���������� ������ � ������
                          // ��� ��� ����� ��� tip_zap==0 ������������� ID_CONFIG...ID_NOISE
};
struct SCONFIG_DAT  //��� ������ 1
{
   SCFGBEK CfgBek[32]; //������������ ��� � ������
   CONFIG_TRG CfgTrig[8]; //������������ ���������� ����
   SCFGBEP CfgBep[2]; //������������ ��� � ������
};
struct SCONFIG_DAT_S  //��� �������
{
   SCFGBEK_S CfgBek[25]; //������������ ��� � ������
   SCFGBEP_S CfgBep[2]; //������������ ��� � ������
};

struct SMONIT_DAT
{
   unsigned long MaskaBek; // ������� ����� �������������� � ������ ��� ��� ���
   short NBek; //���������� �������������� � ������ ��� ��� ���
   //��� ������ ��������������� ��� ����� ���� MonPds,MonPdsL,MonAmpL, MonShumTV ��� MonBek
   union
   {
     SMonADC MonPds[32]; //2 ���������� ����������� ����������� ���
     SMonADC MonPdsL[32]; //3 ���������� ����������� ����������� ��� � ������ ������� ���������
     SMonADC MonAmpL[32]; //4 ���������� ����������� ��������� ���
     SMonShumTV MonShumTV[32]; //5 ���������� ����������� ����� ���, ����������� � ���������� ���
     SMonBek MonBek[32]; //6 ���������� ����������� ��������� �����
     SMonADC_SCT MonPds_SCT[2];//9 ���������� ����������� ����������� ���
     SMonShumTV_SCT MonShumTV_SCT[2];//10 ���������� ����������� ����� ���, ����������� � ���������� ���
     SMonBep MonBep[2]; //8 ���������� ����������� ��������� �����
   }data;
};
struct SEVENTBEK_DAT
{
   unsigned long exp_count; //- 4 ����� : ����� �������.
   unsigned long Nrun; //- 4 ����� : ����� ����.
   unsigned short trgnvd;   // - 2 �����: �������� ������
   short Nlam ;             //- 2 ����� : ���-�� ����������� �������.
   short NlamSKT ;             //- 2 ����� : ���-�� ����������� ������� ���.
   short NfifoA ;             //- 2 ����� : ���-�� ������� � FIFO TrA.
   short NfifoB ;             //- 2 ����� : ���-�� ������� � FIFO TrB.
   short NfifoC ;             //- 2 ����� : ���-�� ������� � FIFO TrC.
   short NfifoSKT ;             //- 2 ����� : ���-�� ������� � FIFO TrSKT.
   unsigned long Weit_time; //- 4 ����� : ����� �������� ����� ������� � 100 ����.
   unsigned long All_time[2]; //- [0]=10000000; 100 ���� ��������� �����
                              //  [1] - ��������� ����� ����� �� ������� ��������� � ���
   unsigned long Pressure; //��������
   unsigned long Temperature; //�����������
   unsigned long id_DECOR; //������� ������� � ������ ������ (������)
   unsigned short StatusReg[8][2]; //���������� ��������� ��������� ��� �����
   unsigned long MaskaBek; // ������� ����� �������������� � ������ ���
   unsigned long MaskaBep; // ������� ����� �������������� � ������ ��� (������ ��� ������ ����)
   short NBek; //���������� �������������� � ������ ��� (�� 30 ��� ���)
   short NBep; //���������� �������������� � ������ ��� (�� 2)
   SEvtBek EventBek[32]; //��������������� ������ ������ ������� �� ���
   SEvtBep EventBep[2]; //��������������� ������ ������ ������� �� ���
};
#pragma pack(pop)
struct SStatTrig
{
   unsigned long nsumtriger[9]; // ���-�� ������� � ������������� ����� �������
     			        // [0] - ������ �� ���������
   unsigned long nspeedtriger[9]; // ���-�� �������� � ���������� ���������
    			          //  ������ �������
   double speedtriger[9]; // ������� �������� � ������� ������ �������
   double Nlam[9];   //����� ���-�� ����������� ���
   double NlamSKT[9];   //����� ���-�� ����������� SKT
   double NfifoA[9]; //����� ���-�� ��� FIFO TrA
   double NfifoB[9]; //����� ���-�� ��� FIFO TrB
   double NfifoC[9]; //����� ���-�� ��� FIFO TrC
   double NfifoSKT[9]; //����� ���-�� ��� FIFO TrSKT
   double NlamSred[9];   //������� ���-�� ����������� ���
   double NlamSKTSred[9];   //������� ���-�� ����������� ��� ���
   double NfifoASred[9]; //������� ���-�� ��� FIFO TrA
   double NfifoBSred[9]; //������� ���-�� ��� FIFO TrB
   double NfifoCSred[9]; //������� ���-�� ��� FIFO TrC
   double NfifoSKTSred[9]; //������� ���-�� ��� FIFO TrSKT
};
//��������� ��� ����������� ������� ���� ���������� �� ����������
struct SStatExp
{
   TDateTimeKadr dt; //������� ����� (UTC)
   TDateTimeKadr dtStart; //����� ������� ���� (UTC)
   unsigned long Nrun; //������� ����� ����
   unsigned long exp_count; //������� ����� �������
   unsigned short trgnvd; //������� �������
   unsigned short trgskt; //������� ������� ���
   long num_file; //���������� ����� ����� � ����
   long lenTofile; //���������� ���������� � ����� ������
   double LiveTime; //������ ����� ����� ����
   double FullTime; //������ ����������� ����� ����
   unsigned short NhitSCT[2]; //[ibep] ���������� ����������� ��� ���
   double SumAmpSCT[2]; //[ibep] ��������� ��������� ��� �� ������� ����������
   double timforspeed; // ������� ������ ��� ��������� ������ �������
   SStatTrig StatTrig[8]; //���������� ��������� �� ������ ��������� �������� � CfgTrig
};
extern SStatExp StatExp; //���������� ����������
extern SStatExp StatExpShow; //���������� ���������� ��� �����������

extern SNoiseTrigNvd NoiseTrigNvd;
extern SCONFIG_DAT Config_Dat;
extern SMonADC MonPds[32]; //���������� ����������� ����������� ���
extern SMonADC_SCT MonPds_SCT[2]; //���������� ����������� ����������� ���
extern SMonADC MonPdsL[32]; //���������� ����������� ����������� ��� � ������ ������� ���������
extern SMonADC MonAmpL[32]; //���������� ����������� ��������� ���
extern SMonShumTV MonShumTV[32]; //���������� ����������� ����� ���, ����������� � ���������� ���
extern SMonShumTV_SCT MonShumTV_SCT[2]; //���������� ����������� ����� ���, ����������� � ���������� ���

extern SMonBek MonBek[32]; //���������� ����������� �����
extern SMonBep MonBep[2]; //���������� ����������� �����
extern unsigned long MaskaEventBek; // ������� ����� ���������� � ������� ���
extern short NEventBek; //���������� ���������� � ������� ���
extern SEvtBek EventBek[32]; //��������������� ������ ������ ������� �� ���
//extern SEvtBek EventBekShow[32]; //������ ������ ������� ��� �����������

extern unsigned long MaskaEventBep; // ������� ����� ���������� � ������� ��� (��� ����)
extern short NEventBep; //���������� ���������� � ������� ���
extern SEvtBep EventBep[2]; //��������������� ������ ������ ������� �� ���
//extern SEvtBep EventBepShow[2]; //������ ������ ������� ��� �����������

extern HEADER_REC header_rec; //��������� ������
extern SMONIT_DAT Monit_Dat;  //������ ��������������� ��� ������
extern SEVENTBEK_DAT Event_Dat; //������ �������
//extern SEVENTBEK_DAT Event_DatShow; //������ ������� ��� �����������
#endif