//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "driver.h"
#include <math.h>
//#include "cpp\src\fasttransforms.cpp"
//#include "cpp\src\alglibinternal.cpp"
//#include "cpp\src\ap.cpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;




// Внимание, понадобится добавить что-нибудь,
// не забудь внести это в заголовочный файл:)

TCHAR            dev[10][20];
int              ADCBase = 0x310;
int              ADCDRQ = 7;
int              ADCIRQ = 12;
int              DriverIndex = 0;
bool             bS = false;
const int       *BaseList;
const int       *DRQList;
const int       *IRQList;
int NumberOfChannels =3;
int BlockSize=1024;
int Frequency=200000;
double h=0.001;//0.02;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/* TODO
сохранение сменить флаг!   +
экстраполяция
ручная регулировка шага
уменьшить задержку  + теперь 500мс
кнопки очистки и фильтра на все вкладки +
предусмотреть "единичное измерение"
возможность записи "поверх" - т.е. удалять предыдущие значения и писать поверх новые
фукнция удаления определенного интервала точек

 экстраполяция фукнции
 увеличение количества точек








1. Сохранение настроек
2. работа с графиками
3. +разобраться с приёмом данных
 +функция отлажена и работает   
 +порядок каналов обратный
 


4. сохранение результатов в файл  +
нужно сохранять отдельные точки, возможность выбора оставить за пользователем
+также возможно сохранения с заданным шагом.
5. +чтение из файла
+чтение нужно сделать с выводом на произвольный график
6. нормальные комментарии


и хорошо бы найти способ объявления шаблонной функции, при котором она известна
из других файлов:))

*/

void __fastcall TForm1::LoadLa7(void)
{
;
}


void __fastcall TForm1::FormCreate(TObject *Sender)
{

// Просматриваем все доступные драйверы и помещаем в список только те,
    // что поддерживают передачу данных по DMA
    DllClient LAxClient1;
    IFactory* LAxFactory1;
    IADCDevice* pADC1;
    TCHAR   dev1[10][20];
    int var_caps1=0;
    int n;
    int z = 0;
    for(n=0; n<10; ++n, var_caps1=0)
    {
        if(!DllClient::GetRegName(dev1[n], 20-1, n,
                                  CompanyNameDRV,
                                  ProductNameDRV,
                                  VersionNameDRV)) break;
        // загружаем драйвер
        LAxFactory1 = (IFactory*)LAxClient1.LoadRegistered(CompanyNameDRV,
                                                         ProductNameDRV,
                                                         VersionNameDRV,
                                                         dev1[n],
                                                         "StaticFactory");
        if(LAxFactory1 == 0) continue;
        // создаём объект (получаем интерфейс драйвера)
        pADC1 = (IADCDevice*)LAxFactory1->Create(_T("IADCDevice"), 0);
        if(pADC1 == 0) continue;
        // определяем тип драйвера
        pADC1->Get(ADC_GET_CAPABILITY, &var_caps1);
        if (var_caps1 & ADC_CAPS_DMA)
        {
            uiDriverName->Items->Add(dev1[n]);
            for (int k=0; k<20; k++)
                dev[z][k] = dev1[n][k];
            z++;
        }
        //-----------------------------------------------
        // получаем маску для выделения номера канала
        //pADC1->Get(ADC_GET_CHANNELMASK,&z);
        //Memo1->Lines->Add(z);
        //-------------------------------------------
    float       MaxFreq;   //Максимальная частота
    // Допустимые частоты квантования и размеры блоков
    //Узнаем максимальную частоту
    pADC1->Get(ADC_GET_MAXFREQ, &MaxFreq);
    //Устанавливаем слайдеры
    uiFreq->TickStyle = tsNone;
    uiBlockSize->TickStyle = tsNone;
    uiBlockSize->Max = 65536;
    uiBlockSize->Min = 1024;
    uiBlockSize->Position=1024;
    uiBlockSize2->Text=1024;

    uiFreq->Max = MaxFreq;
    uiFreq->Min = 20000;
    uiFreq->LineSize = (MaxFreq - 20000)/20;
    uiFreq->Position = 200000;
    //----Задаём максимальные значения для графика:---------------------------
    Chart1->LeftAxis->Maximum=10;
    Chart1->LeftAxis->Minimum=-10;
    //------------------------------------------------------------------------
    Chan1->ItemIndex=0;
    Chan2->ItemIndex=0;
    GainKoefFaradey->ItemIndex=0;
    //------------------------------------------------------------------------

        // освобождаем драйвер
        pADC1->Release();
    }
    uiDriverName->ItemIndex = 0;
    uiDriverNameChange(this);
    //-----------------------------------------------------------------
    // Тут заполняем необходимые данные интерфейса

    // это прямая отсекающая высокие частоты
    Series5->AddXY(0,-10,"",clBlack);
    Series5->AddXY(0,1000,"",clBlack);

    GainKoefFoygt->ItemIndex=0;
}
//---------------------------------------------------------------------------

