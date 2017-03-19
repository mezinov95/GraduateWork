#ifndef ConfigH
#define ConfigH

#include <vector>
#include <sstream>
#include <fstream.h>
#include <stdio.h>
#include <string.h>

#define MAXBEP 2 //Максимальное количество БЭПов
#define MAXBEK 32 //Максимальное количество БЭКов

#pragma pack(push, 1)
//Коды для поджигателя светодиодов КСМ
struct SLEDKSM
{
  unsigned char base; //Базовое напряжение
  unsigned char diapazon; //Диапазон
  unsigned char pmt[6]; //Коды подстветок
};
//Структура конфигурации одного модуля (для чтения конфигурационного файла)
struct SCFGKSM
{
  char enable; //0- неиспользуется, 1- используется
  char MaskaPMT; //Битовая маска используемых ФЭУ
  unsigned short TrigDelay; //Задержка сигнала "Хранение" в нсек
  unsigned short Porog;//Порог дискриминаторов 0..255
  unsigned char Trword; //Слово в маске триггеров (всего 16 слов - 128 бит)
  unsigned char Trbit; //бит в слове в маске триггеров (всего 16 бит)
  unsigned char TrA; //участие в триггере A
  unsigned char TrB; //участие в триггере B
  unsigned char TrC; //участие в триггере C
  unsigned short x,y,z; // Координаты положения в системе НЕВОД, в мм
  SLEDKSM Led; //Коды внутримодульных подсветок
};
//Структура конфигурации одного модуля (для контроля)
struct SCFGKSM_S
{
  char enable; //0- неиспользуется, 1- используется
  char MaskaPMT; //Битовая маска используемых ФЭУ
  unsigned short x,y,z; // Координаты положения в системе НЕВОД, в мм
};

//Структура конфигурации БЕК из файла bek.cfg
struct SCFGBEK
{
  char enable; //0- неиспользуется, 1- используется
  char idBEK; // идентификатор: 0-КСМ или 1-СКТ
  char NumberBEK; //Номер БЭК
  char MaskaKSM; //Битовая маска используемых КСМ по данным CfgKSM
  SCFGKSM CfgKSM[4]; //Конфигурация и параметры КСМ, подключённых к БЭК
};
//Структура конфигурации БЕК (для контроля)
struct SCFGBEK_S
{
  char enable; //0- неиспользуется, 1- используется
  char idBEK; // идентификатор: 0-КСМ или 1-СКТ
  char NumberBEK; //Номер БЭК
  char MaskaKSM; //Битовая маска используемых КСМ по данным CfgKSM
  SCFGKSM_S CfgKSM[4]; //Конфигурация и параметры КСМ, подключённых к БЭК
};

//Структура конфигурации одного модуля БЭП (для чтения конфигурационного файла)
struct SCFGKSM_SCT
{
  char enable; //0- неиспользуется, 1- используется
  char MaskaPMT; //Битовая маска используемых ФЭУ
  unsigned short TrigDelay; //Задержка сигнала "Хранение" в нсек
  unsigned short Porog;//Порог дискриминаторов 0..255
  unsigned char ipl[5]; //Номер плоскости счётчика (4-12)
  unsigned char istr[5]; //Номер струны счётчика (1-5)
  unsigned char idownup[5]; //0-нижний, 1-верхний
  unsigned char Trword[5]; //[pmt] Слово в маске триггеров (всего 16 слов - 128 бит)
  unsigned char Trbit[5]; // [pmt] бит в слове в маске триггеров (всего 16 бит)
  unsigned char TrSCT[5]; //[pmt] участие в триггере SCT
  unsigned short x[5],y[5],z[5]; //[pmt] Координаты положения в системе НЕВОД, в мм
};
//Структура конфигурации одного модуля БЭП (для контроля)
struct SCFGKSM_SCT_S
{
  char enable; //0- неиспользуется, 1- используется
  char MaskaPMT; //Битовая маска используемых ФЭУ
  unsigned char ipl[5]; //Номер плоскости счётчика (4-12)
  unsigned char istr[5]; //Номер струны счётчика (1-5)
  unsigned char idownup[5]; //0-нижний, 1-верхний
  unsigned short x[5],y[5],z[5]; //[pmt] Координаты положения в системе НЕВОД, в мм
};

//Структура конфигурации БЕП из файла bep.cfg
struct SCFGBEP
{
  char enable; //0- неиспользуется, 1- используется
  char idBEK; // идентификатор: 0-КСМ или 1-СКТ
  char NumberBEK; //Номер БЭП
  unsigned char MaskaKSM; //Битовая маска используемых контроллеров по данным CfgKSM
  SCFGKSM_SCT CfgKSM[8]; //Конфигурация и параметры контроллеров, подключённых к БЭП
};
//Структура конфигурации БЕП (для контроля)
struct SCFGBEP_S
{
  char enable; //0- неиспользуется, 1- используется
  char idBEK; // идентификатор: 0-КСМ или 1-СКТ
  char NumberBEK; //Номер БЭП
  unsigned char MaskaKSM; //Битовая маска используемых контроллеров по данным CfgKSM
  SCFGKSM_SCT_S CfgKSM[8]; //Конфигурация и параметры контроллеров, подключённых к БЭП
};

