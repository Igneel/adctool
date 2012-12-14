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
bool             StopDAQ = true;      // ����, �� ���� �������� ����
HANDLE           hIRQEvent;           // ��������� �� �������
HANDLE           hDriverThread = 0;   // ��������� �� �����
const int        DriverThreadStackSize = 4096;// ������ ����� ������
DWORD WINAPI     DriverThread (LPVOID );// ������� ������
short            DrvData[DATA_SIZE]; // ������ ������
double x,y;                // ��� �������� ����������� �������� �������.
double MZR=0.30517e-3;     // ������� �������� ������
// ����������� ������������� �� ���������� ����:




void Stop()            // ��������� ���������
{
    StopDAQ = true; // ���������� ����
    if ( hIRQEvent )
        CloseHandle (hIRQEvent); // ��������� �����
    hIRQEvent = hDriverThread = 0;
}

void Start()
{
    DWORD ThreadId;
    StopDAQ = false;
    // �������� Event'� ��� ������� �� ����������
    hIRQEvent = CreateEvent (NULL,FALSE,FALSE,NULL);
    // ������ ������ �����
    hDriverThread = CreateThread (NULL,
                                 DriverThreadStackSize,
                                 DriverThread,
                                 NULL,
                                 0,
                                 &ThreadId);
    // ��������� ���������� ������
    SetThreadPriority(hDriverThread, THREAD_PRIORITY_NORMAL);
}
//---------- ������� ������ --------------------------------------------------

/*
�������� ��� ���:
1. ����� �������, ������������ ����� ������
2. ������������ ����� ���������� ������
3. ����� ����������� � �������� �����
4. ������ ����� ���������� 361 ���, � �����������
5. ����� ������ �� ������
6. ��������� ����, ���� ���������� ��� ��� ������������ ����������
7. ��� ����������


 */


DWORD WINAPI DriverThread (LPVOID )
{
    double prevB=-10;
    short Chan1d[512]={0}; // ������ �������(��������) ������
    short Chan2d[512]={0}; // ������ �������(�������) ������
    float m_fFreq=0;       // ������� ������������ ���
    //-------------------------------------------------------------------------
    // ��������� ����� � ��������� � ��������� ����������
    DllClient LAxClient;
    // ��������� �������
    IFactory *LAxFactory = (IFactory*)LAxClient.LoadRegistered(
        CompanyNameDRV, ProductNameDRV, VersionNameDRV,
        dev[DriverIndex], "StaticFactory");
    if(LAxFactory == 0) return -11;
    // �������� ��������� ��������
    IADCDevice* pADC = (IADCDevice*)LAxFactory->Create(_T("IADCDevice"), 0);
    if(pADC == 0)       return -12;
    // ��������� ���������� ����������: �������� ������, DRQ, IRQ
    pADC->Setup(ADCBase, ADCDRQ, ADCIRQ, hIRQEvent);
    int z;
    // ������������� ���������������� ����� ������
    //--------------------------------------------------------------------------
    pADC->Get(ADC_SET_ADCMODE_DIFF,&z );
    //--------------------------------------------------------------------------

    // �������������� ��������� ���������� ���������
    ADCParametersDMA a;
    a.m_nStartOf =       ADCPARAM_START_TIMER; // ������ ��� �� �������
    a.m_nIntOf =         ADCPARAM_INT_TC;      // ���������� �� ������� TC
    a.m_nDMABlockSize =  BlockSize;              // ������ ����� ���������� ������
    a.m_nDMAMode =       0;                    // ����� DMA �������� (0 - Single, 1 - AutoInit)
    a.m_fFreqStart =     Frequency;               // ������� �������������, ��
    a.m_nTimerStart =    0;                    // ����� ������ ������� ��� �������� ���
    a.m_fFreqPack =      0;                    // ������� ������ (����� ��������)
    a.m_nTimerPack =     0;                    // ����� ������ ������� ��� ������������ ������
    a.m_nFirstChannel =  Form1->Chan1->ItemIndex;// ������ ����� � ������ ����������������
    a.m_nChannelNumber = NumberOfChannels;     // ������� ����� ������� �����������
    a.m_nGain =          1;                    // ����������� ��������
    // ������������� ���������� ���������
    pADC->Init(ADC_INIT_MODE_INIT, &a, &m_fFreq);
    while (!StopDAQ)
    {
        // ������ ��������������
        Sleep(500);
        // ������� ������ ����, ���� ����� ���������� �� ����!
        pADC->Start();
        // �������� ����������

        //-------------------�������-----------------------------------------
       /* for(int b=0;b<100;b++)
        {
        x=rand()%2;
        y=rand()%6;
       switch(Form1->PC->ActivePageIndex)
            {
            case 0:
            // ������ �������, ����� ������� ������� "FirstTab"
            Form1->Series1->AddY(x,"",clRed);
            Form1->Series2->AddY(y,"",clGreen);
            Form1->Series3->AddXY(y,x,"",clYellow);
            Form1->Memo1->Lines->Add(Form1->Series1->XValues->Last());
            break;
            case 1:
            // ��������� ��������������������
            if(!Form1->ResCurveIndex->ItemIndex)
            Form1->SeriesRes1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesRes2->AddXY(y,x,"",clBlue);
            break;
            case 2:
            // ������ �����
            if(!Form1->HallCurveIndex->ItemIndex)
            Form1->SeriesHall1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesHall2->AddXY(y,x,"",clBlue);
            ;
            break;
            case 3:
            // ���������������� � ��������� �������
            if(!Form1->FaradeyCurveIndex->ItemIndex)
            Form1->SeriesFaradey1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFaradey2->AddXY(y,x,"",clBlue);
            break;

            case 4:
            // ���������������� � ��������� ������
            if(!Form1->FoygtCurveIndex->ItemIndex)
            Form1->SeriesFoygt1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFoygt2->AddXY(y,x,"",clBlue);
            default:
            ShowMessage("������! �������� ������� �������� ����������� ���������!");
            }

          }  */

        //---------------------------����� �������----------------------------
        //---------- ��������� ������ ----------------------------------------
        if ( WaitForSingleObject(hIRQEvent,500) == WAIT_OBJECT_0 )
        // ���� �� ��������� �������� �� ������� ����������   // �� ���������)
        {
            // �������� ���������� ��������� �� ������ DMA
            pADC->GetData(ADC_DATA_MODE_DATAFROMDMA | ADC_DATA_MODE_CONVERT2INT16,
             (char *)DrvData, BlockSize, 0);

            // ������������� ������ �� �������
            for(int i=0,b=0;(i+2)<1024;i+=NumberOfChannels,b++)
            {
            
            /* ������������������ ������� �����: 2 1 0 */
            // ����� ����� 2 ������������, ��� ���������� ��� ������������
            //------------------------------------------------------------------
            Chan1d[b]=DrvData[i+2];
            Chan2d[b]=DrvData[i+1];
            //------------------------------------------------------------------

            }
            // ����������
            heapSort(Chan1d,BlockSize/3);
            heapSort(Chan2d,BlockSize/3);
            // ���������� �������� ��������
            x=Middle(Chan1d,BlockSize/3)*MZR;
            y=Middle(Chan2d,BlockSize/3)*MZR;

            //����� �� ������
            // � ����������� �� ���� ��� ��������
            //Form1->Memo1->Lines->Add(y-prevB);
            //Form1->Memo1->Lines->Add(prevB);
            if(fabs(y-prevB)>=h)   // ���������� �� ���, ���������� ��� �����
            {
            prevB=y;
            y*=10;     // ��������� ������ � ��
            //if(prevB-2<=0.01) // ���� �������� 2 ��, ����������� ���������
            //StopDAQ=true;
            switch(Form1->PC->ActivePageIndex)
            {
            case 0:
            // ������ �������, ����� ������� ������� "FirstTab"
            Form1->Series1->AddY(x,"",clRed);
            Form1->Series2->AddY(y,"",clGreen);
            Form1->Series3->AddXY(y,x,"",clYellow);
            Form1->Memo1->Lines->Add(Form1->Series1->XValues->Last());
            break;
            case 1:
            // ��������� ��������������������
            if(!Form1->ResCurveIndex->ItemIndex)
            Form1->SeriesRes1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesRes2->AddXY(y,x,"",clBlue);
            break;
            case 2:
            // ������ �����
            if(!Form1->HallCurveIndex->ItemIndex)
            Form1->SeriesHall1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesHall2->AddXY(y,x,"",clBlue);
            ;
            break;
            case 3:
            // ���������������� � ��������� �������
            if(!Form1->FaradeyCurveIndex->ItemIndex)
            Form1->SeriesFaradey1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFaradey2->AddXY(y,x,"",clBlue);
            break;

            case 4:
            // ���������������� � ��������� ������
            if(!Form1->FoygtCurveIndex->ItemIndex)
            Form1->SeriesFoygt1->AddXY(y,x,"",clRed);
            else
            Form1->SeriesFoygt2->AddXY(y,x,"",clBlue);
            default:
            ShowMessage("������! �������� ������� �������� ����������� ���������!");
            }
            }


            // ����� ����������
            // �� �� ���� � �������� �� �������� � ���������� �� �������������
            pADC->ClearInterrupt();
        }

        // ������� ��������������
        pADC->Stop();
    }

    // ������������ ��������
    prevB=-10;
    pADC->Release();

    return 1;
}



