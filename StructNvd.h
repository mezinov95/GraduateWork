#ifndef StructNVD_H
#define StructNVD_H
#include "config.h"
#include "trignvd.h"
//Структуры для оформления данных ЭК НЕВОД
#pragma pack(push,1)
//структура для хранения даты и времени в ввиде удобном для
//сравнения по критериям раньше, позже
#ifndef dTDateTimeKadr
#define dTDateTimeKadr
struct TDateTimeKadr
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
};
#endif
#pragma pack(pop)

//Шумы триггерных сигналов в Гц
struct SNoiseTrigNvd
{
  unsigned short NoiseA[128]; //По максимуму
  unsigned short NoiseB[128]; //По максимуму
  unsigned short NoiseC[128]; //По максимуму
  unsigned short NoiseSKT[128]; //По максимуму
};
#pragma pack(push,1)
//Данные мониторинга одного БЭКа
struct SMonADC
{
   unsigned short ToSave; //Флаг наличия новых данных, 1 - надо их сохранить.
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned short maskapmt[4];  //[ksm] маска ФЭУ измерений
   unsigned short nzap; //Кoличество запусков измерения
   unsigned short nsum[4][6][2]; //[ksm][pmt][dinod12, dinod9] кол-во измерений в спектре Пьедесталов
   float sred[4][6][2]; //[ksm][pmt][dinod12, dinod9] Пьедесталы
   float sigma[4][6][2]; //[ksm][pmt][dinod12, dinod9] сигма пьедесталов
};
struct SMonADC_S //Для контроля
{
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned char maskapmt[4];  //[ksm] маска ФЭУ измерений
   unsigned short nzap; //Кoличество запусков измерения
   unsigned short nsum[4][6][2]; //[ksm][pmt][dinod12, dinod9] кол-во измерений в спектре Пьедесталов
   short sred[4][6][2]; //[ksm][pmt][dinod12, dinod9] Пьедесталы *10
   short sigma[4][6][2]; //[ksm][pmt][dinod12, dinod9] сигма пьедесталов *10
};

struct SMonShumTV
{
   unsigned short ToSave; //Флаг наличия новых данных, 1 - надо их сохранить.
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned short maskapmt[4];  //[ksm] маска ФЭУ измерений
   float noisePMT[4][6]; //[ksm][pmt] Шумы ФЭУ в кГц
   float tbek[4];//Температура в БЭК: Tout, T3,T2,T1
   float vbek[5]; //Напряжения  в БЭК: V1,V2,Vcc,V3,V4
};
struct SMonShumTV_S //Для контроля
{
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned char maskapmt[4];  //[ksm] маска ФЭУ измерений
   unsigned short noisePMT[4][6]; //[ksm][pmt] Шумы ФЭУ в Гц
   unsigned short tbek[4];//Температура в БЭК: Tout, T3,T2,T1  *10
   short vbek[5]; //Напряжения  в БЭК: V1,V2,Vcc,V3,V4 *100
};

struct SMonBek
{
   unsigned short ToSave; //Флаг наличия новых данных, 1 - надо их сохранить.
   TDateTimeKadr dt; //Время измерения шумов триггеров(UTC)
   unsigned short maskaTrA; //Маска КСМ измерения шумов триггера A
   unsigned short maskaTrB; //Маска КСМ измерения шумов триггера B
   unsigned short maskaTrC; //Маска КСМ измерения шумов триггера C
   unsigned short noiseTrA[4]; //[ksm] Шумы триггерного сигнала A
   unsigned short noiseTrB[4]; //[ksm] Шумы триггерного сигнала B
   unsigned short noiseTrC[4]; //[ksm] Шумы триггерного сигнала C
};
struct SMonBek_S //Для контроля
{
   TDateTimeKadr dt; //Время измерения шумов триггеров(UTC)
   unsigned short noiseTrA[4]; //[ksm] Шумы триггерного сигнала A
   unsigned short noiseTrB[4]; //[ksm] Шумы триггерного сигнала B
   unsigned short noiseTrC[4]; //[ksm] Шумы триггерного сигнала C
};

