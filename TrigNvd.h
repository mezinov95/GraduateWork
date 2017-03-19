#ifndef TrgNvdH
#define TrgNvdH

struct TRGEXPOZITION { // экспозиция, чтение
	unsigned short STATUS_L; // статусный регистр
	unsigned short STATUS_H;

	unsigned short TIME_L;	// регистр живого времени и уст таймера чтение
	unsigned short TIME_H;

	unsigned short AREG_L;   // регистры входных сигналов
	unsigned short AREG_H;
	unsigned short BREG_L;
	unsigned short BREG_H;
	unsigned short DREG_L;
	unsigned short DREG_H;
	unsigned short EREG_L;
	unsigned short EREG_H;


	unsigned short S_TM_L;	//время, секунды
	unsigned short S_TM_H;
	unsigned short NS_TM_L;  // время, наносекунды
	unsigned short NS_TM_H;
	unsigned short NS_COR_L; // коррекция, наносекунды
	unsigned short NS_COR_H;
	unsigned short S_SET_L;  // установка времени, секунды
	unsigned short S_SET_H;


	unsigned short A_RS_FIFO[128];  // FIFO состояния входных сигналов

	};

struct TRGNOISE {     // измерение уровня шума
	unsigned short SC_FIFO[128];  // FIFO счётчиков чтение

	unsigned short TIME_L;	// регистр живого времени запись
	unsigned short TIME_H;

	unsigned short CONTROL_L;  // контрольный регистр чтение/запись
	unsigned short CONTROL_H;
};

struct TRGPORTS {
	unsigned short GCTRL;     // регистры управления портами запись

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

	unsigned short DIDCODE; // регистры кодов индентификации плат расширения чтение
	unsigned short EIDCODE;
	unsigned short FIDCODE;
};

struct TRGTIMERS {   // настройка таймеров
	unsigned short GATEWIDTH;   // установка ширины ворот задержки
	unsigned short MODE;      // режимы системы запись

	unsigned short PDL_CTRL;   // управление задержками, чтение запись
	unsigned short PDL_DATA;
};

struct TRGTESTREG {  // тестовые регистры
	unsigned short CCTRL_L;   // регистр порта С запись
	unsigned short CCTRL_H;

	unsigned short SCRATCH;   // тестовый регистр чтение запись

	unsigned short REVISION;  // идентификационный номер прошивки чтение

	unsigned short TST_FIFO[128];	//тестовое FIFO запись чтение
};

struct TRGMASKS {  // регистры масок, запись
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
   unsigned short index; //Порядковый номер в конфигурации
   unsigned long base; //базовый адрес (если 0, то не используется)
   char name[8]; //имя регистра
   unsigned short idmaster;// 1- мастерный триггер, по нему ловим события
   unsigned short outmask; //Маска внешних триггеров
   unsigned short porsum;  //Пороговая сумма
   unsigned short gatew;   //Ширина ворот // Для СКТ ширина ворот ожидания на отдельных плоскостях
   unsigned long timernoise; //Время измерения шумов в единицах 100 нсек
                             //1 секунда = 10 000 000
   unsigned short idmaska;// 0-всё закрыто, 1- по конфигурации БЭК, 2-всё открыто
   unsigned short mode; //режим работы блока
   unsigned short d1; //задержка 1 в единицах 10 нсек
   unsigned short d2; //задержка 2 в единицах 10 нсек
   unsigned short d3; //задержка 3 в единицах нсек
   unsigned short d4; //задержка 4 в единицах нсек
   unsigned short TrgMaska[8]; //128 бит маска входных сигналов
   unsigned short gateUD; //ширина ворот ожидания сигнала между плоскостями СКТ
   unsigned short porsumU; //Пороговая сумма для верхней плоскостей СКТ
   unsigned short porsumD; //Пороговая сумма для нижней плоскостей СКТ
}CONFIG_TRG;
#pragma pack(pop)

extern CONFIG_TRG CfgTrig[8]; //Конфигурация триггерных плат
extern TRGEXPOZITION ExpTrigFull[8]; //Данные события с триггерных плат

//Завершение работы
extern void ExitTrgNvd();
//Инициализация триггерной системы
extern int InitTrigNvd();
//Проверям наличие события, возвращает триггерную маску события
extern int CheckTrgNvd(TRGEXPOZITION &exptrig);
//Чтение расширенной информации по событию
extern int ReadEventTrgNvd();
//Сброс хранения, перед ожиданием события
extern int ClearTrgNvd();
//Сброс и установка хранения, чтобы не дёргался
extern int ResetTrgNvd();
//Установка хранения, чтобы не дёргался, для перехода в мониторирование
extern int SetTrgNvd();
//Набор счётчиков шумов триггерных сигналов за время timeout
extern int NoiseTrgNvd(int timeout);

#endif