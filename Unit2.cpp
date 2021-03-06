//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <iostream.h>
#include <fstream.h>
#include <dir.h>
#include <string.h>

#include "StructNvd.h"
#include "StructDcr.h"
#include "NVDGraph.h" //�������� ������ TNVDGraph ��� �������
#include "Help.h"  // ����� "�������"

void ShowStatistics(); // �������� �������

enum event_type {EVENT60C, TELESCOPIC, PMT, LINKING_COEFFICIENT};

TDateTimeKadr CurrentDT;
TDateTimeKadr PreviousDT;
TDateTimeKadr CurrentDTNoise;
TDateTimeKadr PreviousDTNoise;
TDateTimeKadr CurrentDTNoiseABC;
TDateTimeKadr PreviousDTNoiseABC;

HEADER_REC header_rec; //��������� ������
SCONFIG_DAT Config_Dat; //������������
SMONIT_DAT Monit_Dat;  //������ ��������������� ��� ������
SEVENTBEK_DAT Event_Dat; //������ �������


SMonADC MonPds[32]; //���������� ����������� ����������� ���
SMonADC MonPdsL[32]; //���������� ����������� ����������� ��� � ������ ������� ���������
SMonADC MonAmpL[32]; //���������� ����������� ��������� ���
SMonShumTV MonShumTV[32]; //���������� ����������� ����� ���, ����������� � ���������� ���
SMonBek MonBek[32]; //���������� ����������� �����
SEvtBek EventBek[32]; //��������������� ������ ������ ������� �� ���

SMonADC_SCT MonPds_SCT[2]; //���������� ����������� ����������� ���
SMonBep MonBep[2]; //���������� ����������� �����
SMonShumTV_SCT MonShumTV_SCT[2]; //���������� ����������� ����� ���, ����������� � ���������� ���
SEvtBep EventBep[2]; //��������������� ������ ������ ������� �� ���

// �E���
// ���������� �������� N8,N16,N64,Nread ��� ��������� ������������
struct CNF_SET CNF_S;  //���������� ��� ���������� ������� ������
struct STR_DECOR *DcrF=NULL;
short id_eventDCR=0; //������� ����, ��� ���� �������� ������ ������� �� �����

//-------- ���������� � ������ EDAT -----------
unsigned char Lenadd[2]; // Lenadd[0] ���-�� ���� � bufadd ��� ������
			 // Lenadd[1] - 0 ������ �� ������ ���, 1-����.
unsigned short Bufadd[20];

//---------------------------------------------------------------------------
char bstop[4]; //����� ������
char DirRead[256]; //���������� ����
char NameFileSearch[256]; //����� ������
char NameFileRead[256]; //��� �������� ��������� �����
char DirResult[256]; //���������� �����������
int measurement = 0; // ���������� ����� ���������

short strqsm[32]={4,4,4,4,3,3,3,4,4,4,4,3,3,3,4,4,4,4,3,3,3,4,4,4,4,0,0,0,0,0,0,0};

#define MAXCHAN 1 //������������ ���������� ������������ �������
TNVDGraph *Graph1 = NULL;
TNVDGraph *Graph2 = NULL;
TNVDGraph *Graph3 = NULL;
TNVDGraph *Graph4 = NULL;
TNVDGraph *Graph5 = NULL;  // ����������� ������������� ������
TNVDGraph *Graph6 = NULL;
TNVDGraph *Graph7 = NULL;
TNVDGraph *Graph8 = NULL;
TNVDGraph *Graph9 = NULL;
TNVDGraph *Graph10 = NULL;    // ������������� �� ��������� ��������� � "60�"
TNVDGraph *Graph11 = NULL;  // ���� ���
TNVDGraph *Graph12 = NULL;  // ���� ���������� ��������
TNVDGraph *Graph13 = NULL;   // ������ ��� ��� ������������ ���������
TNVDGraph *Graph14 = NULL;
TNVDGraph *Graph15 = NULL;

unsigned long TmpNpoint[MAXCHAN];  // ��� ����������
unsigned long TmpNpoint2[MAXCHAN];  // ��� ��������

long DATA1[MAXCHAN][4096];
long DATA2[MAXCHAN][4096];
long DATA3[MAXCHAN][4096];
long DATA4[MAXCHAN][4096];
long DATA5[MAXCHAN][4096];
long DATA6[MAXCHAN][4096];
long DATA7[MAXCHAN][4096];
long DATA8[MAXCHAN][4096];
long DATA9[MAXCHAN][4096];
long DATA10[MAXCHAN][4096];  // ������������� ��� �� ���������
long DATA11[MAXCHAN][4096];
long DATA12A[MAXCHAN][4096];  // ���� ����������� ������� �
long DATA12B[MAXCHAN][4096];   // ���� ����������� ������� B
long DATA12C[MAXCHAN][4096];    // ���� ����������� ������� �
long DATA13[MAXCHAN][4096];  // ������ ��� ��� ������������ ���������
long DATA14LK[MAXCHAN][4096];  // ����������� ������. ����������� �� �������
long DATA14Amp9[MAXCHAN][4096];
long DATA14Pds9[MAXCHAN][4096];
long DATA14Amp12[MAXCHAN][4096];
long DATA14Pds12[MAXCHAN][4096];
long DATA14Noise[MAXCHAN][4096];
long DATA14NoiseA[MAXCHAN][4096];
long DATA14NoiseB[MAXCHAN][4096];
long DATA14NoiseC[MAXCHAN][4096];
long DATA15[MAXCHAN][4096];
ofstream events60C;
ofstream BarGraphSave; // ����,� ������� ����� ������������ ������� ����������� � ��� ��������� ��� �������� ���������
ifstream BarGraphOpen; // ����, �� �������� ����� ����������� �����������

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

void ReadBargraph() // ���������� ����������� �� �����
{
	if (!access("D:\\Mezinov\\BarGraph\\bargraph.txt",0))  // ���� ���� ����������
	{
		BarGraphOpen.open("D:\\Mezinov\\BarGraph\\bargraph.txt",ios::in);
		char buff[256]; // ������ ������������� ����������� ��������
		int buff_int = 0;

		BarGraphOpen >> buff;  // ��������� �������� �����������
		Graph1->Caption = buff;  // ������������� �������� �����������

		BarGraphOpen >> buff;  // ��������� �������� ��� X
		Graph1->LabelX = buff;

		BarGraphOpen >> buff;  // ��������� �������� ��� Y
		Graph1->LabelY = buff;

		BarGraphOpen >> buff;  // ��������� X_min
		Graph1->XminAll = buff_int;

		BarGraphOpen >> buff;  // ��������� X_max
		Graph1->XmaxAll = buff_int;

		for (int i = 0; i < 4096; i++)
		{
			BarGraphOpen >> buff;
			DATA1[0][i] = StrToInt(buff);
		}
		BarGraphOpen.close();
	}
}

