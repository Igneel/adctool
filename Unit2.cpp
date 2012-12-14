//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <inifiles.hpp>
#include "Unit2.h"
#include "Unit1.h"
#include <math.h>
#include <time.h>

TLineSeries *SaveSeries;
TStringList *tts=new TStringList();
TIniFile *Ini;  // указатель на Ini файл
String sFile;   // путь к файлу настроек
void WritingTSL (TLineSeries *SaveSeries,TStringList * tts); // формирование списка точек
void SavingResults(TStringList *tts,String FileName );
void PresetPointsSaver(TLineSeries * inS,double *preset,int length,double h=0.001);
String MakeFileName();


/*

BUGS FIXED:
*сохранение неправильных значений пресетов при перезапуске программы
*неправильное сохранение пресетов при нажатии кнопки "сохранить настройки"
и ранее существовавшем пресете с бОльшим кол-вом точек

*/


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSaveForm *SaveForm;
//---------------------------------------------------------------------------
__fastcall TSaveForm::TSaveForm(TComponent* Owner)
    : TForm(Owner)
{
//int indexp=Form1->PC->ActivePageIndex*4+1;
//int p=AllSeries[indexp];
//SaveSeries=(TLineSeries *)IntToPtr( p);
SaveSeries=(TLineSeries *)IntToPtr(AllSeries[Form1->PC->ActivePageIndex*4+1]);
ComboBox1->ItemIndex=Form1->PC->ActivePageIndex*4+1;
}
//---------------------------------------------------------------------------
void __fastcall TSaveForm::SaveModeClick(TObject *Sender)
{
Label1->Caption=SaveMode->Items->Strings[SaveMode->ItemIndex];    
}
//---------------------------------------------------------------------------
void __fastcall TSaveForm::SaveButClick(TObject *Sender)// щелкнули по кнопке
{

// настройка компонентов------------
Memo1->Text="Введите сюда описание файла/измеряемым результатам."
"Информация об имени файлов с результатами помещается "
"сюда автоматически";

SaveDialog1->FileName=MakeFileName();  // имя сформировали
//----------------------------------
// определяем что нужно сохранять

if(Results->ItemIndex==1) // если сохраняем только один график
{
    SaveForm->SaveDialog1->Title="Выберите файл для сохранения данных";
    if(SaveDialog1->Execute()) // если пользователь определился с файлом
        {
        //нужный указатель уже сохранен в SaveSeries
        SaveSeries=Form1->GetSelectedSeries(ComboBox1->ItemIndex);
        WritingTSL(SaveSeries,tts);
        SavingResults(tts,SaveDialog1->FileName);
        }
}
else // если сохраняем несколько графиков
{
    for(int i=0;i<LB1->Items->Count;i++)
    {
        SaveForm->SaveDialog1->Title="Выберите файл для сохранения данных";
        if(LB1->Selected[i])
        if(SaveDialog1->Execute()) // если пользователь определился с файлом
        { 
        ComboBox1->ItemIndex=i;
        ComboBox1Change(this);
        WritingTSL((TLineSeries *)IntToPtr(AllSeries[i]),tts);
        SavingResults(tts,SaveDialog1->FileName);
        Memo1->Text="Введите сюда описание файла/измеряемым результатам."
        "Информация об имени файлов с результатами помещается "
        "сюда автоматически";
        }
    }
}
tts->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TSaveForm::Edit1KeyPress(TObject *Sender, char &Key)
{
if(Key=='.')
Key=',';
;
}
//---------------------------------------------------------------------------

String MakeFileName()
{
SYSTEMTIME st;
GetSystemTime(&st);          // получаем дату
String ti;
ti="data."+(st.wDay<10?("0"+IntToStr(st.wDay)):IntToStr( st.wDay))+".";
ti+=(st.wMonth<10?("0"+IntToStr(st.wMonth)):IntToStr( st.wMonth))+".";
ti+=IntToStr( st.wYear);     // формируем имя файла по умолчанию

return ti+".txt";
}

// формируем описание
void SavingResults(TStringList *tts,String FileName )
{


SaveForm->Memo1->Lines->Insert(0,"Файл описания результатов.  "+ SaveForm->Memo2->Text);

if(SaveForm->Prepare->ItemIndex==0) // если пишем всё в один файл
{
SaveForm->Memo1->Lines->Add("Данные идут в этом же файле ниже");
tts->Add(SaveForm->Memo1->Text);
}

//------------------------------------------------------------------------------

tts->SaveToFile(FileName); // сохраняем данные

if(SaveForm->Prepare->ItemIndex==1) // если в разные файлы
{
    tts->Clear();    // ощищаем
    SaveForm->Memo1->Lines->Add("Файл с данными сохранен под именем "+FileName);
    tts->Add(SaveForm->Memo1->Text);    // пишем информацию о данных

    SaveForm->SaveDialog1->Title="Выберите файл для сохранения описания";
    if(SaveForm->SaveDialog1->Execute()) // сохраняем
        tts->SaveToFile(SaveForm->SaveDialog1->FileName);
}

}

// формирование содержимого файла ---------------------------------------------------------
void WritingTSL (TLineSeries *SaveSeries,TStringList * tts)
{
//SaveForm->Memo1->Lines->Add("WritingTSL");
if(SaveSeries->XValues->Count()==0) // если график пуст
{
ShowMessage("Этот график пуст! Сохранять нечего:)");
return;
}
//-----режим сохранения --------------------------------------------------------
double prev;
int length;
length=SaveForm->Memo3->Lines->Count;
        double *presetM=new double [length];
switch(SaveForm->SaveMode->ItemIndex) // выбираем режим сохранения результатов
{
    case 0: // заданный шаг по оси х
        tts->Add(FloatToStr(SaveSeries->XValues->Value[0])+(char)9+
        FloatToStr(SaveSeries->YValues->Value[0]));
        prev=SaveSeries->XValues->Value[0];
        for(int i=1;i<SaveSeries->XValues->Count();i++)
        {
            if(fabs(prev-SaveSeries->XValues->Value[i])>=StrToFloat(SaveForm->Edit1->Text))
            {
                tts->Add(FloatToStr(SaveSeries->XValues->Value[i])+(char)9+
                FloatToStr(SaveSeries->YValues->Value[i]));
                prev=SaveSeries->XValues->Value[i];
            }
        }
break;
    case 1: // заданный шаг по оси y
        tts->Add(FloatToStr(SaveSeries->XValues->Value[0])+(char)9+
        FloatToStr(SaveSeries->YValues->Value[0]));
        prev=SaveSeries->YValues->Value[0];
        for(int i=1;i<SaveSeries->XValues->Count();i++)
        {
            if(fabs(prev-SaveSeries->YValues->Value[i])>=StrToFloat(SaveForm->Edit1->Text))
            {
                tts->Add(FloatToStr(SaveSeries->XValues->Value[i])+(char)9+
                FloatToStr(SaveSeries->YValues->Value[i]));
                prev=SaveSeries->YValues->Value[i];
            }
        }
break;
//------------------------------------------------------------------------------------
 case 2: // заданные точки

        for(int i=0;i<length;i++)
        {
        String s=SaveForm->Memo3->Lines->Strings[i];
        if(s!="")
        presetM[i]=s.ToDouble();
        else
        continue;
        }
        PresetPointsSaver(SaveSeries,presetM,length);

        /*
        tts->Add(FloatToStr(SaveSeries->XValues->Value[0])+(char)9+
        FloatToStr(SaveSeries->YValues->Value[0]));
        prev=SaveSeries->YValues->Value[0];
        for(int i=1;i<SaveSeries->XValues->Count();i++)
        {
            if(fabs(prev-SaveSeries->YValues->Value[i])>=StrToFloat(SaveForm->Edit1->Text))
            {
                tts->Add(FloatToStr(SaveSeries->XValues->Value[i])+(char)9+
                FloatToStr(SaveSeries->YValues->Value[i]));
                prev=SaveSeries->YValues->Value[i];
            }
        }                 */
        delete [] presetM;
break;
//-----------------------------------------------------------------------------------------
default: // все значения
    for(int i=0;i<SaveSeries->XValues->Count();i++)
    {
        tts->Add(FloatToStr(SaveSeries->XValues->Value[i])+(char)9+
        FloatToStr(SaveSeries->YValues->Value[i])); // все значения

    }
}
  
}



void __fastcall TSaveForm::FormCreate(TObject *Sender)
// вызов для отображения пояснений к имена графиков
{
ComboBox1Change(this);
HANDLE f;
sFile= ExtractFilePath(Application->ExeName)+"\\SaveSettings.ini";
if(!FileExists(sFile))
{
    f=CreateFileA(sFile.c_str(),GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(f!=INVALID_HANDLE_VALUE)
    {
        CloseHandle(f);

        Ini= new TIniFile(sFile);

        //сохранеие настроек
        Ini->WriteInteger("Main","Сохраняемые результаты",Results->ItemIndex);
        Ini->WriteInteger("Main","Способ сохранения",Prepare->ItemIndex);
        Ini->WriteInteger("Main","Режим сохранения",SaveMode->ItemIndex);
        Ini->WriteString("Main","Заданный шаг",Edit1->Text);
        Ini->WriteInteger("Магнитное поле","Количество предустановок",1);
        for(int i=0;i<1;i++)
        {
            double x=-2;
            for(int j=0;j<21;j++,x+=0.2)
                if(fabs(x)>1E-10)
                    Ini->WriteString("Preset"+IntToStr(i+1),j,x);
                else
                    Ini->WriteString("Preset"+IntToStr(i+1),j,0);
        }
        Ini->UpdateFile();
    }
}
else
{
Ini= new TIniFile(sFile);
Results->ItemIndex=Ini->ReadInteger("Main","Сохраняемые результаты",1);
Prepare->ItemIndex=Ini->ReadInteger("Main","Способ сохранения",1);
SaveMode->ItemIndex=Ini->ReadInteger("Main","Режим сохранения",3);
Edit1->Text=Ini->ReadString("Main","Заданный шаг","0,02");
int n=Ini->ReadInteger("Магнитное поле","Количество предустановок",1);
for(int i=0;i<n;i++)
{
PresetPoints->Items->Add("Preset"+IntToStr(i+1));
}
PresetPoints->ItemIndex=0;
PresetPointsChange(this);
//чтение настроек
}
}
//---------------------------------------------------------------------------

// пояснения к именам графиков,  а так же меняется указатель на текущий график
void __fastcall TSaveForm::ComboBox1Change(TObject *Sender) //
{
switch(ComboBox1->ItemIndex)
{
case 0:
case 1:
case 2:
case 3:
case 4:
Memo2->Text="Режим отладки";
break;
case 5:
Memo2->Text="Магнитосопротивление. График 1. Если использовалась"
" подгонка - она присутствует на этом графике";
break;
case 6:
Memo2->Text="Магнитосопротивление. График 2. Если использовалась"
" подгонка - она НЕ присутствует на этом графике!";
break;
case 7:
Memo2->Text="Магнитосопротивление. Разложение по частотам.";
break;
case 8:
Memo2->Text="Магнитосопротивление. График 3. после обработки преобразованием Фурье";
break;
case 9:
Memo2->Text="Эффект Холла. График 1. Если использовалась"
" подгонка - она присутствует на этом графике";
break;
case 10:
Memo2->Text="Эффект Холла. График 2. Если использовалась"
" подгонка - она НЕ присутствует на этом графике!";
break;
case 11:
Memo2->Text="Эффект Холла. Разложение по частотам.";
break;
case 12:
Memo2->Text="Эффект Холла. График 3. после обработки преобразованием Фурье";
break;
case 13:
Memo2->Text="Фотопроводимость в геометрии Фарадея. График 1. Если использовалась"
" подгонка - она присутствует на этом графике";
break;
case 14:
Memo2->Text="Фотопроводимость в геометрии Фарадея. График 2. Если использовалась"
" подгонка - она НЕ присутствует на этом графике!";
break;
case 15:
Memo2->Text="Фотопроводимость в геометрии Фарадея. Разложение по частотам.";
break;
case 16:
Memo2->Text="Фотопроводимость в геометрии Фарадея. График 3. после обработки преобразованием Фурье";
break;
case 17:
Memo2->Text="Фотопроводимость в геометрии Фойгта. График 1. Если использовалась"
" подгонка - она присутствует на этом графике";
break;
case 18:
Memo2->Text="Фотопроводимость в геометрии Фойгта. График 2. Если использовалась"
" подгонка - она НЕ присутствует на этом графике!";
break;
case 19:
Memo2->Text="Фотопроводимость в геометрии Фойгта. Разложение по частотам.";
break;
case 20:
Memo2->Text="Фотопроводимость в геометрии Фойгта. График 3. после обработки преобразованием Фурье";
break;
}
SaveSeries=(TLineSeries *)IntToPtr(AllSeries[ComboBox1->ItemIndex]);
}
//---------------------------------------------------------------------------


// сохраняем один или много графиков
void __fastcall TSaveForm::ResultsClick(TObject *Sender)
{
if(Results->ItemIndex==0)
{
ComboBox1->Enabled=0;
LB1->Enabled=1;
}
if(Results->ItemIndex==1)
{
ComboBox1->Enabled=1;
LB1->Enabled=0;
}
}
//---------------------------------------------------------------------------
// запоминание нового набора точек
void __fastcall TSaveForm::Button1Click(TObject *Sender)
{
// читаем количество предустановок
int n=Ini->ReadInteger("Магнитное поле","Количество предустановок",1);
// увеличиваем их на 1
Ini->WriteInteger("Магнитное поле","Количество предустановок",++n);
// а теперь самое интересное - пишем текущее отображение из memo3 в новую предустановку
Ini->EraseSection("Preset"+IntToStr(n));
for(int i=0;i<Memo3->Lines->Count;i++)
{
 // очищаем ini мало ли чего там было
// записываем точки
Ini->WriteString("Preset"+IntToStr(n),i,Memo3->Lines->Strings[i]);
}
// загружаем в программу новый пресет из ini-файла
for(int i=PresetPoints->Items->Count;i<n;i++)
{
PresetPoints->Items->Add("Preset"+IntToStr(i+1));
}
Ini->UpdateFile();
}
//---------------------------------------------------------------------------
// загружаем нужный набор точек
void __fastcall TSaveForm::PresetPointsChange(TObject *Sender)
{

String Section=PresetPoints->Items->Strings[PresetPoints->ItemIndex];
int i=0;
Memo3->Clear();
while(Ini->ValueExists(Section,IntToStr(i)))
Memo3->Lines->Add(Ini->ReadString(Section,i++,0));
}
//---------------------------------------------------------------------------



void __fastcall TSaveForm::FormClose(TObject *Sender, TCloseAction &Action)
{
//сохранеие настроек
        Ini->WriteInteger("Main","Сохраняемые результаты",Results->ItemIndex);
        Ini->WriteInteger("Main","Способ сохранения",Prepare->ItemIndex);
        Ini->WriteInteger("Main","Режим сохранения",SaveMode->ItemIndex);
        Ini->WriteString("Main","Заданный шаг",Edit1->Text);
        /*Ini->WriteInteger("Магнитное поле","Количество предустановок",PresetPoints->Items->Count);
        for(int i=0;i<PresetPoints->Items->Count;i++)
        {
            double x=-2;
            for(int j=0;j<21;j++,x+=0.2)
                if(fabs(x)>1E-10)
                    Ini->WriteString("Preset"+IntToStr(i+1),j,x);
                else
                    Ini->WriteString("Preset"+IntToStr(i+1),j,0);
        }         */
        Ini->UpdateFile();

SaveForm->DoDestroy();
SaveForm->~TSaveForm();
SaveForm=NULL;
}
//---------------------------------------------------------------------------
int PresetPointFinder(double *in,int N, double preset,double delta)
{
double pd=fabs(in[0]-preset);
for(int j=0;j<N;j++)
{
if(fabs(in[j]-preset)>pd)
break;
pd=fabs(in[j]-preset);
if(fabs(in[j]-preset)<=delta)
        return j;


}
delta*=2;
pd=fabs(in[0]-preset);
for(int j=0;j<N;j++)
{
if(fabs(in[j]-preset)>pd)
break;
pd=fabs(in[j]-preset);
if(fabs(in[j]-preset)<=delta)
        return j;
}
delta*=2;
pd=fabs(in[0]-preset);
for(int j=0;j<N;j++)
{
if(fabs(in[j]-preset)>pd)
break;
pd=fabs(in[j]-preset);
if(fabs(in[j]-preset)<=delta)
        return j;
}
return -1;
}

void PresetPointsSaver(TLineSeries * inS,double *preset,int length,double h)
{
int N=inS->XValues->Count();
double *in=new double [N];
for(int i=0;i<N;i++)
in[i]=inS->XValues->Value[i];
double delta=0.5*h;

for(int i=0;i<length;i++)
{
int index=PresetPointFinder(in,N, preset[i], delta*10);
if(index==-1) continue;
tts->Add(FloatToStr(inS->XValues->Value[index])+(char)9+inS->YValues->Value[index]);
}

delete [] in;
}



void __fastcall TSaveForm::Button2Click(TObject *Sender)
{
// читаем количество предустановок
int n=Ini->ReadInteger("Магнитное поле","Количество предустановок",1);
// уменьшаем их на 1
Ini->WriteInteger("Магнитное поле","Количество предустановок",--n);
Ini->EraseSection(PresetPoints->Items->Strings[PresetPoints->ItemIndex]);
}
//---------------------------------------------------------------------------

