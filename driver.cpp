//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "driver.h"
#include "Unit1.h"
#include <math.h>
//#include "cpp\src\fasttransforms.h"
//#include "cpp\src\alglibinternal.h"
//#include "cpp\src\ap.h"
//#include "cpp\src\fasttransforms.cpp"
//#include "cpp\src\alglibinternal.cpp"
//#include "cpp\src\ap.cpp"

#define DATA_SIZE 0x10000
//---------------------------------------------------------------------------
#pragma package(smart_init)
/*######################################################################*/
bool             StopDAQ = true;      // флаг, по нему работает цикл
HANDLE           hIRQEvent;           // указатель на событие
HANDLE           hDriverThread = 0;   // указатель на поток
const int        DriverThreadStackSize = 4096;// размер стека потока
DWORD WINAPI     DriverThread (LPVOID );// функция потока
short            DrvData[DATA_SIZE]; // массив данных
double x,y;                // тут хранятся усредненные значения каналов.
double MZR=0.30517e-3;     // младший значащий разряд
// Зависимость сопротивления от магнитного поля:




void Stop()            // остановка измерения
{
    StopDAQ = true; // сбрасываем флаг
    if ( hIRQEvent )
        CloseHandle (hIRQEvent); // закрываем поток
    hIRQEvent = hDriverThread = 0;
}

void Start()
{
    DWORD ThreadId;
    StopDAQ = false;
    // Создание Event'а для реакции на прерывание
    hIRQEvent = CreateEvent (NULL,FALSE,FALSE,NULL);
    // Создаём второй поток
    hDriverThread = CreateThread (NULL,
                                 DriverThreadStackSize,
                                 DriverThread,
                                 NULL,
                                 0,
                                 &ThreadId);
    // установка приоритета потока
    SetThreadPriority(hDriverThread, THREAD_PRIORITY_NORMAL);
}
//---------- Функция потока --------------------------------------------------

/*
Работает это так:
1. После запуска, определяется режим работы
2. Определяется место сохранения данных
3. Точки сохраняются с заданным шагом
4. Каждая точка измеряется 361 раз, и усредняется
5. Вывод данных на график
6. Обработка шума, пока происходит над уже усредненными значениями
7. При достижении


 */