void SetHistogramProperties(TNVDGraph* histogram)
{
	histogram->Align=alClient;
	histogram->Type=gtHistogram;
	histogram->ColorBk=clGreen;
	histogram->Kind=pgBar;
	histogram->Maska=0;//������� ����� �������� �������
	histogram->TypeBufX=tbUChar;
	histogram->SetBufX(NULL,NULL,0);
	histogram->TypeBufY[0]=tbLong;
	TmpNpoint[0]=4096; // �������������� ������������ �������� �� ��� �
	histogram->SetNPoint(&(TmpNpoint[0]),0);
	histogram->NBuf=1;
	histogram->AvtoScale=true;
	histogram->AvtoScaleX=true;
	histogram->AvtoScaleY=true;
	histogram->XminAll=0;
	histogram->XmaxAll=TmpNpoint[0];
	histogram->YminAll=0;
	histogram->YmaxAll=4096;  // �������������� ������������ �������� �� ��� y
	histogram->Maska=1;
}


void SetGraphProperties(TNVDGraph* graph)
{
	graph->Align=alClient;
	graph->Type=gtHistogram;
	graph->ColorBk=clGreen;
	graph->Kind=pgCombo;
	graph->Maska=0;//������� ����� �������� �������
	graph->TypeBufX=tbUChar;
	graph->SetBufX(NULL,NULL,0);
	graph->TypeBufY[0]=tbLong;
	TmpNpoint2[0]= 3600; // �������������� ������������ �������� �� ��� �
	graph->SetNPoint(&(TmpNpoint2[0]),0);
	graph->NBuf=1;
	graph->AvtoScale = true;
	graph->AvtoScaleX = true;
	graph->AvtoScaleY = false;
	graph->XminAll=0;
	graph->XmaxAll=TmpNpoint2[0];
	graph->YminAll = 1000;
	graph->YmaxAll = 1100;  // �������������� ������������ �������� �� ��� y
	graph->Maska=1;
}



TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)   // ����������� ��� �������� ����������
{
	//ReadBargraph(); // ���������� ����������� �� �����
	Graph1 = new TNVDGraph(GroupBox1);
	Graph1->Name="Graph1";
	Graph1->Parent=GroupBox1;
	Graph1->Caption="";
	Graph1->SetBufY(DATA1[0],NULL,0);
    Graph1->LabelY="N";
	Graph1->LabelX="N_Lam";
	SetHistogramProperties(Graph1);
	//ReadBargraph(); // ���������� ����������� �� �����
	//ShowStatistics();  // ����� ���������� �� ������ �����������

	Graph2 = new TNVDGraph(GroupBox3);
	Graph2->Name="Graph2";
	Graph2->Parent=GroupBox3;
	Graph2->Caption="";
	Graph2->SetBufY(DATA2[0],NULL,0);
    Graph2->LabelY="N";
	Graph2->LabelX="N_Pmt";
	SetHistogramProperties(Graph2);

	Graph3 = new TNVDGraph(GroupBox4);
	Graph3->Name="Graph3";
	Graph3->Parent=GroupBox4;
	Graph3->Caption="";
	Graph3->LabelY="N";
	Graph3->LabelX="��������� �������� ���";
	Graph3->SetBufY(DATA3[0],NULL,0);
	SetHistogramProperties(Graph3);

	Graph4 = new TNVDGraph(GroupBox5);
	Graph4->Name="Graph4";
	Graph4->Parent=GroupBox5;
	Graph4->Caption="";
	Graph4->SetBufY(DATA4[0],NULL,0);
	Graph4->LabelY="N";
	Graph4->LabelX="��������� �������� ���";
	SetHistogramProperties(Graph4);

	Graph5 = new TNVDGraph(GroupBox7);
	Graph5->Name="Graph5";
	Graph5->Parent=GroupBox7;
	Graph5->Caption="";
	Graph5->SetBufY(DATA5[0],NULL,0);
	Graph5->LabelY="N";
	Graph5->LabelX="����������� ������ *0.1";
	SetHistogramProperties(Graph5);

	Graph6 = new TNVDGraph(GroupBox8);
	Graph6->Name="Graph6";
	Graph6->Parent=GroupBox8;
	Graph6->Caption="";
	Graph6->SetBufY(DATA6[0],NULL,0);
	Graph6->LabelY="N";
	Graph6->LabelX="��������� �� 12-� ������ *0.1";
	SetHistogramProperties(Graph6);

	Graph7 = new TNVDGraph(GroupBox9);
	Graph7->Name="Graph7";
	Graph7->Parent=GroupBox9;
	Graph7->Caption="";
	Graph7->SetBufY(DATA7[0],NULL,0);
	Graph7->LabelY="N";
	Graph7->LabelX="��������� �� 12-� ������";
	SetHistogramProperties(Graph7);

	Graph8 = new TNVDGraph(GroupBox10);
	Graph8->Name="Graph8";
	Graph8->Parent=GroupBox10;
	Graph8->Caption="";
	Graph8->SetBufY(DATA8[0],NULL,0);
	Graph8->LabelY="N";
	Graph8->LabelX="��������� �� 9-� ������ *0.1";
	SetHistogramProperties(Graph8);

	Graph9 = new TNVDGraph(GroupBox11);
	Graph9->Name="Graph9";
	Graph9->Parent=GroupBox11;
	Graph9->Caption="";
	Graph9->SetBufY(DATA9[0],NULL,0);
	Graph9->LabelY="N";
	Graph9->LabelX="�������� �� 9-� ������ *0.1";
	SetHistogramProperties(Graph9);

	Graph10 = new TNVDGraph(GroupBox12);
	Graph10->Name="Graph10";
	Graph10->Parent=GroupBox12;
	Graph10->Caption="";
	Graph10->SetBufY(DATA10[0],NULL,0);
	Graph10->LabelY="N";
	Graph10->LabelX="��������� ��������� *100";
	SetHistogramProperties(Graph10);

	Graph11 = new TNVDGraph(GroupBox13);
	Graph11->Name="Graph11";
	Graph11->Parent=GroupBox13;
	Graph11->Caption="";
	Graph11->SetBufY(DATA11[0],NULL,0);
	Graph11->LabelY="N";
	Graph11->LabelX="���� ��� (��)";
	SetHistogramProperties(Graph11);

	Graph12 = new TNVDGraph(GroupBox15);
	Graph12->Name="Graph12";
	Graph12->Parent=GroupBox15;
	Graph12->Caption="";
	Graph12->SetBufY(DATA12A[0],NULL,0);
	Graph12->LabelY="N";
	Graph12->LabelX="��� ����������� ������� �";
	SetHistogramProperties(Graph12);

	Graph13 = new TNVDGraph(GroupBox17);
	Graph13->Name="Graph13";
	Graph13->Parent=GroupBox17;
	Graph13->Caption="";
	Graph13->SetBufY(DATA13[0],NULL,0);
	Graph13->LabelY="N";
	Graph13->LabelX="��������� ��� (12-� �����)";
	SetHistogramProperties(Graph13);

	Graph14 = new TNVDGraph(GroupBox18);
	Graph14->Name = "Graph14";
	Graph14->Parent = GroupBox18;
	Graph14->Caption = "";
	Graph14->SetBufY(DATA14LK[0],NULL,0);
	Graph14->LabelY = "*0.1";
	Graph14->LabelX = "Measurement";
	SetGraphProperties(Graph14);
	Graph14->YminAll = 1010;
	Graph14->YmaxAll = 1050;

	Graph15 = new TNVDGraph(GroupBox6);
	Graph15->Name = "Graph15";
	Graph15->Parent = GroupBox6;
	Graph15->Caption = "";
	Graph15->SetBufY(DATA15[0],NULL,0);
	Graph15->LabelY = "����������� ������";
	Graph15->LabelX = "��������� ������� 12-�� ������";
	SetGraphProperties(Graph15);
	Graph15->AvtoScale = true;
	Graph15->AvtoScaleX = true;
	Graph15->AvtoScaleY = true;
    Graph15->Kind=pgPoint;
}


