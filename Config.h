#ifndef ConfigH
#define ConfigH

#include <vector>
#include <sstream>
#include <fstream.h>
#include <stdio.h>
#include <string.h>

#define MAXBEP 2 //������������ ���������� �����
#define MAXBEK 32 //������������ ���������� �����

#pragma pack(push, 1)
//���� ��� ����������� ����������� ���
struct SLEDKSM
{
  unsigned char base; //������� ����������
  unsigned char diapazon; //��������
  unsigned char pmt[6]; //���� ����������
};
//��������� ������������ ������ ������ (��� ������ ����������������� �����)
struct SCFGKSM
{
  char enable; //0- ��������������, 1- ������������
  char MaskaPMT; //������� ����� ������������ ���
  unsigned short TrigDelay; //�������� ������� "��������" � ����
  unsigned short Porog;//����� ��������������� 0..255
  unsigned char Trword; //����� � ����� ��������� (����� 16 ���� - 128 ���)
  unsigned char Trbit; //��� � ����� � ����� ��������� (����� 16 ���)
  unsigned char TrA; //������� � �������� A
  unsigned char TrB; //������� � �������� B
  unsigned char TrC; //������� � �������� C
  unsigned short x,y,z; // ���������� ��������� � ������� �����, � ��
  SLEDKSM Led; //���� ��������������� ���������
};
//��������� ������������ ������ ������ (��� ��������)
struct SCFGKSM_S
{
  char enable; //0- ��������������, 1- ������������
  char MaskaPMT; //������� ����� ������������ ���
  unsigned short x,y,z; // ���������� ��������� � ������� �����, � ��
};

//��������� ������������ ��� �� ����� bek.cfg
struct SCFGBEK
{
  char enable; //0- ��������������, 1- ������������
  char idBEK; // �������������: 0-��� ��� 1-���
  char NumberBEK; //����� ���
  char MaskaKSM; //������� ����� ������������ ��� �� ������ CfgKSM
  SCFGKSM CfgKSM[4]; //������������ � ��������� ���, ������������ � ���
};
//��������� ������������ ��� (��� ��������)
struct SCFGBEK_S
{
  char enable; //0- ��������������, 1- ������������
  char idBEK; // �������������: 0-��� ��� 1-���
  char NumberBEK; //����� ���
  char MaskaKSM; //������� ����� ������������ ��� �� ������ CfgKSM
  SCFGKSM_S CfgKSM[4]; //������������ � ��������� ���, ������������ � ���
};

//��������� ������������ ������ ������ ��� (��� ������ ����������������� �����)
struct SCFGKSM_SCT
{
  char enable; //0- ��������������, 1- ������������
  char MaskaPMT; //������� ����� ������������ ���
  unsigned short TrigDelay; //�������� ������� "��������" � ����
  unsigned short Porog;//����� ��������������� 0..255
  unsigned char ipl[5]; //����� ��������� �������� (4-12)
  unsigned char istr[5]; //����� ������ �������� (1-5)
  unsigned char idownup[5]; //0-������, 1-�������
  unsigned char Trword[5]; //[pmt] ����� � ����� ��������� (����� 16 ���� - 128 ���)
  unsigned char Trbit[5]; // [pmt] ��� � ����� � ����� ��������� (����� 16 ���)
  unsigned char TrSCT[5]; //[pmt] ������� � �������� SCT
  unsigned short x[5],y[5],z[5]; //[pmt] ���������� ��������� � ������� �����, � ��
};
//��������� ������������ ������ ������ ��� (��� ��������)
struct SCFGKSM_SCT_S
{
  char enable; //0- ��������������, 1- ������������
  char MaskaPMT; //������� ����� ������������ ���
  unsigned char ipl[5]; //����� ��������� �������� (4-12)
  unsigned char istr[5]; //����� ������ �������� (1-5)
  unsigned char idownup[5]; //0-������, 1-�������
  unsigned short x[5],y[5],z[5]; //[pmt] ���������� ��������� � ������� �����, � ��
};

//��������� ������������ ��� �� ����� bep.cfg
struct SCFGBEP
{
  char enable; //0- ��������������, 1- ������������
  char idBEK; // �������������: 0-��� ��� 1-���
  char NumberBEK; //����� ���
  unsigned char MaskaKSM; //������� ����� ������������ ������������ �� ������ CfgKSM
  SCFGKSM_SCT CfgKSM[8]; //������������ � ��������� ������������, ������������ � ���
};
//��������� ������������ ��� (��� ��������)
struct SCFGBEP_S
{
  char enable; //0- ��������������, 1- ������������
  char idBEK; // �������������: 0-��� ��� 1-���
  char NumberBEK; //����� ���
  unsigned char MaskaKSM; //������� ����� ������������ ������������ �� ������ CfgKSM
  SCFGKSM_SCT_S CfgKSM[8]; //������������ � ��������� ������������, ������������ � ���
};

