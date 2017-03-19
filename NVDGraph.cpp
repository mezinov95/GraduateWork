//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <stdio.h>

#include "NVDGraph.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TNVDGraph *)
{
   new TNVDGraph(NULL);
}
//---------------------------------------------------------------------------
__fastcall TNVDGraph::TNVDGraph(TComponent* Owner)
   : TGraphicControl(Owner)
{
   int i;
   FUpdateCount=1;
   Canvas->Font->Name="Arial";
   FFont=Canvas->Font;
   TH=0;
   FColorBk=Canvas->Brush->Color;
   MyCanvas=Canvas;
   Height=200;  // Полная высота области рисования
   Width=300;   // Полная ширина области рисования
   Axtop=25;
   Axbot=40;
   Delt0=5; Delt1=3; //Длина штрихов больших и маленьких
   xp=40; yp=Height-Axbot;
   lx=Width-xp-xp/2; ly=yp-Axtop;
   FAvtoScale=false;

  TDateTime DT;
  DT=Now();
  unsigned short year, month, day;
  unsigned short hour, minute, second, msec;
  DT.DecodeDate(&year, &month, &day);
  DT.DecodeTime(&hour, &minute, &second, &msec);
  DTZero.dt.day=day;
  DTZero.dt.month=month;
  DTZero.dt.year=year;
  DTZero.tm.hour=hour;
  DTZero.tm.minute=minute;
  DTZero.tm.second=second;
  DTZero.tm.hsecond=0;
  xdtmin=DTZero;
  AddDeltaTime(xdtmin,60,xdtmax);
  ydtmin=DTZero;
  ydtmax=xdtmax;

  xmin=0; xmax=100;
  ymin=0; ymax=100;
  nwx=1.;
  Items.xdtmin=xdtmin; Items.xdtmax=xdtmax;
  Items.ydtmin=ydtmin; Items.ydtmax=ydtmax;
  Items.xmin=0; Items.xmax=100;
  Items.ymin=0; Items.ymax=100;
  Items.nwx=1.;
  for(i=0; i < MAXNBUF; i++)
  {
   Item[i].xdtmin=xdtmin; Item[i].xdtmax=xdtmax;
   Item[i].ydtmin=ydtmin; Item[i].ydtmax=ydtmax;
   Item[i].xmin=0; Item[i].xmax=100;
   Item[i].ymin=0; Item[i].ymax=100;
   Item[i].nwx=1.;
  }
  divide=1.;
  FUseForAll=true;// используются общие параметры отображения
  FUseForAllX=false;// используются общие параметры отображения X
  FUseForAllY=false;// используются общие параметры отображения Y
  FUseScale=false; //Пока не будем использовать ScaleX..Y
  FScaleX=1.; //Коэффициент домножения
  FScaleY=1.; //Коэффициент домножения
  idDrawTextAX=true;
  idDrawTextAY=true;
  NoAxes=false;
  FFastDraw=true;
  FGrouped=true;
  for(int i=0; i < MAXNBUF; i++)
     TypeBufY[i]=tbUChar;
  TypeBufX=tbUChar;
  TypeBufIdY=tbChar;
  TypeBufIdX=tbChar;
  FKind=pgPoint;
  FNBuf=0;
   memset(FBufY,0,sizeof(FBufY));
   memset(FBufX,0,sizeof(FBufX));
   memset(FBufIdY,0,sizeof(FBufIdY));
   memset(FBufIdX,0,sizeof(FBufIdX));
   memset(FGetVarY,0,sizeof(FGetVarY));
   memset(FGetVarX,0,sizeof(FGetVarX));
   memset(FGetVarIdY,0,sizeof(FGetVarIdY));
   memset(FGetVarIdX,0,sizeof(FGetVarIdX));
   memset(FGetVarYDT,0,sizeof(FGetVarYDT));
   memset(FGetVarXDT,0,sizeof(FGetVarXDT));
   memset(FNPoint,0,sizeof(FNPoint));
   memset(FIDPointX,0,sizeof(FIDPointX));
   memset(FIDPointY,0,sizeof(FIDPointY));
   MaskaPaint=-1;
   MaskaViewBuf=0;
   memdc=0;
   FPenColor[0]=clBlack; FPenColor[1]=clMaroon;
   FPenColor[2]=clGreen; FPenColor[3]=clOlive;
   FPenColor[4]=clNavy; FPenColor[5]=clPurple;
   FPenColor[6]=clTeal; FPenColor[7]=clGray;
   FPenColor[8]=clSilver; FPenColor[9]=clRed;
   FPenColor[10]=clLime; FPenColor[11]=clYellow;
   FPenColor[12]=clBlue; FPenColor[13]=clFuchsia;
   FPenColor[14]=clAqua; FPenColor[15]=clInfoBk;
   ControlStyle<<csOpaque;
   FLabelX="LabelX";
   FLabelY="LabelY";
   FGridY=false;
   FGridX=false;
   FUseforHit=false;
   FNvdUserDraw=NULL;
   FUpdateCount=0;
}
__fastcall TNVDGraph::~TNVDGraph()
{
  //default
}
//---------------------------------------------------------------------------
bool __fastcall TNVDGraph::SaveToFile(AnsiString filename,bool id_emf)
{
  TMetafile *pMetafile=NULL;
  TMetafileCanvas *pCanvas=NULL;
  bool oldfast;
  if(filename.Length()==0)return false;
  pMetafile = new TMetafile;
  pMetafile->Enhanced=id_emf;
  pMetafile->Width=Width;//8300;
  pMetafile->Height=Height; //4250;
  pCanvas = new TMetafileCanvas(pMetafile,Canvas->Handle);
  oldfast=FFastDraw;
  FFastDraw=false;
  pCanvas->Font->Assign(MyCanvas->Font);
  pCanvas->Brush->Color=ColorBk;
  pCanvas->Pen->Assign(MyCanvas->Pen);
  MyCanvas=pCanvas;
  Paint();
  FFastDraw=oldfast;
  MyCanvas=Canvas;
  delete pCanvas;
  pMetafile->SaveToFile(filename);
  delete pMetafile;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetAvtoScale(bool Value)//Автоподбор границ
{
  if(FAvtoScale !=Value)
  {
    FAvtoScale=Value;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetAvtoScaleX(bool Value)//Автоподбор границ для X
{
  if(FAvtoScaleX !=Value)
  {
    FAvtoScaleX=Value;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetAvtoScaleY(bool Value)//Автоподбор границ для Y
{
  if(FAvtoScaleY !=Value)
  {
    FAvtoScaleY=Value;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetLabelX(AnsiString label)//Подпись оси X
{
  if(FLabelX !=label)
  {
    FLabelX=label;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetLabelY(AnsiString label)//Подпись оси Y
{
  if(FLabelY !=label)
  {
    FLabelY=label;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetGridY(bool Value)
{
  if(FGridY !=Value)
  {
    FGridY=Value;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetGridX(bool Value)
{
  if(FGridX !=Value)
  {
    FGridX=Value;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetUseforHit(bool Value)
{
  if(FUseforHit !=Value)
  {
    FUseforHit=Value;
    StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetNameItem(int numitem,AnsiString &Value)
{
   if(numitem >MAXNBUF-1)return;
   if(numitem < 0)return;
   if(FNameItem[numitem] !=Value)
   {
    FNameItem[numitem]=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TNVDGraph::GetNameItem(int numitem)
{
   if(numitem >MAXNBUF-1)return "";
   if(numitem < 0)return "";
   return FNameItem[numitem];
}
//---------------------------------------------------------------------------
 //Установка текущих параметров в режиме UseForAll=true
void __fastcall TNVDGraph::ItemsToWork()
{
  if(FUseForAll==false && !FUseForAllX && !FUseForAllY && FGrouped==false)return;
  if(FGrouped || FUseForAll||FUseForAllX)
  {
   xmin=Items.xmin; xmax=Items.xmax;
   xdtmin=Items.xdtmin; xdtmax=Items.xdtmax;
   if(Items.nwx==0)Items.nwx=1;
   nwx=Items.nwx;
   onex=Items.onex;
     //Надо сделать xmax кратным на шаг гистограммы
    if(FKind==pgBar)
    {
     dxmax=(int((xmax-xmin)/nwx)+1)*nwx-(xmax-xmin);
     xmax +=dxmax;//Для гистограммы увеличим на dxmax
     if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
     {
       AddDeltaTime(DTZero,xmax,xdtmax);
     }
    }
   }
  if(FGrouped || FUseForAll || FUseForAllY)
  {
   ymin=Items.ymin; ymax=Items.ymax;
   ydtmin=Items.ydtmin; ydtmax=Items.ydtmax;
   oney=Items.oney;
  }
}
//---------------------------------------------------------------------------
 //Сохранение текущих параметров в режиме UseForAll=true
void __fastcall TNVDGraph::WorkToItems()
{
  if(FUseForAll==false && !FUseForAllX && !FUseForAllY && FGrouped==false)return;
  if(FGrouped || FUseForAll ||FUseForAllX)
  {
    Items.onex=onex;
    Items.dxmax=dxmax;
  }
  if(FGrouped || FUseForAll || FUseForAllY)
   Items.oney=oney;
}
//---------------------------------------------------------------------------
 //Установка текущих параметров в режиме UseForAll=false
void __fastcall TNVDGraph::ItemToWork(int numitem)
{
  if(FUseForAll==true)return;
  if(FGrouped)
  {
   xmin=Items.xmin; xmax=Items.xmax;
   xdtmin=Items.xdtmin; xdtmax=Items.xdtmax;
   ymin=Items.ymin; ymax=Items.ymax;
   ydtmin=Items.ydtmin; ydtmax=Items.ydtmax;
   if(Items.nwx==0)Items.nwx=1;
   nwx=Items.nwx;
   onex=Items.onex; oney=Items.oney;
     //Надо сделать xmax кратным на шаг гистограммы
    if(FKind==pgBar)
    {
     dxmax=(int((xmax-xmin)/nwx)+1)*nwx-(xmax-xmin);
     xmax +=dxmax;//Для гистограммы увеличим на dxmax
     if(FTypeBufX==tbTime  || FTypeBufX==tbFuncDT)
     {
       AddDeltaTime(DTZero,xmax,xdtmax);
     }
    }
  }
  else
  {
   if(!FUseForAllX)
   {
     xmin=Item[numitem].xmin; xmax=Item[numitem].xmax;
     xdtmin=Item[numitem].xdtmin; xdtmax=Item[numitem].xdtmax;
    if(Item[numitem].nwx==0)Item[numitem].nwx=1;
    nwx=Item[numitem].nwx;
    onex=Item[numitem].onex;
      //Надо сделать xmax кратным на шаг гистограммы
     if(FKind==pgBar)
     {
      dxmax=(int((xmax-xmin)/nwx)+1)*nwx-(xmax-xmin);
      xmax +=dxmax;//Для гистограммы увеличим на dxmax
      if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
      {
        AddDeltaTime(DTZero,xmax,xdtmax);
      }
     }
   }
   else
   {
    xmin=Items.xmin; xmax=Items.xmax;
    xdtmin=Items.xdtmin; xdtmax=Items.xdtmax;
    if(Items.nwx==0)Items.nwx=1;
    nwx=Items.nwx;
    onex=Items.onex;
     //Надо сделать xmax кратным на шаг гистограммы
    if(FKind==pgBar)
    {
     dxmax=(int((xmax-xmin)/nwx)+1)*nwx-(xmax-xmin);
     xmax +=dxmax;//Для гистограммы увеличим на dxmax
     if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
     {
       AddDeltaTime(DTZero,xmax,xdtmax);
     }
    }
   }
   if(!FUseForAllY)
   {
     ymin=Item[numitem].ymin; ymax=Item[numitem].ymax;
     ydtmin=Item[numitem].ydtmin; ydtmax=Item[numitem].ydtmax;
     oney=Item[numitem].oney;
   }
   else
   {
    ymin=Items.ymin; ymax=Items.ymax;
    ydtmin=Items.ydtmin; ydtmax=Items.ydtmax;
    oney=Items.oney;
   }
  }
}
//---------------------------------------------------------------------------
 //Сохранение текущих параметров в режиме UseForAll=false
void __fastcall TNVDGraph::WorkToItem(int numitem)
{
  if(FUseForAll==true)return;
  if(FGrouped)
  {
   Items.onex=onex; Items.oney=oney;
   Items.dxmax=dxmax;
  }
  else
  {
   if(FUseForAllX)
   {
     Items.onex=onex;
     Items.dxmax=dxmax;
   }
   else
   {
    Item[numitem].onex=onex;
    Item[numitem].dxmax=dxmax;
   }
   if(FUseForAllY)
   {
     Items.oney=oney;
   }
   else
   {
    Item[numitem].oney=oney;
   }
  }

}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetUseScale(bool Value)
{
   if(FUseScale !=Value)
   {
    FUseScale=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetUseForAll(bool Value)
{
   if(FUseForAll !=Value)
   {
    FUseForAll=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetUseForAllX(bool Value)
{
   if(FUseForAllX !=Value)
   {
    FUseForAllX=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetUseForAllY(bool Value)
{
   if(FUseForAllY !=Value)
   {
    FUseForAllY=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetScaleX(double Value)
{
   if(Value==0.)return;
   if(FScaleX !=Value)
   {
    FScaleX=Value;
    if(FUseScale)StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetScaleY(double Value)
{
   if(Value==0.)return;
   if(FScaleY !=Value)
   {
    FScaleY=Value;
    if(FUseScale)StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetFastDraw(bool Value)
{
   if(FFastDraw !=Value)
   {
    FFastDraw=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetGrouped(bool Value)
{
   if(FGrouped !=Value)
   {
    FGrouped=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetMaska(int Value)
{
   if(MaskaViewBuf !=Value)
   {
    MaskaViewBuf=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetType(TNVDGraphType Value)
{
   if(FType !=Value)
   {
    FType=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetTypeBufX(TNVDGraphTypeBuf Value)
{
   if(FTypeBufX !=Value)
   {
    FTypeBufX=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetTypeBufY(int numitem,TNVDGraphTypeBuf Value)
{
   if(numitem >MAXNBUF-1)return;
   if(numitem < 0)return;
   if(FTypeBufY[numitem] !=Value)
   {
    FTypeBufY[numitem]=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
TNVDGraphTypeBuf __fastcall TNVDGraph::GetTypeBufY(int numitem)
{
   if(numitem >MAXNBUF-1)return tbChar;
   if(numitem < 0)return tbChar;
   return FTypeBufY[numitem];
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetTypeBufIdX(TNVDGraphTypeBuf Value)
{
   if(FTypeBufIdX !=Value)
   {
    FTypeBufIdX=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetTypeBufIdY(TNVDGraphTypeBuf Value)
{
   if(FTypeBufIdY !=Value)
   {
    FTypeBufIdY=Value;
    StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetBufY(Pointer ABuf, long *AID, int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FBufY[Value] !=ABuf || FIDPointY[Value] != AID)
   {
    FBufY[Value]=ABuf;
    FIDPointY[Value]=AID;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetFuncY(TNvdGetVar AFunc, TNvdGetVar AFuncId, int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FGetVarY[Value] !=AFunc || FGetVarIdY[Value] != AFuncId)
   {
    FGetVarY[Value]=AFunc;
    FGetVarIdY[Value]=AFuncId;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetFuncX(TNvdGetVar AFunc, TNvdGetVar AFuncId, int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FGetVarX[Value] !=AFunc || FGetVarIdX[Value] != AFuncId)
   {
    FGetVarX[Value]=AFunc;
    FGetVarIdX[Value]=AFuncId;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetFuncYDT(TNvdGetVarDT AFunc, TNvdGetVar AFuncId, int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FGetVarYDT[Value] !=AFunc || FGetVarIdY[Value] != AFuncId)
   {
    FGetVarYDT[Value]=AFunc;
    FGetVarIdY[Value]=AFuncId;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetFuncXDT(TNvdGetVarDT AFunc, TNvdGetVar AFuncId, int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FGetVarXDT[Value] !=AFunc || FGetVarIdX[Value] != AFuncId)
   {
    FGetVarXDT[Value]=AFunc;
    FGetVarIdX[Value]=AFuncId;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetBufX(Pointer ABuf, long *AID,int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FBufX[Value] !=ABuf || FIDPointX[Value] != AID)
   {
    FBufX[Value]=ABuf;
    FIDPointX[Value]=AID;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetBufIdX(Pointer ABuf,int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FBufIdX[Value] !=ABuf)
   {
    FBufIdX[Value]=ABuf;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetNPoint(unsigned long *ANPoint, int Value)
{
   if(Value >MAXNBUF-1)return;
   if(Value < 0)return;
   if(FNPoint[Value] !=ANPoint)
   {
    FNPoint[Value] =ANPoint;
    if(FNBuf > Value && (MaskaViewBuf&(1<<Value)))
      StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
unsigned long __fastcall TNVDGraph::GetNPoint(int Value)
{
   if(Value >MAXNBUF-1)return 0;
   if(Value < 0)return 0;
   if(FNPoint[Value]==NULL)return 0;
   return *FNPoint[Value];
}
//---------------------------------------------------------------------------
bool __fastcall TNVDGraph::GetValueXY(int index,  int numitem, double &x, double &y)
{
 int i,n,j;
   char *acharx, *achary;
   unsigned char *aucharx, *auchary;
   short *ashortx, *ashorty;
   unsigned short *aushortx, *aushorty;
   long *alongx,*alongy;
   unsigned long *aulongx,*aulongy;
   float *afloatx,*afloaty;
   double *adoublex,*adoubley;
   long *aidx,*aidy;
   TDateTimeKadr *atimex,*atimey;

   TNvdGetVar afuncy,afuncx,afuncidx,afuncidy;
   TNvdGetVarDT afuncydt,afuncxdt;

   i=numitem;
   if(i < 0 || i >=MAXNBUF)return false;
      if( (MaskaViewBuf&(1<<i))==0)return false; //В маске нет
      if( (FTypeBufY[i]!=tbFunc && !FBufY[i] ) &&
          (FTypeBufY[i]!=tbFuncDT && !FBufY[i] ) )return false; //Адрес буфера не задан
      if(FTypeBufY[i]==tbFunc && !FGetVarY[i])return false; //Адрес функции не задан
      if(FTypeBufY[i]==tbFuncDT && !FGetVarYDT[i])return false; //Адрес функции не задан
      if(!FNPoint[i])return false; //Адрес c количеством точек не задан
      n=*FNPoint[i];
      if(n==0)return false;
      if(index >= n)return false;
      switch(FTypeBufX)
      {
        case tbChar: acharx=(char *)FBufX[i]; break;
        case tbUChar: aucharx=(unsigned char *)FBufX[i]; break;
        case tbShort: ashortx=(short *)FBufX[i]; break;
        case tbUShort: aushortx=(unsigned short *)FBufX[i]; break;
        case tbInt:
        case tbLong: alongx=(long *)FBufX[i]; break;
        case tbUInt:
        case tbULong: aulongx=(unsigned long *)FBufX[i]; break;
        case tbFloat: afloatx=(float *)FBufX[i]; break;
        case tbDouble: adoublex=(double *)FBufX[i]; break;
        case tbFunc: afuncx=FGetVarX[i]; break;
        case tbTime: atimex=(TDateTimeKadr *)FBufX[i]; break;
        case tbFuncDT: afuncxdt=FGetVarXDT[i]; break;
      }
      switch(FTypeBufY[i])
      {
        case tbChar: achary=(char *)FBufY[i]; break;
        case tbUChar: auchary=(unsigned char *)FBufY[i]; break;
        case tbShort: ashorty=(short *)FBufY[i]; break;
        case tbUShort: aushorty=(unsigned short *)FBufY[i]; break;
        case tbInt:
        case tbLong: alongy=(long *)FBufY[i]; break;
        case tbUInt:
        case tbULong: aulongy=(unsigned long *)FBufY[i]; break;
        case tbFloat: afloaty=(float *)FBufY[i]; break;
        case tbDouble: adoubley=(double *)FBufY[i]; break;
        case tbFunc: afuncy=FGetVarY[i]; break;
        case tbTime: atimey=(TDateTimeKadr *)FBufY[i]; break;
        case tbFuncDT: afuncydt=FGetVarYDT[i]; break;
      }
      aidx=FIDPointX[i];
      aidy=FIDPointY[i];
      afuncidx=FGetVarIdX[i];
      j=index; //Пробегаемся по массиву точек
        if(afuncidx)
        {
          if(afuncidx(j,i)==0)return false; //Эту точку не рисовать
        }
        if(aidx)
        {
          if(aidx[j]==0)return false; //Эту точку не рисовать
        }
        if(aidy)
        {
          if(aidy[j]==0)return false; //Эту точку не рисовать
        }
        //Вытащим точку из буферов
        if( (FTypeBufX!=tbFunc &&FBufX[i]) || (FTypeBufX==tbFunc &&FGetVarX[i])
            || (FTypeBufX==tbFuncDT &&FGetVarXDT[i]))
        {
          switch(FTypeBufX)
          {
            case tbChar: x=acharx[j]; break;
            case tbUChar: x=aucharx[j]; break;
            case tbShort: x=ashortx[j]; break;
            case tbUShort: x=aushortx[j]; break;
            case tbInt:
            case tbLong: x=alongx[j]; break;
            case tbUInt:
            case tbULong: x=aulongx[j]; break;
            case tbFloat: x=afloatx[j]; break;
            case tbDouble: x=adoublex[j]; break;
            case tbFunc: x=afuncx(j,i); break;
            case tbTime: x=*(double*)&atimex[j]; break;
            case tbFuncDT: afuncxdt(j,*(TDateTimeKadr *)&x,i); break;
          }
        }
        else { x=j;}
        switch(FTypeBufY[i])
        {
          case tbChar: y=achary[j]; break;
          case tbUChar: y=auchary[j]; break;
          case tbShort: y=ashorty[j]; break;
          case tbUShort: y=aushorty[j];break;
          case tbInt:
          case tbLong: y=alongy[j]; break;
          case tbUInt:
          case tbULong: y=aulongy[j]; break;
          case tbFloat: y=afloaty[j]; break;
          case tbDouble: y=adoubley[j]; break;
          case tbFunc: y=afuncy(j,i); break;
          case tbTime: y=*(double*)&atimey[j]; break;
          case tbFuncDT: afuncydt(j,*(TDateTimeKadr *)&x,i); break;
        }
       return true;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetNBuf(int Value)
{
   if(Value >MAXNBUF)Value=MAXNBUF;
   if(Value < 0)Value=0;
	if (FNBuf != Value)
   {
     FNBuf = Value;
     StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetGraphKind(TNVDGraphKind Value)
{
   if (FKind != Value)
   {
     FKind = Value;
     StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetCaption(String Value)
{
   if (Caption != Value)
   {
   Caption = Value;
   StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetAxesLeft(long Value)
{
   if (xp != Value)
   {
   xp = Value;
   StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetAxesTop(long Value)
{
   if (Axtop != Value)
   {
   Axtop=Value;
   StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetAxesBot(long Value)
{
   if (yp != Height-Value)
   {
   Axbot=Value;
   yp = Height-Axbot;
   StyleChanged(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetFont(TFont* Value)
{
  MyCanvas->Font->Assign(Value);
  StyleChanged(this);
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetColorBk(TColor Value)
{
  if(FColorBk!=Value)
  {
     FColorBk=Value;
     MyCanvas->Brush->Color=Value;
     StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetXmin(double Value)
{
  if(FUseScale)Value /=FScaleX;
  if (Items.xmin != Value)
  {
     xmin=Value;
     Items.xmin=Value;
     if(UseForAllX || UseForAll || FGrouped)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetXmax(double Value)
{
  if(FUseScale)Value /=FScaleX;
  if (Items.xmax != Value)
  {
     xmax=Value;
     Items.xmax=Value;
     if(UseForAllX || UseForAll || FGrouped)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetXDTmin(TDateTimeKadr& Value)
{
//  if(FUseScale)Value /=FScaleX;
  if (Items.xdtmin.time != Value.time || Items.xdtmin.date != Value.date)
  {
     xmin=GetDeltaTime(DTZero,Value);
     Items.xmin=xmin;
     xdtmin=Value;
     Items.xdtmin=Value;
     if(UseForAllX || UseForAll || FGrouped)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetXDTmax(TDateTimeKadr& Value)
{
//  if(FUseScale)Value /=FScaleX;
  if (Items.xdtmax.time != Value.time || Items.xdtmax.date != Value.date)
  {
     xmax=GetDeltaTime(DTZero,Value);
     Items.xmax=xmax;
     xdtmax=Value;
     Items.xdtmax=Value;
     if(UseForAllX || UseForAll || FGrouped)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetXsum(double Value)
{
  if(Value==0)Value=1;
  if (Items.nwx != Value || Value > 0)
  {
     nwx=Value;
     Items.nwx=Value;
     if(UseForAllX || UseForAll || FGrouped)
        if(FType==gtHistogram)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetYmin(double Value)
{
  if(FUseScale)Value /=FScaleY;
  if (Items.ymin != Value)
  {
     ymin=Value;
     Items.ymin=Value;
     if(UseForAllY || UseForAll || FGrouped)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetYmax(double Value)
{
  if(FUseScale)Value /=FScaleY;
  if (Items.ymax != Value)
  {
     ymax=Value;
     Items.ymax=Value;
     if(UseForAllY || UseForAll || FGrouped)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetXmin()
{
  if(FUseScale)return  (Items.xmin*FScaleX);
  else return Items.xmin;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetXmax()
{
  if(FUseScale)return  (Items.xmax*FScaleX);
  else return Items.xmax;
}
//---------------------------------------------------------------------------
TDateTimeKadr __fastcall TNVDGraph::GetXDTmin()
{
  return Items.xdtmin;
}
//---------------------------------------------------------------------------
TDateTimeKadr __fastcall TNVDGraph::GetXDTmax()
{
  return Items.xdtmax;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetXsum()
{
  return Items.nwx;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetYmin()
{
  if(FUseScale)return  (Items.ymin*FScaleY);
  else return Items.ymin;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetYmax()
{
  if(FUseScale)return  (Items.ymax*FScaleY);
  else return Items.ymax;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneXmin(int numitem,double Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
  if(FUseScale)Value /=FScaleX;
  if (Item[numitem].xmin != Value)
  {
     Item[numitem].xmin=Value;
     if(FUseForAll==false && !FUseForAllX)
     StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneXmax(int numitem,double Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
  if(FUseScale)Value /=FScaleX;
  if (Item[numitem].xmax != Value)
  {
     Item[numitem].xmax=Value;
     if(FUseForAll==false && !FUseForAllX)
     StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneXDTmin(int numitem,TDateTimeKadr& Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
//  if(FUseScale)Value /=FScaleX;
  if (Item[numitem].xdtmin.time != Value.time || Item[numitem].xdtmin.date != Value.date)
  {
     Item[numitem].xmin=GetDeltaTime(DTZero,Value);
     Item[numitem].xdtmin=Value;
     if(FUseForAll==false && !FUseForAllX)
     StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneXDTmax(int numitem,TDateTimeKadr& Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
//  if(FUseScale)Value /=FScaleX;
  if (Item[numitem].xdtmax.time != Value.time || Item[numitem].xdtmax.date != Value.date)
  {
     Item[numitem].xmax=GetDeltaTime(DTZero,Value);
     Item[numitem].xdtmax=Value;
     if(FUseForAll==false && !FUseForAllX)
     StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneXsum(int numitem,double Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
   if(Value==0)Value=1;
  if (Item[numitem].nwx != Value || Value > 0)
  {
     Item[numitem].nwx=Value;
     if(FType==gtHistogram)
      if(FUseForAll==false && !FUseForAllX)StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneYmin(int numitem,double Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
  if(FUseScale)Value /=FScaleY;
  if (Item[numitem].ymin != Value)
  {
     Item[numitem].ymin=Value;
     if(FUseForAll==false && !FUseForAllY)
      StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetOneYmax(int numitem,double Value)
{
   if(numitem >=MAXNBUF)return;
   if(numitem < 0 )return;
  if(FUseScale)Value /=FScaleY;
  if (Item[numitem].ymax != Value)
  {
     Item[numitem].ymax=Value;
     if(FUseForAll==false && !FUseForAllY)
      StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetOneXmin(int numitem)
{
   if(numitem >=MAXNBUF)return 0;
   if(numitem < 0 )return 0;
  if(FUseScale)return  (Item[numitem].xmin*FScaleX);
  else return Item[numitem].xmin;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetOneXmax(int numitem)
{
   if(numitem >=MAXNBUF)return 0;
   if(numitem < 0 )return 0;
  if(FUseScale)return  (Item[numitem].xmax*FScaleX);
  else return Item[numitem].xmax;
}
//---------------------------------------------------------------------------
TDateTimeKadr __fastcall TNVDGraph::GetOneXDTmin(int numitem)
{
   if(numitem >=MAXNBUF)return DTZero;
   if(numitem < 0 )return DTZero;
   return Item[numitem].xdtmin;
}
//---------------------------------------------------------------------------
TDateTimeKadr __fastcall TNVDGraph::GetOneXDTmax(int numitem)
{
   if(numitem >=MAXNBUF)return DTZero;
   if(numitem < 0 )return DTZero;
   return Item[numitem].xdtmax;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetOneXsum(int numitem)
{
   if(numitem >=MAXNBUF)return 0;
   if(numitem < 0 )return 0;
  return Item[numitem].nwx;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetOneYmin(int numitem)
{
   if(numitem >=MAXNBUF)return 0;
   if(numitem < 0 )return 0;
  if(FUseScale)return  (Item[numitem].ymin*FScaleY);
  else return Item[numitem].ymin;
}
//---------------------------------------------------------------------------
double __fastcall TNVDGraph::GetOneYmax(int numitem)
{
   if(numitem >=MAXNBUF)return 0;
   if(numitem < 0 )return 0;
  if(FUseScale)return  (Item[numitem].ymax*FScaleY);
  else return Item[numitem].ymax;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::StyleChanged(TObject *Sender)
{
    if(FUpdateCount==0)
	Invalidate(); // Force paint
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Paint(void)
{
   int W,H;
   int i,k;
   memdc=0;
   if(FNBuf==0 || MaskaViewBuf==0)return;
  HDC dc; //handle to a display contect
//  HDC memdc; //handle to a display contect
  HBITMAP membitmap; //handle to a bitmap
  HBITMAP oldmembitmap; //handle to a bitmap
  HPEN pen; //handle to a pen
  HPEN oldpen; //handle to a pen
  HBRUSH brush; //handle to a brush
  HBRUSH oldbrush; //handle to a brush
  HFONT font; //handle to a Font
  HFONT oldfont; //handle to a Font
   W=Width;
   H=Height;
  if(FFastDraw)
  {
  //retrieve a handle to the screen dc so we can create one like it
  dc = GetDC(0);
  //create a dc that is compatable with the screen
  memdc = CreateCompatibleDC(dc);

  //create a bitmap surface thats compatible with the screen
  //note: if you want a monochrome bitmap pass zero instead of "dc"
  membitmap = CreateCompatibleBitmap(dc, W, H);

  //release the screen dc as soon as possible
  ReleaseDC(0, dc);
  //select our bitmap surface into our dc and remember the old bitmap
  oldmembitmap = SelectObject(memdc, membitmap);
  //initialize our dc by painting it white,
  //otherwise it will contain random pixel values
  PatBlt(memdc,
         0, 0,
         W, H,
         WHITENESS);
  pen=MyCanvas->Pen->Handle;
  brush=MyCanvas->Brush->Handle;
  font=MyCanvas->Font->Handle;
  //select our new pen into the dc, and remember the old default pen
  oldpen = SelectObject(memdc, pen);
  //select our new brush into the dc, and remember the old brush
  oldbrush = SelectObject(memdc, brush);
  //select our new font into the dc, and remember the old font
  oldfont = SelectObject(memdc, font);
  }
   if(!memdc)
   {
    if(!NoAxes)MyCanvas->FillRect(Rect(0,0,W,H));
    else Clear();
   }
   GoDraw();
   if(FFastDraw)
   {
    //copy form our dc to the forms canvas
    if(!NoAxes)
       BitBlt(MyCanvas->Handle,  //destination
         0, 0,      //x,y start point on the form
         W, H,  //the width and height of our bitmap
         memdc,     //the source dc
         0, 0,      //blt from 0,0 in the source
         SRCCOPY);  //COPY form source operation
    else
    {
      SetAxesPar(0);
    if(FGrouped)
    {
       BitBlt(MyCanvas->Handle,  //destination
         xp+1, yp-ly+1,      //x,y start point on the form
         lx-1,ly-1,  //the width and height of our bitmap
         memdc,     //the source dc
         xp+1, yp-ly+1,      //blt from 0,0 in the source
         SRCCOPY);  //COPY form source operation
//     MyCanvas->Rectangle(xp+1,yp-1,xp+lx-1,yp-ly+1);
    }
    else
    {
     k=-1;
//     for(i=0; i < MAXNBUF; i++)
     for(i=MAXNBUF-1; i >=0; i--)
     {
      if( (MaskaViewBuf&(1<<i))==0)continue; //В маске нет
       k++;
      if( (MaskaPaint&(1<<i))==0)continue; //В маске нет
       Setyp(k);
       BitBlt(MyCanvas->Handle,  //destination
         xp+1, yp-ly+1,      //x,y start point on the form
         lx-1,ly-1,  //the width and height of our bitmap
         memdc,     //the source dc
         xp+1, yp-ly+1,      //blt from 0,0 in the source
         SRCCOPY);  //COPY form source operation
     }
    }

    }
    //select the old default font into our dc
    SelectObject(memdc, oldfont);
    //select the old default brush into our dc
    SelectObject(memdc, oldbrush);
    //select the old default pen into our dc
    SelectObject(memdc, oldpen);
    //select the old default bitmap into our dc
    SelectObject(memdc, oldmembitmap);
    //delete the bitmap we created
    DeleteObject(membitmap);
    //delete the dc we created
    DeleteDC(memdc);
   }
   memdc=0;
//   Canvas->Rectangle(xp,yp,xp+lx,yp+ly);
//   Canvas->Font=Font;
//   Canvas->TextOut(2, 2, Caption);
}
//---------------------------------------------------------------------------
// analr - распаковывает число a в виде am*10**n ,
//                n - возвращаемое значение
//				1.<= am < 10.
long __fastcall TNVDGraph::analr(double a, double* am)
{
	long i, n=0 ;
	double da=1. ;
	*am = a ;
	if( fabs(a) >= 1. )
	{
		for(i=0; i < 99; i++)
		{
			*am /= da ;
			if(fabs(*am) < 10.)return n ;
			if(i==0)da *= 10.;
			n++ ;
		}
	}
	else
	{
		for(i=0; i < 99; i++)
		{
			*am *= da ;
			if(fabs(*am) > 1.)return n;
			if(i==0)da *= 10.;
			n-- ;
		}
	}
	return n ;
}
//---------------------------------------------------------------------------
// xp,yp - точка начала оси, lx - длина оси
// xminn,xmaxx - границы оси
void __fastcall TNVDGraph::x_axes(double xminn, double xmaxx, long xp,long yp,long lx)
{
        HPEN oldpen;
	char buf[40] ;
	long i,j,n,np, kmin, kmax, kdx,k,kz,ix,ndel,ndelm,lzx,lzy;
	double xmin1, xmax1, dx ,f, xd,smes,stepen,dxx,fd;
// ======Определение кол-ва делений оси ndel и шаг dxx =====
// Определим смещение оси smes ( если xminn=654843.3 xmaxx=654843.5)
	lzx=MyCanvas->TextWidth("0");
	lzy=MyCanvas->TextHeight("0")-TH+5+Delt0;
	smes=0. ; stepen = 1. ;
	if(xmaxx == xminn)if(xminn)xmaxx += xminn/10. ; else xmaxx=1. ;
	f=fabs(xmaxx-xminn) ;
	k=analr(f,&xd) ;				// -1
	kmin= analr(xminn,&xmin1) ; // 5
	kmax= analr(xmaxx,&xmax1) ; // 5
	if(xmin1*xmax1 > 0.)  // если одного знака
	{
		if(kmax == kmin)
		{
			n = k+1;      // -1
			if( kmax-k > 2)
			{
			 f=pow(10,n) ;
			 smes=floor(xmaxx/f)*f ; // округлили вниз на f ( 654843 )
			 if(xmax1 < 0.)smes = smes+f;
			}
		}
	}
	if(smes)
	{
		if(xminn != smes)xminn -= smes ; else xminn=0.;
		xmaxx -= smes ;
		if(fabs(xmaxx) < 1.)
		{
		  k=analr(xmaxx,&xd);
		  xd=floor(xd/1.e-3+0.5)*1.e-3 ;
		  xmaxx=xd*pow(10,k) ;
		}
	}
// Определим степень оси stepen ( если xminn=2.6E-7 xmaxx=2.7E-5)
	kmin= analr(xminn,&xmin1) ; // -7
	kmax= analr(xmaxx,&xmax1) ; // -5
	k=kmax ;  // -5
	if(kmin > k) k = kmin; // -7
	if( k > 5 || k < -2)stepen=pow(10,k) ;
	xminn /= stepen ; xmaxx /= stepen ;
// Определим сколько позиций для подписи xmin1 и xmax1
	kmin= analr(xminn,&xmin1) ;
	kmax= analr(xmaxx,&xmax1) ;
// Определим сколько позиций добавить для шага
	np = lzx ;		// первое приближение для расстояния одного шага в pixel
	k = kmin ;
	if(kmax > k) k = kmax;
	k = abs(k)+2 ;                      // +1 на точку
	if(xmin1 < 0. || xmax1 < 0.)k++ ;		// +1 на знак
 if(!FUseforHit)
 {
m0:
	xd = np*(xmaxx-xminn)/(lx-4) ;
// нормируем xd кратным  1, 2, 2.5, 5
	kdx = analr(xd,&dx) ;
	f = fabs(dx) ;
	dx = 1. ;
	if( f > 1. ) dx = 2. ;
//	if( f > 2. ) dx = 2.5 ;
	if( f > 2. ) dx = 5. ;
//	if( f > 2.5 ) dx = 5. ;
	if( f > 5. ) { dx = 1. ; kdx++ ; }
	if( xd < 0.) dx = -dx ;
	xd = dx * pow(10, kdx ) ;
	kz = abs(k)+1 ;
	if( kdx > k ) kz = abs(kdx)+1 ;
	if( k*kdx < 0) kz = abs(k) + abs(kdx)+1 ;
	if( kz*lzx >= np+lzx ){ np += lzx ; goto m0 ; }
	dxx = xd ;
  }
// Рисуем Ось
	if(!memdc)MyCanvas->MoveTo(xp,yp) ;
        else ::MoveToEx(memdc,xp,yp,NULL);
	if(!memdc)MyCanvas->LineTo(xp+lx,yp);
        else ::LineTo(memdc,xp+lx,yp);
// Рисуем деления и подписи
	onex=(xmaxx-xminn)/(lx-4) ; // коэффициент перевода в пикселы
        if(FUseforHit)
  	    dxx=16.;
	f=dxx/onex;		// расстояние между большими делениями
	ndelm=1 ;
	if(f/2. >= Delt0+1.)ndelm=2 ;
        if(FUseforHit)
        {
	 if(f/4. >= Delt0+1.)ndelm=4 ;
	 if(f/16. >= Delt0+1.)ndelm=16 ;
        }
        else
        {
	 if(f/5. >= Delt0+1.)ndelm=5 ;
	 if(f/10. >= Delt0+1.)ndelm=10 ;
        }
        if(divide !=1.)ndelm=5;
	dx=dxx/ndelm ;
//	xminn = floor(xminn/dx)*dx ;
//	if(xmaxx < 0.)xmaxx = ceil(xmaxx/dx)*dx ;
//	else  xmaxx = floor(xmaxx/dx)*dx ;
	onex=(xmaxx-xminn)/(lx-4) ; // коэффициент перевода в пикселы
	ndel=long((xmaxx-xminn)/dxx+0.5)+2 ;
	f = floor(xminn/dxx)*dxx ; // первое значение для деления
	for( i = 0; i < ndel; i++)
	{
	  ix= long((f-xminn)/onex+0.5)+xp+2;
	  if(ix < xp) goto m1 ;
	  if(ix > xp+lx)break ;
	  if(!memdc)MyCanvas->MoveTo(ix,yp);
          else ::MoveToEx(memdc,ix,yp,NULL);
	  if(!memdc)MyCanvas->LineTo(ix,yp+Delt0);
          else ::LineTo(memdc,ix,yp+Delt0);
          if(FGridX)
          {
	   if(!memdc)
           {
            MyCanvas->Pen->Style=psDash;
            MyCanvas->LineTo(ix,yp-ly);
            MyCanvas->Pen->Style=psSolid;
           }
           else
           {
            MyCanvas->Pen->Style=psDash;
            oldpen=SelectObject(memdc,MyCanvas->Pen->Handle);
            ::LineTo(memdc,ix,yp-ly);
            SelectObject(memdc,oldpen);
            MyCanvas->Pen->Style=psSolid;
           }
          }
	  f=floor(f/dxx+0.5)*dxx;
         if(idDrawTextAX)
         {
          if(FUseforHit)
  	   sprintf(buf,"%lg",f/divide/16.);
          else
	   sprintf(buf,"%lg",f/divide);
          if(!memdc)
	    MyCanvas->TextOut(ix-MyCanvas->TextWidth(buf)/2,yp+2+Delt0,buf );
          else
            ::TextOut(memdc,ix-MyCanvas->TextWidth(buf)/2,yp+2+Delt0,buf,
              strlen(buf));
         }
// Нарисуем маленькие деления
m1:  fd=f+dx;
	  for(j=1 ; j < ndelm ;j++)
	  {
		 ix = long((fd-xminn)/onex+0.5)+xp+2;
		 if(ix < xp) goto m2 ;
		 if(ix > xp+lx)break;
		 if(!memdc)MyCanvas->MoveTo(ix,yp);
                 else ::MoveToEx(memdc,ix,yp,NULL);
		 if(!memdc)MyCanvas->LineTo(ix,yp+Delt1);
                 else ::LineTo(memdc,ix,yp+Delt1);
m2:	 fd=floor((fd+dx)/dx+0.5)*dx;
	  }
	  f=floor((f+dxx)/dxx+0.5)*dxx;
	}
	  if(idDrawTextAX && (smes || stepen != 1.))
	  {
		  buf[0]=0;
		  if(stepen != 1.)sprintf(buf,"*%.0le",stepen);
		  i=strlen(buf);
		  if(smes)sprintf(&buf[i]," %+lg",smes);
                  if(!memdc)
		    MyCanvas->TextOut(xp+lx-MyCanvas->TextWidth(buf),yp+lzy,buf );
                  else
                   ::TextOut(memdc,xp+lx-MyCanvas->TextWidth(buf),yp+lzy,buf,
                    strlen(buf));
	  }
   onex=(xmaxx-xminn)*stepen/(lx-4) ; // коэффициент перевода в пикселы
   if(FUseScale)onex=(xmaxx-xminn)*stepen/FScaleX/(lx-4) ; // коэффициент перевода в пикселы
//   if(FUseScale)onex=(xmaxx-xminn)/FScaleX/(lx-4) ; // коэффициент перевода в пикселы
}
//---------------------------------------------------------------------------
// xp,yp - точка начала оси, ly - длина оси
// yminn,ymaxx - границы оси
void __fastcall TNVDGraph::y_axes(double yminn, double ymaxy, long xp,long yp,long ly)
{
        HPEN oldpen;
	char buf[40] ;
	long i,j,n,np, kmin, kmax, kdy,k,iy,ndel,ndelm,lzy;
	double ymin1, ymax1, dy ,f, yd,smes,stepen,dyy,fd;
// ======Определение кол-ва делений оси ndel и шаг dyy =====
// Определим смещение оси smes ( если yminn=654843.3 ymaxy=654843.5)
	lzy=MyCanvas->TextHeight("0")-TH;
	smes=0. ; stepen = 1. ;
	if(ymaxy == yminn)if(yminn)ymaxy += yminn/10. ; else ymaxy=1. ;
	f=fabs(ymaxy-yminn) ;
	k=analr(f,&yd) ;				// -1
	kmin= analr(yminn,&ymin1) ; // 5
	kmax= analr(ymaxy,&ymax1) ; // 5
	if(ymin1*ymax1 > 0.)  // если одного знака
	{
		if(kmax == kmin)
		{
			n = k+1;      // -1
			if( kmax-k > 2)
			{
			 f=pow(10,n) ;
			 smes=floor(ymaxy/f)*f ; // округлили вниз на f ( 654843 )
			 if(ymax1 < 0.)smes = smes+f;
			}
		}
	}
	if(smes)
	{
		if(yminn != smes)yminn -= smes ; else yminn=0.;
		ymaxy -= smes ;
		if(fabs(ymaxy) < 1.)
		{
		  k=analr(ymaxy,&yd);
		  yd=floor(yd/1.e-3+0.5)*1.e-3 ;
		  ymaxy=yd*pow(10,k) ;
		}
	}
// Определим степень оси stepen ( если yminn=2.6E-7 ymaxy=2.7E-5)
	kmin= analr(yminn,&ymin1) ; // -7
	kmax= analr(ymaxy,&ymax1) ; // -5
	k=kmax ;  // -5
	if(kmin > k) k = kmin; // -7
	if( k > 5 || k < -2)stepen=pow(10,k) ;
	yminn /= stepen ; ymaxy /= stepen ;
// Определим сколько позиций для подписи ymin1 и ymax1
//	kmin=
	 analr(yminn,&ymin1) ;
//	kmax=
	 analr(ymaxy,&ymax1) ;
// Определим сколько позиций добавить для шага
	np = 2*lzy ;	// первое приближение для расстояния одного шага в pixel
	oney = (ymaxy-yminn)/(ly-4) ;
m0:
	yd = np*oney ;
// нормируем yd кратным  1, 2, 2.5, 5
	kdy = analr(yd,&dy) ;
	f = fabs(dy) ;
	dy = 1. ;
	if( f > 1. ) dy = 2. ;
//     	if( f > 2. ) dy = 2.5 ;
     	if( f > 2. ) dy = 5. ;
//	if( f > 2.5 ) dy = 5. ;
	if( f > 5. ) { dy = 1. ; kdy++ ; }
	if( yd < 0.) dy = - dy ;
	yd = dy * pow(10, kdy ) ;
	if( yd/oney < np ){ np += lzy ; goto m0 ; }
	dyy = yd ;
// Рисуем Ось
	if(!memdc)MyCanvas->MoveTo(xp,yp) ;
        else ::MoveToEx(memdc,xp,yp,NULL);
	if(!memdc)MyCanvas->LineTo(xp,yp-ly);
        else ::LineTo(memdc,xp,yp-ly);
// Рисуем деления и подписи
	oney=(ymaxy-yminn)/(ly-4) ; // коэффициент перевода в пикселы
	f=dyy/oney;		// расстояние между большими делениями
	ndelm=1. ;
	if(f/2. >= Delt0+1.)ndelm=2 ;
	if(f/5. >= Delt0+1.)ndelm=5 ;
	if(f/10. >= Delt0+1.)ndelm=10 ;
        if(divide!=1)ndelm=5;
	dy=dyy/ndelm ;
//	yminn = floor(yminn/dy)*dy ;
//	if(ymaxy < 0.)ymaxy = ceil(ymaxy/dy)*dy ;
//	else  ymaxy = floor(ymaxy/dy)*dy ;
	oney=(ymaxy-yminn)/(ly-4) ; // коэффициент перевода в пикселы
	ndel=long((ymaxy-yminn)/dyy+0.5)+2 ;
	f = floor(yminn/dyy)*dyy ; // первое значение для деления
	for( i = 0; i < ndel; i++)
	{
	  iy= yp - long((f-yminn)/oney+0.5)-2;
	  if(iy > yp) goto m1 ;
	  if(iy < yp-ly)break ;
	  if(!memdc)MyCanvas->MoveTo(xp,iy);
          else ::MoveToEx(memdc,xp,iy,NULL);
	  if(!memdc)MyCanvas->LineTo(xp-Delt0,iy);
          else ::LineTo(memdc,xp-Delt0,iy);
          if(FGridY)
          {
	   if(!memdc)
           {
            MyCanvas->Pen->Style=psDash;
            MyCanvas->LineTo(xp+lx,iy);
            MyCanvas->Pen->Style=psSolid;
           }
           else
           {
            MyCanvas->Pen->Style=psDash;
            oldpen=SelectObject(memdc,MyCanvas->Pen->Handle);
            ::LineTo(memdc,xp+lx,iy);
            SelectObject(memdc,oldpen);
            MyCanvas->Pen->Style=psSolid;
           }
          }
	  f=floor(f/dyy+0.5)*dyy;
	  sprintf(buf,"%lg",f/divide);
          if(idDrawTextAY)
          {
          if(!memdc)
	    MyCanvas->TextOut(xp-MyCanvas->TextWidth(buf)-Delt0,iy-lzy/2,buf );
          else
            ::TextOut(memdc,xp-MyCanvas->TextWidth(buf)-Delt0,iy-lzy/2,buf,
             strlen(buf));
          }
// Нарисуем маленькие деления
m1:  fd=f+dy;
	  for(j=1 ; j < ndelm ;j++)
	  {
		 iy =yp - long((fd-yminn)/oney+0.5)-2;
		 if(iy > yp) goto m2 ;
		 if(iy < yp-ly)break;
		 if(!memdc)MyCanvas->MoveTo(xp,iy);
                 else ::MoveToEx(memdc,xp,iy,NULL);
		 if(!memdc)MyCanvas->LineTo(xp-Delt1,iy);
                 else ::LineTo(memdc,xp-Delt1,iy);
m2:	 fd=floor((fd+dy)/dy+0.5)*dy;
	  }
	  f=floor((f+dyy)/dyy+0.5)*dyy;
	}
	  if( idDrawTextAY && (smes || stepen != 1.))
	  {
		  buf[0]=0;
		  if(stepen != 1.)sprintf(buf,"*%.0le",stepen);
		  i=strlen(buf);
		  if(smes)sprintf(&buf[i]," %+lg",smes);
                  if(!memdc)
		    MyCanvas->TextOut(xp-MyCanvas->TextWidth(buf),yp-ly-3*lzy/2,buf );
                  else
                   ::TextOut(memdc,xp-MyCanvas->TextWidth(buf),yp-ly-3*lzy/2,buf,
                    strlen(buf));
	  }
   oney=(ymaxy-yminn)*stepen/(ly-4) ; // коэффициент перевода в пикселы
     if(FUseScale)oney=(ymaxy-yminn)*stepen/FScaleY/(ly-4) ; // коэффициент перевода в пикселы
//     if(FUseScale)oney=(ymaxy-yminn)/FScaleY/(ly-4) ; // коэффициент перевода в пикселы
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Draw_Box()
{
  if(!memdc)
  {
  MyCanvas->MoveTo(xp,yp-ly);
  MyCanvas->LineTo(xp+lx,yp-ly);
  MyCanvas->MoveTo(xp+lx,yp);
  MyCanvas->LineTo(xp+lx,yp-ly);
  }
  else
  {
   ::MoveToEx(memdc,xp,yp-ly,NULL);
   ::LineTo(memdc,xp+lx,yp-ly);
   ::MoveToEx(memdc,xp+lx,yp,NULL);
   ::LineTo(memdc,xp+lx,yp-ly);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Draw_PointRect(double x,double y,double lenx,double leny)
{ TRect rt;
  long ix1,iy1,ix2,iy2;
  double x1,y1,x2,y2;
  if(x-lenx/2. > xmax)return;
  if(x+lenx/2. < xmin)return;
  if(y-leny/2. > ymax)return;
  if(y+leny/2. < ymin)return;
  x1=x-lenx/2.;
  x2=x+lenx/2.;
  y1=y-leny/2.;
  y2=y+leny/2.;
  if(x1 < xmin)x1=xmin;
  if(x2 > xmax)x2=xmax;
  if(y1 < ymin)y1=ymin;
  if(y2 > ymax)y2=ymax;
  ix1 = long((x1-xmin)/onex+0.5)+xp+2;
  iy2 = yp - long((y1-ymin)/oney+0.5)-2 ;
  ix2 = long((x2-xmin)/onex+0.5)+xp+2;
  iy1 = yp - long((y2-ymin)/oney+0.5)-2 ;
  if(!memdc)MyCanvas->Rectangle(ix1,iy1,ix2+1,iy2+1);
  else
    ::Rectangle(memdc,ix1,iy1,ix2+1,iy2+1);

/*  MyCanvas->MoveTo(ix1-1,iy1);
  MyCanvas->LineTo(ix1-5,iy1);
  MyCanvas->MoveTo(ix2+1,iy1);
  MyCanvas->LineTo(ix2+5,iy1);
  MyCanvas->MoveTo(ix1,iy2+1);
  MyCanvas->LineTo(ix1,iy2+5);
  MyCanvas->MoveTo(ix2,iy2+1);
  MyCanvas->LineTo(ix2,iy2+5);
*/
/*  Draw_Line(x-lenx/2.,y-leny/2.,x+lenx/2.,y-leny/2.);
  Draw_Line(x+lenx/2.,y-leny/2.,x+lenx/2.,y+leny/2.);
  Draw_Line(x+lenx/2.,y+leny/2.,x-lenx/2.,y+leny/2.);
  Draw_Line(x-lenx/2.,y+leny/2.,x-lenx/2.,y-leny/2.);
*/
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Draw_Rect(double x0,double y0,double x1,double y1)
{
  Draw_Line(x0,y0,x1,y0);
  Draw_Line(x1,y0,x1,y1);
  Draw_Line(x1,y1,x0,y1);
  Draw_Line(x0,y1,x0,y0);
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Draw_Point(double x,double y)
{ long ix,iy;
  if(x < xmin)return;
  if(x > xmax)return;
  if(y < ymin)return;
  if(y > ymax)return;
  ix = long((x-xmin)/onex+0.5)+xp+2;
  iy = yp - long((y-ymin)/oney+0.5)-2 ;
  if(!memdc)MyCanvas->Rectangle(ix-2,iy-2,ix+2,iy+2);
  else ::Rectangle(memdc,ix-2,iy-2,ix+2,iy+2);
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Draw_Line(double x1,double y1,double x2,double y2)
{ long ix1,iy1,ix2,iy2;
  double t1[2],t2[2];
  if(x1 < xmin && x2 < xmin)return;
  if(x1 > xmax && x2 > xmax)return;
  if(y1 < ymin && y2 < ymin)return;
  if(y1 > ymax && y2 > ymax)return;
  if(x1 >= xmin && x1 <= xmax)
    if(x2 >= xmin && x2 <= xmax)
      if(y1 >= ymin && y1 <= ymax)
       if(y2 >= ymin && y2 <= ymax)
        goto m1;
  t1[0]=x1; t1[1]=y1;
  t2[0]=x2; t2[1]=y2;
  if(Into_rect(t1,t2))return;
  x1=t1[0]; y1=t1[1];
  x2=t2[0]; y2=t2[1];
m1:  ix1 = long((x1-xmin)/onex+0.5)+xp+2;
  iy1 = yp - long((y1-ymin)/oney+0.5)-2 ;
  if(!memdc)MyCanvas->MoveTo(ix1,iy1);
  else ::MoveToEx(memdc,ix1,iy1,NULL);
  ix2 = long((x2-xmin)/onex+0.5)+xp+2;
  iy2 = yp - long((y2-ymin)/oney+0.5)-2 ;
  if(!memdc)MyCanvas->LineTo(ix2,iy2);
  else ::LineTo(memdc,ix2,iy2);
}
//---------------------------------------------------------------------------
short __fastcall TNVDGraph::Into_rect(double *t1,double *t2) // отсечение по границам
{
 short i,ix1=0,ix2=0,iy1=0,iy2=0;
 short id;
 double p1[2],p2[2],a,b,x,y;
 for(i=0; i < 2; i++){ p1[i]=t1[i]; p2[i]=t2[i]; }
 if( t1[0] < xmin)ix1=1;
 if( t1[0] > xmax)ix1=2;
 if( t2[0] < xmin)ix2=1;
 if( t2[0] > xmax)ix2=2;
 if( t1[1] < ymin)iy1=1;
 if( t1[1] > ymax)iy1=2;
 if( t2[1] < ymin)iy2=1;
 if( t2[1] > ymax)iy2=2;
 if((ix1+ix2+iy1+iy2) == 0)return 0;
 if(ix1 == 1 && ix2 == 1)return 1;
 if(ix1 == 2 && ix2 == 2)return 1;
 if(iy1 == 1 && iy2 == 1)return 1;
 if(iy1 == 2 && iy2 == 2)return 1;
/*
	X=X1+a*(X2-X1)  Точка (X,Y) лежит внутри отрезка (X1,Y1)(X2,Y2)
	Y=Y1+a*(Y2-Y1)      если  0 <= a <= 1 .
*/
// Вертикальные и горизонтальные линии
	if(fabs(p1[0]-p2[0]) < 0.1)
	{
	 if(iy1 == 1)t1[1]=ymin ;
	 if(iy1 == 2)t1[1]=ymax ;
	 if(iy2 == 1)t2[1]=ymin ;
	 if(iy2 == 2)t2[1]=ymax ;
	 return 0;
	}
	if(fabs(p1[1]-p2[1]) < 0.1)
	{
	 if(ix1 == 1)t1[0]=xmin ;
	 if(ix1 == 2)t1[0]=xmax ;
	 if(ix2 == 1)t2[0]=xmin ;
	 if(ix2 == 2)t2[0]=xmax ;
	 return 0;
	}
    id=0;
	 a=(xmin-p1[0])/(p2[0]-p1[0]);
	 if(a >= 0. && a <= 1. )
	 {
		y = p1[1]+a*(p2[1]-p1[1]) ;
		 if(y >= ymin && y <= ymax )
			{
			  if(p2[0] > p1[0]){ t1[0]=xmin; t1[1]=y ; }
			  else
			  { t2[0]=xmin; t2[1]=y ; }
              id=1;
			}
	 }
	 a=(xmax-p1[0])/(p2[0]-p1[0]);
	 if(a >= 0. && a <= 1. )
	 {
		y = p1[1]+a*(p2[1]-p1[1]) ;
		 if(y >= ymin && y <= ymax )
			{
			  if(p2[0] < p1[0]){ t1[0]=xmax; t1[1]=y ; }
			  else
			  { t2[0]=xmax; t2[1]=y ; }
              id=1;
			}
	 }

	 b=(ymin-p1[1])/(p2[1]-p1[1]);
	 if(b >= 0. && b <= 1. )
	 {
		x = p1[0]+b*(p2[0]-p1[0]) ;
		 if(x >= xmin && x <= xmax )
			{
			  if(p2[1] > p1[1]){ t1[0]=x; t1[1]=ymin ; }
			  else
			  { t2[0]=x; t2[1]=ymin ; }
              id |=2 ;
			}
	 }

	 b=(ymax-p1[1])/(p2[1]-p1[1]);
	 if(b >= 0. && b <= 1. )
	 {
		x = p1[0]+b*(p2[0]-p1[0]) ;
		 if(x >= xmin && x <= xmax )
			{
			  if(p2[1] < p1[1]){ t1[0]=x; t1[1]=ymax ; }
			  else
			  { t2[0]=x; t2[1]=ymax ; }
              id |=2 ;
			}
	 }
     if(id==0)return 1;
	 return 0;
}
//---------------------------------------------------------------------------
// Получение позиции в графике по позиции на экране
// Return 0- не в области графика
//        1- Режим Grouped
//        2+i - номер графика + 2
short __fastcall TNVDGraph::Get_xy(int pixel_x,int pixel_y,double *x,double *y)
{
   int i,k;
   SetAxesPar(1);
     if(pixel_x < xp || pixel_x > xp+lx )return 0;
    ItemsToWork();
    if(FGrouped)
    {
	if(pixel_y > yp || pixel_y < yp-ly )return 0;
        if(FUseScale)
        {
	 *x=(xmin+(pixel_x-xp-2)*onex)*FScaleX;
	 *y=(ymin+(yp-pixel_y-2)*oney)*FScaleY;
        }
        else
        {
	 *x=xmin+(pixel_x-xp-2)*onex;
	 *y=ymin+(yp-pixel_y-2)*oney;
        }
	return 1;
    }
    SetAxesPar(0);
     k=0;
     for(i=MAXNBUF-1; i >=0; i--)
//     for(i=0; i < MAXNBUF; i++)
     {
      if( (MaskaViewBuf&(1<<i))==0)continue; //В маске нет
       Setyp(k);
       k++;
	if(pixel_y > yp || pixel_y < yp-ly )continue;
        ItemToWork(i);
        if(FUseScale)
        {
	 *x=(xmin+(pixel_x-xp-2)*onex)*FScaleX;
	 *y=(ymin+(yp-pixel_y-2)*oney)*FScaleY;
        }
        else
        {
	 *x=xmin+(pixel_x-xp-2)*onex;
	 *y=ymin+(yp-pixel_y-2)*oney;
        }
	return i+2;
     }
     return 0;
}
//Рисуем столбец гистограммы
// x - текущая ячейка
// x1 - следующая ячейка за исходным
// y - значение по Y для исходной ячейки
// nwxx - Ширина ячейкм для гистограмирования
// ifirst - =0- надо инициализировать суммы
void __fastcall TNVDGraph::Draw_Histo(double x, double x1,
                            double y, double nwxx,int ifirst)
{
  static int i0,i,imin,imax;
  static double x0,xc,xc0,sum,sum0;
  if(ifirst==0) //Инициализация
  {
    sum=0;
    sum0=0;
    xc0=0;
    x0= x; //Запомним первый элемент массива
    i0=0; //Это номер текущей ячейки
    //Определим какие столбы видны на экране
    imin=0;
    imax=(xmax-dxmax-xmin)/nwxx;
  }
  //найдём номер ячейки куда попала эта точка
  i=(x-xmin)/nwxx;
  if(i < i0)return; //значения должны быть упорядочены по возрастанию
  if(i != i0 ) //Новая ячейка
  {
   //надо нарисовать готовый столб, если он не пустой
   if(sum && (i0 >=imin && i0 <=imax))
   {
    xc=xmin+i0*nwxx+nwxx/2.;
    if(FKind==pgPoint || FKind==pgCombo)
       Draw_Point(xc,sum);
    if(FKind==pgLine || FKind==pgCombo)
     if(sum0 && sum)
     {
       if(xc0!=xc)Draw_Line(xc0,sum0,xc,sum);
     }
    if(FKind==pgBar)
     Draw_PointRect(xc,sum/2.,nwxx,sum);
     sum0=sum;
     xc0=xc;
   }
   i0=i; //Запомним номер текущей ячейки
   sum=0; //Пока в ней ничего нет
  }
  sum +=y; //Суммируем
  if(x==x1)//Последняя ячейка
  {
   //надо нарисовать готовый столб, если он не пустой
   if(sum && (i0 >=imin && i0 <=imax))
   {
    xc=xmin+i0*nwxx+nwxx/2.;
    if(FKind==pgPoint || FKind==pgCombo)
       Draw_Point(xc,sum);
    if(FKind==pgLine || FKind==pgCombo)
     if(sum0 && sum)
     {
       if(xc0!=xc)Draw_Line(xc0,sum0,xc,sum);
     }
    if(FKind==pgBar)
    Draw_PointRect(xc,sum/2.,nwxx,sum);
   }
  }
  return;
}
//Рисуем столбец гистограммы из программы
// x - текущая ячейка
void __fastcall TNVDGraph::Draw_SPos(double x,int nomerbuf)
{
  int i0,i1,i2,i,k,imin,imax,n;
  double x0,xc,sum,xmaxsave,y,nwxx;
   char *acharx, *achary;
   unsigned char *aucharx, *auchary;
   short *ashortx, *ashorty;
   unsigned short *aushortx, *aushorty;
   long *alongx,*alongy;
   unsigned long *aulongx,*aulongy;
   float *afloatx,*afloaty;
   double *adoublex,*adoubley;
   TNvdGetVar afuncy,afuncx,afuncidx,afuncidy;
    if(FType !=gtHistogram)return; //только для гистограмм
   if(FNBuf==0 || MaskaViewBuf==0)return;
    if(nomerbuf < 0)return; //Номер буфера не правильный
    if(nomerbuf >= FNBuf)return; //Номер буфера не доступный
    if( (MaskaViewBuf&(1<<nomerbuf))==0)return; //в маске нет
    if(!FBufY[nomerbuf])return; //Адрес буфера не задан
    if(!FNPoint[nomerbuf])return; //Адрес c количеством точек не задан
  //Инициализация
     ItemsToWork();
     ItemToWork(nomerbuf);
     nwxx=nwx;//запомним
//     xmaxsave=xmax; //запомним
     //Надо сделать xmax кратным на шаг гистограммы
//     dxmax=(int((xmax-xmin)/nwxx)+1)*nwxx-(xmax-xmin);
//     xmax +=dxmax;//Для гистограммы увеличим на dxmax
    //Определим какие столбы видны на экране
    imin=0;
    imax=(xmax-dxmax-xmin)/nwxx;
  //найдём номер ячейки куда попала эта точка
  i0=(x-xmin)/nwxx;
  if( i0 < imin || i0 > imax)goto mend;
  i1=i0*nwxx+xmin+0.5; //начальный элемент массива в столбе
  i2=i1+nwxx+0.5; //последний элемент массива в столбе
  if(FKind==pgLine || FKind==pgCombo)
  {
//    xmax=xmaxsave;
    NoAxes=true;
    MaskaPaint=(1<<nomerbuf);//Перерисовывать только этот кусок
    Paint();
    MaskaPaint=-1; //Перерисовывать всё
    NoAxes=false;
    WorkToItems();
    WorkToItem(nomerbuf);
    return;
  }
  if(i2 > xmax)i2=xmax+0.5;
   i=nomerbuf;
      switch(FTypeBufY[nomerbuf])
      {
        case tbChar: achary=(char *)FBufY[i]; break;
        case tbUChar: auchary=(unsigned char *)FBufY[i]; break;
        case tbShort: ashorty=(short *)FBufY[i]; break;
        case tbUShort: aushorty=(unsigned short *)FBufY[i]; break;
        case tbInt:
        case tbLong: alongy=(long *)FBufY[i]; break;
        case tbUInt:
        case tbULong: aulongy=(unsigned long *)FBufY[i]; break;
        case tbFloat: afloaty=(float *)FBufY[i]; break;
        case tbDouble: adoubley=(double *)FBufY[i]; break;
        case tbFunc: afuncy=FGetVarY[i]; break;
      }
   n=*FNPoint[i]; //всего точек
  sum=0;
  if(i2 >=n)i2=n-1;
  if(i1 >=n)goto mend;
  SetAxesPar(0);
   if(!FGrouped)
   {
     k=0;
     for(i=MAXNBUF-1; i >=0; i--)
//     for(i=0; i < MAXNBUF; i++)
     {
      if( (MaskaViewBuf&(1<<i))==0)continue; //В маске нет
      if(i==nomerbuf)
      {
       Setyp(k);
       break;
      }
       k++;
     }
   }
  for(i=i1; i < i2; i++)
  {
    if(i < xmin)continue;
//    if(i > xmax)break;
        switch(FTypeBufY[nomerbuf])
        {
          case tbChar: y=achary[i]; break;
          case tbUChar: y=auchary[i];break;
          case tbShort: y=ashorty[i];break;
          case tbUShort: y=aushorty[i];break;
          case tbInt:
          case tbLong: y=alongy[i];break;
          case tbUInt:
          case tbULong: y=aulongy[i];break;
          case tbFloat: y=afloaty[i];break;
          case tbDouble: y=adoubley[i];break;
          case tbFunc: if(afuncy)y=afuncy(i,nomerbuf); break;
        }
    sum +=y;
  }
   if(sum)
   {
     TBrushStyle savstyle=MyCanvas->Brush->Style;
     TColor savbrushcolor=MyCanvas->Brush->Color;
     TColor savpencolor=MyCanvas->Pen->Color;
     MyCanvas->Brush->Style=bsSolid;
     MyCanvas->Brush->Color=clWhite;
     MyCanvas->Pen->Color=FPenColor[nomerbuf];
    xc=xmin+i0*nwxx+nwxx/2.;
    if(FKind==pgPoint || FKind==pgCombo)
       Draw_Point(xc,sum);
    if(FKind==pgBar)
       Draw_PointRect(xc,sum/2.,nwxx,sum);
     MyCanvas->Brush->Style=savstyle;
     MyCanvas->Brush->Color=savbrushcolor;
     MyCanvas->Pen->Color=savpencolor;
   }
mend:
//  xmax=xmaxsave; //восстановим значение
  WorkToItems(); //запомним
  WorkToItem(nomerbuf); //запомним
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::Clear()
{
   int i,k;
 //Очистка области внутри графика
     TBrushStyle savstyle=MyCanvas->Brush->Style;
     TColor savbrushcolor=MyCanvas->Brush->Color;
     TColor savpencolor=MyCanvas->Pen->Color;
     MyCanvas->Brush->Style=bsSolid;
     MyCanvas->Brush->Color=FColorBk;
     MyCanvas->Pen->Color=FColorBk;
     SetAxesPar(0);
    if(FGrouped)
    {
     MyCanvas->Rectangle(xp+1,yp-1,xp+lx,yp-ly+1);
    }
    else
    {
     k=-1;
     for(i=MAXNBUF-1; i >=0; i--)
//     for(i=0; i < MAXNBUF; i++)
     {
      if( (MaskaViewBuf&(1<<i))==0)continue; //В маске нет
       k++;
      if( (MaskaPaint&(1<<i))==0)continue; //В маске нет
       Setyp(k);
      MyCanvas->Rectangle(xp+1,yp-1,xp+lx,yp-ly+1);
     }
    }
     MyCanvas->Brush->Style=savstyle;
     MyCanvas->Brush->Color=savbrushcolor;
     MyCanvas->Pen->Color=savpencolor;
}
//---------------------------------------------------------------------------
//Вычисляем положение и размеры осей
void __fastcall TNVDGraph::SetAxesPar(int idall)
{
    int i,Ly,zazor;
     lx=Width-xp-xp/2;
     yp=Height-Axbot;
     ly=yp-Axtop;
     if(idall || FGrouped)return; //Окантовка всех графиков
     Ly=ly;
     ngr=0;
    for(i=0; i < MAXNBUF; i++)
    {
     if(MaskaViewBuf&(1<<i))ngr++; //Количество графиков
    }
    zazor=10;
    if(!FUseForAll && !FUseForAllX)zazor +=MyCanvas->TextHeight("0")-TH;
    ly=(Ly-zazor*(ngr-1))/ngr;
}
void __fastcall TNVDGraph::Setyp(int k)
{
  if(FGrouped)return; //Для единого не меняем
  int zazor=10;
  int Yp=Height-Axbot;
  int Ly=Yp-Axtop;
  if(k==ngr-1){ yp=Yp; return; }
  if(!FUseForAll && !FUseForAllX)zazor +=MyCanvas->TextHeight("0")-TH;
   yp=Yp-Ly+ly*(k+1)+zazor*k;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::GoDraw()
{
   HPEN oldpen;
   TColor Colorold;
   int i,j,k,j0,j1,n,ifirsthisto;
   double x,x1,y,y1,xmaxsave;
   char *acharx, *achary;
   unsigned char *aucharx, *auchary;
   short *ashortx, *ashorty;
   unsigned short *aushortx, *aushorty;
   long *alongx,*alongy;
   unsigned long *aulongx,*aulongy;
   float *afloatx,*afloaty;
   double *adoublex,*adoubley;
   long *aidx,*aidy;
   TDateTimeKadr *atimex,*atimey;
   char *ametkax;
   bool idMetka;
   TNvdGetVar afuncy,afuncx,afuncidx,afuncidy;
   TNvdGetVarDT afuncydt,afuncxdt;
   char buf[40] ;
   if(FNBuf==0 || MaskaViewBuf==0)return;
//   W=Width;
//   H=Height;
//   if(!memdc)MyCanvas->FillRect(Rect(0,0,W,H));
      ItemsToWork(); //загрузка xmin,xmax,ymin,ymax,nwx,onex,oney,dxmax
//     xmaxsave=xmax; //запомним
    idMetka=false;
    SetAxesPar(1); //положение и размеры всей области графика
     if(FUseForAll ||FUseForAllX || FUseForAllY ||FGrouped)if(FAvtoScale)SetGran(-1);//Определяем границы
    if(FGrouped)
    {
     Draw();
    }
    if(Caption.Length() && !NoAxes)
    {
       if(!memdc)
         MyCanvas->TextOut(xp+lx/2-MyCanvas->TextWidth(Caption.c_str())/2,
           yp-ly-(MyCanvas->TextHeight(Caption.c_str())-TH),Caption.c_str());
       else
         ::TextOut(memdc,xp+lx/2-MyCanvas->TextWidth(Caption.c_str())/2,
          yp-ly-(MyCanvas->TextHeight(Caption.c_str())-TH),Caption.c_str(),Caption.Length());
    }
    if(FLabelX.Length() && !NoAxes)
    {
       if(!memdc)
         MyCanvas->TextOut(xp+lx/2-MyCanvas->TextWidth(FLabelX.c_str())/2,
           yp+2+Delt0+MyCanvas->TextHeight(FLabelX.c_str())-TH,FLabelX.c_str());
       else
         ::TextOut(memdc,xp+lx/2-MyCanvas->TextWidth(FLabelX.c_str())/2,
          yp+2+Delt0+MyCanvas->TextHeight(FLabelX.c_str())-TH,FLabelX.c_str(),FLabelX.Length());
    }
    if(FLabelY.Length() && !NoAxes)
    {
       if(!memdc)
         MyCanvas->TextOut(xp-MyCanvas->TextWidth(FLabelY.c_str())/2,
           yp-ly-3*(MyCanvas->TextHeight(FLabelY.c_str())-TH)/2,FLabelY.c_str());
       else
         ::TextOut(memdc,xp-MyCanvas->TextWidth(FLabelY.c_str())/2,
          yp-ly-3*(MyCanvas->TextHeight(FLabelY.c_str())-TH)/2,FLabelY.c_str(),FLabelY.Length());
    }
    k=-1;
    SetAxesPar(0); //положение и размеры графика
    Colorold=MyCanvas->Pen->Color;
    for(i=MAXNBUF-1; i >=0; i--)
//    for(i=0; i < MAXNBUF; i++)
    {
      if( (MaskaViewBuf&(1<<i))==0)continue; //В маске нет
      if(!FGrouped)
      {
       k++;
       if( (MaskaPaint&(1<<i))==0)continue; //В маске нет
       Setyp(k);
      if(FUseForAll || FUseForAllX)//Единые границы и параметры для всех графиков
      {
       if(k !=ngr-1)idDrawTextAX=false;
       else idDrawTextAX=true;
      }
       ItemToWork(i);
       if(!FUseForAll && (!FUseForAllX || !FUseForAllY))if(FAvtoScale)SetGran(i);//Определяем границы
       Draw();
       WorkToItem(i);
       if(!NoAxes)
       {
//       sprintf(buf,"%d",i);
       if(!memdc)
         MyCanvas->TextOut(xp+lx+5,yp-ly,FNameItem[i]);
       else
         ::TextOut(memdc,xp+lx+5,yp-ly,FNameItem[i].c_str(),
          FNameItem[i].Length());
       }
      }
      if( (FTypeBufY[i]!=tbFunc && !FBufY[i]) &&
          (FTypeBufY[i]!=tbFuncDT && !FBufY[i]) )continue; //Адрес буфера не задан
      if(FTypeBufY[i]==tbFunc && !FGetVarY[i])continue; //Адрес функции не задан
      if(FTypeBufY[i]==tbFuncDT && !FGetVarYDT[i])continue; //Адрес функции не задан
      if(!FNPoint[i])continue; //Адрес c количеством точек не задан
      switch(FTypeBufX)
      {
        case tbChar: acharx=(char *)FBufX[i]; break;
        case tbUChar: aucharx=(unsigned char *)FBufX[i]; break;
        case tbShort: ashortx=(short *)FBufX[i]; break;
        case tbUShort: aushortx=(unsigned short *)FBufX[i]; break;
        case tbInt:
        case tbLong: alongx=(long *)FBufX[i]; break;
        case tbUInt:
        case tbULong: aulongx=(unsigned long *)FBufX[i]; break;
        case tbFloat: afloatx=(float *)FBufX[i]; break;
        case tbDouble: adoublex=(double *)FBufX[i]; break;
        case tbFunc: afuncx=FGetVarX[i]; break;
        case tbTime: atimex=(TDateTimeKadr *)FBufX[i]; break;
        case tbFuncDT: afuncxdt=FGetVarXDT[i]; break;
      }
      switch(FTypeBufY[i])
      {
        case tbChar: achary=(char *)FBufY[i]; break;
        case tbUChar: auchary=(unsigned char *)FBufY[i]; break;
        case tbShort: ashorty=(short *)FBufY[i]; break;
        case tbUShort: aushorty=(unsigned short *)FBufY[i]; break;
        case tbInt:
        case tbLong: alongy=(long *)FBufY[i]; break;
        case tbUInt:
        case tbULong: aulongy=(unsigned long *)FBufY[i]; break;
        case tbFloat: afloaty=(float *)FBufY[i]; break;
        case tbDouble: adoubley=(double *)FBufY[i]; break;
        case tbFunc: afuncy=FGetVarY[i]; break;
        case tbTime: atimey=(TDateTimeKadr *)FBufY[i]; break;
        case tbFuncDT: afuncydt=FGetVarYDT[i]; break;
      }
      n=*FNPoint[i];
      MyCanvas->Pen->Color=FPenColor[i];
      if(memdc)oldpen=SelectObject(memdc,MyCanvas->Pen->Handle);
      ifirsthisto=0; //для начала формирования гистограммы
      aidx=FIDPointX[i];
      aidy=FIDPointY[i];
      afuncidx=FGetVarIdX[i];
      ametkax=(char *)FBufIdX[i];
      j0=-1; j1=-1;
      for(j=0; j < n; j++) //Пробегаемся по массиву точек
      {
        if(afuncidx)
        {
          if(afuncidx(j,i)==0)continue; //Эту точку не рисовать
        }
        if(aidx)
        {
          if(aidx[j]==0)continue; //Эту точку не рисовать
        }
        if(aidy)
        {
          if(aidy[j]==0)continue; //Эту точку не рисовать
        }
        if(j0 < 0){ j0=j; j1=-1;}
         else if(j1 < 0)j1=j;
              else {j0=j1; j1=j; }
//        if(j<n-1)j1=j+1; else j=j1; //Для второй точки линии
        //Вытащим точку из буферов
        if( (FTypeBufX !=tbFunc && FBufX[i])||(FTypeBufX ==tbFunc && FGetVarX[i]) ||
            (FTypeBufX ==tbFuncDT && FGetVarXDT[i]) )
        {
          switch(FTypeBufX)
          {
            case tbChar: x=acharx[j0]; if(j1>=0)x1=acharx[j1]; break;
            case tbUChar: x=aucharx[j0];if(j1>=0)x1=aucharx[j1]; break;
            case tbShort: x=ashortx[j0];if(j1>=0)x1=ashortx[j1]; break;
            case tbUShort: x=aushortx[j0];if(j1>=0)x1=aushortx[j1]; break;
            case tbInt:
            case tbLong: x=alongx[j0];if(j1>=0)x1=alongx[j1]; break;
            case tbUInt:
            case tbULong: x=aulongx[j0];if(j1>=0)x1=aulongx[j1]; break;
            case tbFloat: x=afloatx[j0];if(j1>=0)x1=afloatx[j1]; break;
            case tbDouble: x=adoublex[j0];if(j1>=0)x1=adoublex[j1]; break;
            case tbFunc: x=afuncx(j0,i); if(j1>=0)x1=afuncx(j1,i); break;
            case tbTime: x=GetDeltaTime(DTZero,atimex[j0]);if(j1>=0)x1=GetDeltaTime(DTZero,atimex[j1]); break;
            case tbFuncDT:
                  {
                    TDateTimeKadr dt;
                     afuncxdt(j0,dt,i);
                    x=GetDeltaTime(DTZero,dt);
                    if(j1>=0)
                    {
                     afuncxdt(j1,dt,i);
                     x1=GetDeltaTime(DTZero,dt);
                    }
                    break;
                  }
          }
        }
        else { x=j0; x1=j1;}
        switch(FTypeBufY[i])
        {
          case tbChar: y=achary[j0];if(j1>=0)y1=achary[j1]; break;
          case tbUChar: y=auchary[j0];if(j1>=0)y1=auchary[j1]; break;
          case tbShort: y=ashorty[j0];if(j1>=0)y1=ashorty[j1]; break;
          case tbUShort: y=aushorty[j0];if(j1>=0)y1=aushorty[j1]; break;
          case tbInt:
          case tbLong: y=alongy[j0];if(j1>=0)y1=alongy[j1]; break;
          case tbUInt:
          case tbULong: y=aulongy[j0];if(j1>=0)y1=aulongy[j1]; break;
          case tbFloat: y=afloaty[j0];if(j1>=0)y1=afloaty[j1]; break;
          case tbDouble: y=adoubley[j0];if(j1>=0)y1=adoubley[j1]; break;
          case tbFunc: y=afuncy(j0,i); if(j1>=0)y1=afuncy(j1,i); break;
          case tbTime: y=GetDeltaTime(DTZero,atimey[j0]);if(j1>=0)y1=GetDeltaTime(DTZero,atimey[j1]); break;
          case tbFuncDT:
                  {
                    TDateTimeKadr dt;
                     afuncydt(j0,dt,i);
                    y=GetDeltaTime(DTZero,dt);
                    if(j1>=0)
                    {
                     afuncydt(j1,dt,i);
                     y1=GetDeltaTime(DTZero,dt);
                    }
                    break;
                  }
        }
        //Метка
        if(ametkax && ametkax[j])
        {
          if(!FGrouped || !idMetka)
          {
            Draw_PointRect(x+(x1-x)/2.,ymax-(ymax-ymin)/20.,x1-x,(ymax-ymin)/10.);
            Draw_PointRect(x+(x1-x)/2.,ymin+(ymax-ymin)/20.,x1-x,(ymax-ymin)/10.);
          }
        }
        if(FType==gtGraphic)
        {
         if(FKind==pgLine ||FKind==pgCombo)
           if(j0>=0 && j1>=0)Draw_Line(x,y,x1,y1);
         if(FKind ==pgPoint||FKind==pgCombo)
         {
           if(j0>=0 && j1 <0)Draw_Point(x,y);
           if(j0>=0 && j1 >=0)Draw_Point(x1,y1);
         }
         if(FKind ==pgBar)
           //if(j0>=0 && j1>=0 && y)Draw_PointRect(x,y/2,x1-x,fabs(y));
           if(j0>=0 && j1>=0)
           {
             Draw_Line(x,y,x1,y);
             Draw_Line(x1,y,x1,y1);
           }
           else
            if(j0==0)
             Draw_Line(x,0,x,y);
        }
        else //Histogram
        {
         if(x < xmin)continue;
         if(j0>=0 && j1>=0)
         {
          Draw_Histo(x, x1, y, nwx, ifirsthisto);
          ifirsthisto=1; //гистограмму уже начали
         }
        }
      } //for NPoint
      if(FGrouped)idMetka=true;
        if(FType==gtGraphic)
        {
         if(FKind ==pgBar)
           //if(j0>=0 && j1>=0 && y1)
             //Draw_PointRect(x,y1/2,x1-x,fabs(y1));
           if(j0>=0 && j1>=0)
           {
             Draw_Line(x1,y1,x1+1,y1);
             Draw_Line(x1+1,0,x1+1,y1);
           }
        }
        if(FType==gtHistogram)
        {
         if(j1>=0)
         {
          Draw_Histo(x1, x1, y1, nwx, ifirsthisto);
          ifirsthisto=1; //гистограмму уже начали
         }
        }
      if(memdc)SelectObject(memdc,oldpen);
      else MyCanvas->Pen->Color=Colorold;
      if(FNvdUserDraw)FNvdUserDraw(memdc,MyCanvas,xmin,xmax,i);
    }//for MAXNBUF
    MyCanvas->Pen->Color=Colorold;
    idDrawTextAX=true;
    WorkToItems();
//    xmax=xmaxsave; //восстановим значение
//   Canvas->Rectangle(xp,yp,xp+lx,yp+ly);
//   Canvas->Font=Font;
//   Canvas->TextOut(2, 2, Caption);

}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::SetGran(int numitem)
{
 int i,k,n,j,maska,nx,n1,n2;
   char *acharx, *achary;
   unsigned char *aucharx, *auchary;
   short *ashortx, *ashorty;
   unsigned short *aushortx, *aushorty;
   long *alongx,*alongy;
   unsigned long *aulongx,*aulongy;
   float *afloatx,*afloaty;
   double *adoublex,*adoubley;
   long *aidx,*aidy;
   TDateTimeKadr *atimex,*atimey;
   double x,y,ysum;
   TNvdGetVar afuncy,afuncx,afuncidx,afuncidy;
   TNvdGetVarDT afuncydt,afuncxdt;
 double minx[MAXNBUF],maxx[MAXNBUF],miny[MAXNBUF],maxy[MAXNBUF];
   if(!FAvtoScaleX && !FAvtoScaleY)return;
   memset(minx,0,sizeof(minx));
   memset(maxx,0,sizeof(maxx));
   memset(miny,0,sizeof(miny));
   memset(maxy,0,sizeof(maxy));
   maska=0;
    for(i=0; i < MAXNBUF; i++)
    {
      if(numitem >=0 && numitem != i)continue;
      if( (MaskaViewBuf&(1<<i))==0)continue; //В маске нет
      if( (FTypeBufY[i]!=tbFunc && !FBufY[i]) &&
          (FTypeBufY[i]!=tbFuncDT && !FBufY[i]) )continue; //Адрес буфера не задан
      if(FTypeBufY[i]==tbFunc && !FGetVarY[i])continue; //Адрес функции не задан
      if(FTypeBufY[i]==tbFuncDT && !FGetVarYDT[i])continue; //Адрес функции не задан
      if(!FNPoint[i])continue; //Адрес c количеством точек не задан
      switch(FTypeBufX)
      {
        case tbChar: acharx=(char *)FBufX[i]; break;
        case tbUChar: aucharx=(unsigned char *)FBufX[i]; break;
        case tbShort: ashortx=(short *)FBufX[i]; break;
        case tbUShort: aushortx=(unsigned short *)FBufX[i]; break;
        case tbInt:
        case tbLong: alongx=(long *)FBufX[i]; break;
        case tbUInt:
        case tbULong: aulongx=(unsigned long *)FBufX[i]; break;
        case tbFloat: afloatx=(float *)FBufX[i]; break;
        case tbDouble: adoublex=(double *)FBufX[i]; break;
        case tbFunc: afuncx=FGetVarX[i]; break;
        case tbTime: atimex=(TDateTimeKadr *)FBufX[i]; break;
        case tbFuncDT: afuncxdt=FGetVarXDT[i]; break;
      }
      switch(FTypeBufY[i])
      {
        case tbChar: achary=(char *)FBufY[i]; break;
        case tbUChar: auchary=(unsigned char *)FBufY[i]; break;
        case tbShort: ashorty=(short *)FBufY[i]; break;
        case tbUShort: aushorty=(unsigned short *)FBufY[i]; break;
        case tbInt:
        case tbLong: alongy=(long *)FBufY[i]; break;
        case tbUInt:
        case tbULong: aulongy=(unsigned long *)FBufY[i]; break;
        case tbFloat: afloaty=(float *)FBufY[i]; break;
        case tbDouble: adoubley=(double *)FBufY[i]; break;
        case tbFunc: afuncy=FGetVarY[i]; break;
        case tbTime: atimey=(TDateTimeKadr *)FBufY[i]; break;
        case tbFuncDT: afuncydt=FGetVarYDT[i]; break;
      }
      n=*FNPoint[i];
      if(n==0)continue;
      aidx=FIDPointX[i];
      aidy=FIDPointY[i];
      afuncidx=FGetVarIdX[i];
      k=0; //признак первой точки
      nx=0; //для гистограмм
      ysum=0;
      for(j=0; j < n; j++) //Пробегаемся по массиву точек
      {
        if(afuncidx)
        {
          if(afuncidx(j,i)==0)continue; //Эту точку не рисовать
        }
        if(aidx)
        {
          if(aidx[j]==0)continue; //Эту точку не рисовать
        }
        if(aidy)
        {
          if(aidy[j]==0)continue; //Эту точку не рисовать
        }
        //Вытащим точку из буферов
        if( (FTypeBufX!=tbFunc &&FBufX[i]) || (FTypeBufX==tbFunc &&FGetVarX[i]) ||
            (FTypeBufX==tbFuncDT &&FGetVarXDT[i]) )
        {
          switch(FTypeBufX)
          {
            case tbChar: x=acharx[j]; break;
            case tbUChar: x=aucharx[j]; break;
            case tbShort: x=ashortx[j]; break;
            case tbUShort: x=aushortx[j]; break;
            case tbInt:
            case tbLong: x=alongx[j]; break;
            case tbUInt:
            case tbULong: x=aulongx[j]; break;
            case tbFloat: x=afloatx[j]; break;
            case tbDouble: x=adoublex[j]; break;
            case tbFunc: x=afuncx(j,i); break;
            case tbTime: x=GetDeltaTime(DTZero,atimex[j]); break;
            case tbFuncDT:
               {
                TDateTimeKadr dt;
                afuncxdt(j,dt,i);
                x=GetDeltaTime(DTZero,dt);
                break;
               }
          }
        }
        else { x=j;}
        switch(FTypeBufY[i])
        {
          case tbChar: y=achary[j]; break;
          case tbUChar: y=auchary[j]; break;
          case tbShort: y=ashorty[j]; break;
          case tbUShort: y=aushorty[j];break;
          case tbInt:
          case tbLong: y=alongy[j]; break;
          case tbUInt:
          case tbULong: y=aulongy[j]; break;
          case tbFloat: y=afloaty[j]; break;
          case tbDouble: y=adoubley[j]; break;
          case tbFunc: y=afuncy(j,i); break;
          case tbTime: y=GetDeltaTime(DTZero,atimey[j]); break;
          case tbFuncDT:
               {
                TDateTimeKadr dt;
                afuncydt(j,dt,i);
                y=GetDeltaTime(DTZero,dt);
                break;
               }
        }
         maska |= (1<<i); //Пометим что есть
        if(FType==gtHistogram)
        {
          ysum +=y;
          nx++ ;
          if(nx < nwx)continue;
          else nx=0;
        }
          if(k==0)
          {
           if(FType==gtGraphic)
           {
            k=1; minx[i]=x; maxx[i]=x; miny[i]=y; maxy[i]=y;
           }
           else
           {
            if(ysum){k=1; minx[i]=x; maxx[i]=x;}
             miny[i]=ysum; maxy[i]=ysum;
           }

          }
          else
          {
           if( (FType==gtGraphic) ||((FType==gtHistogram) && ysum))
           {
            if(x < minx[i])minx[i]=x;
            if(x > maxx[i])maxx[i]=x;
           }
           if( FType==gtGraphic)
           {
            if(y < miny[i])miny[i]=y;
            if(y > maxy[i])maxy[i]=y;
           }
           else
           {
            if(ysum < miny[i])miny[i]=ysum;
            if(ysum > maxy[i])maxy[i]=ysum;
           }
          }
         if(nx==0)ysum=0;
      } //for NPoint
      if(nx)
      if( FType==gtHistogram)
      {
       if(ysum)
       {
        if(x < minx[i])minx[i]=x;
        if(x > maxx[i])maxx[i]=x;
       }
        if(ysum < miny[i])miny[i]=ysum;
        if(ysum > maxy[i])maxy[i]=ysum;
      }
      if( FType==gtHistogram)
      {
        minx[i]=minx[i]-nwx+1;
        if(minx[i] < 0)minx[i]=0;
      }
    }//for MAXNBUF
    if(numitem >=0)
    {
      if(maska&(1<<numitem))
      {
        if(!FUseForAllX && FAvtoScaleX)
        {
         Item[numitem].xmin=minx[numitem];
         Item[numitem].xmax=maxx[numitem];
         xmin=minx[numitem];
         xmax=maxx[numitem];
         if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
         {
           AddDeltaTime(DTZero,xmin,xdtmin);
           AddDeltaTime(DTZero,xmax,xdtmax);
           Item[numitem].xdtmin=xdtmin;
           Item[numitem].xdtmax=xdtmax;
         }
      //Надо сделать xmax кратным на шаг гистограммы
         if(FType==gtHistogram ||FKind==pgBar)
         {
          dxmax=(int((xmax-xmin)/nwx)+1)*nwx-(xmax-xmin);
          xmax +=dxmax;//Для гистограммы увеличим на dxmax
          if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
          {
            AddDeltaTime(DTZero,xmax,xdtmax);
          }
         }
        }
        if(!FUseForAllY && FAvtoScaleY)
        {
         Item[numitem].ymin=miny[numitem];
         Item[numitem].ymax=maxy[numitem];
         ymin=miny[numitem];
         ymax=maxy[numitem];
         if(FTypeBufY[i]==tbTime || FTypeBufY[i]==tbFuncDT)
         {
           AddDeltaTime(DTZero,ymin,ydtmin);
           AddDeltaTime(DTZero,ymax,ydtmax);
           Item[numitem].ydtmin=ydtmin;
           Item[numitem].ydtmax=ydtmax;
         }
        }
      }
      return;
    }
    if(maska==0)return;
    k=0;
    for(i=0; i < MAXNBUF; i++)
    {
      if( (maska&(1<<i))==0)continue; //В маске нет
      if(k==0)
      {
        k=1;
        if(FAvtoScaleX)
        {
          xmin=minx[i];
          xmax=maxx[i];
        }
        if(FAvtoScaleY)
        {
          ymin=miny[i];
          ymax=maxy[i];
        }
      }
      else
      {
        if(FAvtoScaleX)
        {
          if(minx[i] < xmin)xmin=minx[i];
          if(maxx[i]>xmax)xmax=maxx[i];
        }
        if(FAvtoScaleY)
        {
          if(miny[i]< ymin)ymin=miny[i];
          if(maxy[i]>ymax)ymax=maxy[i];
        }
      }
    }
   if(FAvtoScaleX)
   {
    Items.xmin=xmin;
    Items.xmax=xmax;
    if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
    {
       AddDeltaTime(DTZero,xmin,xdtmin);
       AddDeltaTime(DTZero,xmax,xdtmax);
       Items.xdtmin=xdtmin;
       Items.xdtmax=xdtmax;
    }
   }
   if(FAvtoScaleY)
   {
    Items.ymin=ymin;
    Items.ymax=ymax;
   }
    //Надо сделать xmax кратным на шаг гистограммы
   if(FAvtoScaleX)
    if(FType==gtHistogram || FKind==pgBar)
    {
     dxmax=(int((xmax-xmin)/nwx)+1)*nwx-(xmax-xmin);
     xmax +=dxmax;//Для гистограммы увеличим на dxmax
     if(FTypeBufX==tbTime || FTypeBufX==tbFuncDT)
       AddDeltaTime(DTZero,xmax,xdtmax);
    }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::BeginUpdate(void)
{
  FUpdateCount++;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::EndUpdate(void)
{
  FUpdateCount--;
  if(FUpdateCount <=0)
  {
   FUpdateCount=0;
   StyleChanged(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::DrawXMarker(double x, TColor Color)
{
  if(x < xmin)return;
  if(x > xmax)return;
     TBrushStyle savstyle=MyCanvas->Brush->Style;
     TColor savbrushcolor=MyCanvas->Brush->Color;
     TColor savpencolor=MyCanvas->Pen->Color;
     MyCanvas->Brush->Style=bsSolid;
     MyCanvas->Brush->Color=Color;
     MyCanvas->Pen->Color=Color;
  int ix,iy1,iy2;
  ix = long((x-xmin)/onex+0.5)+xp+2;
  iy1 = yp -2 ;
  iy2 = yp - long((ymax-ymin)/oney+0.5)-2 ;
  if(!memdc)
  {
   //MyCanvas->Rectangle(ix-2,iy1-2,ix+2,iy2+2);
   MyCanvas->MoveTo(ix-2,iy1);
   MyCanvas->LineTo(ix-2,iy2);
   MyCanvas->LineTo(ix+2,iy2);
   MyCanvas->LineTo(ix+2,iy1);
   MyCanvas->LineTo(ix-2,iy1);
  }
  else
  {
    HPEN pen; //handle to a pen
    HPEN oldpen; //handle to a pen
    pen=MyCanvas->Pen->Handle;
    oldpen = SelectObject(memdc, pen);
   //::Rectangle(memdc,ix-2,iy1-2,ix+2,iy2+2);
   MoveToEx(memdc,ix-2,iy1,NULL);
   LineTo(memdc,ix-2,iy2);
   LineTo(memdc,ix+2,iy2);
   LineTo(memdc,ix+2,iy1);
   LineTo(memdc,ix-2,iy1);
   SelectObject(memdc, oldpen);
  }

     MyCanvas->Brush->Style=savstyle;
     MyCanvas->Brush->Color=savbrushcolor;
     MyCanvas->Pen->Color=savpencolor;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::DrawYMarker(double y, TColor Color)
{
  if(y < ymin)return;
  if(y > ymax)return;
     TBrushStyle savstyle=MyCanvas->Brush->Style;
     TColor savbrushcolor=MyCanvas->Brush->Color;
     TColor savpencolor=MyCanvas->Pen->Color;
     MyCanvas->Brush->Style=bsSolid;
     MyCanvas->Brush->Color=Color;
     MyCanvas->Pen->Color=Color;
  int ix1,ix2,iy;
  ix1 = xp+2;
  ix2 = long((xmax-xmin)/onex+0.5)+xp+2;
  iy = yp - long((y-ymin)/oney+0.5)-2 ;

  if(!memdc)
  {
   //MyCanvas->Rectangle(ix1-2,iy-2,ix2+2,iy+2);
   MyCanvas->MoveTo(ix1,iy-2);
   MyCanvas->LineTo(ix2,iy-2);
   MyCanvas->LineTo(ix2,iy+2);
   MyCanvas->LineTo(ix2,iy-2);
   MyCanvas->LineTo(ix1,iy-2);
  }
  else
  {
    HPEN pen; //handle to a pen
    HPEN oldpen; //handle to a pen
    pen=MyCanvas->Pen->Handle;
    oldpen = SelectObject(memdc, pen);
   //::Rectangle(memdc,ix1-2,iy-2,ix2+2,iy+2);
   MoveToEx(memdc,ix1,iy-2,NULL);
   LineTo(memdc,ix2,iy-2);
   LineTo(memdc,ix2,iy+2);
   LineTo(memdc,ix1,iy+2);
   LineTo(memdc,ix1,iy-2);
   SelectObject(memdc, oldpen);
  }
//  if(!memdc)MyCanvas->Rectangle(ix1-2,iy-2,ix2+2,iy+2);
//  else ::Rectangle(memdc,ix1-2,iy-2,ix2+2,iy+2);

     MyCanvas->Brush->Style=savstyle;
     MyCanvas->Brush->Color=savbrushcolor;
     MyCanvas->Pen->Color=savpencolor;
}
//---------------------------------------------------------------------------
void __fastcall TNVDGraph::AddDeltaTime(TDateTimeKadr &dtstart, double deltasec,
                                        TDateTimeKadr &dtstop)
{
	//long hsecond;
//	unsigned long second;
	long mon,day,hour,min,sec,hsec;
	long mon1,day1,hour1,min1,sec1,hsec1,year1;
	long mon2,day2,hour2,min2,sec2,hsec2,year2;
	long i,isLY;
	long day_mon[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};

        year1=dtstart.dt.year;
        mon1=dtstart.dt.month;
        day1=dtstart.dt.day;
	hour1=dtstart.tm.hour;
	min1=dtstart.tm.minute;
	sec1=dtstart.tm.second;
	hsec1=dtstart.tm.hsecond;
        //Определим целое число дней
        long dday=deltasec/60/60/24; //Количество полных дней
        //Определим количество секунд в неполном дне
        long dsec=deltasec-dday*60.*60.*24;
        //Определим остаток полных часов
        long dhour=dsec/3600;
        //Определим остаток полных минут
        long dmin=(dsec-dhour*3600)/60;
        //Определим остаток секунд
        dsec=(dsec-dhour*3600-dmin*60);
        //Начинаем прибавлять
          //Прибавим секунды
       if(dsec)
       {
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
          sec1 +=dsec;
        if(sec1 >= 60)
        {
           min1++;  sec1 -=60;
           if(min1 >=60)
           {
             hour1++; min1 -=60;
             if(hour1 >= 24)
             {
               hour1 -=24; day1++;
               if(day1 > day_mon[mon1])
               {
                 day1 -=day_mon[mon1]; mon1++;
                 if(mon1 > 12)
                 {
                   mon1 -=12; year1++;
                 }
               }
             }
           }
        }
        if(sec1 < 0)
        {
           min1--; sec1 +=60;
           if(min1 < 0)
           {
             hour1--; min1 +=60;
             if(hour1 < 0)
             {
               hour1 +=24; day1--;
               if(day1 < 1)
               {
                 mon1--;
                 if(mon1 < 1)
                 {
                   mon1 +=12; year1--;
                 }
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
                 day1 =day_mon[mon1];
               }
             }
           }
        }
       }//dsec
       //Прибавим минуты
       if(dmin)
       {
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
          min1 +=dmin;
           if(min1 >=60)
           {
             hour1++; min1 -=60;
             if(hour1 >= 24)
             {
               hour1 -=24; day1++;
               if(day1 > day_mon[mon1])
               {
                 day1 -=day_mon[mon1]; mon1++;
                 if(mon1 > 12)
                 {
                   mon1 -=12; year1++;
                 }
               }
             }
           }
           if(min1 < 0)
           {
             hour1--; min1 +=60;
             if(hour1 < 0)
             {
               hour1 +=24; day1--;
               if(day1 < 1)
               {
                 mon1--;
                 if(mon1 < 1)
                 {
                   mon1 +=12; year1--;
                 }
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
                 day1 =day_mon[mon1];
               }
             }
           }
        }//dmin
          //Прибавим часы
         //Определим високосность в текущем году
        if(dhour)
        {
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
          hour1 +=dhour;
             if(hour1 >= 24)
             {
               hour1 -=24; day1++;
               if(day1 > day_mon[mon1])
               {
                 day1 -=day_mon[mon1]; mon1++;
                 if(mon1 > 12)
                 {
                   mon1 -=12; year1++;
                 }
               }
             }
             if(hour1 < 0)
             {
               hour1 +=24; day1--;
               if(day1 < 1)
               {
                 mon1--;
                 if(mon1 < 1)
                 {
                   mon1 +=12; year1--;
                 }
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
                 day1 =day_mon[mon1];
               }
             }
       }//dhour
      //Прибавляем дни
      if(dday)
      {
        //Прибавлять будем по одному дню
        int delta=1;
        if(dday < 0)delta=-1;
        while(dday)
        {
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
          day1 +=delta;
               if(day1 > day_mon[mon1])
               {
                 day1 -=day_mon[mon1]; mon1++;
                 if(mon1 > 12)
                 {
                   mon1 -=12; year1++;
                 }
               }
               if(day1 < 1)
               {
                 mon1--;
                 if(mon1 < 1)
                 {
                   mon1 +=12; year1--;
                 }
         //Определим високосность в текущем году
	  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	  if(isLY)day_mon[2]=29; else day_mon[2]=28;
                 day1 =day_mon[mon1];
               }
          dday -=delta;
        }
      }//dday
        dtstop.dt.year=year1;
        dtstop.dt.month=mon1;
        dtstop.dt.day=day1;
	dtstop.tm.hour=hour1;
	dtstop.tm.minute=min1;
	dtstop.tm.second=sec1;
	dtstop.tm.hsecond=hsec1;
}
//---------------------------------------------------------------------------
//Подпрограмма вычисления разницы между временными метками в секундах
double __fastcall TNVDGraph::GetDeltaTime(TDateTimeKadr &dtstart, TDateTimeKadr &dtstop)
{
	double hsecond;
//	unsigned long second;
	unsigned long mon,day,hour,min,sec,hsec;
	unsigned long mon1,day1,hour1,min1,sec1,hsec1,year1;
	unsigned long mon2,day2,hour2,min2,sec2,hsec2,year2;
	unsigned long i,isLY;
	unsigned long day_mon[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
        TDateTimeKadr dt1, dt2;
        double znak=1.;
        dt1=dtstart; dt2=dtstop;
        //Упорядочим по возрастанию времени
        if(dtstart.date > dtstop.date ||
           (dtstart.date == dtstop.date && dtstart.time > dtstop.time))
        {
          dt1=dtstop; dt2=dtstart;
          znak=-1;
        }
		  hsecond=0;
//		  second=0;
		  year1=dt1.dt.year;
		  mon1=dt1.dt.month;
		  day1=dt1.dt.day;
	hour1=dt1.tm.hour;
	min1=dt1.tm.minute;
	sec1=dt1.tm.second;
	hsec1=dt1.tm.hsecond;
		  year2=dt2.dt.year;
m0:
	day=0; hour=0; min=0; sec=0; hsec=0;
		  if(year2 > year1)
		  {
			mon2=12; day2=31;
	 hour2=24; min2=0; sec2=0; hsec2=0;
		  }
		  else
		  {
			 mon2=dt2.dt.month;
			 day2=dt2.dt.day;
	  hour2=dt2.tm.hour;
	  min2=dt2.tm.minute;
	  sec2=dt2.tm.second;
	  hsec2=dt2.tm.hsecond;
		  }
		  isLY=(year1%4==0)&&( (year1%100 !=0)|| (year1%400==0));
	if(isLY)day_mon[2]=29; else day_mon[2]=28;
	  for(i=mon1; i < mon2; i++)day +=day_mon[i];
	  day +=day2-day1;
	  hour =day*24-hour1+hour2;
	  min =hour*60-min1+min2;
	  sec =min*60-sec1+sec2;
	  hsec=sec*100-hsec1+hsec2;
	  hsecond +=hsec;
		  if(year2>year1)
		  {
			 year1++; mon1=1; day1=1;
	  hour1=0; min1=0; sec1=0; hsec1=0;
			 goto m0;
		  }
 return double(znak*hsecond/100.);
}
//---------------------------------------------------------------------------
//Подпрограмма получение позции x в double по времени
double __fastcall TNVDGraph::GetXDTdouble(TDateTimeKadr &dt)
{
   return GetDeltaTime(DTZero,dt);
}
//---------------------------------------------------------------------------
//Подпрограмма получение точки отсчёта времени
TDateTimeKadr __fastcall TNVDGraph::GetDTZero()
{
   return DTZero;
}
//---------------------------------------------------------------------------
//Округление времени вверх до кратности id
//id= 0-год, 1- месяц, 2- день, 3- час, 4- минута, 5-секунда, 6 - hsecond
void __fastcall TNVDGraph::RoundTimeUp(TDateTimeKadr &d,int id)
{
  unsigned long i,isLY;
  long day_mon[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
   //Определим високосность в текущем году
   isLY=(d.dt.year%4==0)&&( (d.dt.year%100 !=0)|| (d.dt.year%400==0));
   if(isLY)day_mon[2]=29; else day_mon[2]=28;

  switch(id)
  {
      case 0: //год
              if(d.time || d.dt.day > 1 || d.dt.month > 1)
                d.dt.year++;
              break;
      case 1: //месяц
              if(d.time || d.dt.day > 1)
              {
                d.dt.month++;
                if(d.dt.month > 12)
                {
                 d.dt.month -=12;
                 d.dt.year++;
                }
              }
              break;
      case 2: //день
              if(d.time)
              {
                d.dt.day++;
                if(d.dt.day > day_mon[d.dt.month])
                {
                 d.dt.day=1;
                 d.dt.month++;
                 if(d.dt.month > 12)
                 {
                  d.dt.month -=12;
                  d.dt.year++;
                 }
                }
              }
              break;
       case 3: //час
              if(d.tm.minute || d.tm.second || d.tm.hsecond)
              {
                 d.tm.hour++;
                 if(d.tm.hour >= 24)
                 {
                   d.tm.hour -= 24;
                   d.dt.day++;
                   if(d.dt.day > day_mon[d.dt.month])
                   {
                    d.dt.day=1;
                    d.dt.month++;
                    if(d.dt.month > 12)
                    {
                     d.dt.month -=12;
                     d.dt.year++;
                    }
                   }
                 }
              }
              break;
        case 4: //минута
              if(d.tm.second || d.tm.hsecond)
              {
                d.tm.minute++;
                if(d.tm.minute >=60)
                {
                   d.tm.minute -=60;
                   d.tm.hour++;
                   if(d.tm.hour >= 24)
                   {
                     d.tm.hour -= 24;
                     d.dt.day++;
                     if(d.dt.day > day_mon[d.dt.month])
                     {
                      d.dt.day=1;
                      d.dt.month++;
                      if(d.dt.month > 12)
                      {
                       d.dt.month -=12;
                       d.dt.year++;
                      }
                     }
                   }
                }
              }
              break;
        case 5: //секунда
              if(d.tm.hsecond)
              {
                d.tm.second++;
                if(d.tm.second >=60)
                {
                  d.tm.second-=60;
                  d.tm.minute++;
                  if(d.tm.minute >=60)
                  {
                     d.tm.minute -=60;
                     d.tm.hour++;
                     if(d.tm.hour >= 24)
                     {
                       d.tm.hour -= 24;
                       d.dt.day++;
                       if(d.dt.day > day_mon[d.dt.month])
                       {
                        d.dt.day=1;
                        d.dt.month++;
                        if(d.dt.month > 12)
                        {
                         d.dt.month -=12;
                         d.dt.year++;
                        }
                       }
                     }
                  }
                }
              }
              break;
        default:
              break;
  }
 //Окончательно
    switch(id)
    {
      case 0: d.dt.month=1;
      case 1: d.dt.day=1;
      case 2: d.tm.hour=0;
      case 3: d.tm.minute=0;
      case 4: d.tm.second=0;
      case 5: d.tm.hsecond=0;
      default:
            break;
    }
}
//---------------------------------------------------------------------------
//Округление времени вниз до кратности id
//id= 0-год, 1- месяц, 2- день, 3- час, 4- минута, 5-секунда, 6 - hsecond
void __fastcall TNVDGraph::RoundTimeDown(TDateTimeKadr &d,int id)
{
    switch(id)
    {
      case 0: d.dt.month=1;
      case 1: d.dt.day=1;
      case 2: d.tm.hour=0;
      case 3: d.tm.minute=0;
      case 4: d.tm.second=0;
      case 5: d.tm.hsecond=0;
      default:
            break;
    }
}
//---------------------------------------------------------------------------
//--------- Рисование временной оси ------------------------
// xp,yp - точка начала оси, lx - длина оси
// dt_from,dt_to - границы оси
void __fastcall TNVDGraph::DrawAxesTime(TDateTimeKadr &dt_from,
                 TDateTimeKadr &dt_to, long xp,long yp,long lx)
{
 //Предустановки
//  HDC memdc=NULL;
//  TCanvas *MyCanvas=CV;
//  TFont *FFont=CV->Font;
//  int Delt0=5; //Длина штрихов больших
//  int Delt1=3; //Длина штрихов маленьких
//  int TH=0; //Коррекция высоты фонта
//  MyCanvas->Pen->Color=clBlack;
//  MyCanvas->Font->Name="Arial";
//  bool FGridX=false;

  //Локальные переменные
   //Названия месяцов
  char *smonth[]={"","Jan","Feb","Mar","Apr","May","Jun",
                     "Jul","Aug","Sep","Oct","Nov","Dec"};
//  HPEN oldpen;
  long lzx,lzy; //ширина и высота изображения символа
//  long ndel; //количество делений
  long ndstep; //разница в единицах шага
  long step; //расстояние между делениями в единицах шага
//  TDateTimeKadr dt_step; //шаг делений в формате TDateTimeKadr
//  double stepsec; //шаг делений в секундах
  int iddel0; //идентификатор больших делений
  int iddel1; //идентификатор маленьких делений
  int idconst; //идентификатор константы
  double dsecond; //интервал внутри границ в секундах
  //копируем временные границы
  TDateTimeKadr dtfrom=dt_from;
  TDateTimeKadr dtto=dt_to;
  char bufconst[40];//комментарий константы
  char buf[40],bufdop[40];
  idconst=-1; //константа пока неопределена
  bufconst[0]=0; //комментария константы пока нет
  //Размер изображения символа
  lzx=MyCanvas->TextWidth("0");
  lzy=MyCanvas->TextHeight("0")-TH+5+Delt0;

  //Если метки начала и конца одинаковы
  //то добавим 60 секунд к концу
  if( (dtfrom.date==dtto.date) && (dtfrom.time==dtto.time))
  {
    AddDeltaTime(dtfrom,60.,dtto);
  }
// Определим временную константу оси
  if(dtfrom.dt.year == dtto.dt.year)
  {
    idconst=0; //год не меняется
    if(dtfrom.dt.month == dtto.dt.month)
    {
      idconst=1; //месяц не меняется
      if(dtfrom.dt.day == dtto.dt.day)
      {
        idconst=2; //день не меняется
        if(dtfrom.tm.hour == dtto.tm.hour)
        {
          idconst=3; //час не меняется
          if(dtfrom.tm.minute == dtto.tm.minute)
          {
            idconst=4; //минута не меняется
            if(dtfrom.tm.second == dtto.tm.second)
            {
             idconst=5; //секунда не меняется
            }//second
          }//minute
        }//hour
      }//day
    }//month
  }//year
 //Заполним комментарий константы
  for(int i=0; i <=idconst; i++)
  {
    switch(i)
    {
      case 0: //Год
           sprintf(bufconst+strlen(bufconst),"%04d",dtfrom.dt.year);
           break;
      case 1: //месяц
           sprintf(bufconst+strlen(bufconst),"-%s",smonth[dtfrom.dt.month]);
           break;
      case 2: //день
           sprintf(bufconst+strlen(bufconst),"-%02d",dtfrom.dt.day);
           break;
      case 3: //час
           sprintf(bufconst+strlen(bufconst)," %02d",dtfrom.tm.hour);
           break;
      case 4: //минута
           sprintf(bufconst+strlen(bufconst),":%02d",dtfrom.tm.minute);
           break;
      case 5: //секунда
           sprintf(bufconst+strlen(bufconst),":%02d",dtfrom.tm.second);
           break;
    }
  }
// ======Определение кол-ва делений оси =====
  //Интервал в секундах
  dsecond=GetDeltaTime(dtfrom,dtto);
  if(dsecond < 0) //Упорядочим начало и конец по возрастанию
  {
    dtfrom=dt_to;
    dtto=dt_from;
    dsecond =-dsecond;
  }
  // Начинаем выбирать начиная от idconst+1
  iddel0=idconst;
  ndstep=0;
 char *maskafulltext="0000-MMM-00";
 //вычислим размер занимаемый под дополнительную подпись к делению
 long lenfulltext=MyCanvas->TextWidth(maskafulltext);
 do
 {
  iddel0++;
  //Определим сколько таких делений разместится на оси
  switch(iddel0)
  {
   case 0: //в годах
        ndstep =dtto.dt.year-dtfrom.dt.year;
        break;
   case 1: //в месяцах
        ndstep +=dtto.dt.month-dtfrom.dt.month+ndstep*12;
        break;
   case 2: //в днях
        ndstep =dsecond/(24*3600);
        break;
   case 3: //в часах
        ndstep =dsecond/3600;
        break;
   case 4: //в минутах
        ndstep =dsecond/60;
        break;
   case 5: //в секундах
        ndstep =dsecond;
        break;
   case 6: //в сотых долях секунды
        ndstep =dsecond*100;
        break;
  }
 }while(iddel0 < 6 && ndstep < 2);
 if(iddel0==6)
 {
   //рисуем ось стандартным образом
   //максимум и минимум по разнице в hsecond
   // Но не забыв поставить штрих на месте idconst+1
   // если iddel0 не равен idconst+1
   return;
 }
 onex=dsecond/(lx-4); //количество секунд на пиксел
 if(iddel0 < 5) //может потребоваться более глубокая разбивка
 {
  int ichange=1;
  while(ichange)
  {
   ichange=0;
   switch(iddel0)
   {
    case 0:
           if(365.*24.*3600./onex > 2.*lenfulltext){ iddel0++; ichange=1;}
           break;
    case 1:
           if(28.*24.*3600./onex > 2.*lenfulltext){ iddel0++; ichange=1;}
           break;
    case 2:
           if(24.*3600./onex > 2.*lenfulltext){ iddel0++; ichange=1;}
           break;
    case 3:
           if(3600./onex > 2.*lenfulltext){ iddel0++; ichange=1;}
           break;
    case 4:
           if(60./onex > 2.*lenfulltext){ iddel0++; ichange=1;}
           break;
    default: break;
   }
  }
 }
// Рисуем Ось
	if(!memdc)MyCanvas->MoveTo(xp,yp) ;
        else ::MoveToEx(memdc,xp,yp,NULL);
	if(!memdc)MyCanvas->LineTo(xp+lx,yp);
        else ::LineTo(memdc,xp+lx,yp);
 //Рисуем большие деления начиная с начала оси
 step=1; //начальный шаг, динамически скорректируем
 int ntemp;
                //   год  месяц  день    час   минута  секунда
 char *maskatext[]={"0000","MMM","00", "00:00","00:00", "00:00"};

 //вычислим размер занимаемый под подпись к делению
 long lenonetext=MyCanvas->TextWidth(maskatext[iddel0]);
 long ixprev=-1; //позиция на экране предыдущей метки
 // Деления будем рисовать справа и слева от "центральной" метки 
 // находящейся на месте кратном максимально значимому временному
 // делению
 TDateTimeKadr dtcentr=dtfrom; //время предыдущей метки
 //Сделаем кратной метке по позиции idconst+1
  RoundTimeUp(dtcentr,idconst+1);
 //Определим шаг step
 int ivector=1; //направление первого пробного движения от dtcentr в сторону dtto
 if(fabs(GetDeltaTime(dtcentr,dtto)) < fabs(GetDeltaTime(dtcentr,dtfrom)))
  ivector=-1; //От dtcentr в сторону dtfrom
 //Положение на экране метки dtcentr
  //Определим расстояние в секундах от начала до текущей метки
   double ds=GetDeltaTime(dtfrom,dtcentr);
   //Позиция метки на экране
    ixprev=long(ds/onex+0.5)+xp+2;
   long dlen=0; //расстояние в пикселах между большими делениями
   for(;;)
   {
     TDateTimeKadr dtcur=dtcentr; //время текущей метки
      //Вычислим время в этом месте
      switch(iddel0)
      {
        case 0: //годы
            dtcur.dt.year = dtcur.dt.year+step*ivector;
            break;
        case 1: //месяцы
             ntemp=dtcur.dt.month+(step%12)*ivector;
             if(ntemp <= 0)
             {
              ntemp +=12;
              dtcur.dt.year =dtcur.dt.year-1;
             }
             if(ntemp > 12)
             {
              ntemp -=12;
              dtcur.dt.year =dtcur.dt.year+1;
             }
             dtcur.dt.month=ntemp;
             dtcur.dt.year +=ivector*step/12;
            break;
        case 2: //дни
            AddDeltaTime(dtcur,ivector*24.*3600.*step,dtcur);
            break;
        case 3: //часы
            AddDeltaTime(dtcur,ivector*3600.*step,dtcur);
            break;
        case 4: //минуты
            AddDeltaTime(dtcur,ivector*60.*step,dtcur);
            break;
        case 5: //секунды
            AddDeltaTime(dtcur,ivector*step,dtcur);
            break;
      }//iddel0
    //Определим расстояние в секундах от начала до текущей метки
     double ds=GetDeltaTime(dtfrom,dtcur);
     //Позиция метки на экране
     long ix=long(ds/onex+0.5)+xp+2;
     //Определим, не пересекаемся ли по тексту с предыдущей меткой
      dlen=abs(ix-ixprev)-lenonetext;
      if(dlen >= lzx)
      {
        dlen=abs(ix-ixprev); //запомним расстояние между делениями
        break;
      }
      step++;
   }
 //Определим сколько делать маленьких делений
 iddel1=iddel0;
 long stepm=0; //первая оценка количества маленьких делений
 if(step==1)//Значит маленькие деления будут в других единицах
 {
  iddel1++;
 }
 //Произведём оптимизацию количества делений в зависимости от
 // единицы деления
 switch(iddel1)
 {
    case 0: //деления по годам
    case 2: //деления по дням
    case 6: //деления по долям секунды
	if(dlen >= Delt0+1.)stepm=1 ;
	if(dlen/2. >= Delt0+1.)stepm=2 ;
	if(dlen/5. >= Delt0+1.)stepm=5 ;
	if(dlen/10. >= Delt0+1.)stepm=10 ;
        if(iddel1==iddel0 && stepm > step )stepm=step;
        break;
    case 1: //деления по месяцам
	if(dlen >= Delt0+1.)stepm=1 ;
	if(dlen/2. >= Delt0+1.)stepm=2 ;
	if(dlen/6. >= Delt0+1.)stepm=6 ;
	if(dlen/10. >= Delt0+1.)stepm=12 ;
        break;
    case 3: //деления по часам
	if(dlen >= Delt0+1.)stepm=1 ;
	if(dlen/2. >= Delt0+1.)stepm=2 ;
	if(dlen/6. >= Delt0+1.)stepm=6 ;
	if(dlen/12. >= Delt0+1.)stepm=12 ;
        if(iddel1==iddel0 && stepm > step)stepm=step;
        break;
    case 4: //деления по минутам
    case 5: //деления по секундам
	if(dlen >= Delt0+1.)stepm=1 ;
	if(dlen/2. >= Delt0+1.)stepm=2 ;
	if(dlen/5. >= Delt0+1.)stepm=5 ;
	if(dlen/6. >= Delt0+1.)stepm=6 ;
	if(dlen/15. >= Delt0+1.)stepm=15 ;
	if(dlen/20. >= Delt0+1.)stepm=20 ;
	if(dlen/30. >= Delt0+1.)stepm=30 ;
        if(iddel1==iddel0 && stepm > step)stepm=step;
        break;
 }
 //Шаг step определили, теперь можно сосредоточится на рисовании делений
 TDateTimeKadr dtprev;
  dtprev=dtfrom;
 int iddop=0; //признак рисования дополнительной строки
 for(int iv=0; iv < 2; iv++)
 {
   for(int i=0;;i+=step)
   {
     TDateTimeKadr dtcur=dtcentr; //время текущей метки
      //Вычислим время в этом месте
      switch(iddel0)
      {
        case 0: //годы
            dtcur.dt.year = dtcur.dt.year+i*ivector;
            break;
        case 1: //месяцы
             ntemp=dtcur.dt.month+(i%12)*ivector;
             if(ntemp <= 0)
             {
              ntemp +=12;
              dtcur.dt.year =dtcur.dt.year-1;
             }
             if(ntemp > 12)
             {
              ntemp -=12;
              dtcur.dt.year =dtcur.dt.year+1;
             }
             dtcur.dt.month=ntemp;
             dtcur.dt.year +=ivector*i/12;
            break;
        case 2: //дни
            AddDeltaTime(dtcur,ivector*24.*3600.*i,dtcur);
            break;
        case 3: //часы
            AddDeltaTime(dtcur,ivector*3600.*i,dtcur);
            break;
        case 4: //минуты
            AddDeltaTime(dtcur,ivector*60.*i,dtcur);
            break;
        case 5: //секунды
            AddDeltaTime(dtcur,ivector*i,dtcur);
            break;
      }//iddel0
    //Определим расстояние в секундах от начала до текущей метки
     double ds=GetDeltaTime(dtfrom,dtcur);
     //Позиция метки на экране
     long ix=long(ds/onex+0.5)+xp+2;
    if(ix < xp || ix > xp+lx)break;
    if(iv==0 || i)
    {
     if(!memdc)MyCanvas->MoveTo(ix,yp);
     else ::MoveToEx(memdc,ix,yp,NULL);
     if(!memdc)MyCanvas->LineTo(ix,yp+Delt0);
     else ::LineTo(memdc,ix,yp+Delt0);
     if(idDrawTextAX)
     {
       bufdop[0]=0;
       switch(iddel0)
       {
        case 0: //год
               sprintf(buf,"%04d",dtcur.dt.year);
               break;
        case 1: //месяц
               sprintf(buf,"%s",smonth[dtcur.dt.month]);
               if(iddel0-idconst > 1 && dtcur.dt.year !=dtprev.dt.year
                   && (dtcur.dt.month==1 ||(ivector==1 &&dtcur.dt.year !=dtprev.dt.year)))
                sprintf(bufdop,"%04d",dtcur.dt.year);
               break;
        case 2: //день
               sprintf(buf,"%02d",dtcur.dt.day);
               if(iddel0-idconst > 1 && (dtcur.dt.day==1 || (ivector==1 &&dtcur.dt.month !=dtprev.dt.month)))
               {
                 if(dtcur.dt.year !=dtprev.dt.year || dtcur.dt.month !=dtprev.dt.month)
                  sprintf(bufdop,"%04d-%s",dtcur.dt.year,smonth[dtcur.dt.month]);
               }
               break;
        case 3: //час
               sprintf(buf,"%02d:00",dtcur.tm.hour);
               if(iddel0-idconst > 1 &&  (dtcur.time==0 || (ivector==1 &&dtcur.date !=dtprev.date)))
               {
                  sprintf(bufdop,"%04d-%s-%02d",dtcur.dt.year,smonth[dtcur.dt.month],
                           dtcur.dt.day);
               }
               break;
        case 4: //минута
               if(idconst !=iddel0-1 || dtcur.tm.hour != dtprev.tm.hour)
                sprintf(buf,"%02d:%02d",dtcur.tm.hour,dtcur.tm.minute);
               else
                sprintf(buf,"%02d",dtcur.tm.minute);
               if(iddel0-idconst > 1 && (dtcur.time==0 || (ivector==1 &&dtcur.date !=dtprev.date)))
               {
                  sprintf(bufdop,"%04d-%s-%02d",dtcur.dt.year,smonth[dtcur.dt.month],
                           dtcur.dt.day);
               }
               break;
        case 5: //секунда
               if(idconst !=iddel0-1 || dtcur.tm.minute != dtprev.tm.minute)
                sprintf(buf,"%02d:%02d",dtcur.tm.minute,dtcur.tm.second);
               else
                sprintf(buf,"%02d",dtcur.tm.second);
               if(iddel0-idconst > 1 && (dtcur.time==0 || (ivector==1 &&dtcur.date !=dtprev.date)))
               {
                  sprintf(bufdop,"%04d-%s-%02d %02d:%02d",dtcur.dt.year,smonth[dtcur.dt.month],
                           dtcur.dt.day,dtcur.tm.hour,dtcur.tm.minute);
               }
               break;
        default:
               buf[0]=0;
               break;
       }
       if(!memdc)
        MyCanvas->TextOut(ix-MyCanvas->TextWidth(buf)/2,yp+2+Delt0,buf );
       else
        ::TextOut(memdc,ix-MyCanvas->TextWidth(buf)/2,yp+2+Delt0,buf,
                  strlen(buf));
       if(bufdop[0])
       {
        iddop=1;
        if(!memdc)
         MyCanvas->TextOut(ix-MyCanvas->TextWidth(bufdop)/2,yp+2+Delt0+lzy/2,bufdop );
        else
         ::TextOut(memdc,ix-MyCanvas->TextWidth(bufdop)/2,yp+2+Delt0+lzy/2,bufdop,
                   strlen(bufdop));
       }
     }
    }
    //рисуем маленькие деления здесь
    for(int j=1; j < stepm; j++)
    {
      int ixx=ix+ivector*j*dlen/stepm;
      if(ixx < xp || ixx > xp+lx)continue;
      if(!memdc)MyCanvas->MoveTo(ixx,yp);
      else ::MoveToEx(memdc,ixx,yp,NULL);
      if(!memdc)MyCanvas->LineTo(ixx,yp+Delt1);
      else ::LineTo(memdc,ixx,yp+Delt1);
    }
     dtprev=dtcur;
   }
   ivector=-ivector;
  }//ivector

  if(idDrawTextAX && idconst >=0 && iddop==0)
  {
   if(idconst==3)
   {
     strcat(bufconst,":00");
   }
   if(idconst==2 && iddel0 > 3)
   {
     sprintf(bufconst+strlen(bufconst)," %02d:00",dtfrom.tm.hour);
   }
   if(!memdc)
    MyCanvas->TextOut(xp+lx/2-MyCanvas->TextWidth(bufconst)/2,yp+lzy,bufconst );
   else
    ::TextOut(memdc,xp+lx/2-MyCanvas->TextWidth(buf)/2,yp+lzy,bufconst,
        strlen(bufconst));
  }
  return;
}
//---------------------------------------------------------------------------
namespace Nvdgraph
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TNVDGraph)};
       RegisterComponents("NEVOD", classes, 0);
   }
}
//---------------------------------------------------------------------------
