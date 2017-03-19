#ifndef StructDCR_H
#define StructDCR_H

// Структура для работы с ДЕКОРОМ
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

#define MAXDCR 1  //максимальное количество ДЕКОР
#define MAXPM 8  //максимальное количество ПМ
#define NCNTR 4  // кол-во контроллеров в одной ЭВМ
#define NCHAN 4  // кол-во каналов в одном контроллере
#define NGROUP8 76 // максимально возможное кол-во групп по 8 бит
#define NGROUP16 38  // в полной конфигурации 38 групп по 16 бит
#define NGROUP64 10 // максимально возможное кол-во групп по 64 бита

#pragma pack(push,1)
struct CONFIG_FROMPM
{
	char wrk_cntr; // NCNTR-бита: 1-найден контроллер, 0-нет
	char nomer_cntr[NCNTR]; // номера контроллеров из плат
};
// Структура конфигурации одного канала
struct CONFIG_ONE
{
	 short include; // 0- не в работе , 1- в работе
	 short Nx; // кол-во бит в камерах X
	 short Ny; // кол-во бит в камерах Y
	 float Xx,Yx,Zx; // координаты первого бита данных в камерах X
	 float Xy,Yy,Zy; // координаты первого бита данных в камерах Y
	 float VXx,VYx,VZx; // Направляющий вектор в камерах X
	 float VXy,VYy,VZy; // Направляющий вектор в камерах Y
};
// Структура конфигурации всего ДЕКОРа
struct CONFIG_DECOR
{
	  struct CONFIG_ONE cnf[128];
};
// Структура данных из триггерной платы
struct TRGCOMMENT
{
  short tip; //Номер физики
  char comment[66]; //комментарий
};
struct TRGCNF
{
	unsigned short port; // Адрес триггерной платы
	unsigned long interval; //Интервал времени для счёта шумов
	short frequence; //Тактовая частота генератора в МГц
	short stepinterval; //Единица для интервала измерений шумов в нсек
	//Загружаем в плату
	short vorota; //Время совпадения в нсек
	short width1; // длительность1 в нсек
	short delay1; // задержка1 в нсек
	short width2; // длительность2 в нсек
	short delay2; // задержка2 в нсек
	short width3; // длительность3 в нсек
	short delay3; // задержка3 в нсек
	short reserv; // резерв
	//-----------
	struct TRGCOMMENT Trgcomment[8]; //идентификаторы битов триггера
	//Загружаем в плату
	unsigned char tabl[256]; //Таблица загрузки триггерных условий
};
// структура данных полученная при мониторировании от всех PM
struct CMONITORALL
{
		 char id_cmonit; // битовый индикатор наличия данных от ПМ
		 CONFIG_FROMPM cnf[MAXPM];
		 short nbad[MAXPM][NCNTR][NCHAN]; // кол-во стрипов с ошибками
};
// структура данных по шумам от одной PM
struct CNOISE
{
	  char maskacntr; // NCNTR-бита: 0-нет данных контроллера, 1-есть
	  short noisebuf[NCNTR*NCHAN]; // запакованные данные по NCHAN слова
											// на каждый не нулевой бит в maskacntr
};
// структура данных всего события со всех PM, это будет записываться на диск.
struct CEVENTALL
{
// Закоментаренное будет записываться безусловно в подпрограмме записи
		char start[6];  // Ключевое слово начала записи
		short	Tip;	// Тип записи:0-Config,1-монитор,2-Experement event,3-Noise
		unsigned long Nrun; // Номер текущего рана
		unsigned long Nevent; // Номер текущего события
		unsigned char  hund;  // hundredths of seconds
		unsigned char  sec;   // seconds
		unsigned char  min;   // minutes
		unsigned char  hour;  // hours
		char day;     // day of the month
		char mon;     // month (1 = Jan)
		short year;     // current year
		unsigned long mcntr; //32 бита : 0-не опрашивался , 1-опрашивался
		short len; // Длина области данных в байтах

	unsigned short triger; // информация по триггеру
	unsigned long Weit_time; // Время ожидания этого события в единицах step
	unsigned char history[2][16]; // предистория по 1/freq(в обратную сторону)
	unsigned short counter[2][8]; // живое время по 8-ок пл-тей в единицах 8*step
	unsigned char maska_lam_chan[16]; // по NCHAN-бита маска LAMов на все PM
	unsigned long Maska_Len_maska_masok; // 32-бита : 0 maska_masok- char
													 //          1 maska_masok- short
	unsigned long maskacntr; //32 бита : 0-нет данных , 1-есть
	short len_alldan; // Длина запакованных сработавших стрипов
	unsigned char buf[1514*8]; // запакованные данные
};

// Структура данных работы с DECOR_MAIN
struct STR_DECOR {
	long err_oper; // маска ошибок появившихся при выполнении операции
					  //Если err_oper&64 != 0, то ошибка триггерной платы
					  // и дальнейшей информации нет
	short maska_packet_err; //от кого есть "ERROR"
	short maska_packet_bad; // маска запорченных пакетов
	//Рабочая конфигурация
	short pm_config; // требуемая конфигурация сети
	short PM_work; // Битовая маска рабочих машин
	short PM_exp; // битовая маска участвующих в эксперименте PM
	short id_cinit[MAXPM]; // индикатор наличия данных
	short len_cinit[MAXPM]; // длина данных
	struct CONFIG_FROMPM CInit[MAXPM];
	//Номер рана
	long Nrun; // Номер запуска программы
	//Конфигурационные данные
	unsigned long configcntr; // маска контроллеров по конфигурации
	struct CONFIG_DECOR CNF; // Структура конфигурации всего ДЕКОРа
	struct TRGCNF Trgcnf[2]; //Конфигурация для триггерной платы
	//Мониторинг
	unsigned long configmonit; // маска контроллеров по мониторингу
	struct CMONITORALL CMonitAll;
	//Шумы
	unsigned short counter[2][8]; //шумы триггеров за 1 сек
	unsigned long confignoise; // маска контроллеров по шумам
	short id_cnoise[MAXPM]; // индикатор наличия данных
	short len_cnoise[MAXPM]; // длина данных
	struct CNOISE CNoise[MAXPM];
	//Событие
	unsigned long configevent; // маска опрашиваемых контроллеров по событию
	short len_ceventall; // длина данных
	struct CEVENTALL CEventAll; // Структура одного события ДЕКОРа
					  };
#pragma pack(pop)

#pragma pack(push,1)

struct CNF_SET{
    //Для графического отображения события
    short id_gscreen; //индикатор нахождения в графическом режиме
    short id_gscrbuf; //индикатор необходимости заполнения буффера экрана
    short id_gnoise; //индикатор наличия данных по шумам
    unsigned short noise_chan[NCNTR*MAXPM][NCHAN]; // шумы для вывода на экран
    //для распаковки
    short id_SetN; // Признак заполнения N8,N16,N64,Nread;
    short N8[NCNTR*MAXPM]; // кол-во групп по 8 бит
    short N16[NCNTR*MAXPM]; // кол-во групп по 16 бит
    short N64[NCNTR*MAXPM]; // кол-во групп по 64 бита
    short Nread[NCNTR*MAXPM][NCHAN]; // Кол-во чтений слов данных из одного канала
           };

#pragma pack(pop)

extern struct CNF_SET CNF_S;
extern struct STR_DECOR *DcrF;
extern short id_eventDCR;//Признак того, что были получены данные события от ДЕКОР

#endif
