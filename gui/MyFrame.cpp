/*  This file is part of k8055gui.

    k8055gui is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    k8055gui is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Copyleft (C) 2008 by Etienne Mouragnon
    echienne@gmail.com
*/

#include "MyFrame.h"
#include <k8055.h>
#include <iostream>

using namespace std;

/* set debug to 0 to not print excess info */
static int DEBUG = 0;

/* constructor, build the graphical interface */
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{

  panel= new wxPanel(this);

  //Card Address
  new wxStaticBox(panel,-1,_("Card Address"), wxPoint(5,6),wxSize(112,45));
  SK5=new wxCheckBox(panel,IDC_SK5,_("SK5"), wxPoint(10,20));
  SK5->SetValue(1);
  SK6=new wxCheckBox(panel,IDC_SK6,_("SK6"), wxPoint(62,20));
  SK6->SetValue(1);

  Connect=new wxButton(panel,IDC_CONNECT,_("Connect"),wxPoint(5,60),wxSize(112,30));

  //Connection status
  ConnectionStatus=new wxTextCtrl(panel,-1,_("..."),wxPoint(8,95),wxSize(112,40),wxNO_BORDER|wxTE_LEFT|wxTE_MULTILINE);
  ConnectionStatus->SetEditable(false);
  ConnectionStatus->SetBackgroundColour(panel->GetBackgroundColour());

  //Test buttons
  SetAllDigit=new wxButton(panel,IDC_SETALLDIGITAL,_("Set All Digital"),wxPoint(5,160),wxSize(112,30));
  ClearAllDigit=new wxButton(panel,IDC_CLEARALLDIGITAL,_("Clear All Digital"),wxPoint(5,190),wxSize(112,30));

  SetAllAnalg=new wxButton(panel,IDC_SETALLANALOG,_("Set All Analog"),wxPoint(5,230),wxSize(112,30));
  ClearAllAnalg=new wxButton(panel,IDC_CLEARALLANALOG,_("Clear All Analog"),wxPoint(5,260),wxSize(112,30));

  OutputTest=new wxToggleButton(panel,IDC_OUTPUTTEST,_("Output Test"),wxPoint(5,306),wxSize(112,30));

  //Sliders
  new wxStaticBox(panel,-1,_("DA1"), wxPoint(130,6),wxSize(50,330));
  AnalgOutput1=new wxSlider(panel,IDC_DA1, 0 , 0, 255, wxPoint(140,30), wxSize(30,295),wxSL_VERTICAL|wxSL_INVERSE|wxSL_LABELS);

  new wxStaticBox(panel,-1,_("DA2"), wxPoint(190,6),wxSize(50,330));
  AnalgOutput2=new wxSlider(panel,IDC_DA2, 0 , 0, 255, wxPoint(200,30), wxSize(30,295),wxSL_VERTICAL|wxSL_INVERSE|wxSL_LABELS);

  //Gauges
  new wxStaticBox(panel,-1,_("AD1"), wxPoint(250,6),wxSize(50,330));
  AnalgInput1=new wxGauge(panel,-1,255, wxPoint(265,30), wxSize(20,270),wxGA_VERTICAL);

  AnalgInput1Text=new wxTextCtrl(panel,-1,_("0"),wxPoint(255,310),wxSize(40,20),wxNO_BORDER|wxTE_CENTRE);
  AnalgInput1Text->SetEditable(false);
  AnalgInput1Text->SetBackgroundColour(panel->GetBackgroundColour());

  new wxStaticBox(panel,-1,_("AD2"), wxPoint(310,6),wxSize(50,330));
  AnalgInput2=new wxGauge(panel,-1,255, wxPoint(325,30), wxSize(20,270),wxGA_VERTICAL);

  AnalgInput2Text=new wxTextCtrl(panel,-1,_("0"),wxPoint(315,310),wxSize(40,20),wxNO_BORDER|wxTE_CENTRE);
  AnalgInput2Text->SetEditable(false);
  AnalgInput2Text->SetBackgroundColour(panel->GetBackgroundColour());


  //Inputs
  new wxStaticBox(panel,-1,_("Inputs"), wxPoint(370,6),wxSize(170,45));
  I1=new wxCheckBox(panel,IDC_DIN1,_("1"), wxPoint(380,20));
  I2=new wxCheckBox(panel,IDC_DIN2,_("2"), wxPoint(410,20));
  I3=new wxCheckBox(panel,IDC_DIN3,_("3"), wxPoint(440,20));
  I4=new wxCheckBox(panel,IDC_DIN4,_("4"), wxPoint(470,20));
  I5=new wxCheckBox(panel,IDC_DIN5,_("5"), wxPoint(500,20));

  //Outputs
  new wxStaticBox(panel,-1,_("Outputs"), wxPoint(370,54),wxSize(260,45));
  O1=new wxCheckBox(panel,IDC_DOUT1,_("1"), wxPoint(380,70));
  O2=new wxCheckBox(panel,IDC_DOUT2,_("2"), wxPoint(410,70));
  O3=new wxCheckBox(panel,IDC_DOUT3,_("3"), wxPoint(440,70));
  O4=new wxCheckBox(panel,IDC_DOUT4,_("4"), wxPoint(470,70));
  O5=new wxCheckBox(panel,IDC_DOUT5,_("5"), wxPoint(500,70));
  O6=new wxCheckBox(panel,IDC_DOUT6,_("6"), wxPoint(530,70));
  O7=new wxCheckBox(panel,IDC_DOUT7,_("7"), wxPoint(560,70));
  O8=new wxCheckBox(panel,IDC_DOUT8,_("8"), wxPoint(590,70));

  //Counters
  new wxStaticBox(panel,-1,_("Counter1"), wxPoint(370,110),wxSize(125,226));
  Counter1Text=new wxTextCtrl(panel,-1,_("0"),wxPoint(385,140),wxSize(90,20));
  Counter1Text->SetEditable(false);

  Counter1ResetButton=new wxButton(panel,IDC_RSTC1,_("Reset"),wxPoint(385,165),wxSize(90,30));
 
  wxString choix[4]={_("0ms"), _("2ms"), _("10ms") , _("1000ms")};
  Counter1Debounce=new wxRadioBox(panel,IDC_DBCT1,_("Debounce Time"),wxPoint(375,215),wxSize(115,115),4, choix);
  Counter1Debounce->SetSelection(1);


  new wxStaticBox(panel,-1,_("Counter2"), wxPoint(505,110),wxSize(125,226));
  Counter2Text=new wxTextCtrl(panel,-1,_("0"),wxPoint(520,140),wxSize(90,20));
  Counter2Text->SetEditable(false);

  Counter2ResetButton=new wxButton(panel,IDC_RSTC2,_("Reset"),wxPoint(520,165),wxSize(90,30));

  Counter2Debounce=new wxRadioBox(panel,IDC_DBCT2,_("Debounce Time"),wxPoint(510,215),wxSize(115,115),4, choix);
  Counter2Debounce->SetSelection(1);

  address=0;//default address
  connected=0;//start not connected
}