//----- ������� ������������� ���������� ------------------------------------
// ����� ������������ � ������ ������� ����������, �.�. ��������� �����������
// ������� ����� ������ ������� �������...
template<typename T>
void downHeap(T a[], long k, long n) {
        //  ��������� ����������� ���������� ��������
        //  �� ���������: a[k+1]...a[n]  - ��������
        //  �����:  a[k]...a[n]  - ��������
        T new_elem;
        long child;
        new_elem = a[k];

        while(k <= n/2) {               // ���� � a[k] ���� ����
                child = 2*k;
                //  �������� �������� ���� 
                if( child < n && a[child] < a[child+1] ) 
                        child++;
                if( new_elem >= a[child] ) break; 
                // ����� 
                a[k] = a[child];        // ��������� ���� ������ 
                k = child;
        }
        a[k] = new_elem;
}

template<typename T>
void heapSort(T a[], long size) {
        long i;
        T temp;

        // ������ ��������
        for(i=size/2-1; i >= 0; i--) downHeap(a, i, size-1);

        // ������ a[0]...a[size-1] ��������

        for(i=size-1; i > 0; i--) {
                // ������ ������ � ���������
                temp=a[i]; a[i]=a[0]; a[0]=temp;
                // ��������������� ��������������� a[0]...a[i-1]
                downHeap(a, 0, i-1);
        }
}
// ���������� �������� �������� �������������� �������, ������� ��������
// ��������� ��� �������� ��������� �������.
// ���������� ��������� �� ������ � ��� ������
template<typename T>
double Middle(T a[], long size)
{
int i;
double Sum=0;
for(i=size/4;i<size-size/4;i++) 
    Sum+=a[i];
return Sum/(size%2==0?(size/2+1):(size/2+2));
} 
// ������� �������������� �����
// ���������� ���������� Alglab