DWORD WINAPI DriverThread (LPVOID )
{
    double prevB=-10;
    short Chan1d[512]={0}; // данные первого(нулевого) канала
    short Chan2d[512]={0}; // данные второго(первого) канала
    float m_fFreq=0;       // частота возвращаемая АЦП
    //-------------------------------------------------------------------------
    // Установка связи с драйвером и настройка устройства
    DllClient LAxClient;
    // Загружаем драйвер
    IFactory *LAxFactory = (IFactory*)LAxClient.LoadRegistered(
        CompanyNameDRV, ProductNameDRV, VersionNameDRV,
        dev[DriverIndex], "StaticFactory");
    if(LAxFactory == 0) return -11;
    // Получаем интерфейс драйвера
    IADCDevice* pADC = (IADCDevice*)LAxFactory->Create(_T("IADCDevice"), 0);
    if(pADC == 0)       return -12;
    // Установка параметров устройства: базового адреса, DRQ, IRQ
    pADC->Setup(ADCBase, ADCDRQ, ADCIRQ, hIRQEvent);
    int z;
    // устанавливаем дифференциальный режим работы
    //--------------------------------------------------------------------------
    pADC->Get(ADC_SET_ADCMODE_DIFF,&z );
    //--------------------------------------------------------------------------

    // Инициализируем структуру параметров измерения
    ADCParametersDMA a;
    a.m_nStartOf =       ADCPARAM_START_TIMER; // Запуск АЦП от таймера
    a.m_nIntOf =         ADCPARAM_INT_TC;      // Прерывания по сигналу TC
    a.m_nDMABlockSize =  BlockSize;              // Размер блока собираемых данных
    a.m_nDMAMode =       0;                    // Режим DMA передачи (0 - Single, 1 - AutoInit)
    a.m_fFreqStart =     Frequency;               // Частота дискретизации, Гц
    a.m_nTimerStart =    0;                    // Номер канала таймера для запусков АЦП
    a.m_fFreqPack =      0;                    // Частота кадров (режим отключен)
    a.m_nTimerPack =     0;                    // Номер канала таймера для формирования кадров
    a.m_nFirstChannel =  Form1->Chan1->ItemIndex;// Первый канал в режиме автосканирования
    a.m_nChannelNumber = NumberOfChannels;     // Сколько всего каналов сканировать
    a.m_nGain =          1;                    // Коэффициент усиления
    // Инициализация параметров измерения
    pADC->Init(ADC_INIT_MODE_INIT, &a, &m_fFreq);
    while (!StopDAQ)
    {
        // Запуск преобразования
        Sleep(500);
        // сделать таймер сюда, дабы время процессора не есть!
        pADC->Start();
        // Ожидание прерывания

        //-------------------Отладка-----------------------------------------
       /* for(int b=0;b<100;b++)
        {
        x=rand()%2;
        y=rand()%6;
       switch(Form1->PC->ActivePageIndex)
            {
            case 0:
            // случай отладки, когда активна вкладка "FirstTab"
            Form1->Series1->AddY(x,"",clRed);
            Form1->Series2->AddY(y,"",clGreen);
            Form1->Series3->AddXY(y,x,"",clYellow);
            Form1->Memo1->Lines->Add(Form1->Series1->XValues->Last());
            break;
            case 1:
            // измерение магнитосопротивления
            if(!Form1->ResCurveIndex->ItemIndex)
            Form1->SeriesRes1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesRes2->AddXY(y,x,"",clBlue);
            break;
            case 2:
            // Эффект Холла
            if(!Form1->HallCurveIndex->ItemIndex)
            Form1->SeriesHall1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesHall2->AddXY(y,x,"",clBlue);
            ;
            break;
            case 3:
            // Фотопроводимость в геометрии Фарадея
            if(!Form1->FaradeyCurveIndex->ItemIndex)
            Form1->SeriesFaradey1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFaradey2->AddXY(y,x,"",clBlue);
            break;

            case 4:
            // Фотопроводимость в геометрии Фойгта
            if(!Form1->FoygtCurveIndex->ItemIndex)
            Form1->SeriesFoygt1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFoygt2->AddXY(y,x,"",clBlue);
            default:
            ShowMessage("Ошибка! возможно вызвана неполным обновлением программы!");
            }

          }  */

        //---------------------------Конец отладки----------------------------
        //---------- Получение данных ----------------------------------------
        if ( WaitForSingleObject(hIRQEvent,500) == WAIT_OBJECT_0 )
        // надо бы проверить чистится ли очередь прерываний   // не очищается)
        {
            // Получаем результаты измерения из буфера DMA
            pADC->GetData(ADC_DATA_MODE_DATAFROMDMA | ADC_DATA_MODE_CONVERT2INT16,
             (char *)DrvData, BlockSize, 0);

            // распределение данных по каналам
            for(int i=0,b=0;(i+2)<1024;i+=NumberOfChannels,b++)
            {
            
            /* Последовательность каналов такая: 2 1 0 */
            // канал номер 2 игнорируется, ибо измеряется для стабильности
            //------------------------------------------------------------------
            Chan1d[b]=DrvData[i+2];
            Chan2d[b]=DrvData[i+1];
            //------------------------------------------------------------------

            }
            // сортировка
            heapSort(Chan1d,BlockSize/3);
            heapSort(Chan2d,BlockSize/3);
            // нахождение среднего значения
            x=Middle(Chan1d,BlockSize/3)*MZR;
            y=Middle(Chan2d,BlockSize/3)*MZR;

            //вывод на график
            // в зависимости от того что измеряем
            //Form1->Memo1->Lines->Add(y-prevB);
            //Form1->Memo1->Lines->Add(prevB);
            if(fabs(y-prevB)>=h)   // отклонение на шаг, записываем эту точку
            {
            prevB=y;
            y*=10;     // переводим вольты в Тл
            //if(prevB-2<=0.01) // если достигли 2 Тл, заканчиваем измерения
            //StopDAQ=true;
            switch(Form1->PC->ActivePageIndex)
            {
            case 0:
            // случай отладки, когда активна вкладка "FirstTab"
            Form1->Series1->AddY(x,"",clRed);
            Form1->Series2->AddY(y,"",clGreen);
            Form1->Series3->AddXY(y,x,"",clYellow);
            Form1->Memo1->Lines->Add(Form1->Series1->XValues->Last());
            break;
            case 1:
            // измерение магнитосопротивления
            if(!Form1->ResCurveIndex->ItemIndex)
            Form1->SeriesRes1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesRes2->AddXY(y,x,"",clBlue);
            break;
            case 2:
            // Эффект Холла
            if(!Form1->HallCurveIndex->ItemIndex)
            Form1->SeriesHall1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesHall2->AddXY(y,x,"",clBlue);
            ;
            break;
            case 3:
            // Фотопроводимость в геометрии Фарадея
            if(!Form1->FaradeyCurveIndex->ItemIndex)
            Form1->SeriesFaradey1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFaradey2->AddXY(y,x,"",clBlue);
            break;

            case 4:
            // Фотопроводимость в геометрии Фойгта
            if(!Form1->FoygtCurveIndex->ItemIndex)
            Form1->SeriesFoygt1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFoygt2->AddXY(y,x,"",clBlue);
            default:
            ShowMessage("Ошибка! возможно вызвана неполным обновлением программы!");
            }
            }


            // Сброс прерываний
            // ну он хоть и работает но ожидание в полсекунды не выдерживается
            pADC->ClearInterrupt();
        }

        // Останов преобразования
        pADC->Stop();
    }

    // Освобождение драйвера
    prevB=-10;
    pADC->Release();

    return 1;
}