/* ********************************************************************** */
/* associated functions to events */

void MyFrame::OnBnClickedSK5(wxCommandEvent& WXUNUSED(event))
{
    if (!connected)
    {
       address=3-SK5->GetValue()-2*SK6->GetValue();
    }
    if (DEBUG) cout<<"SK5 value is "<<SK5->GetValue()<<endl<<"Board Address is "<<address<<endl;
}
void MyFrame::OnBnClickedSK6(wxCommandEvent& WXUNUSED(event))
{
    if (!connected)
    {
      address=3-SK5->GetValue()-2*SK6->GetValue();
    }
    if (DEBUG) cout<<"SK6 value is "<<SK6->GetValue()<<endl<<"Board Address is "<<address<<endl;
}

void MyFrame::OnBnClickedConnect(wxCommandEvent& WXUNUSED(event))
{
     address=3-SK5->GetValue()-2*SK6->GetValue();
     if (OpenDevice(address)==-1)
     {
        connected=0;
        ConnectionStatus->Clear();
        *ConnectionStatus<<_("Not connected");
        wxMessageBox(_T("OpenDevice failed!\nCheck Connection and Card Address"), _T("Error"), wxOK | wxICON_ERROR, this);
     }
     else
     {     
        connected=1;
        WriteAll();
        ConnectionStatus->Clear();
        *ConnectionStatus<<_("Connected at address ")<<address;
        wxMessageBox(_T("Connection with device OK"), _T("USB Board status"), wxOK | wxICON_INFORMATION, this); 
     }
}