struct SEvtBek
{
  short id_bek[2]; //[0]-Индикатор запроса данных события данного БЭК, [1]- КСМ(0) или СКТ(1)
  short maskaksm; //Маска используемых KSM
  short maskahit[4]; //[ksm] маска сработавших ФЭУ
  unsigned short acp[4][6][2]; //[ksm][pmt][12d,9d] Коды АЦП
  unsigned short fifoA[4];//[ksm]
  unsigned short fifoB[4];
  unsigned short fifoC[4];
  unsigned short maskatrig[4]; //[ksm] маска битов триггеров, в которых КСМ участвовал, бит:0-A,1-B,2-C
};
#pragma pack(pop)

#pragma pack(push,1)
//Данные мониторинга одного БЭПа
struct SMonADC_SCT
{
   unsigned short ToSave; //Флаг наличия новых данных, 1 - надо их сохранить.
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned char maskapmt[8];  //[ksm] маска ФЭУ измерений
   unsigned short nzap; //Кoличество запусков измерения
   unsigned short nsum[8][5]; //[ksm][pmt] кол-во измерений в спектре Пьедесталов
   float sred[8][5]; //[ksm][pmt] Пьедесталы
   float sigma[8][5]; //[ksm][pmt] сигма пьедесталов
};
struct SMonADC_SCT_S //Для контроля
{
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned char maskapmt[8];  //[ksm] маска ФЭУ измерений
   unsigned short nzap; //Кoличество запусков измерения
   unsigned short nsum[8][5]; //[ksm][pmt] кол-во измерений в спектре Пьедесталов
   short sred[8][5]; //[ksm][pmt] Пьедесталы  *10
   short sigma[8][5]; //[ksm][pmt] сигма пьедесталов *10
};

struct SMonShumTV_SCT
{
   unsigned short ToSave; //Флаг наличия новых данных, 1 - надо их сохранить.
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned char maskapmt[8];  //[ksm] маска ФЭУ измерений
   float noisePMT[8][5]; //[ksm][pmt] Шумы ФЭУ в кГц
   float tbek[4];//Температура в БЭК: Tout, T3,T2,T1
   float vbek[5]; //Напряжения  в БЭК: V1,V2,Vcc,V3,V4
};
struct SMonShumTV_SCT_S //Для контроля
{
   TDateTimeKadr dt; //Дата и Время измерения (UTC)
   unsigned char maskapmt[8];  //[ksm] маска ФЭУ измерений
   unsigned short noisePMT[8][5]; //[ksm][pmt] Шумы ФЭУ в Гц
   unsigned short tbek[4];//Температура в БЭК: Tout, T3,T2,T1 *10
   short vbek[5]; //Напряжения  в БЭК: V1,V2,Vcc,V3,V4 *100
};

struct SMonBep
{
   unsigned short ToSave; //Флаг наличия новых данных, 1 - надо их сохранить.
   TDateTimeKadr dt; //Время измерения шумов триггеров(UTC)
   unsigned short maskaTrSCT[8]; //[ksm] Маска ФЭУ измерения шумов триггера СКТ
   unsigned short noiseTrSCT[8][5]; //[ksm][pmt] Шумы триггерного сигнала СКТ
};
struct SMonBep_S //Для контроля
{
   TDateTimeKadr dt; //Время измерения шумов триггеров(UTC)
   unsigned short noiseTrSCT[8][5]; //[ksm][pmt] Шумы триггерного сигнала СКТ
};

struct SEvtBep
{
  short id_bek[2]; //[0]-Индикатор запроса данных события данного БЭП, [1]- КСМ(0) или СКТ(1)
  short maskaksm; //Маска используемых KSM
  unsigned char maskahit[8]; //[ksm] маска сработавших ФЭУ
  unsigned short acp[8][5]; //[ksm][pmt] Коды АЦП
  unsigned short fifoSCT[8][5];//[ksm][pmt]
  unsigned char maskatrig[8]; //[ksm] битовая маска, участвовавших в триггере СКТ счётчиков
};
#pragma pack(pop)

