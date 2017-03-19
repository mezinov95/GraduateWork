//---------------------------------------------------------------------------

#ifndef NVDGraphH
#define NVDGraphH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

//структура для хранения даты и времени в ввиде удобном для
//сравнения по критериям раньше, позже
#ifndef dTDateTimeKadr
#define dTDateTimeKadr
#pragma pack(push,1)
typedef struct tagTDateTimeKadr
{
   union{
    struct{
     unsigned char hsecond; //сотые секунды
     unsigned char second;  //секунда
     unsigned char minute;  //минута
     unsigned char hour; //час
    }tm;
    unsigned long time;
   };
   union{
    struct{
     unsigned char day; //день
     unsigned char month; //месяц
     unsigned short year; //год
    }dt;
    unsigned long date;
   };
}TDateTimeKadr;
#pragma pack(pop)
#endif

enum TNVDGraphKind { pgNone, pgPoint, pgLine, pgCombo, pgBar};
enum TNVDGraphType { gtGraphic, gtHistogram};
enum TNVDGraphTypeBuf { tbChar,tbUChar, tbShort, tbUShort,
                        tbInt,tbUInt, tbLong,tbULong,
                        tbFloat,tbDouble,tbFunc,tbTime,tbFuncDT};
#define MAXNBUF 32
#pragma pack(push,8)
//=====GraphItem - Структура параметров для одного графика =====
// работаем с ней при UseForAll=false
typedef struct tagGraphItem
{
 TDateTimeKadr xdtmin,xdtmax; //Для режима tbTime при рисовании оси
 TDateTimeKadr ydtmin,ydtmax; //Для режима tbTime при рисовании оси
 double xmax,xmin,ymax,ymin,nwx;//Границы и кол-во в столбе для гистограммы
 // временные переменные для внутреннего использования.
 double onex,oney,dxmax;
}GraphItem;
typedef double __fastcall (__closure *TNvdGetVar)(int index,int nomerbuf=0);
typedef void __fastcall (__closure *TNvdUserDraw)(HDC hdc,TCanvas *CV,double xmin, double xmax,int nomerbuf);
typedef int __fastcall (__closure *TNvdGetVarDT)(int index,TDateTimeKadr &DT, int nomerbuf=0);

//---------------------------------------------------------------------------
class PACKAGE TNVDGraph : public TGraphicControl
{
private:
         TDateTimeKadr DTZero; //Точка отсчёта времени
         TDateTimeKadr xdtmin,xdtmax;
         TDateTimeKadr ydtmin,ydtmax;
	 long Delt0,Delt1,xp,yp,lx,ly,Axtop,Axbot,ngr;
	 double onex,oney,xmax,xmin,ymax,ymin,divide,dxmax;
         double nwx; //Размер ячейки для гистограммы
         bool NoAxes; //Если true оси и бокс не рисуем
         int MaskaPaint; //Маска нуждающихся в перерисовке буферов
         int TH; //Коррекция высоты фонта
    bool FAvtoScale; //Автоматическое определение границ
    bool FAvtoScaleX; //для X при автоматическом определении границ
    bool FAvtoScaleY; //для Y при автоматическом определении границ
    bool FUseForAll; // Если true используются общие параметры отображения
    bool FUseForAllX; // Если true используются общие параметры отображения X
    bool FUseForAllY; // Если true используются общие параметры отображения Y
    bool FUseScale; //Если true то ScaleX и ScaleY применяется
    double FScaleX; // дополнительный коэффициент для умножения
    double FScaleY; // дополнительный коэффициент для умножения
    bool FFastDraw; //Если true рисование через Bitmap в памяти
    bool FGrouped; //На одном графике или на разных
    bool FGridY; //Горизонтальная сетка
    bool FGridX; //Вертикальная сетка
    bool FUseforHit; //Использование подписей оси X для гистограммы хитов    TNVDGraphType FType; //Тип: график или гистограмма
    TNVDGraphType FType; //Тип: график или гистограмма
    TNVDGraphTypeBuf FTypeBufX; //Тип буфера X
    TNVDGraphTypeBuf FTypeBufY[MAXNBUF]; //Тип буфера Y
    TNVDGraphTypeBuf FTypeBufIdX; //Тип буфера IdX
    TNVDGraphTypeBuf FTypeBufIdY; //Тип буфера IdY