bool CheckMessageQueue()
{
	MSG msg;
	while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
	{
		if( msg.message == WM_QUIT ) return FALSE;
		else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	return TRUE;
}

double Round(double Number, short Precision)  // ������� ��� ���������� � �������� ���������
{
	unsigned short n = pow(10, Precision);
	Number *= n;
	if ((Number - (int)Number) >= 0.5)
	{
		Number++;
	}
	Number = (int)Number;
	Number /= n;
	return Number;
}

void ShowStatistics(long DATA[MAXCHAN][4096], TMemo *memo, int multiplier)
{
	double summ1 = 0;  // ����� ���������� �������� � �������. ������������� ���������� ��� �������� �������� � ���������
	double summ2 = 0;   // ������������� ���������� ��� �������� �������� � ���������
	double average_value = 0.0;
	memo->Clear();
	// ������� ������� ��������
	for (int i = 0; i < 4096; i++)
	{
		if (DATA[0][i] != 0)
		{
			summ1 = summ1 + DATA[0][i];
			summ2 = summ2 + DATA[0][i] * i;
			average_value = (double)summ2 / summ1;
			average_value = Round(average_value, 2);
		}
	}
	if (multiplier == 1)
	{
		average_value = average_value / 10;
		memo->Lines->Add("������� ��������: " + FloatToStr(average_value));
		average_value = average_value * 10;
	}
	else if (multiplier == 100)
	{
		average_value = average_value * 100;
		memo->Lines->Add("������� ��������: " + FloatToStr(average_value));
		average_value = average_value / 100;
	}
	else
	{
		memo->Lines->Add("������� ��������: " + FloatToStr(average_value));
	}
	// ������� ���������
	summ1 = summ2 = 0.001;
	double dispersion = 0.0;
	for (int i = 0; i < 4096; i++)
	{
		if (DATA[0][i] != 0)
		{
			summ2 = summ2 + (i - average_value)*(i - average_value) * DATA[0][i] ;
			summ1 = summ1 + DATA[0][i];
			dispersion = (double)summ2 / (summ1 - 1);
			dispersion = Round(dispersion, 2);
		}
	}
	if (multiplier == 1)
	{
		dispersion = dispersion / 10;
		memo->Lines->Add("���������: " + FloatToStr(dispersion));
	}
	else if (multiplier == 100)
	{
		dispersion = dispersion * 100;
		memo->Lines->Add("���������: " + FloatToStr(dispersion));
	}
	else
	{
		memo->Lines->Add("���������: " + FloatToStr(dispersion));
	}
	// ������� ������������������ ���������� (�����)
	double sigma = sqrt(dispersion);
	sigma = Round(sigma, 2);
	memo->Lines->Add("������������������ ����������: " + FloatToStr(sigma));
}


void Events60C()  // ������� 9
{
	if (Event_Dat.trgnvd & 1)  // ���� ������ ������� - ������� 60�
	{
		double total_amp = 0.0;  // ��������� ���������
		short Npmt  = 0;  // ����� ����������� ���
		short Nlam  = Event_Dat.Nlam;  // ����� ����������� ���
		for(int bec=0;bec < 25;bec++)
		{
			for(int qsm=0;qsm<strqsm[bec];qsm++)
			{
				for(int pmt=0;pmt<6;pmt++)
				{
					//���� ��� ��������
					if((EventBek[bec].maskahit[qsm]&(1<<pmt))!=0)
					{
						Npmt++;
						double dyn12=EventBek[bec].acp[qsm][pmt][0] - MonPds[bec].	sred[qsm][pmt][0];
						double dyn9=EventBek[bec].acp[qsm][pmt][1] - MonPds[bec].	sred[qsm][pmt][1];
						total_amp += dyn12;
					}
				}
			}
		}
		total_amp = Round(total_amp,2);
		DATA1[0][Nlam]++;  // ����������� ������� �����������
		DATA2[0][Npmt]++;  // ����������� ������� �����������
		int total = (int)total_amp / 100;
		DATA10[0][total]++;  // ����������� ������� �����������
		Graph1->Repaint();
		Graph2->Repaint();
		Graph10->Repaint();

		Form1->Memo1->Lines->Add(IntToStr(Event_Dat.Nrun) + "\t" + IntToStr(Event_Dat.exp_count) + "\t" + IntToStr(Nlam) + "\t" + IntToStr(Npmt) + "\t" + FloatToStr(total_amp));
		ShowStatistics(DATA1, Form1->StatisticsMemo,0);
		ShowStatistics(DATA2, Form1->Memo9,0);
		ShowStatistics(DATA10, Form1->Memo15,100);
		if (Form1->CheckBox1->Checked == true)
		{
			events60C << Event_Dat.Nrun << "\t"<< Event_Dat.exp_count << "\t" << Nlam << "\t" << Npmt << "\t" << total_amp << "\n";
		}
	}
}

void TelescopicEvent(short plane, short struna) // ���������� ��� ��������� ��� ����������� ����������, ������� ��������� ���� ��� ������(������� 10)
{
	if (Event_Dat.trgnvd & (1 << 10))   // ���� ��������� ��� ���������
	{
		short Nlam_SCT = Event_Dat.NlamSKT; // ���������� ����������� ��������� ���
		if (Nlam_SCT == 2)
		{
			float ampSCT0 = 0.0; // ��������� ������������ � ������ ��������� �������� ���
			float ampSCT1 = 0.0; // ��������� ������������ � ������� ��������� �������� ���
			short pl0, pl1; // ������ ���������� ���������, ����������� � ������ � ������� ����������  �������������
			short str0, str1; // ������ ����� ���������, ����������� � ������ � ������� ���������� �������������
			for (int maa = 0; maa < 8; maa++) // ��������� �� ������� ���� ([0])
			{
				for (int pmt=0; pmt < 5; pmt++)
				{
					if(((EventBep[0].maskahit[maa])&(1<<pmt))!=0) //  ���� ����� �����-�� ����������� ������� � ������ ���������
					{
						pl0 = Config_Dat.CfgBep[0].CfgKSM[maa].ipl[pmt]; // ����� ��������� ��������, ������������ � ������ ���������
						str0 = Config_Dat.CfgBep[0].CfgKSM[maa].istr[pmt]; // ����� ������ ������������ � ������ ��������� ��������
						ampSCT0 = EventBep[0].acp[maa][pmt] - MonPds_SCT[0].sred[maa][pmt];
					}
				}
			}
			for (int maa = 0; maa < 8; maa++) // �������� �� �������� ���� ([1])
			{
				for (int pmt=0; pmt < 5; pmt++)
				{
					if(((EventBep[1].maskahit[maa])&(1<<pmt))!=0) //  ���� ����� �����-�� ����������� ������� � ������ ���������
					{
						pl1 = Config_Dat.CfgBep[1].CfgKSM[maa].ipl[pmt]; // ����� ��������� ��������, ������������ � ������� ���������
						str1 = Config_Dat.CfgBep[1].CfgKSM[maa].istr[pmt]; // ����� ������ ������������ � ������� ��������� ��������
						ampSCT1 = EventBep[1].acp[maa][pmt] - MonPds_SCT[1].sred[maa][pmt];
					}
				}
			}
			ampSCT0 = Round(ampSCT0,2);
			ampSCT1 = Round(ampSCT1,2);
			if  (plane == pl0 && plane == pl1 && pl0 == pl1 && struna == str0 && struna == str1 && str0==str1) // ���� ��������� � ������ ����������� ��������� � ������� � ������ ��������� ��������� � ���, ��� ������ � ���������� � ��������� ����� �����
			{
				double amp0 = Round(ampSCT0,2);
				double amp1 = Round(ampSCT1,2);
				DATA3[0][(int)amp0]++;
				Graph3->Repaint();
				ShowStatistics(DATA3, Form1->Memo10,0);
				DATA4[0][(int)amp1]++;
				Graph4->Repaint();
				ShowStatistics(DATA4, Form1->Memo11,0);
				Form1->Memo2->Lines->Add(IntToStr(Event_Dat.Nrun) + "\t" + IntToStr(Event_Dat.exp_count) + "\t" + FloatToStr(amp0) + "\t" + FloatToStr(amp1));

                if (Form1->ViewPmtResponse->Checked == true)  // ���� ���� �������� ������ ��� ��������� � ����������
				{
					int bek = StrToInt(Form1->TelescopicBek->Text) - 1;
					int qsm = StrToInt(Form1->TelescopicQsm->Text) - 1;
					int pmt = StrToInt(Form1->TelescopicPmt->Text) - 1;
					if((EventBek[bek].maskahit[qsm]&(1<<pmt))!=0)  // ���� ��� ��������, �� ����� ������� ���������
					{
						double dyn12 = EventBek[bek].acp[qsm][pmt][0] - MonPds[bek].sred[qsm][pmt][0];
						DATA13[0][(int)dyn12]++;
						Graph13->Repaint();
						ShowStatistics(DATA13,Form1->Memo14,0);
					}
					else  // ���� ��� �� ��������, �� �� �������� ���������
					{
						double dyn12 = EventBek[bek].acp[qsm][pmt][0];
						DATA13[0][(int)dyn12]++;
						Graph13->Repaint();
						ShowStatistics(DATA13,Form1->Memo14,0);
					}
				}
			}
		}
	}
}

void LinkingCoefficient(short bek, short qsm, short pmt)
{
	if((EventBek[bek].maskahit[qsm]&(1<<pmt))!=0)
	{
		double dyn12 = EventBek[bek].acp[qsm][pmt][0] - MonPds[bek].	sred[qsm][pmt][0];
		double dyn9 = EventBek[bek].acp[qsm][pmt][1] - MonPds[bek].	sred[qsm][pmt][1];
		double linking_coeff = dyn12 / dyn9;
		if (dyn12 < 4096)   // ����� �� ������� �� ������� �������
		{
			DATA15[0][(int)dyn12] = linking_coeff;
			Graph15->Repaint();
		}
	}
}

void ReadNad(event_type type)
{
	int Nevent=0;
	int tip_zapDcr=0;
	strcpy(DirRead, Form1->PathToRun->Text.c_str()); // ������������ ������ ���� � ���� �� �����
	strcpy(NameFileSearch,DirRead);
	strcat(NameFileSearch,"\\*.nad");
	struct ffblk ffile; // ��� ������ ������
	strcpy(DirResult,"Result");
	mkdir(DirResult);
	// ������� ������� ��������� ������
	if(DcrF==NULL)
	{
		DcrF = new STR_DECOR;
	}
	memset(DcrF,0,sizeof(STR_DECOR));
	memset(&MonPds,0,sizeof(MonPds));
	memset(&MonPdsL,0,sizeof(MonPdsL));
	memset(&MonAmpL,0,sizeof(MonAmpL));
	memset(&MonShumTV,0,sizeof(MonShumTV));
	memset(&MonBek,0,sizeof(MonBek));
	memset(&MonPds_SCT,0,sizeof(MonPds_SCT));
	memset(&MonShumTV_SCT,0,sizeof(MonShumTV_SCT));
	memset(&MonBep,0,sizeof(MonBep));

	//Memo1->Lines->Add("123");
	int done=findfirst(NameFileSearch,&ffile,FA_ARCH);

	do
	{
		strcpy(NameFileRead,DirRead);
		strcat(NameFileRead,"\\");
		strcat(NameFileRead,ffile.ff_name);
		int handle=open(NameFileRead,O_RDONLY|O_BINARY);
		if(handle==-1)
		{
			//printf("\r\n   No open %s",NameFileRead); continue;
			//getch();
		}
		while(read(handle,&header_rec,sizeof(header_rec))==sizeof(header_rec))
		{
			if(!CheckMessageQueue())  // ���� IF �������� �
			{
				return;
			}
			switch(header_rec.tip_zap)
			{
				case HDR_W:
					tip_zapDcr=header_rec.lendata;
					{
						switch(tip_zapDcr)
						{
							case ID_CONFIG:
								read(handle,&DcrF->configcntr,4);
								read(handle,&DcrF->CNF,sizeof(CONFIG_DECOR));
								read(handle,DcrF->Trgcnf,2*sizeof(TRGCNF));
							break;
							case ID_MONIT:
								read(handle,&DcrF->configmonit,4);
								read(handle,&DcrF->CMonitAll,sizeof(CMONITORALL));
							break;
							case ID_NOISE:
								read(handle,DcrF->counter,sizeof(DcrF->counter));
								read(handle,&DcrF->confignoise,sizeof(DcrF->confignoise));
								read(handle,DcrF->id_cnoise,sizeof(DcrF->id_cnoise));
								read(handle,DcrF->len_cnoise,sizeof(DcrF->len_cnoise));
								read(handle,&DcrF->CNoise,sizeof(DcrF->CNoise));
								memset(CNF_S.noise_chan,0,sizeof(CNF_S.noise_chan));
								for(int i=0; i < MAXPM; i++)
								{
									if(DcrF->id_cnoise[i]==0)continue;
									if(DcrF->len_cnoise[i]==0)continue;
									char *ptr=(char *)DcrF->CNoise[i].noisebuf;
									int k=0;
									for(int j=0; j < NCNTR; j++)
									{
										if( (DcrF->CNoise[i].maskacntr&(1<<j)) == 0)continue; // ��� ������ ���������
										memcpy(CNF_S.noise_chan[i*NCNTR+j],&ptr[k],8);
										k +=8;
									}
								}
							 break;

						}
					}
				break;
				case CFGNVD_W:
					read(handle,&Config_Dat,header_rec.lendata);
				break;
				case MONPDS_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbek=0;
						for(int ibek=0; ibek < 32; ibek++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibek)) ==0)continue;
							memcpy(&MonPds[ibek],&Monit_Dat.data.MonPds[nbek],sizeof(MonPds[ibek]));
							nbek++;
							if(nbek >=Monit_Dat.NBek)break;
						}
					}
				break;
				case MONPDSL_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbek=0;
						for(int ibek=0; ibek < 32; ibek++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibek)) ==0)continue;
							memcpy(&MonPdsL[ibek],&Monit_Dat.data.MonPdsL[nbek],sizeof(MonPdsL[ibek]));
							nbek++;
							if(nbek >=Monit_Dat.NBek)break;
						}
					}
				break;
				case MONAMPL_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbek=0;
						for(int ibek=0; ibek < 32; ibek++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibek)) == 0)continue;
							memcpy(&MonAmpL[ibek],&Monit_Dat.data.MonAmpL[nbek],sizeof(MonAmpL[ibek]));
							nbek++;
							if(nbek >=Monit_Dat.NBek)break;
						}
					}
					// ��, ��� �������� ���� � ���� case - �������� �. ������ ����������� ������ ��� ������� ���
					/*if (type == LINKING_COEFFICIENT)
					{
						for (int bek = 0; bek < 32; bek++)
						{
							for(int qsm=0;qsm<strqsm[bek];qsm++)
							{
								for(int pmt=0;pmt<6;pmt++)
								{
									float amp12 = MonAmpL[bek].sred[qsm][pmt][0];  // ��������� 12-�� ������
									float amp9  = MonAmpL[bek].sred[qsm][pmt][1];  // ��������� 9-�� ������
									float pedestal12 = MonPdsL[bek].sred[qsm][pmt][0];
									float pedestal9 = MonPdsL[bek].sred[qsm][pmt][1];
									double linking_coefficient = Round((amp12 - pedestal12)/(amp9 - pedestal9),2);
									//Form1->Memo3->Lines->Add(IntToStr(bek+1) + "\t" + IntToStr(qsm+1) + "\t" + IntToStr(pmt+1) + "\t" + FloatToStr(linking_coefficient));
									DATA5[0][(int)linking_coefficient]++;
									Graph5->Repaint();
								}
							}
						}
						return;
					}*/
					if (type == PMT)
					{
						CurrentDT = header_rec.datetime;
						if (CurrentDT.tm.minute != PreviousDT.tm.minute)
						{

                         	int bek = StrToInt(Form1->Bek->Text) - 1;
							int qsm = StrToInt(Form1->Qsm->Text) - 1;
							int pmt = StrToInt(Form1->Pmt->Text) - 1;
							double amp12 = MonAmpL[bek].sred[qsm][pmt][0];  // ��������� 12-�� ������
							double amp9  = 10*MonAmpL[bek].sred[qsm][pmt][1];  // ��������� 9-�� ������
							double pedestal12 = 10*MonPdsL[bek].sred[qsm][pmt][0];
							double pedestal9 = 10*MonPdsL[bek].sred[qsm][pmt][1];
							double linking_coefficient = 10*Round((10*amp12 - pedestal12)/(amp9 - pedestal9),2);
							double sigma_pedestal9 = MonPdsL[bek].sred[qsm][pmt][0];  // ����� ���������� 9-�� ������
							double sigma_pedestal12 = MonPdsL[bek].sred[qsm][pmt][1];  // ����� ���������� 12-�� ������
							DATA5[0][(int)linking_coefficient]++;
							ShowStatistics(DATA5,Form1->Memo3,1);
							DATA6[0][(int)pedestal12]++;
							ShowStatistics(DATA6,Form1->Memo5,1);
							DATA7[0][(int)amp12]++;
							ShowStatistics(DATA7,Form1->Memo6,0);
							DATA8[0][(int)pedestal9]++;
							ShowStatistics(DATA8,Form1->Memo7,1);
							DATA9[0][(int)amp9]++;
							ShowStatistics(DATA9,Form1->Memo8,1);
							Graph5->Repaint();
							Graph6->Repaint();
							Graph7->Repaint();
							Graph8->Repaint();
							Graph9->Repaint();
							DATA14LK[0][measurement] = linking_coefficient;
							DATA14Amp9[0][measurement] = amp9;
							DATA14Amp12[0][measurement] = amp12;
							DATA14Pds9[0][measurement] = pedestal9;
							DATA14Pds12[0][measurement] = pedestal12;
							Graph14->Repaint();
							measurement++;
							unsigned short year = header_rec.datetime.dt.year;
							unsigned char month = header_rec.datetime.dt.month;
							unsigned char day = header_rec.datetime.dt.day;
							AnsiString date = IntToStr(day) + "." + IntToStr(month) + "." + IntToStr(year);
							Form1->Date->Caption = "����: " + date;
							unsigned char hour = header_rec.datetime.tm.hour;
							unsigned char minute = header_rec.datetime.tm.minute;
							unsigned char second = header_rec.datetime.tm.second;
							AnsiString time = IntToStr(hour) + ":" + IntToStr(minute) + ":" + IntToStr(second);
							Form1->Time->Caption = "�����: " + time;

							amp9  = Round(amp9 / 10, 2);
							amp12  = Round(amp12, 2);
							pedestal12 = Round(pedestal12 / 10, 2);
							pedestal9 = Round(pedestal9 / 10, 2);
							linking_coefficient = Round(linking_coefficient / 10, 2);
							sigma_pedestal12 = Round(sigma_pedestal12, 2);
							sigma_pedestal9 = Round(sigma_pedestal9, 2);
							Form1->Memo4->Lines->Add(IntToStr(measurement) + "\t" + date + "\t" + time + "\t" + linking_coefficient + "\t" + pedestal12 + "\t" + sigma_pedestal12 + "\t" + amp12 + "\t" + pedestal9 + "\t" + sigma_pedestal9 + "\t" + amp9);
						}
						PreviousDT = CurrentDT;
					}
				break;
				case MONSHUMTV_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbek=0;
						for(int ibek=0; ibek < 32; ibek++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibek)) ==0)continue;
							memcpy(&MonShumTV[ibek],&Monit_Dat.data.MonShumTV[nbek],sizeof(MonShumTV[ibek]));
							nbek++;
							if(nbek >=Monit_Dat.NBek)break;
						}
					}
					CurrentDTNoise = header_rec.datetime;
					if (type == PMT)
					{
						if (CurrentDTNoise.tm.minute != PreviousDTNoise.tm.minute)
						{
							int bek = StrToInt(Form1->Bek->Text) - 1;
							int qsm = StrToInt(Form1->Qsm->Text) - 1;
							int pmt = StrToInt(Form1->Pmt->Text) - 1;
							float noise = MonShumTV[bek].noisePMT[qsm][pmt] * 1000;
							DATA11[0][(int)noise]++;
							ShowStatistics(DATA11,Form1->Memo12, 0);
							DATA14Noise[0][measurement] = noise;
							Graph11->Repaint();
							Graph14->Repaint();
						}
						PreviousDTNoise = CurrentDTNoise;
					}
				break;
				case MONBEK_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbek=0;
						for(int ibek=0; ibek < 32; ibek++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibek)) ==0)continue;
							memcpy(&MonBek[ibek],&Monit_Dat.data.MonBek[nbek],sizeof(MonBek[ibek]));
							nbek++;
							if(nbek >= Monit_Dat.NBek)break;
						}
					}
					CurrentDTNoiseABC = header_rec.datetime;
					if (type == PMT)
					{
						if (CurrentDTNoiseABC.tm.minute != PreviousDTNoiseABC.tm.minute)
						{
							int bek = StrToInt(Form1->Bek->Text) - 1;
							int qsm = StrToInt(Form1->Qsm->Text) - 1;
							int pmt = StrToInt(Form1->Pmt->Text) - 1;
							unsigned short noiseA = MonBek[bek].noiseTrA[qsm];
							unsigned short noiseB = MonBek[bek].noiseTrB[qsm];
							unsigned short noiseC = MonBek[bek].noiseTrC[qsm];
							DATA12A[0][noiseA]++;
							DATA12B[0][noiseB]++;
							DATA12C[0][noiseC]++;
							DATA14NoiseA[0][measurement] = noiseA;
							DATA14NoiseB[0][measurement] = noiseB;
							DATA14NoiseC[0][measurement] = noiseC;
							Graph12->Repaint();
							Graph14->Repaint();
							if (Form1->RadioButton1->Checked)
							{
								ShowStatistics(DATA12A,Form1->Memo13,0);
							}
							else if (Form1->RadioButton2->Checked)
							{
								ShowStatistics(DATA12B,Form1->Memo13,0);
							}
							else if (Form1->RadioButton3->Checked)
							{
								ShowStatistics(DATA12C,Form1->Memo13,0);
							}
						}
						PreviousDTNoiseABC = CurrentDTNoiseABC;
					}
				break;
				case MONPDS_SCT_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbep=0;
						for(int ibep=0; ibep < 2; ibep++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibep)) ==0)continue;
							memcpy(&MonPds_SCT[ibep],&Monit_Dat.data.MonPds_SCT[nbep],sizeof(MonPds[ibep]));
							nbep++;
							if(nbep >=Monit_Dat.NBek)
							{
								break;
							}
						}
					}
				break;
				case MONSHUMTV_SCT_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbep=0;
						for(int ibep=0; ibep < 2; ibep++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibep)) ==0)continue;
							memcpy(&MonShumTV_SCT[ibep],&Monit_Dat.data.MonShumTV_SCT[nbep],sizeof(MonShumTV_SCT[ibep]));
							nbep++;
							if(nbep >=Monit_Dat.NBek)
							{
								break;
							}
						}
					}
				break;
				case MONBEP_W:
					read(handle,&Monit_Dat,header_rec.lendata);
					{
						int nbep=0;
						for(int ibep=0; ibep < 2; ibep++)
						{
							if( (Monit_Dat.MaskaBek&(1<<ibep)) ==0)continue;
							memcpy(&MonBep[ibep],&Monit_Dat.data.MonBep[nbep],sizeof(MonBep[ibep]));
							nbep++;
							if(nbep >=Monit_Dat.NBek)
							{
								break;
							}
						}
					}
				break;
				case EVENT_W:
					id_eventDCR=0; // ������ ������� � ������ ���� �� ��������
					memset(&Event_Dat,0,sizeof(Event_Dat));
					read(handle,&Event_Dat,header_rec.lendata);
					memset(EventBek,0,sizeof(EventBek));
					memset(EventBep, 0, sizeof(EventBep));

					if((Event_Dat.NBep) && (Event_Dat.NBek < 32))
					{
						//����� �������� � ������ BEP
						int lendataBep=Event_Dat.NBep*sizeof(Event_Dat.EventBep[0]);
						int smes=header_rec.lendata-lendataBep;
						char *from=((char *)&Event_Dat) + smes;
						memcpy(Event_Dat.EventBep,from,lendataBep);
						//�� ������ ������ ������� ����� � Event_Dat.EventBek
						int NBek=Event_Dat.NBek;
						memset(&Event_Dat.EventBek[NBek], 0,(32-NBek)*sizeof(Event_Dat.EventBek[0]));
					}
					int nbek=0;
					for(int ibek=0; ibek < 32; ibek++)
					{
						if( (Event_Dat.MaskaBek&(1<<ibek)) ==0)continue;
						memcpy(&EventBek[ibek],&Event_Dat.EventBek[nbek],sizeof(EventBek[ibek]));
						nbek++;
						if(nbek >=Event_Dat.NBek)break;
					}

					int nbep=0;
					for(int ibep=0; ibep < 2; ibep++)
					{
						if( (Event_Dat.MaskaBep&(1<<ibep)) ==0)continue;
						memcpy(&EventBep[ibep],&Event_Dat.EventBep[nbep],sizeof(EventBep[ibep]));
						nbep++;
						if(nbep >=Event_Dat.NBep)break;
					}
					//���������� � �������� start5 � start6
					read(handle,Lenadd,sizeof(Lenadd));
					if(Lenadd[0]) // ���-�� ���� � Bufadd ��� ������
					{
						read(handle,Bufadd,Lenadd[0]*2);
					}
					if(Lenadd[1])// 0 ������ �� ������ ���, 1-����.
					{
						read(handle,(char *)&DcrF->configevent,4);
						read(handle,(char *)&DcrF->len_ceventall,2);
						if(DcrF->len_ceventall > sizeof(CEVENTALL))break; //return ERR_DCR_W;
						if(DcrF->len_ceventall)
						{
							read(handle,(char *)&DcrF->CEventAll,DcrF->len_ceventall);
							id_eventDCR=1; // ������ ������� � ������ ��������
						}

					}

					//��������� �������
					if (type == EVENT60C)
					{
						Events60C();
					}
					else if (type == TELESCOPIC)
					{
						short plane = StrToInt(Form1->Plane->Text);
						short struna = StrToInt(Form1->Struna->Text);
						TelescopicEvent(plane,struna);
					}
					else if (type == LINKING_COEFFICIENT)
					{
						short bek = StrToInt(Form1->BekLK->Text) - 1;
						short qsm = StrToInt(Form1->QsmLK->Text) - 1;
						short pmt = StrToInt(Form1->PmtLK->Text) - 1;
						LinkingCoefficient(bek, qsm, pmt);
					}
					Nevent++; // ����������� ����� �������
				break;
				//����� ��������� �������
				deafult:
					lseek(handle,header_rec.lendata,SEEK_CUR);
				break;
			}//tip_zap
			read(handle,bstop,4);
			if(memcmp(bstop,"stop",4))
			{
				//printf("%s Error read stop\n",ffile.ff_name);
				break;
			}
		};
		close(handle);
	}while( (done=findnext(&ffile)) ==0);
	findclose(&ffile);
	//printf("All Event=%ld , Last Event:%ld\t Life time: %ld\n",Nevent,Event_Dat.exp_count,Event_Dat.All_time[1]);
}