//Структура для записи события на диск
#define HDR_W 0 //Для записи конфигурации и мониторинга ДЕКОР
  #define ID_CONFIG 0  //Конфигурация ДЕКОР
  #define ID_MONIT 1   //Мониторинг ДЕКОР
  #define ID_EVENT 2   // Не используется
  #define ID_NOISE 3   // Шумы ДЕКОР
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
   char start[5]; // слово "start" - метка начала записи.
   unsigned char tip_zap; //тип записи:
                          // =0,1 - конфигурация НЕВОДа.
                          // =2,3,4,5,6 - данные мониторирования НЕВОДа.
                          // =7 - данные события.
   TDateTimeKadr datetime; //Дата и время (UTC)
   unsigned long lendata; //Длина следующих за зоголовком данных в байтах
                          // Или для ДЕКОР при tip_zap==0 идентификатор ID_CONFIG...ID_NOISE
};
struct SCONFIG_DAT  //Тип записи 1
{
   SCFGBEK CfgBek[32]; //Конфигурация БЭК в НЕВОДе
   CONFIG_TRG CfgTrig[8]; //Конфигурация триггерных плат
   SCFGBEP CfgBep[2]; //Конфигурация БЭП в НЕВОДе
};
struct SCONFIG_DAT_S  //Для контрля
{
   SCFGBEK_S CfgBek[25]; //Конфигурация БЭК в НЕВОДе
   SCFGBEP_S CfgBep[2]; //Конфигурация БЭП в НЕВОДе
};

struct SMONIT_DAT
{
   unsigned long MaskaBek; // битовая Маска присутствующих в данных БЭК или БЭП
   short NBek; //Количество присутствующих в данных БЭК или БЭП
   //Для данных мониторирования это может быть MonPds,MonPdsL,MonAmpL, MonShumTV или MonBek
   union
   {
     SMonADC MonPds[32]; //2 Результаты мониторинга пьедесталов БЭК
     SMonADC MonPdsL[32]; //3 Результаты мониторинга пьедесталов БЭК в режиме нулевой подсветки
     SMonADC MonAmpL[32]; //4 Результаты мониторинга подсветки БЭК
     SMonShumTV MonShumTV[32]; //5 Результаты мониторинга шумов ФЭУ, температуры и напряжений БЭК
     SMonBek MonBek[32]; //6 Результаты мониторинга триггеров БЭКов
     SMonADC_SCT MonPds_SCT[2];//9 Результаты мониторинга пьедесталов БЭП
     SMonShumTV_SCT MonShumTV_SCT[2];//10 Результаты мониторинга шумов ФЭУ, температуры и напряжений БЭП
     SMonBep MonBep[2]; //8 Результаты мониторинга триггеров БЭПов
   }data;
};
struct SEVENTBEK_DAT
{
   unsigned long exp_count; //- 4 байта : номер события.
   unsigned long Nrun; //- 4 байта : номер рана.
   unsigned short trgnvd;   // - 2 байта: Триггеры НЕВОДа
   short Nlam ;             //- 2 байта : кол-во сработавших модулей.
   short NlamSKT ;             //- 2 байта : кол-во сработавших модулей СКТ.
   short NfifoA ;             //- 2 байта : кол-во модулей с FIFO TrA.
   short NfifoB ;             //- 2 байта : кол-во модулей с FIFO TrB.
   short NfifoC ;             //- 2 байта : кол-во модулей с FIFO TrC.
   short NfifoSKT ;             //- 2 байта : кол-во модулей с FIFO TrSKT.
   unsigned long Weit_time; //- 4 байта : время ожидания этого события в 100 нсек.
   unsigned long All_time[2]; //- [0]=10000000; 100 нсек генератор тиков
                              //  [1] - суммарное живое время от запуска программы в сек
   unsigned long Pressure; //Давление
   unsigned long Temperature; //Температура
   unsigned long id_DECOR; //Признак наличия в данных ДЕКОРа (резерв)
   unsigned short StatusReg[8][2]; //Содержимое статусных регистров без купюр
   unsigned long MaskaBek; // битовая Маска присутствующих в данных БЭК
   unsigned long MaskaBep; // битовая Маска присутствующих в данных БЭП (только два первых бита)
   short NBek; //Количество присутствующих в данных БЭК (до 30 без БЭП)
   short NBep; //Количество присутствующих в данных БЭП (до 2)
   SEvtBek EventBek[32]; //Комбинированные данные одного события от БЭК
   SEvtBep EventBep[2]; //Комбинированные данные одного события от БЭП
};
#pragma pack(pop)
struct SStatTrig
{
   unsigned long nsumtriger[9]; // кол-во событий с установленным битом тригера
     			        // [0] - тригер не записался
   unsigned long nspeedtriger[9]; // кол-во тригеров в набираемом интервале
    			          //  живого времени
   double speedtriger[9]; // частота тригеров в единицу живого времени
   double Nlam[9];   //Сумма кол-во сработавших КСМ
   double NlamSKT[9];   //Сумма кол-во сработавших SKT
   double NfifoA[9]; //Сумма кол-во КСМ FIFO TrA
   double NfifoB[9]; //Сумма кол-во КСМ FIFO TrB
   double NfifoC[9]; //Сумма кол-во КСМ FIFO TrC
   double NfifoSKT[9]; //Сумма кол-во КСМ FIFO TrSKT
   double NlamSred[9];   //Среднее кол-во сработавших КСМ
   double NlamSKTSred[9];   //Среднее кол-во сработавших КСМ СКТ
   double NfifoASred[9]; //Среднее кол-во КСМ FIFO TrA
   double NfifoBSred[9]; //Среднее кол-во КСМ FIFO TrB
   double NfifoCSred[9]; //Среднее кол-во КСМ FIFO TrC
   double NfifoSKTSred[9]; //Среднее кол-во КСМ FIFO TrSKT
};
//Структура для отображения текущей стат информации по экспозиции
struct SStatExp
{
   TDateTimeKadr dt; //Текущее время (UTC)
   TDateTimeKadr dtStart; //Время запуска рана (UTC)
   unsigned long Nrun; //Текущий номер рана
   unsigned long exp_count; //Текущий номер события
   unsigned short trgnvd; //Текущий триггер
   unsigned short trgskt; //Текущий триггер СКТ
   long num_file; //Порядковый номер файла в ране
   long lenTofile; //Количество записанных в файле байтов
   double LiveTime; //Полное живое время рана
   double FullTime; //Полное календарное время рана
   unsigned short NhitSCT[2]; //[ibep] Количество сработавших ФЭУ СКТ
   double SumAmpSCT[2]; //[ibep] Суммарные амплитуды СКТ за вычетом пьедестала
   double timforspeed; // счетчик секунд для интервала живого времени
   SStatTrig StatTrig[8]; //Статистика триггеров по блокам регистров заданных в CfgTrig
};
extern SStatExp StatExp; //Статистика экспозиции
extern SStatExp StatExpShow; //Статистика экспозиции для отображения