    TNvdGetVar FGetVarX[MAXNBUF]; //Адреса функций для доступа к массивам X
    TNvdGetVar FGetVarY[MAXNBUF]; //Адреса функций для доступа к массивам Y
    TNvdGetVar FGetVarIdX[MAXNBUF]; //Адреса функций для доступа к массивам IdX
    TNvdGetVar FGetVarIdY[MAXNBUF]; //Адреса функций для доступа к массивам IdY
    TNvdGetVarDT FGetVarXDT[MAXNBUF]; //Адреса функций для доступа к массивам X
    TNvdGetVarDT FGetVarYDT[MAXNBUF]; //Адреса функций для доступа к массивам Y

    Pointer FBufY[MAXNBUF]; //Адреса массивов Y
    Pointer FBufX[MAXNBUF]; //Адреса массивов X
    Pointer FBufIdX[MAXNBUF]; //Адреса массивов IdX
    Pointer FBufIdY[MAXNBUF]; //Адреса массивов IdY

    unsigned long *FNPoint[MAXNBUF] ; //Адрес количества точек в буферах
    long *FIDPointY[MAXNBUF] ; //Признак наличия точки в буферах
    long *FIDPointX[MAXNBUF] ; //Признак наличия точки в буферах

    GraphItem Items; //здесь общие параметры при UseForAll=false
    GraphItem Item[MAXNBUF]; //локальные параметры при UseForAll=false
    AnsiString FNameItem[MAXNBUF]; //Названия для режима Grouped=false
    int FNBuf; //Количество задействованных буферов
    int MaskaViewBuf; //Маска отображаемых на графике буферов
    TNVDGraphKind FKind; //Стиль отображения
    TFont *FFont;
    TColor FColorBk;
    TColor FPenColor[MAXNBUF];
     HDC memdc; //Для быстрой отрисовки
    AnsiString FLabelX;
    AnsiString FLabelY;
    int FUpdateCount;
    TNvdUserDraw FNvdUserDraw; //Пользовательская подпрограмма рисования
void __fastcall SetAvtoScale(bool Value);//Автоподбор границ
void __fastcall SetAvtoScaleX(bool Value);//Автоподбор границ для X
void __fastcall SetAvtoScaleY(bool Value);//Автоподбор границ для Y
void __fastcall SetLabelX(AnsiString label);//Подпись оси X
void __fastcall SetLabelY(AnsiString label);//Подпись оси Y
void __fastcall SetAxesPar(int idall);//Вычисляем положение и размеры осей
void __fastcall Setyp(int k); //вычисляем координату yp начала осей
void __fastcall GoDraw();
void __fastcall SetGran(int numitem);
void __fastcall SetUseForAll(bool Value);
void __fastcall SetUseForAllX(bool Value);
void __fastcall SetUseForAllY(bool Value);
void __fastcall SetUseScale(bool Value);
void __fastcall SetScaleX(double Value);
void __fastcall SetScaleY(double Value);
void __fastcall SetFastDraw(bool Value);
void __fastcall SetMaska(int Value);
void __fastcall SetGrouped(bool Value);
void __fastcall SetType(TNVDGraphType Value);
void __fastcall SetTypeBufX(TNVDGraphTypeBuf Value);
void __fastcall SetTypeBufY(int numitem,TNVDGraphTypeBuf Value);
TNVDGraphTypeBuf __fastcall GetTypeBufY(int numitem);
void __fastcall SetTypeBufIdX(TNVDGraphTypeBuf Value);
void __fastcall SetTypeBufIdY(TNVDGraphTypeBuf Value);
void __fastcall SetNBuf(int Value);
void __fastcall SetGraphKind(TNVDGraphKind Value);
void __fastcall SetCaption(String Value);
void __fastcall SetAxesLeft(long Value);
void __fastcall SetAxesTop(long Value);
void __fastcall SetAxesBot(long Value);
void __fastcall SetFont(TFont* Value);
void __fastcall SetColorBk(TColor Value);
void __fastcall SetXmin(double Value);
void __fastcall SetXmax(double Value);
void __fastcall SetXsum(double Value);
void __fastcall SetYmin(double Value);
void __fastcall SetYmax(double Value);
void __fastcall SetOneXmin(int numitem,double Value);
void __fastcall SetOneXmax(int numitem,double Value);
void __fastcall SetOneXDTmin(int numitem,TDateTimeKadr& Value);
void __fastcall SetOneXDTmax(int numitem,TDateTimeKadr& Value);
void __fastcall SetOneXsum(int numitem,double Value);
void __fastcall SetOneYmin(int numitem,double Value);
void __fastcall SetOneYmax(int numitem,double Value);
double __fastcall GetXmin();
double __fastcall GetXmax();
double __fastcall GetXsum();
double __fastcall GetYmin();
double __fastcall GetYmax();
double __fastcall GetOneXmin(int numitem);
double __fastcall GetOneXmax(int numitem);
TDateTimeKadr __fastcall GetOneXDTmin(int numitem);
TDateTimeKadr __fastcall GetOneXDTmax(int numitem);
double __fastcall GetOneXsum(int numitem);
double __fastcall GetOneYmin(int numitem);
double __fastcall GetOneYmax(int numitem);
TColor __fastcall GetPenColor(int numitem){return FPenColor[numitem];}
void __fastcall SetPenColor(int numitem,TColor Value){if(numitem>=0&&numitem<=MAXNBUF)FPenColor[numitem]=Value;}
void __fastcall SetGridY(bool Value);
void __fastcall SetGridX(bool Value);
void __fastcall SetUseforHit(bool Value);
void __fastcall SetNameItem(int numitem, AnsiString &Value);
AnsiString __fastcall GetNameItem(int numitem);
void __fastcall ItemsToWork(); //Установка текущих параметров
void __fastcall WorkToItems(); //Сохранение текущих параметров
void __fastcall ItemToWork(int numitem); //Установка текущих параметров
void __fastcall WorkToItem(int numitem); //Сохранение текущих параметров
long __fastcall analr(double a, double* am);
short __fastcall Into_rect(double *t1,double *t2) ; // отсечение по границам
protected:
	virtual void __fastcall Paint(void);
public:
    TCanvas *MyCanvas;
         bool idDrawTextAX; //Признак рисования подписей к делениям
         bool idDrawTextAY; //Признак рисования подписей к делениям
   __property Canvas;
   __fastcall TNVDGraph(TComponent* Owner);
	__fastcall ~TNVDGraph(void);
void __fastcall SetXDTmin(TDateTimeKadr& Value);
void __fastcall SetXDTmax(TDateTimeKadr& Value);
TDateTimeKadr __fastcall GetXDTmin();
TDateTimeKadr __fastcall GetXDTmax();
   bool __fastcall SaveToFile(AnsiString filename,bool id_emf);
   void __fastcall SetFuncY(TNvdGetVar AFuncY, TNvdGetVar AFuncIdY, int Value);
   void __fastcall SetFuncX(TNvdGetVar AFuncX, TNvdGetVar AFuncIdX, int Value);
   void __fastcall SetFuncYDT(TNvdGetVarDT AFuncY, TNvdGetVar AFuncIdY, int Value);
   void __fastcall SetFuncXDT(TNvdGetVarDT AFuncX, TNvdGetVar AFuncIdX, int Value);
   void __fastcall SetBufY(Pointer ABuf, long *AID, int Value);
   void __fastcall SetBufX(Pointer ABuf, long *AID, int Value);
   void __fastcall SetBufIdX(Pointer ABuf,int Value);
   void __fastcall SetNPoint(unsigned long *ANPoint, int Value);
   unsigned long __fastcall GetNPoint(int Value);
   bool __fastcall GetValueXY(int index, int nombuf, double &x, double &y);
	void __fastcall StyleChanged(TObject* Owner);
	 short __fastcall Get_xy(int pixel_x,int pixel_y,double *x,double *y);
	 void __fastcall x_axes(double xminn, double xmaxx, long xp,long yp,long lx);
	void __fastcall y_axes(double yminn, double ymaxy, long xp,long yp,long ly);
	 void __fastcall Draw_Box();
	 void __fastcall Draw_PointRect(double x,double y,double lenx,double leny);
	 void __fastcall Draw_Rect(double x0,double y0,double x1,double y1);
	 void __fastcall Draw_Point(double x,double y);
	 void __fastcall Draw_Line(double x1,double y1,double x2, double y2);
         //Гистограмму на экран
	 void __fastcall Draw_Histo(double x,double x1, double y, double nwxx,int ifirst);
         void __fastcall Draw_SPos(double x,int nomerbuf);
	 void __fastcall Clear();//очистка области внутри графика
         void __fastcall BeginUpdate(void);
         void __fastcall EndUpdate(void);
         void __fastcall DrawXMarker(double x, TColor Color);
         void __fastcall DrawYMarker(double y, TColor Color);
         //Прибавление к дате секунд
         void __fastcall AddDeltaTime(TDateTimeKadr &dtstart, double deltasec,
                                      TDateTimeKadr &dtstop);
         //Подпрограмма вычисления разницы между временными метками в секундах
         double __fastcall GetDeltaTime(TDateTimeKadr &dtstart, TDateTimeKadr &dtstop);
         //Подпрограмма получение позции x в double по времени
         double __fastcall GetXDTdouble(TDateTimeKadr &dt);
         //Подпрограмма получение точки отсчёта времени
         TDateTimeKadr __fastcall GetDTZero();
         //Округление времени вверх до кратности id
         //id= 0-год, 1- месяц, 2- день, 3- час, 4- минута, 5-секунда, 6 - hsecond
         void __fastcall RoundTimeUp(TDateTimeKadr &d,int id);
         //Округление времени вниз до кратности id
         //id= 0-год, 1- месяц, 2- день, 3- час, 4- минута, 5-секунда, 6 - hsecond
         void __fastcall RoundTimeDown(TDateTimeKadr &d,int id);
         //Рисование временной оси
         void __fastcall DrawAxesTime(TDateTimeKadr &dt_from,
                            TDateTimeKadr &dt_to, long xp,long yp,long lx);
         void __fastcall SetUserDraw(TNvdUserDraw UserDraw)
         {
           if(FNvdUserDraw !=UserDraw)
           {
             FNvdUserDraw=UserDraw;
             StyleChanged(this);
           }
         };
	 void __fastcall Draw()
	 {
           if(NoAxes)return;
           if(FUseScale)
           {
             if(FTypeBufX==tbTime ||FTypeBufX==tbFuncDT)
                DrawAxesTime(xdtmin,xdtmax,xp,yp,lx);
             else
	        x_axes(xmin*FScaleX, xmax*FScaleX, xp,yp,lx);
	     y_axes(ymin*FScaleY, ymax*FScaleY, xp,yp,ly);
           }
           else
           {
             if(FTypeBufX==tbTime ||FTypeBufX==tbFuncDT)
                DrawAxesTime(xdtmin,xdtmax,xp,yp,lx);
             else
  	        x_axes(xmin, xmax, xp,yp,lx);
	     y_axes(ymin, ymax, xp,yp,ly);
           }
	  Draw_Box();
	 }
__published:
   __property Align ;
	__property bool AvtoScale = {read=FAvtoScale, write=SetAvtoScale, default=false};
	__property bool AvtoScaleX = {read=FAvtoScaleX, write=SetAvtoScaleX, default=false};
	__property bool AvtoScaleY = {read=FAvtoScaleY, write=SetAvtoScaleY, default=false};
	__property bool UseForAll = {read=FUseForAll, write=SetUseForAll, default=true};
	__property bool UseForAllX = {read=FUseForAllX, write=SetUseForAllX, default=false};
	__property bool UseForAllY = {read=FUseForAllY, write=SetUseForAllY, default=false};
	__property bool UseScale = {read=FUseScale, write=SetUseScale, default=false};
	__property double ScaleX = {read=FScaleX, write=SetScaleX, default=1};
	__property double ScaleY = {read=FScaleY, write=SetScaleY, default=1};
	__property bool FastDraw = {read=FFastDraw, write=SetFastDraw, default=false};
		__property int Maska = {read=MaskaViewBuf, write=SetMaska, default=0};
	__property bool Grouped = {read=FGrouped, write=SetGrouped, default=true};
	__property TNVDGraphType Type = {read=FType, write=SetType, default=1};
	__property TNVDGraphTypeBuf TypeBufX = {read=FTypeBufX, write=SetTypeBufX, default=1};
	__property TNVDGraphTypeBuf TypeBufY[int numitem] = {read=GetTypeBufY, write=SetTypeBufY};
	__property TNVDGraphTypeBuf TypeBufIdX = {read=FTypeBufIdX, write=SetTypeBufIdX, default=1};
	__property TNVDGraphTypeBuf TypeBufIdY = {read=FTypeBufIdY, write=SetTypeBufIdY, default=1};
	__property int NBuf = {read=FNBuf, write=SetNBuf, default=0};
	__property TNVDGraphKind Kind = {read=FKind, write=SetGraphKind, default=1};
   __property  long AxesLeft={read=xp, write=SetAxesLeft, nodefault};
   __property  long AxesTop={read=Axtop, write=SetAxesTop, nodefault};
   __property  long AxesBot={read=Axbot, write=SetAxesBot, nodefault};
	__property	Height;
	__property	Width;
   __property  TFont *Font={read=FFont,write=SetFont,nodefault};
   __property  double XminAll={read=GetXmin, write=SetXmin, default=0};
   __property  double XmaxAll={read=GetXmax, write=SetXmax, default=100};
//   __property  TDateTimeKadr XDTminAll={read=GetXDTmin, write=SetXDTmin};
//   __property  TDateTimeKadr XDTmaxAll={read=GetXDTmax, write=SetXDTmax};
   //Для гистограммы размер ячейки
   __property  double XsumAll={read=GetXsum, write=SetXsum, default=1};
   __property  double YminAll={read=GetYmin, write=SetYmin, default=0};
   __property  double YmaxAll={read=GetYmax, write=SetYmax, default=100};