//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	Memo1->Lines->Add("N_run\tN_event\tN_lam\tN_pmt\tSum_Amp"); // �������� ��� ������� 60�
	if (CheckBox1->Checked == true)  // ���� ����� ������� "���������� � ���� .nad"
	{
		if (!access(PathToDatFile->Text.c_str(),0))  // ���� ���� ����������
		{
			events60C.open(PathToDatFile->Text.c_str(),ios::app); // ��������� ���� ��� �������� � ����� �����
		}
		else   // ���� ���� �� ����������
		{
			events60C.open(PathToDatFile->Text.c_str(),ios::out); // ��������� ���� ��� ������
			events60C << "N_run\tN_event\tN_lam\tN_pmt\tTotal_amp\n";
		}
	}
	ReadNad(EVENT60C);
}

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	BarGraphSave.open("D:\\Mezinov\\BarGraph\\bargraph.txt",ios::out); // ��������� ���� ��� ������
	BarGraphSave << Graph1->Caption.c_str();  // ��������� �������� �����������
	BarGraphSave << "\n";
	BarGraphSave << Graph1->LabelX.c_str();
	BarGraphSave << "\n";
	BarGraphSave << Graph1->LabelY.c_str();
	BarGraphSave << "\n";
	BarGraphSave << Graph1->XminAll;
	BarGraphSave << "\n";
	BarGraphSave << Graph1->XmaxAll;
	BarGraphSave << "\n";
	for (int i = 0; i < 4096; i++)
	{
		BarGraphSave << DATA1[0][i];
		BarGraphSave << "\n";
	}
	BarGraphSave.close();
	Form1->Close();

}