void MyFrame::OnBnClickedSetalldigital(wxCommandEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"SetAllDigital clicked!"<<endl;
    O1->SetValue(1);
    O2->SetValue(1);
    O3->SetValue(1);
    O4->SetValue(1);
    O5->SetValue(1);
    O6->SetValue(1);
    O7->SetValue(1);
    O8->SetValue(1);
    WriteDigital();

}

void MyFrame::OnBnClickedClearalldigital(wxCommandEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"ClearAllDigital clicked!"<<endl;
    O1->SetValue(0);
    O2->SetValue(0);
    O3->SetValue(0);
    O4->SetValue(0);
    O5->SetValue(0);
    O6->SetValue(0);
    O7->SetValue(0);
    O8->SetValue(0);
    WriteDigital();
}

void MyFrame::OnBnClickedSetallanalog(wxCommandEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"SetAllAnalog clicked!"<<endl;
    AnalgOutput1->SetValue(255);
    AnalgOutput2->SetValue(255);
    WriteAnalog();
}

void MyFrame::OnBnClickedClearallanalog(wxCommandEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"ClearAllAnalog clicked!"<<endl;
    AnalgOutput1->SetValue(0);
    AnalgOutput2->SetValue(0);
    WriteAnalog();
}

void MyFrame::OnBnClickedOutputtest(wxCommandEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"OutputTest clicked!"<<endl;
    cout<<"Not yet implemented!"<<endl;
//    O1->SetValue(1);
//    O2->SetValue(0);
//    O3->SetValue(0);
//    O4->SetValue(0);
//    O5->SetValue(0);
//    O6->SetValue(0);
//    O7->SetValue(0);
//    O8->SetValue(0);
//    WriteDigital();
}

void MyFrame::OnNMCustomdrawDa1(wxScrollEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"Da1 scrolled!"<<endl;
    WriteAnalog();
}
void MyFrame::OnNMCustomdrawDa2(wxScrollEvent& WXUNUSED(event))
{
    if (DEBUG) cout<<"Da2 scrolled!"<<endl;
    WriteAnalog();
}

void MyFrame::OnBnClickedDin1(wxCommandEvent& WXUNUSED(event))
{
     I1->SetValue(!(I1->GetValue()));
     if (DEBUG) cout<<"Din1 clicked!"<<endl;
}

void MyFrame::OnBnClickedDin2(wxCommandEvent& WXUNUSED(event))
{
     I2->SetValue(!(I2->GetValue()));
     if (DEBUG) cout<<"Din2 clicked!"<<endl;
}

void MyFrame::OnBnClickedDin3(wxCommandEvent& WXUNUSED(event))
{
     I3->SetValue(!(I3->GetValue()));
     if (DEBUG) cout<<"Din3 clicked!"<<endl;
}

void MyFrame::OnBnClickedDin4(wxCommandEvent& WXUNUSED(event))
{
     I4->SetValue(!(I4->GetValue()));
     if (DEBUG) cout<<"Din4 clicked!"<<endl;
}

void MyFrame::OnBnClickedDin5(wxCommandEvent& WXUNUSED(event))
{
     I5->SetValue(!(I5->GetValue()));
     if (DEBUG) cout<<"Din5 clicked!"<<endl;
}