extern SNoiseTrigNvd NoiseTrigNvd;
extern SCONFIG_DAT Config_Dat;
extern SMonADC MonPds[32]; //Результаты мониторинга пьедесталов БЭК
extern SMonADC_SCT MonPds_SCT[2]; //Результаты мониторинга пьедесталов БЭП
extern SMonADC MonPdsL[32]; //Результаты мониторинга пьедесталов БЭК в режиме нулевой подсветки
extern SMonADC MonAmpL[32]; //Результаты мониторинга подсветки БЭК
extern SMonShumTV MonShumTV[32]; //Результаты мониторинга шумов ФЭУ, температуры и напряжений БЭК
extern SMonShumTV_SCT MonShumTV_SCT[2]; //Результаты мониторинга шумов ФЭУ, температуры и напряжений БЭП

extern SMonBek MonBek[32]; //Результаты мониторинга БЭКов
extern SMonBep MonBep[2]; //Результаты мониторинга БЭПов
extern unsigned long MaskaEventBek; // битовая Маска опрошенных в событии БЭК
extern short NEventBek; //Количество опрошенных в событии БЭК
extern SEvtBek EventBek[32]; //Комбинированные данные одного события от БЭК
//extern SEvtBek EventBekShow[32]; //Данные одного события для отображения

extern unsigned long MaskaEventBep; // битовая Маска опрошенных в событии БЭП (два бита)
extern short NEventBep; //Количество опрошенных в событии БЭП
extern SEvtBep EventBep[2]; //Комбинированные данные одного события от БЭП
//extern SEvtBep EventBepShow[2]; //Данные одного события для отображения

extern HEADER_REC header_rec; //Заголовок записи
extern SMONIT_DAT Monit_Dat;  //данные мониторирования для записи
extern SEVENTBEK_DAT Event_Dat; //Данные события
//extern SEVENTBEK_DAT Event_DatShow; //Данные события для отображения
#endif