void __fastcall TForm1::N3Click(TObject *Sender)
{
	//��������� ���� ����������
	Graph1->ColorBk=clWhite; // �������� ���� ���������. ��� ����� ��� ����������
	SaveDialog1->Options.Clear();
	SaveDialog1->Options<<ofOverwritePrompt<<ofHideReadOnly<<ofEnableSizing;
	if(!SaveDialog1->Execute())return;
	Graph1->SaveToFile(SaveDialog1->FileName.c_str(),true);
	Graph1->ColorBk=clGreen;  // ������������� �������������� ���� ���������
}



void __fastcall TForm1::Button4Click(TObject *Sender)
{
	if (AutoScaleX->Checked == false)
	{
		Graph10->AvtoScaleX = false;
		if (x_min->Text == "" || x_max->Text == "")
		{
			Application->MessageBoxA("������� x_min � x_max","������",0);
			return;
		}
		Graph10->XminAll = StrToInt(x_min->Text.c_str());
		Graph10->XmaxAll = StrToInt(x_max->Text.c_str());
	}
	else
	{
	   Graph10->AvtoScaleX = true;
	}

	if (AutoScaleY->Checked == false)
	{
		Graph10->AvtoScaleY = false;
		if (y_min->Text == "" || y_max->Text == "")
		{
			Application->MessageBoxA("������� y_min � y_max","������",0);
			return;
		}
		Graph10->YminAll = StrToInt(y_min->Text.c_str());
		Graph10->YmaxAll = StrToInt(y_max->Text.c_str());
	}
	else
	{
	   Graph10->AvtoScaleY = true;
	}
}