// при смене имени драйвера - загружается выбранный драйвер
void __fastcall TForm1::uiDriverNameChange(TObject *Sender)
{
    DllClient   LAxClient1;
    IFactory*   LAxFactory1;
    IADCDevice* pADC1;

    int         n;
    int         u;

    n = uiDriverName->ItemIndex;
    DriverIndex = n;

    // Загружаем выбранный драйвер и заполняем комбо-боксы параметрами
    // устройства: базовыми адресами, DRQ, IRQ
    LAxFactory1 = (IFactory*)LAxClient1.LoadRegistered(CompanyNameDRV,
                                                       ProductNameDRV,
                                                       VersionNameDRV,
                                                       dev[n],
                                                       "StaticFactory");
    if(LAxFactory1 == 0) return;
    pADC1 = (IADCDevice*)LAxFactory1->Create(_T("IADCDevice"), 0);
    if(pADC1 == 0) return;

    // Получаем список базовых адресов
    int index_def = 0;
    pADC1->Get(ADC_GET_BASELIST, &BaseList);
    uiBase->Clear();
    for (u=0; u<BaseList[u]; u++)
        uiBase->Items->Add(IntToHex(BaseList[u],3) + "h");
    pADC1->Get(ADC_GET_DEFAULTBASEINDEX, &index_def);
    uiBase->ItemIndex = index_def;
    ADCBase = BaseList[uiBase->ItemIndex];

    // Получаем список каналов DMA
    pADC1->Get(ADC_GET_DRQLIST, &DRQList);
    uiDRQ->Clear();
    for (u=0; u<DRQList[u]; u++)
        uiDRQ->Items->Add(DRQList[u]);
    pADC1->Get(ADC_GET_DEFAULTDRQINDEX, &index_def);
    uiDRQ->ItemIndex = index_def;
    ADCDRQ = DRQList[uiDRQ->ItemIndex];

    // Получаем список номеров IRQ
    pADC1->Get(ADC_GET_IRQLIST, &IRQList);
    uiIRQ->Clear();
    for (u=0; u<IRQList[u]; u++)
        uiIRQ->Items->Add(IRQList[u]);
    uiIRQ->ItemIndex = 2;
    ADCIRQ = IRQList[uiIRQ->ItemIndex];

    // Освобождаем объект
    pADC1->Release();    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiBaseChange(TObject *Sender)
{
    ADCBase = BaseList[uiBase->ItemIndex];    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiDRQChange(TObject *Sender)
{
    ADCDRQ = DRQList[uiDRQ->ItemIndex];    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiIRQChange(TObject *Sender)
{
    ADCIRQ = IRQList[uiIRQ->ItemIndex];    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)// выход из программы
{
Form1->Close();
}
//----много кода, выключает/включает графические элементы, во время работы АЦП-
//---------------------------------------
void __fastcall TForm1::uiControlClick(TObject *Sender)
{
bS = !bS;
    if (bS)
    {
        
        uiIRQ->Enabled = false;
        uiDRQ->Enabled = false;
        uiBase->Enabled = false;
        uiDriverName->Enabled = false;
        uiFrenq->Enabled = false;
        uiFreq->Enabled = false;
        uiBlockSize->Enabled=false;
        uiBlockSize2->Enabled=false;
        Chan1->Enabled=false;
        Chan2->Enabled=false;
        CurrentRes->Enabled=0;
        CurrentHall->Enabled=0;
        ResCurveIndex->Enabled=0;
        HallCurveIndex->Enabled=0;
        GainKoefFaradey->Enabled=0;

        //-- кнопки-----------------------------------------------
        uiResFeat->Enabled=0;
        uiFFTRes->Enabled=0;
        uiHallFeat->Enabled=0;
        uiFFTHall->Enabled=0;
        uiFaradeyFeat->Enabled=0;
        uiFFTFaradey->Enabled=0;
        uiFoygtFeat->Enabled=0;
        uiFFTFoygt->Enabled=0;


        // индексы отбрасывания частот для преобразования Фурье
        Edit1->Enabled=0;
        //FFTResCrop->Enabled=0;
        //FFTHallCrop->Enabled=0;
        //FFTFaradeyCrop->Enabled=0;
        //FFTFoygtCrop->Enabled=0;

        CurrentFaradey->Enabled=0;
        CurrentFoygt->Enabled=0;

        FaradeyCurveIndex->Enabled=0;
        FoygtCurveIndex->Enabled=0;
        
        GainKoefFoygt->Enabled=0;

        Start();
        uiControl->Caption = AnsiString("Stop");
        uiResControl->Caption = AnsiString("Stop");
        uiHallControl->Caption = AnsiString("Stop");
        uiFaradeyControl->Caption = AnsiString("Stop");
        uiFoygtControl->Caption = AnsiString("Stop");
        StatusBar->Panels->Items[1]->Text="Проводится измерение";
    }
    else
    {
        GainKoefFaradey->Enabled=1;
        CurrentRes->Enabled=1;
        CurrentHall->Enabled=1;
        ResCurveIndex->Enabled=1;
        HallCurveIndex->Enabled=1;
        uiIRQ->Enabled = true;
        uiDRQ->Enabled = true;
        uiBase->Enabled = true;
        uiDriverName->Enabled = true;
        uiFrenq->Enabled =true;
        uiFreq->Enabled = true;
        uiBlockSize->Enabled=true;
        uiBlockSize2->Enabled=true;
        Chan1->Enabled=true;
        Chan2->Enabled=true;

        uiResFeat->Enabled=1;
        uiFFTRes->Enabled=1;
        uiHallFeat->Enabled=1;
        uiFFTHall->Enabled=1;
        uiFaradeyFeat->Enabled=1;
        uiFFTFaradey->Enabled=1;
        uiFoygtFeat->Enabled=1;
        uiFFTFoygt->Enabled=1;

        // индексы отбрасывания частот для преобразования Фурье
        Edit1->Enabled=1;
        //FFTResCrop->Enabled=1;
        //FFTHallCrop->Enabled=1;
        //FFTFaradeyCrop->Enabled=1;
        //FFTFoygtCrop->Enabled=1;

        CurrentFaradey->Enabled=1;
        CurrentFoygt->Enabled=1;

        FaradeyCurveIndex->Enabled=1;
        FoygtCurveIndex->Enabled=1;
        GainKoefFoygt->Enabled=1;

        Stop();
        uiControl->Caption = AnsiString("Start");
        uiResControl->Caption = AnsiString("Start");
        uiHallControl->Caption = AnsiString("Start");
        uiFaradeyControl->Caption = AnsiString("Start");
        uiFoygtControl->Caption = AnsiString("Start");
        StatusBar->Panels->Items[1]->Text="Готова к работе. Тестовая версия";
    }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::uiFrenqChange(TObject *Sender)
{
uiFreq->Position=StrToInt(uiFrenq->Text);
Frequency=uiFreq->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiBlockSize2Change(TObject *Sender)
{
uiBlockSize->Position=StrToInt(uiBlockSize2->Text);
BlockSize= uiBlockSize->Position;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::uiBlockSizeChange(TObject *Sender)
{
uiBlockSize2->Text=uiBlockSize->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFreqChange(TObject *Sender)
{
uiFrenq->Text=uiFreq->Position;
}
//---------------------------------------------------------------------------
// Подгонка магнитосопротивления
void __fastcall TForm1::uiResFeatClick(TObject *Sender)
{

EvenFeat(SeriesRes1,0);
EvenFeat(SeriesRes2,0);
MidCurve(SeriesRes1,SeriesRes2,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiHallFeatClick(TObject *Sender)
{
OddFeat(SeriesHall1,0);
OddFeat(SeriesHall2,0);
MidCurve(SeriesHall1,SeriesHall2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFaradeyFeatClick(TObject *Sender)
{
EvenFeat(SeriesFaradey1,0);
EvenFeat(SeriesFaradey2,0);
MidCurve(SeriesFaradey1,SeriesFaradey2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFoygtFeatClick(TObject *Sender)
{
FoygtFeat(SeriesFoygt1,SeriesFoygt2,0);
;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// нечетная подгонка
// параметры массив и переменная относительно которой функция нечетна
// 0 для х, 1 для y
void  OddFeat(TLineSeries* a, long index)
{
int size=a->XValues->Count();
if(size==0)
{
ShowMessage("Получен пустой массив данных!!!");
return;
}
TChartValueList * b,*c;
if(!index)
{
c=a->YValues;
b=a->XValues;
}
else
{
b=a->YValues;
c=a->XValues;
}
for(int i=0;i<size/2;i++)
{
b->Value[i]=(b->Value[i]-b->Value[size-1-i])/2;
b->Value[size-1-i]=-b->Value[i];
c->Value[i]=(c->Value[i]-c->Value[size-1-i])/2;
c->Value[size-1-i]=-c->Value[i];
}    
}
//-----------------------------------------------------------------------------
//четная подгонка
// параметры массив и переменная относительно которой функция нечетна
// 0 для х, 1 для y
void  EvenFeat(TLineSeries* a, long index)
{
int size=a->XValues->Count();
if(size==0)
{
ShowMessage("Получен пустой массив данных!!!");
return;
}
TChartValueList * b,*c;
if(!index)
{
c=a->YValues;
b=a->XValues;
}
else
{
b=a->YValues;
c=a->XValues;
}
for(int i=0;i<size/2;i++)
{
b->Value[i]=(b->Value[i]-b->Value[size-1-i])/2;
b->Value[size-1-i]=-b->Value[i];
c->Value[i]=(c->Value[i]+c->Value[size-1-i])/2;
c->Value[size-1-i]=c->Value[i];
}
}
//-----------------------------------------------------------------------------
//----------Подгонка Фотопроводимости в геометрии Фойгта-----------------------
void  FoygtFeat(TLineSeries* a,TLineSeries* b, long index)
{
int size=a->XValues->Count();
if(size==0 || b->XValues->Count()==0)
{
ShowMessage("Получен пустой массив данных!!!");
return;
}

for(int i=0;i<size;i++)
{
a->XValues->Value[i]=(a->XValues->Value[i]-b->XValues->Value[size-1-i])/2;

a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[size-1-i])/2;
}
}
//-----------------------------------------------------------------------------
// усреднение двух кривых
// индекс - зарезервирован, пока не используется
// функция сохраняет результат в ПЕРВЫЙ передаваемый массив!
void  MidCurve(TLineSeries* a, TLineSeries* b, long index)
{

int size=a->XValues->Count();
if(size==0 || b->XValues->Count()==0)
{
ShowMessage("Получен пустой массив данных!!!");
return;
}
if(a->XValues->Count()!=b->XValues->Count())
{
ShowMessage("Разное количество точек на графиках!!!");
return;
}

for(int i=0;i<size;i++)
{
a->XValues->Value[i]=(a->XValues->Value[i]+b->XValues->Value[i])/2;
//b->XValues->Value[i]=a->XValues->Value[i];
a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[i])/2;

}
//-----------------------------------------------------------------------------

}
void __fastcall TForm1::Button3Click(TObject *Sender) // очищаем всё:)
{
Series1->Clear();
Series2->Clear();
Series3->Clear();
SeriesRes1->Clear();
Memo1->Clear();
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
void __fastcall TForm1::uiFFTResClick(TObject *Sender)
{

Tr_Filter((ResCurveIndex->ItemIndex==0?SeriesRes1:SeriesRes2),SeriesFFTRes,Lfilter1->Text.ToInt(),Fd1->Text.ToInt(),
Fp1->Text.ToInt(),Fz1->Text.ToInt());
//Ftransform(SeriesRes1,SeriesFFTRes,out1,StrToInt(FFTResCrop->Text));

}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTHallClick(TObject *Sender)
{
Tr_Filter((HallCurveIndex->ItemIndex==0?SeriesHall1:SeriesHall2),SeriesFFTHall,Lfilter2->Text.ToInt(),Fd2->Text.ToInt(),
Fp2->Text.ToInt(),Fz2->Text.ToInt());
//Ftransform(SeriesHall1,SeriesFFTHall,out2,StrToInt(FFTHallCrop->Text));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTFaradeyClick(TObject *Sender)
{
Tr_Filter((FaradeyCurveIndex->ItemIndex==0?SeriesFaradey1:SeriesFaradey2),SeriesFFTFaradey,Lfilter3->Text.ToInt(),Fd3->Text.ToInt(),
Fp3->Text.ToInt(),Fz3->Text.ToInt());
//Ftransform(SeriesFaradey1,SeriesFFTFaradey,out3,StrToInt(FFTFaradeyCrop->Text));

}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTFoygtClick(TObject *Sender)
{
Tr_Filter((FoygtCurveIndex->ItemIndex==0?SeriesFoygt1:SeriesFoygt2),SeriesFFTFoygt,Lfilter4->Text.ToInt(),Fd4->Text.ToInt(),
Fp4->Text.ToInt(),Fz4->Text.ToInt());
//Ftransform(SeriesFoygt1,SeriesFFTFoygt,out4,StrToInt(FFTFoygtCrop->Text));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{

Series3->Clear();
Series4->Clear();
Series2->Clear();
Series1->Clear();  // убираемся
Series5->Active=false;// чтобы не мешал
long double x=0;
for(int i=0;i<1500;i++)  // пишем тестовую на график
{
//Series2->AddXY(x,5*sin(2*x),"",clBlack);
Series1->AddXY(x,x*x+rand()%3*sin(25*x)+rand()%2*sin(50*x),"",clRed);
//Memo1->Lines->Add(FloatToStr(Series1->XValues->Last()) + "   " + FloatToStr(sin(x)+sin(25*x)+sin(50*x)));
//Series4->AddXY(x,10*sin(x)+sin(50*x)+sin(100*x),"",clGreen);
//Series4->AddXY(x,x*x,"",clBlack);
x+=0.02+rand()%10/1000.;
}

//Ftransform(Series1,Series2,Series3,Edit1->Text.ToInt());

// черная на графике - восстановленный
// красная - тестовый
// синий - спектр


//---------------------
/*SYSTEMTIME st;
GetLocalTime(&st);

Memo1->Lines->Add((String)st.wSecond + "   " + st.wMilliseconds);
Ftransform(Series3,Series1,Series2);
GetLocalTime(&st);

Memo1->Lines->Add((String)st.wSecond+ "   " + st.wMilliseconds); */
//----------------------
//invFtransform(Series1,Series4);
//FFT(Series3,Series4,StrToInt(Edit1->Text.ToInt()));
//Series3->Active=false;
//Series2->Active=false;
//Series1->Active=false;
//Series3->Active=false;
//Series4->Active=false;
//Series5->Active=false;

/*for(int i=0;i<Series1->XValues->Count();i++)
{
if(Series1->YValues->Value[i]!=0)
Series4->AddXY(Series1->XValues->Value[i],Series3->YValues->Value[i]/Series1->YValues->Value[i],"",clBlue);
else
Series4->AddXY(Series1->XValues->Value[i],0,"",clBlue);

}
for(int i=0;i<Series3->XValues->Count();i++)
{
Series3->YValues->Value[i]*=Series4->YValues->Value[i];
}  */

}
//---------------------------------------------------------------------------


void __fastcall TForm1::N5Click(TObject *Sender)  // сохранение
{
AllSeries[0]=PtrToInt(Series1);
AllSeries[1]=PtrToInt(Series2);
AllSeries[2]=PtrToInt(Series3);
AllSeries[3]=PtrToInt(Series4);
AllSeries[4]=PtrToInt(Series5);
AllSeries[5]=PtrToInt(SeriesRes1);
AllSeries[6]=PtrToInt(SeriesRes2);
AllSeries[7]=PtrToInt(SeriesFFTRes);
AllSeries[8]=PtrToInt(out1);
AllSeries[9]=PtrToInt(SeriesHall1);
AllSeries[10]=PtrToInt(SeriesHall2);
AllSeries[11]=PtrToInt(SeriesFFTHall);
AllSeries[12]=PtrToInt(out2);
AllSeries[13]=PtrToInt(SeriesFaradey1);
AllSeries[14]=PtrToInt(SeriesFaradey2);
AllSeries[15]=PtrToInt(SeriesFFTFaradey);
AllSeries[16]=PtrToInt(out3);
AllSeries[17]=PtrToInt(SeriesFoygt1);
AllSeries[18]=PtrToInt(SeriesFoygt2);
AllSeries[19]=PtrToInt(SeriesFFTFoygt);
AllSeries[20]=PtrToInt(out4);

if(!SaveForm)
{
Application->CreateForm(__classid(TSaveForm), &SaveForm);
SaveForm->Visible=true;
}
else
SaveForm->BringToFront();

}
//---------------------------------------------------------------------------

//-------------------Открытие файла------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
if(OpenDialog1->Execute())  // если мы что-то выбрали
{
TLineSeries * S=GetCurrentSeries(1);  // получаем активный график

S->Clear(); // очищаем сначала график, куда пишем:)

TStringList *tts=new TStringList();  // сюда будем загружать из файла
int b;
      tts->LoadFromFile(OpenDialog1->Files->Strings[0]);// загрузили
      for(int i=0;i<tts->Count;i++) // по количеству строк
      {
      if(tts->Strings[i].IsEmpty()) // пустые строки пропускаем
      continue;
      for(b=1;b<tts->Strings[i].Length();b++) // в непустых строках ищем пробел
      if(tts->Strings[i].operator [](b)==' ' || tts->Strings[i].operator [](b)=='\t')
      break;
      S->AddXY(tts->Strings[i].SubString(1,b-1).ToDouble(), // первая часть до пробела это х, вторая после у
      tts->Strings[i].SubString(b+1,tts->Strings[i].Length()).ToDouble(),
      "",clRed);
      }    
}
}
//---------------------------------------------------------------------------

//---------Двигаем полосу "обрезания" для БФП-------------------------------
void __fastcall TForm1::Edit1Change(TObject *Sender)
{
if(Edit1->Text=="")
Edit1->Text=0;
Series5->XValues->Value[0]=Edit1->Text.ToDouble();
Series5->XValues->Value[1]=Edit1->Text.ToDouble();
Series5->Repaint();              // перерисовка графика
}
//----Преобразование Фурье---------------------------------------------------

// теоретически - мусор и его надо отсюда выпилить.
//---------------------------------------------------------------------------
/*void Ftransform(TLineSeries * data,TLineSeries * out,TLineSeries * New,int wCrop)
{
Form1->StatusBar->Panels->Items[1]->Text="Запущено преобразование Фурье";
if(data->XValues->Count()==0) // обработка ошибок:)
{
ShowMessage("Массив данных пуст!");
Form1->StatusBar->Panels->Items[1]->Text="Готова к работе.";
return;
}

// чистим выходной график
out->Clear();

long double w,dx,f1,f2,h,b1,b2,q1,q2,dw; // частота, шаг по х, значения функций, шаг, коэффициенты, шаг по частоте
long double x1,x2;                       // значения х
int l=data->XValues->Count();  // записываем длину массива
//out1.setlength(l);             прощай
long double *outldr=new long double [l];   // выделяем память под прямое преобразование
long double *outldc=new long double [l];
dx=(data->XValues->Last()-data->XValues->First())/(double)(l-1); // определяем среднее изменение по х

dw=(M_PI/dx)/l;         // шаг по частоте
w=0;                     // начальная частота
//Form1->Memo6->Lines->Add(FloatToStr(dx)+ "   " +FloatToStr(dw));
for(int k=0;k<l;k++,w+=dw)
{
outldr[k]=0;         // обнуляем значения массива
outldc[k]=0;
for(int i=0;i<(l-1);i++ )
{
x1=data->XValues->Value[i];// предыдущее значение х
x2=data->XValues->Value[i+1];// следующее х
f1=data->YValues->Value[i];// предыдущее значение y
f2=data->YValues->Value[i+1];// следующее y
h=fabs(x2-x1);               // текущий шаг i+1  
//коэффициенты
b1=w*h;
b2=b1*b1;
//double temp;

// условие |b1|>=0.1
q1=(sqrt(b2)>=0.1)?((1.-cosl(b1))/b2):(1/2.-b2/24.*(1-b2/30.*(1-b2/56.)));
q2=(sqrt(b2)>=0.1)?((1.-sinl(b1)/b1)/b1):(b1/6.*(1.-b2/20.*(1-b2/42.*(1-b2/72.))));
outldr[k]+=h*(q1*(f1*cosl(w*x1)+f2*cosl(w*x2))-q2*(f1*sinl(w*x1)-f2*sinl(w*x2)));
//temp=outldr[k];
outldc[k]+=h*(q1*(f1*sinl(w*x1)+f2*sinl(w*x2))+q2*(f1*cosl(w*x1)-f2*cosl(w*x2)));
//temp=outldc[k];

}

//long double temp=out1[k].x;
out->AddXY(w,fabs(outldr[k]),"",clBlue);  //вывод спектра
//Form1->Memo1->Lines->Add(FloatToStr(w) +"   " + FloatToStr(out1[k].x)+"   " + FloatToStr(out1[k].y));
if(k%50==0)                 // обновляем график каждые 50 значений
Form1->Update();
//Form1->Series4->AddXY(w,sqrt(pow(outldr[k],2)),"",clRed);
//Form1->Memo1->Lines->Add(FloatToStr(outldr[k])+"   "+FloatToStr(outldc[k]));

}
long double *out2dr=new long double [l];   // выделяем память под прямое преобразование
long double *out2dc=new long double [l];

// обрезка
for(int k=(wCrop/dw);k<l;k+=1) // начинаем с нужной частоты перести её в список параметров!!!!
{
outldc[k]=0;
outldr[k]=0;
}
// восстанавливаем функцию
//-----------------------------------------------------------------------------
long double x,w1,w2;    // текущее значение х, и частОты
for(int k=0;k<l;k++)
{
outldc[k]*=-1;         // умножаем комплексную часть на -1  ибо перед ней в преобразовании стоит -
long double f1c,f2c;
out2dc[k]=0;
out2dr[k]=0;

x=data->XValues->Value[k];// если поставить модуль - отрицательная часть будет противоположной.

for(int i=0;i<(l-1);i++)
{

w1=out->XValues->Value[i];
w2=out->XValues->Value[i+1];

f1=outldr[i];  // предыдущее значение y
f2=outldr[i+1];// следующее y
f1c=outldc[i]; // предыдущее значение y
f2c=outldc[i+1];// следующее y

h=w2-w1;               // текущий шаг i+1


//коэффициенты
b1=h;
b2=b1*b1;

q1=(sqrt(b2)>=0.1)?((1.-cosl(b1))/b2):(1/2.-b2/24.*(1-b2/30.*(1-b2/56.)));
q2=(sqrt(b2)>=0.1)?((1.-sinl(b1)/b1)/b1):(b1/6.*(1.-b2/20.*(1-b2/42.*(1-b2/72.))));

// действительная часть
out2dr[k]+=h*(q1*(f1*cosl(w1*x)+f2*cosl(w2*x))-q2*(f1*sinl(w1*x)-f2*sinl(w2*x)))
-        h*(q1*(f1c*sinl(w1*x)+f2c*sinl(w2*x))+q2*(f1c*cosl(w1*x)-f2c*cosl(w2*x)));
// мнимая часть
out2dc[k]+=h*(q1*(f1*sinl(w1*x)+f2*sinl(w2*x))+q2*(f1*cosl(w1*x)-f2*cosl(w2*x)))+
h*(q1*(f1c*cosl(w1*x)+f2c*cosl(w2*x))-q2*(f1c*sinl(w1*x)-f2c*sinl(w2*x)));

}
//----------------------------------
//--------вывод восстановленной функции---------

// коэффициент вывода - пи
New->AddXY(data->XValues->Value[k],(out2dr[k])/(M_PI),"",clBlack);
if(k%50==0)           // обновляем график каждые 50 значений
Form1->Update();
//Form1->Memo1->Lines->Add(FloatToStr(data->XValues->Value[k])
//+"   "+FloatToStr(out2[k].x)+"    "+FloatToStr(out2[k].y));

}
delete[] outldr; // освобождаем память
delete[] outldc;
delete[] out2dr;
delete[] out2dc;
Form1->StatusBar->Panels->Items[1]->Text="Готова к работе.";
}  */


// выбор активного графика
TLineSeries * __fastcall TForm1::GetCurrentSeries(int curve)
{
TLineSeries * SaveSeries;
switch(Form1->PC->ActivePageIndex)
{           // выбираем нужный массив данных для сохранения
    case 0:
            // случай отладки, когда активна вкладка "FirstTab"
        SaveSeries=Form1->Series3;
        break;
    case 1:
        if(curve) // если мы открываем/сохраняем файл, то в зависимости от выборанного графика сохраняем нужный
        SaveSeries=(ResCurveIndex->ItemIndex==0?Form1->SeriesRes1:Form1->SeriesRes2);
        else     
        SaveSeries=Form1->SeriesRes1;
        break;
    case 2:
        if(curve)
        SaveSeries=(HallCurveIndex->ItemIndex==0?Form1->SeriesHall1:Form1->SeriesHall2);
            // Эффект Холла
        else
        SaveSeries=Form1->SeriesHall1;
            ;
        break;
    case 3:
            // Фотопроводимость в геометрии Фарадея
        if(curve)
        SaveSeries=(FaradeyCurveIndex->ItemIndex==0?Form1->SeriesFaradey1:Form1->SeriesFaradey2);
        else
        SaveSeries=Form1->SeriesFaradey1;
        break;
            
    case 4:
            // Фотопроводимость в геометрии Фойгта
        if(curve)
        SaveSeries=(FoygtCurveIndex->ItemIndex==0?Form1->SeriesFoygt1:Form1->SeriesFoygt2);
        else
        SaveSeries=Form1->SeriesFoygt1;
        break;
        // в случае ошибки - предупреждаем пользователя
    default:
        ShowMessage("Неверный индекс! Возможно программа была некорректно обновлена. Обратитесь к разработчику!");
        return NULL;
}
return SaveSeries;  // возвращаем указатель на активный график
}

 // выбор активного графика
 // 

TLineSeries * __fastcall TForm1::GetSelectedSeries(int index)
{
    TLineSeries * SaveSeries;
    switch(index)
    {
    case 0:
    SaveSeries=Series1;
    break;
    case 1:
    SaveSeries=Series2;
    break;
    case 2:
    SaveSeries=Series3;
    break;
    case 3:
    SaveSeries=Series4;
    break;
    case 4:
    SaveSeries=Series5;
    break;
    case 5:
    SaveSeries=SeriesRes1;
    break;
    case 6:
    SaveSeries=SeriesRes2;
    break;
    case 7:
    SaveSeries=SeriesFFTRes;
    break;
    case 8:
    SaveSeries=out1;
    break;
    case 9:
    SaveSeries=SeriesHall1;
    break;
    case 10:
    SaveSeries=SeriesHall2;
    break;
    case 11:
    SaveSeries=SeriesFFTHall;
    break;
    case 12:
    SaveSeries=out2;
    break;
    case 13:
    SaveSeries=SeriesFaradey1;
    break;
    case 14:
    SaveSeries=SeriesFaradey2;
    break;
    case 15:
    SaveSeries=SeriesFFTFaradey;
    break;
    case 16:
    SaveSeries=out3;
    break;
    case 17:
    SaveSeries=SeriesFoygt1;
    break;
    case 18:
    SaveSeries=SeriesFoygt2;
    break;
    case 19:
    SaveSeries=SeriesFFTFoygt;
    break;
    case 20:
    SaveSeries=out4;
    break;
    }
    return SaveSeries;

}

// должна заниматься экстраполяцией функции, но пока просто мусор ибо неотлажена

 
double Interpolate (double *x, double *y, double X, int N)
{
double *l=new double [N]; // массив коэффициентов

double L=0; // итоговое значение функции в точке.

double lv=1,ln=1;

for(int k=0;k<N;k++)
{
for(int i=0;i<N;i++)
{
if(i==k) continue;
lv*=X-x[i];
ln*=x[k]-x[i];
}
if(ln==0) l[k]=0;
l[k]=lv/ln;
L+=y[k]*l[k];
}

delete [] l;

return L;
}


void __fastcall TForm1::Button4Click(TObject *Sender)
{
/*
double x[20]={1,2,3,4,5,6,8,8,9,10,11,12,10,14,15,16,16,18,19,20};
double y[4]={0};
Kramer(x,y);
for(int i=0;i<4;i++)
Memo1->Lines->Add(y[i]);       */

//int z=20; // кол-во интерполируемых точек
int N = Series3->XValues->Count(); // кол-во точек функции
Memo1->Lines->Add(N);
double X=Edit2->Text.ToDouble();

double *x=new double [N];
double *y=new double [N];
for(int i=0;i<N;i++)
{
x[i]=Series3->XValues->Value[i];
y[i]=Series3->YValues->Value[i];
}

double L=Interpolate (x, y, X, N);

Memo1->Lines->Add(L);

delete []x;
delete []y;


/*Series1->Active=1;
Series2->Active=1;
Series3->Active=1;
Series1->Clear();
Series2->Clear();
Series3->Clear();
double x=0;
for(int i=0;i<1000;i++,x+=0.2)
{
Series1->AddXY(x+(rand()%10)/100.,x*x+10*sin(50*x),"",clRed);
}
Form1->Update();
FFT(Series1,Series2,Series3,Edit1->Text.ToInt());
Form1->Update();  */

}
// еще одно преобразование Фурье, тоже надо выпиливать.
//---------------------------------------------------------------------------
/*void FFT(TLineSeries * data, TLineSeries * complexFFT, TLineSeries *out, int crop)
{
if(data->XValues->Count()==0)
{
ShowMessage("Ошибка!Получен пустой массив данных!");
return;
}
alglib::real_1d_array rx1; // объявляем массив действительных чисел
alglib::real_1d_array ry1;
alglib::ae_int_t n=data->XValues->Count();// это количество элементов массива
alglib::complex_1d_array fy1;// комплесный массив
alglib::complex_1d_array fx1;// комплесный массив

double * p=new double[n];// временный массив для данных
for(int i=0;i<n;i++)
{
p[i]=data->YValues->Value[i]; // помещаем данные во временный массив

}
ry1.setcontent(n,p); // помещаем данные в массив действительных чисел.
for(int i=0;i<n;i++)
p[i]=data->XValues->Value[i];
rx1.setcontent(n,p); // помещаем данные в массив действительных чисел.

fftr1d(ry1,n,fy1); // вызываем БФП
fftr1d(rx1,n,fx1); // вызываем БФП

if(complexFFT!=0) // если нам предоставили второй график

for(int i=0;i<=n/2;i++) //выводим на него разложение
{
    complexFFT->AddY(fabs(fx1[i].x),"",clBlue);
    //Form1->Memo1->Lines->Add(FloatToStr(fy1[i].x)+"   "+FloatToStr(fy1[i].y));
}

for(int i=crop;i<n-crop;i++) // убираем лишние частоты
{
    // всё что больше граничной

        fy1[i].x=0;
        fy1[i].y=0;
        //fx1[i].x=0;
        //fx1[i].y=0;

}

fftr1dinv(fy1,n,ry1); // делаем обратное преобразование
fftr1dinv(fx1,n,rx1); // делаем обратное преобразование

for(int i=0;i<n;i++)
{
out->AddXY(rx1[i],ry1[i],"",clBlack);
//out->YValues->Value[i]=ry1[i]; // сохраняем данные обратно
//out->XValues->Value[i]=rx1[i];
}
delete [] p;

}   */




// ВНИМАНИЕ!!! Напрямую не вызывать!!! Пользоваться трамплином!!!---------------
double Filter2 (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = 49; //Длина фильтра 49
long double Fd = Fdisk; //Частота дискретизации входных данных 2000
long double Fs = Fpropysk; //Частота конца полосы пропускания  20
long double Fx = Fzatyh; //Частота начала полосы затухания    30

long double *H= new long double [N] ; //Импульсная характеристика фильтра
long double *H_id= new long double [N]; //Идеальная импульсная характеристика
long double *W= new long double [N]; //Весовая функция

/*long double H [N] = {0}; //Импульсная характеристика фильтра
long double H_id [N] = {0}; //Идеальная импульсная характеристика
long double W [N] = {0}; //Весовая функция        */

//Расчет импульсной характеристики фильтра
//long double Fc = (Fs + Fx) / (2 * Fd);

/*for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2*M_PI*Fc;
else H_id[i] = sinl(2*M_PI*Fc*i )/(M_PI*i);
// весовая функция Блекмена
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1));
H [i] = H_id[i] * W[i];
}    */

//Нормировка импульсной характеристики
/*long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1
 */
//Фильтрация входных данных

H[0]=0.00070253401856393;
H[1]= 0.00231935244351841;
H[2]= 0.00478611811381755;
H[3]= 0.00766633203397237;
H[4]= 0.00944988355874393;
H[5]= 0.00877148684636659;
H[6]= 0.00461539855527225;
H[7]= -0.00221042184558028;
H[8]= -0.00929005323066059;
H[9]= -0.0129106895748147;
H[10]= -0.0101804444782552;
H[11]= -0.000690287144759872;
H[12]= 0.0120807373149672;
H[13]= 0.0217536278253824;
H[14]= 0.021571025476409;
H[15]= 0.00835859632478029;
H[16]= -0.0146791571360858;
H[17]= -0.0374801604699289;
H[18]= -0.0463712861225022;
H[19]= -0.0296456500457865;
H[20]= 0.0164205324303302;
H[21]= 0.0843823910468745;
H[22]= 0.156823842718348;
H[23]= 0.212222423329572;
H[24]= 0.232941458293524;
H[25]= 0.212222423329572;
H[26]= 0.156823842718348;
H[27]= 0.0843823910468745;
H[28]= 0.0164205324303302;
H[29]= -0.0296456500457865;
H[30]= -0.0463712861225022;
H[31]= -0.0374801604699289;
H[32]= -0.0146791571360858;
H[33]= 0.00835859632478029;
H[34]= 0.021571025476409;
H[35]= 0.0217536278253824;
H[36]= 0.0120807373149672;
H[37]= -0.000690287144759872;
H[38]= -0.0101804444782552;
H[39]= -0.0129106895748147;
H[40]= -0.00929005323066059;
H[41]= -0.00221042184558028;
H[42]= 0.00461539855527225;
H[43]= 0.00877148684636659;
H[44]= 0.00944988355874393;
H[45]= 0.00766633203397237;
H[46]= 0.00478611811381755;
H[47]= 0.00231935244351841;
H[48]= 0.00070253401856393;

for (int i=0; i<sizeIn; i++)
{
out[i]=0.;
for (int j=0; j<(i>N-1?N-1:i); j++)// та самая формула фильтра
out[i]+= H[j]*in[i-j];
}
delete [] H;
delete [] H_id;
delete [] W;
return (N-1)/2.0;
}









// ВНИМАНИЕ!!! Напрямую не вызывать!!! Пользоваться трамплином!!!---------------
double Filter (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = length; //Длина фильтра 20
long double Fd = Fdisk; //Частота дискретизации входных данных 2000
long double Fs = Fpropysk; //Частота конца полосы пропускания  10
long double Fx = Fzatyh; //Частота начала полосы затухания    20

long double *H= new long double [N] ; //Импульсная характеристика фильтра
long double *H_id= new long double [N]; //Идеальная импульсная характеристика
long double *W= new long double [N]; //Весовая функция

/*long double H [N] = {0}; //Импульсная характеристика фильтра
long double H_id [N] = {0}; //Идеальная импульсная характеристика
long double W [N] = {0}; //Весовая функция  */

//Расчет импульсной характеристики фильтра
long double Fc = (Fs + Fx) / (2 * Fd);

for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2*M_PI*Fc;
else H_id[i] = sinl(2*M_PI*Fc*i )/(M_PI*i);
// весовая функция Блекмена
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1));
H [i] = H_id[i] * W[i];
}

//Нормировка импульсной характеристики
long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1 

//----------------------------------------------------------------
// печать коэффициентов фильтра
for(int i=0;i<N;i++)
Form1->Memo2->Lines->Add(FloatToStr(H[i]));
//----------------------------------------------------------------

//Фильтрация входных данных
for (int i=0; i<sizeIn; i++)
{
out[i]=0.;
for (int j=0; j<(i>N-1?N-1:i); j++)// та самая формула фильтра
out[i]+= H[j]*in[i-j];
}
delete [] H;
delete [] H_id;
delete [] W;
return (N-1)/2.0;
}
// функция трамплин для фильтра.  ----------------------------------------------
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh)
{
if(!inS->YValues->Count())   // если пустой - ошибка, убегаем.
{
ShowMessage("Пустой массив!");
return 0;
}
int size=inS->YValues->Count();  // получаем размер
size=inS->XValues->Count();  // получаем размер
double *in=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in[i]=inS->YValues->Value[i];
double *out=new double[size]; // выделяем память для выходных значений
double k=Filter(in,out,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
 k*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count();// вычисляем сдвиг фаз
//----------------------------------------------
//---------добавление для фильтрации магнитного поля
// внимание - она пока работает не корректно ибо фильтр наполняется только за
// его длину.
double *in2=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // выделяем память для выходных значений
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count();// вычисляем сдвиг фаз


//----------------------------------------------
 outS->Clear(); // чистим график, перед выводом
for(int i=0;i<size;i++) // выводим
{
outS->AddXY(out2[i]/*inS->XValues->Value[i]*/-k+k2,out[i],"",clBlue);
Form1->Series2->AddY(out2[i]-k+k2,"",clBlue);
Form1->Series3->AddY(inS->XValues->Value[i]-k,"",clRed);
//Form1->Series4->AddY(out[i],"",clBlack);
//Form1->Series5->AddY(out2[i],"",clYellow);

 }
delete[] in;  // прибираемся
delete[] in2;  // прибираемся
delete[] out;
delete[] out2;
return k;
}
//----------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
const int SIZEM=200;
double X[SIZEM][5]={0};
double Xt[5][SIZEM]={0};
if (Series3->XValues->Count()==0)
{
ShowMessage("Пустой график!!!");
return;
}
if (Series3->XValues->Count()>SIZEM)
{
ShowMessage("Слишком много точек - увеличь размер массива!");
return;
}
for(int i=0;i<Series3->XValues->Count();i++)
        {
        X[i][0]=Xt[0][i]=pow(Series3->XValues->Value[i],3);
        X[i][1]=Xt[1][i]=pow(Series3->XValues->Value[i],2);
        X[i][2]=Xt[2][i]=Series3->XValues->Value[i];
        X[i][3]=Xt[3][i]=1;
        X[i][4]=Xt[4][i]=-Series3->YValues->Value[i];
        }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
SeriesRes1->Clear();
SeriesRes2->Clear();
SeriesFFTRes->Clear();
out1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
SeriesHall1->Clear();  
SeriesHall2->Clear();
SeriesFFTHall->Clear();
out2->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
SeriesFaradey1->Clear();
SeriesFaradey2->Clear();
SeriesFFTFaradey->Clear();
out3->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
SeriesFoygt1->Clear();
SeriesFoygt2->Clear();
SeriesFFTFoygt->Clear();
out4->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyPress(TObject *Sender, char &Key)
{
if(Key=='.')
Key=',';
if(((int)Key<48 || (int)Key>57) && Key!=8 && Key!=',')
Key=0;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImpulsKillerClick(TObject *Sender)
{
double x,y;
x=Interval1->Text.ToDouble();
y=Interval2->Text.ToDouble();
KillImpulps(SeriesRes1,x,y);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
for(int i=0;i<SeriesRes1->XValues->Count();i++)
SeriesRes1->XValues->Value[i]*=10;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button10Click(TObject *Sender)
{
Series3->Active=!Series3->Active;
}
//---------------------------------------------------------------------------
// считает определитель 3го порядка, полагает что данные расположены в следующем виде:
//1 2 3
//4 5 6
//7 8 9

double det3(double *x)
{
return x[1]*x[5]*x[9]+x[2]*x[6]*x[7]+x[3]*x[4]*x[8]-x[3]*x[5]*x[7]-x[2]*x[4]*x[9]-x[3]*x[4]*x[8];
}
// считает определитель 4 порядка, разложением по первому столбцу
// предполагает что входные данные расположены следующим образом:
// 1 2 3 4
// 5 6 7 8
// 9 10 11 12
// 13 14 15 16
double det4(double *x)
{
double x1[9]={x[6],x[7],x[8],x[10],x[11],x[12],x[14],x[15],x[16]};
double x2[9]={x[5],x[7],x[8],x[9],x[11],x[12],x[13],x[15],x[16]};
double x3[9]={x[5],x[6],x[8],x[9],x[10],x[12],x[13],x[14],x[16]};
double x4[9]={x[5],x[6],x[7],x[9],x[10],x[11],x[13],x[14],x[15]};
return x[1]*det3(x1)-x[2]*det3(x2)+x[3]*det3(x3)+x[4]-det3(x4);
}

// решает уравнение записанное в виде матрицы, 4 го порядка, методом Крамера,
//входные данные:

/*1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 16
b1 b2 b3 b4*/

void Kramer(double *in,double *out)
{
double x1[16]={in[17],in[2],in[3],in[4],
               in[18],in[6],in[7],in[8],
               in[19],in[10],in[11],in[12],
               in[20],in[14],in[15],in[16]};
double x2[16]={in[1],in[17],in[3],in[4],
               in[5],in[18],in[7],in[8],
               in[9],in[19],in[11],in[12],
               in[13],in[20],in[15],in[16]};
double x3[16]={in[1],in[2],in[17],in[4],
               in[5],in[6],in[18],in[8],
               in[9],in[10],in[19],in[12],
               in[13],in[14],in[20],in[16]};
double x4[16]={in[1],in[2],in[3],in[17],
               in[5],in[6],in[7],in[18],
               in[9],in[10],in[11],in[19],
               in[13],in[14],in[15],in[20]};
double det;
double detx;
det=det4(in);
if(det==0)
{
ShowMessage("Главный определитель равен 0! Продолжение не возможно.");
return;
}

out[0]=det4(x1)/det;
out[1]=det4(x2)/det;
out[2]=det4(x3)/det;
out[3]=det4(x4)/det;

}


// функция трамплин для фильтра.  ----------------------------------------------
double Tr_Filter2(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh)
{
if(!inS->YValues->Count())   // если пустой - ошибка, убегаем.
{
ShowMessage("Пустой массив!");
return 0;
}
int size=inS->YValues->Count();  // получаем размер
double *in=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in[i]=inS->YValues->Value[i];
double *out=new double[size]; // выделяем память для выходных значений
double k=Filter2(in,out,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
 k*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count();// вычисляем сдвиг фаз
outS->Clear(); // чистим график, перед выводом
for(int i=0;i<size;i++) // выводим
outS->AddXY(inS->XValues->Value[i]-k,out[i],"",clBlue);

delete[] in;  // прибираемся
delete[] out;
return k;
}



void __fastcall TForm1::Button11Click(TObject *Sender)
{
Tr_Filter2((ResCurveIndex->ItemIndex==0?SeriesRes1:SeriesRes2),SeriesFFTRes,Lfilter1->Text.ToInt(),Fd1->Text.ToInt(),
Fp1->Text.ToInt(),Fz1->Text.ToInt());
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button12Click(TObject *Sender)
{
// увеличение количества точек на графике в два раза
int length=SeriesRes1->XValues->Count();

double *x=new double [length];
double *y=new double [length];
double *x1=new double [2*length];
double *y1=new double [2*length];
 
for(int i=0;i<length;i++)
 {
        x[i]=SeriesRes1->XValues->Value[i];
        y[i]=SeriesRes1->YValues->Value[i];
 }

 for(int i=0,j=0;i<length;i++,j+=2)
 {
        x1[j]=x[i];
        y1[j]=y[i];
        if (i==length-1)
        break;
        x1[j+1]=(x[i]+x[i+1])/2;
        y1[j+1]=(y[i]+y[i+1])/2;        
 }
 SeriesRes1->Clear();
 for(int i=0;i<2*length;i++)
 SeriesRes1->AddXY(x1[i],y1[i],"",clRed);





delete [] x;
delete [] y;
delete [] x1;
delete [] y1;
;
}
//---------------------------------------------------------------------------
void fillspace(double *xm,double *ym, int l,int r)
{

if(abs(r-l)==1)
{
xm[l+1]=(xm[l]+xm[r])/2;
ym[l+1]=(ym[l]+ym[r])/2;
return;
}
xm[(l+r)/2]=(xm[l]+xm[r])/2;
ym[(l+r)/2]=(ym[l]+ym[r])/2;
fillspace(xm,ym,l,(l+r)/2);
fillspace(xm,ym,(l+r)/2,r);
}
//---------------------------------------------------------------------------
// убирает точки соответствующие всплескам
void KillImpulps(TLineSeries * a,double x, double y)
{

int length=a->XValues->Count();
if(length==0)
{
ShowMessage("Пустой график!!!");
return;
}
double *xm=new double [length];
double *ym=new double [length];

for(int i=0;i<length;i++)
 {
        xm[i]=a->XValues->Value[i];
        ym[i]=a->YValues->Value[i];
 }


// границы интервала
//x=Interval1->Text.ToDouble();
//y=Interval2->Text.ToDouble();
double dx=fabs(xm[0]-x);// начинаем поиск ближайшей точки
int i;
double k=0;
for(i=0;i<length;i++)
{
        k=fabs(xm[i]-x);
        if(k>dx)  // как только пошли на увеличение - выходим из цикла
        {
             break;
        }
        dx=k;
}
int i1=i; // записываем первый индекс
dx=fabs(xm[i]-y);// ищем второй
for(;i<length;i++)
{
        k=fabs(xm[i]-y);
        if(k>dx)
        {
                break;
        }
        dx=k;
}
int i2=i;
int left=i1;
int right=i2;


fillspace(xm,ym,i1,i2);



 a->Clear();
 for(int i=0;i<length;i++)
 {
        a->AddXY(xm[i],ym[i],"",clRed);
                }
delete [] xm;
delete [] ym;
}
// в зависимости от активной вкладки вызывается нужна процедура очистки.
void __fastcall TForm1::N9Click(TObject *Sender)
{
int a=PC->ActivePageIndex;
switch (a)
{
case 1:
Button5Click(Sender);
case 2:
Button6Click(Sender);
case 3:
Button7Click(Sender);
case 4:
Button8Click(Sender);
}
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button13Click(TObject *Sender)
{
int len=SeriesRes1->XValues->Count();

double *x= new double [len];
for(int i=0;i<len;i++)
x[i]=SeriesRes1->XValues->Value[i];

double *y= new double [len];
for(int i=0;i<len;i++)
y[i]=SeriesRes1->YValues->Value[i];

double *yf= new double [len];
for(int i=0;i<len;i++)
yf[i]=SeriesFFTRes->YValues->Value[i];

for(int i=0;i<len;i++)
{
y[i]=fabs(fabs(y[i])-fabs(yf[i]));
SeriesRes2->AddXY(x[i],y[i],"",clBlack);
}

double M=0,sko=0;

for(int i=0;i<len;i++)
{
M+=y[i]/len;
}
for(int i=0;i<len;i++)
{
sko+=pow(y[i]-M,2);
}
sko/=len;
sko=sqrt(sko);

Memo2->Lines->Add("M=" + FloatToStr(M) + "\n");
Memo2->Lines->Add("sko=" + FloatToStr(sko) + "\n");

delete[] x;
delete[] y;
delete[] yf;

}
//---------------------------------------------------------------------------