   __property  double Xmin[int numitem]={read=GetOneXmin, write=SetOneXmin};
   __property  double Xmax[int numitem]={read=GetOneXmax, write=SetOneXmax};
//   __property  TDateTimeKadr XDTmin[int numitem]={read=GetOneXDTmin, write=SetOneXDTmin};
//   __property  TDateTimeKadr XDTmax[int numitem]={read=GetOneXDTmax, write=SetOneXDTmax};
   //Для гистограммы размер ячейки
   __property  double Xsum[int numitem]={read=GetOneXsum, write=SetOneXsum};
   __property  double Ymin[int numitem]={read=GetOneYmin, write=SetOneYmin};
   __property  double Ymax[int numitem]={read=GetOneYmax, write=SetOneYmax};
   __property  AnsiString LabelX={read=FLabelX, write=SetLabelX};
   __property  AnsiString LabelY={read=FLabelY, write=SetLabelY};
   __property  bool GridY={read=FGridY, write=SetGridY};
   __property  bool GridX={read=FGridX, write=SetGridX};
   __property  bool UseforHit={read=FUseforHit, write=SetUseforHit};

	__property	TColor ColorBk={read=FColorBk,write=SetColorBk,default=clWhite};
	__property	TColor PenColor[int numitem]={read=GetPenColor,write=SetPenColor};
        __property      AnsiString NameItem[int numitem]={read=GetNameItem,write=SetNameItem};
	__property	Hint;
	__property	Caption;
   __property	ShowHint;
__property DragCursor ;
__property DragKind ;
__property DragMode ;
__property OnDragDrop ;
__property OnDragOver ;
__property OnEndDrag ;
__property OnMouseDown ;
__property OnMouseMove ;
__property OnMouseUp ;

};
#pragma pack(pop)
//---------------------------------------------------------------------------
#endif