//----- Функции Пирамидальной сортировки ------------------------------------
// лучше использовать её вместо быстрой сортировки, т.к. множество рекурсивных
// вызовов будут сильно грузить систему...
template<typename T>
void downHeap(T a[], long k, long n) {
        //  процедура просеивания следующего элемента
        //  До процедуры: a[k+1]...a[n]  - пирамида
        //  После:  a[k]...a[n]  - пирамида
        T new_elem;
        long child;
        new_elem = a[k];

        while(k <= n/2) {               // пока у a[k] есть дети
                child = 2*k;
                //  выбираем большего сына 
                if( child < n && a[child] < a[child+1] ) 
                        child++;
                if( new_elem >= a[child] ) break; 
                // иначе 
                a[k] = a[child];        // переносим сына наверх 
                k = child;
        }
        a[k] = new_elem;
}

template<typename T>
void heapSort(T a[], long size) {
        long i;
        T temp;

        // строим пирамиду
        for(i=size/2-1; i >= 0; i--) downHeap(a, i, size-1);

        // теперь a[0]...a[size-1] пирамида

        for(i=size-1; i > 0; i--) {
                // меняем первый с последним
                temp=a[i]; a[i]=a[0]; a[0]=temp;
                // восстанавливаем пирамидальность a[0]...a[i-1]
                downHeap(a, 0, i-1);
        }
}
// Нахождение среднего значения сортированного массива, среднее значение
// находится для половины элементов массива.
// передается указатель на массив и его размер
template<typename T>
double Middle(T a[], long size)
{
int i;
double Sum=0;
for(i=size/4;i<size-size/4;i++) 
    Sum+=a[i];
return Sum/(size%2==0?(size/2+1):(size/2+2));
} 
// быстрое преобразование Фурье
// использует библиотеку Alglab
