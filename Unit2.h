//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSaveForm : public TForm
{
__published:	// IDE-managed Components
    TSaveDialog *SaveDialog1;
    TButton *SaveBut;
    TRadioGroup *Prepare;
    TRadioGroup *Results;
    TRadioGroup *SaveMode;
    TEdit *Edit1;
    TMemo *Memo1;
    TLabel *Label1;
        TComboBox *ComboBox1;
        TLabel *Label2;
        TMemo *Memo2;
        TListBox *LB1;
        TMemo *Memo3;
        TLabel *Label3;
        TButton *Button1;
        TComboBox *PresetPoints;
        TButton *Button2;
    void __fastcall SaveModeClick(TObject *Sender);
    void __fastcall SaveButClick(TObject *Sender);
    void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall ResultsClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall PresetPointsChange(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TSaveForm(TComponent* Owner);
    
};
//---------------------------------------------------------------------------
extern PACKAGE TSaveForm *SaveForm;
int AllSeries[26];
//---------------------------------------------------------------------------
#endif