void MyFrame::OnBnClickedDout1(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout1 clicked!"<<endl;
     WriteDigital();

}
void MyFrame::OnBnClickedDout2(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout2 clicked!"<<endl;
     WriteDigital();
}
void MyFrame::OnBnClickedDout3(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout3 clicked!"<<endl;
     WriteDigital();
}
void MyFrame::OnBnClickedDout4(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout4 clicked!"<<endl;
     WriteDigital();
}
void MyFrame::OnBnClickedDout5(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout5 clicked!"<<endl;
     WriteDigital();
}
void MyFrame::OnBnClickedDout6(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout6 clicked!"<<endl;
     WriteDigital();
}
void MyFrame::OnBnClickedDout7(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout7 clicked!"<<endl;
     WriteDigital();
}
void MyFrame::OnBnClickedDout8(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Dout8 clicked!"<<endl;
     WriteDigital();
}

void MyFrame::OnBnClickedResct1(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Resct1 clicked!"<<endl;
     if (connected)
     {      
       ResetCounter(1);
     }
}

void MyFrame::OnBnClickedDbcc1(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Resct1 clicked!"<<endl; 
     WriteDebounce1();
}

void MyFrame::OnBnClickedResct2(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Resct2 clicked!"<<endl;
     if (connected)
     {      
       ResetCounter(2);
     }
}

void MyFrame::OnBnClickedDbcc2(wxCommandEvent& WXUNUSED(event))
{
     if (DEBUG) cout<<"Resct2 clicked!"<<endl;
     WriteDebounce2();
}

//*************************************************************
/* when idle
   read and print values */
void MyFrame::OnIdle(wxIdleEvent& event)
{

     if (DEBUG) cout<<"doing nothing!"<<endl;
     if (connected)
     { 
       //read data
       long int data1, data2, data3, data4, data5;
       ReadAllValues(&data1, &data2, &data3, &data4, &data5);

       //print data on interface
       I1->SetValue(data1 & 0x01);
       I2->SetValue(data1 & 0x02);
       I3->SetValue(data1 & 0x04);
       I4->SetValue(data1 & 0x08);
       I5->SetValue(data1 & 0x10);
       AnalgInput1->SetValue(data2);
       AnalgInput1Text->Clear();
       *AnalgInput1Text<<data2;
       AnalgInput2->SetValue(data3);
       AnalgInput2Text->Clear();
       *AnalgInput2Text<<data3;
       Counter1Text->Clear();
       *Counter1Text<<data4;
       Counter2Text->Clear();
       *Counter2Text<<data5;
     }

}
//*************************************************************



/***************************************************************/
/* write panel values to the board */

/* digital values */
void MyFrame::WriteDigital()
{
     if (connected)
     {
        long word=O1->GetValue()+2*O2->GetValue()+4*O3->GetValue()+8*O4->GetValue()+
        16*O5->GetValue()+32*O6->GetValue()+64*O7->GetValue()+128*O8->GetValue(); 
        WriteAllDigital(word);
     }
}

/* analog values */
void MyFrame::WriteAnalog()
{
     if (connected)
     {      
        //OutputAnalogChannel(1, (long) AnalgOutput1->GetValue());
        //OutputAnalogChannel(2, (long) AnalgOutput2->GetValue()); 
        OutputAllAnalog((long) AnalgOutput1->GetValue(), (long) AnalgOutput2->GetValue());
     }
}

/* debounce1 value */
void MyFrame::WriteDebounce1()
{
     if (connected)
     {
        int choice=Counter1Debounce->GetSelection();
        long DebounceTime;
        switch (choice)
        {
           case 0 : DebounceTime=1; break;
           case 1 : DebounceTime=2; break;
           case 2 : DebounceTime=10; break;
           case 3 : DebounceTime=1000;
       }
       SetCounterDebounceTime(1, DebounceTime);      
     } 
}