void __fastcall TForm1::Button3Click(TObject *Sender)
{
	for (int i = 0; i < 4096; i++)
	{
		DATA1[0][i] = 0;
		DATA2[0][i] = 0;
		DATA10[0][i] = 0;
	}
	Graph1->Repaint();
	Graph2->Repaint();
	Graph10->Repaint();
	Memo1->Clear();
	Memo9->Clear();
	Memo15->Clear();
	StatisticsMemo->Clear();
	x_min->Clear();
	x_max->Clear();
	y_min->Clear();
	y_max->Clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
	if (Plane->Text == "" || Struna->Text == "")
	{
		Application->MessageBoxA("������� ����� ��������� � ����� ������","������",0);
		return;
	}
	if (ViewPmtResponse->Checked == true)
	{
		if (TelescopicBek->Text == "" || TelescopicQsm->Text == "" || TelescopicPmt->Text == "")
		{
			Application->MessageBoxA("������� ���, ��� � ���!","������",0);
			return;
		}
	}
	Memo2->Lines->Add("N_run\tN_event\tAmpSCT0\tAmpSCT1\t"); // �������� ��� ������� 60�
	ReadNad(TELESCOPIC);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{
	if (Bek->Text == "" || Qsm->Text == "" || Pmt->Text == "")
	{
		Application->MessageBoxA("������� ���, ��� � ���!","������",0);
		return;
	}
	if ((StrToInt(Qsm->Text) > 4)   || (StrToInt(Pmt->Text) > 6))
	{
		Application->MessageBoxA("��������� ������������ ����� ��� � ���","������",0);
		return;
	}
	Memo4->Lines->Add("�\tDate\t\tTime\tLK\tPed12\tSPed12\tAmp12\tPed9\tSPed9\tAmp9");
	ReadNad(PMT);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2Click(TObject *Sender)
{
	Form3->Show();
}



//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
	if (AutoscaleXNoise->Checked == false)
	{
		Graph11->AvtoScaleX = false;
		if (XMinNoise->Text == "" || XMaxNoise->Text == "")
		{
			Application->MessageBoxA("������� x_min � x_max","������",0);
			return;
		}
		Graph11->XminAll = StrToInt(XMinNoise->Text.c_str());
		Graph11->XmaxAll = StrToInt(XMaxNoise->Text.c_str());
	}
	else
	{
	   Graph11->AvtoScaleX = true;
	}

	if (AutoscaleYNoise->Checked == false)
	{
		Graph11->AvtoScaleY = false;
		if (YMinNoise->Text == "" || YMaxNoise->Text == "")
		{
			Application->MessageBoxA("������� y_min � y_max","������",0);
			return;
		}
		Graph11->YminAll = StrToInt(YMinNoise->Text.c_str());
		Graph11->YmaxAll = StrToInt(YMaxNoise->Text.c_str());
	}
	else
	{
	   Graph11->AvtoScaleY = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{
	Graph12->LabelX = "��� ����������� ������� �";
	Graph12->SetBufY(DATA12A[0],NULL,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
	Graph12->LabelX = "��� ����������� ������� B";
	Graph12->SetBufY(DATA12B[0],NULL,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton3Click(TObject *Sender)
{
	Graph12->LabelX = "��� ����������� ������� C";
	Graph12->SetBufY(DATA12C[0],NULL,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ViewPmtResponseClick(TObject *Sender)
{
	TelescopicBek->Visible = true;
	TelescopicQsm->Visible = true;
	TelescopicPmt->Visible = true;
	Label51->Visible = true;
	Label52->Visible = true;
	Label53->Visible = true;
	GroupBox17->Visible = true;
	Memo14->Visible = true;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::RadioButton4Click(TObject *Sender)
{
	GroupBox18->Caption = "����������� ������";
	Graph14->SetBufY(DATA14LK[0],NULL,0);
	Graph14->YminAll = 1010;
	Graph14->YmaxAll = 1050;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton5Click(TObject *Sender)
{
	GroupBox18->Caption = "�������� �� 9-� ������";
	Graph14->SetBufY(DATA14Amp9[0],NULL,0);
	Graph14->YminAll = 329;
	Graph14->YmaxAll = 333;
	Graph14->LabelY = "*0.1";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton6Click(TObject *Sender)
{
	GroupBox18->Caption = "��������� �� 9-� ������";
	Graph14->SetBufY(DATA14Pds9[0],NULL,0);
	Graph14->YminAll = 115;
	Graph14->YmaxAll = 120;
	Graph14->LabelY = "*0.1";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton7Click(TObject *Sender)
{
	GroupBox18->Caption = "��������� �� 12-� ������";
	Graph14->SetBufY(DATA14Amp12[0],NULL,0);
	Graph14->YminAll = 2200;
	Graph14->YmaxAll = 2250;
	Graph14->LabelY = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton8Click(TObject *Sender)
{
	GroupBox18->Caption = "��������� �� 12-� ������";
	Graph14->SetBufY(DATA14Pds12[0],NULL,0);
	Graph14->YminAll = 135;
	Graph14->YmaxAll = 140;
	Graph14->LabelY = "*0.1";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton9Click(TObject *Sender)
{
	GroupBox18->Caption = "����";
	Graph14->SetBufY(DATA14Noise[0],NULL,0);
	Graph14->YminAll = 10;
	Graph14->YmaxAll = 2300;
	Graph14->LabelY = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton10Click(TObject *Sender)
{
	GroupBox18->Caption = "��� ����������� ������� �";
	Graph14->SetBufY(DATA14NoiseA[0],NULL,0);
	Graph14->YminAll = 3200;
	Graph14->YmaxAll = 4150;
	Graph14->LabelY = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton11Click(TObject *Sender)
{
	GroupBox18->Caption = "��� ����������� ������� B";
	Graph14->SetBufY(DATA14NoiseB[0],NULL,0);
	Graph14->YminAll = 0;
	Graph14->YmaxAll = 420;
	Graph14->LabelY = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton12Click(TObject *Sender)
{
	GroupBox18->Caption = "��� ����������� ������� C";
	Graph14->SetBufY(DATA14NoiseC[0],NULL,0);
	Graph14->YminAll = 530;
	Graph14->YmaxAll = 700;
	Graph14->LabelY = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
	if (AutoscaleXGraph->Checked == false)
	{
		Graph14->AvtoScaleX = false;
		if (XMinGraph->Text == "" || XMaxGraph->Text == "")
		{
			Application->MessageBoxA("������� x_min � x_max","������",0);
			return;
		}
		Graph14->XminAll = StrToInt(XMinGraph->Text.c_str());
		Graph14->XmaxAll = StrToInt(XMaxGraph->Text.c_str());
	}
	else
	{
	   Graph14->AvtoScaleX = true;
	}

	if (AutoscaleYGraph->Checked == false)
	{
		Graph14->AvtoScaleY = false;
		if (YMinGraph->Text == "" || YMaxGraph->Text == "")
		{
			Application->MessageBoxA("������� y_min � y_max","������",0);
			return;
		}
		Graph14->YminAll = StrToInt(YMinGraph->Text.c_str());
		Graph14->YmaxAll = StrToInt(YMaxGraph->Text.c_str());
	}
	else
	{
	   Graph14->AvtoScaleY = true;
	}
}

void __fastcall TForm1::Button7Click(TObject *Sender)
{
	if (BekLK->Text == "" || QsmLK->Text == "" || PmtLK->Text == "")
	{
		Application->MessageBoxA("������� ���, ��� � ���!","������",0);
		return;
	}
	if ((StrToInt(QsmLK->Text) > 4)   || (StrToInt(PmtLK->Text) > 6))
	{
		Application->MessageBoxA("��������� ������������ ����� ��� � ���","������",0);
		return;
	}
	ReadNad(LINKING_COEFFICIENT);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
	if (AutoscaleXLK->Checked == false)
	{
		Graph15->AvtoScaleX = false;
		if (XMinLK->Text == "" || XMaxLK->Text == "")
		{
			Application->MessageBoxA("������� x_min � x_max","������",0);
			return;
		}
		Graph15->XminAll = StrToInt(XMinLK->Text.c_str());
		Graph15->XmaxAll = StrToInt(XMaxLK->Text.c_str());
	}
	else
	{
	   Graph15->AvtoScaleX = true;
	}

	if (AutoscaleYLK->Checked == false)
	{
		Graph15->AvtoScaleY = false;
		if (YMinLK->Text == "" || YMaxLK->Text == "")
		{
			Application->MessageBoxA("������� y_min � y_max","������",0);
			return;
		}
		Graph15->YminAll = StrToInt(YMinLK->Text.c_str());
		Graph15->YmaxAll = StrToInt(YMaxLK->Text.c_str());
	}
	else
	{
	   Graph15->AvtoScaleY = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{
	for (int i = 0; i < 4096; i++)
	{
		DATA15[0][i] = 0;
	}
	Graph15->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender)
{
	for (int i = 0; i < 4096; i++)
	{
		DATA5[0][i] = 0;
		DATA6[0][i] = 0;
		DATA7[0][i] = 0;
		DATA8[0][i] = 0;
		DATA9[0][i] = 0;
		DATA10[0][i] = 0;
		DATA11[0][i] = 0;
		DATA12A[0][i] = 0;
		DATA12B[0][i] = 0;
		DATA12C[0][i] = 0;
		DATA14LK[0][i] = 0;
		DATA14Amp9[0][i] = 0;
		DATA14Pds9[0][i] = 0;
		DATA14Amp12[0][i] = 0;
		DATA14Pds12[0][i] = 0;
		DATA14Noise[0][i] = 0;
		DATA14NoiseA[0][i] = 0;
		DATA14NoiseB[0][i] = 0;
		DATA14NoiseC[0][i] = 0;
	}
	Graph5->Repaint();
	Graph6->Repaint();
	Graph7->Repaint();
	Graph8->Repaint();
	Graph9->Repaint();
	Graph10->Repaint();
	Graph11->Repaint();
	Graph12->Repaint();
	Graph14->Repaint();
	measurement = 0;
	RadioButton1->Checked = true;
	Memo3->Clear();
	Memo5->Clear();
	Memo6->Clear();
	Memo7->Clear();
	Memo8->Clear();
	Memo12->Clear();
	Memo13->Clear();
	Memo4->Clear();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
    for (int i = 0; i < 4096; i++)
	{
		DATA3[0][i] = 0;
		DATA4[0][i] = 0;
		DATA13[0][i] = 0;
	}
	Graph3->Repaint();
	Graph4->Repaint();
	Graph13->Repaint();
	Memo2->Clear();
	Memo10->Clear();
	Memo11->Clear();
	Memo14->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button14Click(TObject *Sender)
{
	if (SaveDialog1->Execute())
	{
		Memo4->Lines->SaveToFile(SaveDialog1->FileName);
	}
}
//---------------------------------------------------------------------------


