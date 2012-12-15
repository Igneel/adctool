//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
// данный путь должен совпадать с папкой куда установлены драйвера и примеры
#include "J:\Program Files (x86)\Vt\Samples\INTERFACES\IADCDevice.h"
#include "J:\Program Files (x86)\Vt\Samples\INTERFACES\IFactory.h"
#include "J:\Program Files (x86)\Vt\Samples\INTERFACES\DllClient.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Menus.hpp>
#include <Series.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TPageControl *PC;
    TTabSheet *FirstTab;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TChart *Chart1;
    TLineSeries *Series2;
    TMemo *Memo1;
    TButton *uiControl;
    TComboBox *uiDriverName;
    TComboBox *uiDRQ;
    TComboBox *Chan1;
    TComboBox *Chan2;
    TComboBox *uiBase;
    TButton *Button1;
    TTrackBar *uiFreq;
    TTrackBar *uiBlockSize;
    TEdit *uiBlockSize2;
    TEdit *uiFrenq;
    TComboBox *uiIRQ;
    TTabSheet *Resistance;
    TChart *ChartResist;
    TButton *uiResControl;
    TEdit *CurrentRes;
    TLabel *Label9;
    TLineSeries *SeriesRes1;
    TButton *uiResFeat;
    TMemo *Memo2;
    TRadioGroup *ResCurveIndex;
        TButton *Button3;
    TTabSheet *HallEffect;
    TTabSheet *Faradey;
    TTabSheet *Foygt;
    TLineSeries *SeriesRes2;
    TChart *ChartHall;
    TRadioGroup *HallCurveIndex;
    TButton *uiHallFeat;
    TButton *uiHallControl;
    TMemo *Memo3;
    TEdit *CurrentHall;
    TLabel *Label11;
    TLineSeries *SeriesHall1;
    TLineSeries *SeriesHall2;
    TChart *ChartFaradey;
    TLineSeries *SeriesFaradey1;
    TLineSeries *SeriesFaradey2;
    TLabel *Label12;
    TEdit *CurrentFaradey;
    TComboBox *GainKoefFaradey;
    TLabel *Label13;
    TRadioGroup *FaradeyCurveIndex;
    TButton *uiFaradeyControl;
    TButton *uiFaradeyFeat;
    TMemo *Memo4;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TChart *ChartFoygt;
    TLineSeries *SeriesFoygt1;
    TLineSeries *SeriesFoygt2;
    TComboBox *GainKoefFoygt;
    TLabel *Label10;
    TEdit *CurrentFoygt;
    TLabel *Label14;
    TRadioGroup *FoygtCurveIndex;
    TButton *uiFoygtControl;
    TButton *uiFoygtFeat;
    TMemo *Memo5;
    TLineSeries *SeriesFFTFaradey;
    TButton *uiFFTRes;
    TLineSeries *SeriesFFTRes;
    TButton *Button2;
    TLineSeries *Series1;
    TLineSeries *Series4;
        TEdit *Lfilter1;
    TButton *uiFFTHall;
    TButton *uiFFTFaradey;
    TButton *uiFFTFoygt;
    TLineSeries *SeriesFFTHall;
    TStatusBar *StatusBar;
    TLineSeries *SeriesFFTFoygt;
    TLabel *Label15;
        TLineSeries *Series3;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
    TLineSeries *Series5;
    TMemo *Memo6;
        TLineSeries *out1;
        TLineSeries *out2;
        TLineSeries *out3;
        TLineSeries *out4;
        TButton *Button4;
        TEdit *Fd1;
        TEdit *Fp1;
        TEdit *Fz1;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TButton *Button5;
        TButton *Button6;
        TButton *Button7;
        TButton *Button8;
        TLabel *Label16;
        TEdit *Lfilter2;
        TLabel *Label22;
        TEdit *Fd2;
        TEdit *Fp2;
        TLabel *Label23;
        TLabel *Label24;
        TEdit *Fz2;
        TLabel *Label17;
        TEdit *Lfilter3;
        TEdit *Fd3;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *Fp3;
        TEdit *Fz3;
        TLabel *Label27;
        TEdit *Fd4;
        TLabel *Label18;
        TEdit *Lfilter4;
        TLabel *Label28;
        TLabel *Label29;
        TEdit *Fp4;
        TEdit *Fz4;
        TLabel *Label30;
        TEdit *Interval1;
        TEdit *Interval2;
        TButton *ImpulsKiller;
        TButton *Button9;
        TMenuItem *N8;
        TMenuItem *N101;
        TButton *Button10;
        TEdit *Edit2;
        TButton *Button11;
        TButton *Button12;
        TMenuItem *N9;
        TMenuItem *N10;
        TButton *Button13;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall uiDriverNameChange(TObject *Sender);
    void __fastcall uiBaseChange(TObject *Sender);
    void __fastcall uiDRQChange(TObject *Sender);
    void __fastcall uiIRQChange(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall uiControlClick(TObject *Sender);
    void __fastcall uiFrenqChange(TObject *Sender);
    void __fastcall uiBlockSize2Change(TObject *Sender);
    void __fastcall uiBlockSizeChange(TObject *Sender);
    void __fastcall uiFreqChange(TObject *Sender);
    void __fastcall uiResFeatClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
    void __fastcall uiHallFeatClick(TObject *Sender);
    void __fastcall uiFaradeyFeatClick(TObject *Sender);
    void __fastcall uiFoygtFeatClick(TObject *Sender);
    void __fastcall uiFFTResClick(TObject *Sender);
    void __fastcall uiFFTHallClick(TObject *Sender);
    void __fastcall uiFFTFaradeyClick(TObject *Sender);
    void __fastcall uiFFTFoygtClick(TObject *Sender);
    void __fastcall N5Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);

    TLineSeries * __fastcall GetCurrentSeries(int curve);
    TLineSeries * __fastcall GetSelectedSeries(int curve);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
        void __fastcall ImpulsKillerClick(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall N9Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
    

private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
      friend DWORD WINAPI DriverThread (LPVOID );
      void __fastcall LoadLa7(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
extern int                ADCBase;     // базовый адрес платы
extern int                ADCDRQ;      // номер DRQ
extern int                ADCIRQ;      // номер IRQ
extern int                DriverIndex; // номер драйвера в списке
extern TCHAR              dev[10][20]; // список драйверов
extern int NumberOfChannels ;           // количество сканируемых каналов
extern int BlockSize;                   // размер блока
extern int Frequency;                   //частота
extern double h;                        // шаг по магнитному полю.
extern int AllSeries[26];


//void Ftransform(TLineSeries * data,TLineSeries * outSpectr,TLineSeries * newGraphic,int wCrop); // преобразование Фурье
void  OddFeat(TLineSeries* a, long index);
void  EvenFeat(TLineSeries* a, long index);
void  MidCurve(TLineSeries* a, TLineSeries* b, long index);
void  FoygtFeat(TLineSeries* a,TLineSeries* b, long index);
//void FFT(TLineSeries * data, TLineSeries * complexFFT, TLineSeries *out, int crop);
double Filter (const double in[], double out[], int sizeIn, int length=20, double Fdisk=5000, double Fpropysk=15,double Fzatyh=25);
double Tr_Filter(TLineSeries *in,TLineSeries *out,int length=20,double Fdisk=5000, double Fpropysk=15,double Fzatyh=25 );
void KillImpulps(TLineSeries * a,double Left, double Right);

double det3(double *x);
double det4(double *x);
void Kramer(double *in,double * out);


//---------------------------------------------------------------------------
#endif