/* debounce2 value */
void MyFrame::WriteDebounce2()
{
     if (connected)
     {
        int choice=Counter2Debounce->GetSelection();
        long DebounceTime;
        switch (choice)
        {
           case 0 : DebounceTime=1; break;
           case 1 : DebounceTime=2; break;
           case 2 : DebounceTime=10; break;
           case 3 : DebounceTime=1000;
       }
       SetCounterDebounceTime(2, DebounceTime);      
     } 
}

/* all values */
void MyFrame::WriteAll()
{
     if (connected)
     {
       long word=O1->GetValue()+2*O2->GetValue()+4*O3->GetValue()+8*O4->GetValue()+
       16*O5->GetValue()+32*O6->GetValue()+64*O7->GetValue()+128*O8->GetValue(); 
       WriteAllDigital(word);
       OutputAllAnalog((long) AnalgOutput1->GetValue(), (long) AnalgOutput2->GetValue());
       WriteDebounce1();
       WriteDebounce2();
     }
     
}


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(IDC_CONNECT,MyFrame::OnBnClickedConnect)
    EVT_CHECKBOX(IDC_SK5,MyFrame::OnBnClickedSK5)
    EVT_CHECKBOX(IDC_SK6,MyFrame::OnBnClickedSK6)
    EVT_BUTTON(IDC_SETALLDIGITAL,MyFrame::OnBnClickedSetalldigital)
    EVT_BUTTON(IDC_CLEARALLDIGITAL,MyFrame::OnBnClickedClearalldigital)
    EVT_BUTTON(IDC_SETALLANALOG,MyFrame::OnBnClickedSetallanalog)
    EVT_BUTTON(IDC_CLEARALLANALOG,MyFrame::OnBnClickedClearallanalog)
    EVT_TOGGLEBUTTON(IDC_OUTPUTTEST,MyFrame::OnBnClickedOutputtest)
    EVT_COMMAND_SCROLL(IDC_DA1,MyFrame::OnNMCustomdrawDa1)
    EVT_COMMAND_SCROLL(IDC_DA2,MyFrame::OnNMCustomdrawDa2)
    EVT_CHECKBOX(IDC_DIN1,MyFrame::OnBnClickedDin1)
    EVT_CHECKBOX(IDC_DIN2,MyFrame::OnBnClickedDin2)
    EVT_CHECKBOX(IDC_DIN3,MyFrame::OnBnClickedDin3)
    EVT_CHECKBOX(IDC_DIN4,MyFrame::OnBnClickedDin4)
    EVT_CHECKBOX(IDC_DIN5,MyFrame::OnBnClickedDin5)    
    EVT_CHECKBOX(IDC_DOUT1,MyFrame::OnBnClickedDout1)
    EVT_CHECKBOX(IDC_DOUT2,MyFrame::OnBnClickedDout2)
    EVT_CHECKBOX(IDC_DOUT3,MyFrame::OnBnClickedDout3)
    EVT_CHECKBOX(IDC_DOUT4,MyFrame::OnBnClickedDout4)
    EVT_CHECKBOX(IDC_DOUT5,MyFrame::OnBnClickedDout5)
    EVT_CHECKBOX(IDC_DOUT6,MyFrame::OnBnClickedDout6)
    EVT_CHECKBOX(IDC_DOUT7,MyFrame::OnBnClickedDout7)
    EVT_CHECKBOX(IDC_DOUT8,MyFrame::OnBnClickedDout8)

    EVT_BUTTON(IDC_RSTC1,MyFrame::OnBnClickedResct1)
    EVT_BUTTON(IDC_RSTC2,MyFrame::OnBnClickedResct2)

    EVT_RADIOBOX(IDC_DBCT1,MyFrame::OnBnClickedDbcc1)
    EVT_RADIOBOX(IDC_DBCT2,MyFrame::OnBnClickedDbcc2)

    EVT_IDLE(MyFrame::OnIdle)

END_EVENT_TABLE()