//Временная рабочая структура конфигурации счётчика СКТ из файла sct.cfg
struct SCFGSCT
{
  char enable; //0- неиспользуется, 1- используется
  //Позицонные номера
  char ipl; //Номер плоскости (4..12)
  char istr; //номер струны (1..5)
  char idownup; // 0- нижняя плоскость, 1-верхняя плоскость
  unsigned short x,y,z; // Координаты положения (центр счётчика) в системе НЕВОД, в мм
  //Куда подключён
  char NumberBEK; //Номер БЭП  (1..32)
  char icntr; //номер контроллера внутри БЭП (0..7)
  char ipmt; //Номер канала АЦП (0..4)
  unsigned char Trword; //Слово в блоке триггеров SCT (всего 16 слов - 128 бит)
  unsigned char Trbit; //бит в слове в блоке триггеров SCT (всего 16 бит)
};
//Маски для входов триггерных блоков
struct STrigMaska
{
  unsigned short MaskaA[8]; //Триггер А
  unsigned short MaskaB[8]; //Триггер B
  unsigned short MaskaC[8]; //Триггер C
  unsigned short MaskaSKT[8]; //Триггер СКТ
};
#pragma pack(pop)
struct TCRITERYNVD
{ //Low2,Low1,Height1,Height2
  double BEKt[4]; //Температура БЭКа
  double BEKV1[4]; // Напряжение V1
  double BEKV2[4]; // Напряжение V2
  double BEKV3[4]; // Напряжение V3
  double BEKVcc[4]; // Напряжение Vcc
  double TrA[4]; // Триггер A КСМ
  double TrB[4]; // Триггер B КСМ
  double TrC[4]; // Триггер C КСМ
  double PMTnoise[4]; // Шумы ФЭУ
  double PMTpds12sred[4]; // пьедестал ФЭУ 12 динод среднее
  double PMTpds12sig[4]; // пьедестал ФЭУ 12 динод сигма
  double PMTpds9sred[4]; // пьедестал ФЭУ 9 динод среднее
  double PMTpds9sig[4]; // пьедестал ФЭУ 9 динод сигма
  double PMTled12sred[4]; // подсветка ФЭУ 12 динод среднее
  double PMTled12sig[4]; // подсветка ФЭУ 12 динод сигма
  double PMTled9sred[4]; // подсветка ФЭУ 9 динод среднее
  double PMTled9sig[4]; // подсветка ФЭУ 9 динод сигма
};
struct TCRITERYSCT
{ //Low2,Low1,Height1,Height2
  double BEKt[4]; //Температура БЭКа
  double BEKV1[4]; // Напряжение V1
  double BEKV2[4]; // Напряжение V2
  double BEKV3[4]; // Напряжение V3
  double BEKVcc[4]; // Напряжение Vcc
  double TrSCT[4]; // Триггеры SCT
  double PMTnoise[4]; // Шумы ФЭУ
  double PMTpds12sred[4]; // пьедестал ФЭУ 12 динод среднее
  double PMTpds12sig[4]; // пьедестал ФЭУ 12 динод сигма
};
//Конфигурация триггеров
struct TTRIGGERCNF
{
  unsigned long porsum; //Пороговая сумма
  unsigned long gatew;  //Ширина ворот
  unsigned long timernoise; //Таймер для измерения шумов (1 сек = 10 000 000)
  unsigned long maskaout; //маска внешних триггеров
  unsigned long masks[8];// маски
};
//Параметры режима экспозиции
struct TEXPZPARAM
{
  unsigned long tiMonShort; //Интервал времени между измерением пьедесталов в сек
  unsigned long tiMonSred; //Интервал времени между мониторингами в сек
  unsigned long tiMonFull; //Интервал времени между измерениями шумов в сек
  unsigned long tiShow; //Интервал времени для вывода на экран в сек
  unsigned long tiDCRans; //Интервал времени ожидания ответа от ДЕКОРа в сек
  unsigned long tiDCRmon; //Интервал времени ожидания мониторинга от ДЕКОРа в сек
  unsigned long tiDCRnoise; //Интервал времени ожидания шумов от ДЕКОРа в сек
};

extern TEXPZPARAM ExpzParam; //Параметры режима экспозиции из файла Expoz.cfg
extern TTRIGGERCNF TrigCnf; //Конфигурация триггеров из файла Trigger.cfg
extern unsigned long CfgMaskaBek; //Маска необходимых (по конфигурации) в эксперименте БЭК
extern unsigned long CfgMaskaBep; //Маска необходимых (по конфигурации) в эксперименте БЭП
extern unsigned long CfgMaskaBekp; //Маска необходимых (по конфигурации) в эксперименте БЭК и БЭП
extern SCFGBEK CfgBek[32]; //Конфигурация БЭК в НЕВОДе
extern SCFGBEP CfgBep[2]; //Конфигурация БЭП в НЕВОДе
extern SCFGSCT CfgSct[80]; //Конфигурация счётчиков СКТ
extern int BEK2BEP[32]; //Индекс БЭПа в таблице CfgBep по его номеру БЭКа
extern TCRITERYNVD CritNVD; //Критерии работоспособности НЕВОД
extern TCRITERYSCT CritSCT; //Критерии работоспособности СКТ
extern STrigMaska TrigMaska; //Маски для входов триггерных блоков

//Загрузка конфигурации
extern int LoadConfig();
//Загрузка критериев НЕВОД
extern int LoadCritNVD();

#endif