//��������� ������� ��������� ������������ �������� ��� �� ����� sct.cfg
struct SCFGSCT
{
  char enable; //0- ��������������, 1- ������������
  //���������� ������
  char ipl; //����� ��������� (4..12)
  char istr; //����� ������ (1..5)
  char idownup; // 0- ������ ���������, 1-������� ���������
  unsigned short x,y,z; // ���������� ��������� (����� ��������) � ������� �����, � ��
  //���� ���������
  char NumberBEK; //����� ���  (1..32)
  char icntr; //����� ����������� ������ ��� (0..7)
  char ipmt; //����� ������ ��� (0..4)
  unsigned char Trword; //����� � ����� ��������� SCT (����� 16 ���� - 128 ���)
  unsigned char Trbit; //��� � ����� � ����� ��������� SCT (����� 16 ���)
};
//����� ��� ������ ���������� ������
struct STrigMaska
{
  unsigned short MaskaA[8]; //������� �
  unsigned short MaskaB[8]; //������� B
  unsigned short MaskaC[8]; //������� C
  unsigned short MaskaSKT[8]; //������� ���
};
#pragma pack(pop)
struct TCRITERYNVD
{ //Low2,Low1,Height1,Height2
  double BEKt[4]; //����������� ����
  double BEKV1[4]; // ���������� V1
  double BEKV2[4]; // ���������� V2
  double BEKV3[4]; // ���������� V3
  double BEKVcc[4]; // ���������� Vcc
  double TrA[4]; // ������� A ���
  double TrB[4]; // ������� B ���
  double TrC[4]; // ������� C ���
  double PMTnoise[4]; // ���� ���
  double PMTpds12sred[4]; // ��������� ��� 12 ����� �������
  double PMTpds12sig[4]; // ��������� ��� 12 ����� �����
  double PMTpds9sred[4]; // ��������� ��� 9 ����� �������
  double PMTpds9sig[4]; // ��������� ��� 9 ����� �����
  double PMTled12sred[4]; // ��������� ��� 12 ����� �������
  double PMTled12sig[4]; // ��������� ��� 12 ����� �����
  double PMTled9sred[4]; // ��������� ��� 9 ����� �������
  double PMTled9sig[4]; // ��������� ��� 9 ����� �����
};
struct TCRITERYSCT
{ //Low2,Low1,Height1,Height2
  double BEKt[4]; //����������� ����
  double BEKV1[4]; // ���������� V1
  double BEKV2[4]; // ���������� V2
  double BEKV3[4]; // ���������� V3
  double BEKVcc[4]; // ���������� Vcc
  double TrSCT[4]; // �������� SCT
  double PMTnoise[4]; // ���� ���
  double PMTpds12sred[4]; // ��������� ��� 12 ����� �������
  double PMTpds12sig[4]; // ��������� ��� 12 ����� �����
};
//������������ ���������
struct TTRIGGERCNF
{
  unsigned long porsum; //��������� �����
  unsigned long gatew;  //������ �����
  unsigned long timernoise; //������ ��� ��������� ����� (1 ��� = 10 000 000)
  unsigned long maskaout; //����� ������� ���������
  unsigned long masks[8];// �����
};
//��������� ������ ����������
struct TEXPZPARAM
{
  unsigned long tiMonShort; //�������� ������� ����� ���������� ����������� � ���
  unsigned long tiMonSred; //�������� ������� ����� ������������� � ���
  unsigned long tiMonFull; //�������� ������� ����� ����������� ����� � ���
  unsigned long tiShow; //�������� ������� ��� ������ �� ����� � ���
  unsigned long tiDCRans; //�������� ������� �������� ������ �� ������ � ���
  unsigned long tiDCRmon; //�������� ������� �������� ����������� �� ������ � ���
  unsigned long tiDCRnoise; //�������� ������� �������� ����� �� ������ � ���
};

extern TEXPZPARAM ExpzParam; //��������� ������ ���������� �� ����� Expoz.cfg
extern TTRIGGERCNF TrigCnf; //������������ ��������� �� ����� Trigger.cfg
extern unsigned long CfgMaskaBek; //����� ����������� (�� ������������) � ������������ ���
extern unsigned long CfgMaskaBep; //����� ����������� (�� ������������) � ������������ ���
extern unsigned long CfgMaskaBekp; //����� ����������� (�� ������������) � ������������ ��� � ���
extern SCFGBEK CfgBek[32]; //������������ ��� � ������
extern SCFGBEP CfgBep[2]; //������������ ��� � ������
extern SCFGSCT CfgSct[80]; //������������ ��������� ���
extern int BEK2BEP[32]; //������ ���� � ������� CfgBep �� ��� ������ ����
extern TCRITERYNVD CritNVD; //�������� ����������������� �����
extern TCRITERYSCT CritSCT; //�������� ����������������� ���
extern STrigMaska TrigMaska; //����� ��� ������ ���������� ������

//�������� ������������
extern int LoadConfig();
//�������� ��������� �����
extern int LoadCritNVD();

#endif