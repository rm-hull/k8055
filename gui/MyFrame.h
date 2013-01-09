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

#ifndef _MYFRAME_
#define _MYFRAME_

#include <wx/wx.h>
#include <wx/tglbtn.h>

enum
{
  IDC_SK5=1,
  IDC_SK6,
  IDC_CONNECT,
  IDC_SETALLDIGITAL,
  IDC_CLEARALLDIGITAL,
  IDC_SETALLANALOG,
  IDC_CLEARALLANALOG,
  IDC_OUTPUTTEST,
  IDC_DA1,
  IDC_DA2,
  //IDC_AD1,/*read-only so generates no events*/
  //IDC_AD2,
  IDC_DIN1,
  IDC_DIN2,
  IDC_DIN3,
  IDC_DIN4,
  IDC_DIN5,
  IDC_DOUT1,
  IDC_DOUT2,
  IDC_DOUT3,
  IDC_DOUT4,
  IDC_DOUT5,
  IDC_DOUT6,
  IDC_DOUT7,
  IDC_DOUT8,
  IDC_CMPT1,
  IDC_RSTC1,
  IDC_DBCT1,
  IDC_CMPT2,
  IDC_RSTC2,
  IDC_DBCT2
}; 




class MyFrame: public wxFrame
{
protected:    
    long address; // board address 0 -> 3
    bool connected;// true if connected 
public:

    //constructor
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    //associated functions to events
    void OnBnClickedSK5(wxCommandEvent& event);    void OnBnClickedSK6(wxCommandEvent& event);
    void OnBnClickedConnect(wxCommandEvent& event);
    void OnBnClickedSetalldigital(wxCommandEvent& event);
    void OnBnClickedClearalldigital(wxCommandEvent& event);
    void OnBnClickedSetallanalog(wxCommandEvent& event);
    void OnBnClickedClearallanalog(wxCommandEvent& event);
    void OnBnClickedOutputtest(wxCommandEvent& event);
    void OnNMCustomdrawDa1(wxScrollEvent& event);
    void OnNMCustomdrawDa2(wxScrollEvent& event);
    //void OnNMCustomdrawAd1(wxScrollEvent& event);
    //void OnNMCustomdrawAd2(wxScrollEvent& event);
    void OnBnClickedDin1(wxCommandEvent& event);    void OnBnClickedDin2(wxCommandEvent& event);
    void OnBnClickedDin3(wxCommandEvent& event);
    void OnBnClickedDin4(wxCommandEvent& event);
    void OnBnClickedDin5(wxCommandEvent& event);
    void OnBnClickedDout1(wxCommandEvent& event);    void OnBnClickedDout2(wxCommandEvent& event);
    void OnBnClickedDout3(wxCommandEvent& event);
    void OnBnClickedDout4(wxCommandEvent& event);
    void OnBnClickedDout5(wxCommandEvent& event);
    void OnBnClickedDout6(wxCommandEvent& event);
    void OnBnClickedDout7(wxCommandEvent& event);
    void OnBnClickedDout8(wxCommandEvent& event);
    //void OnBnClickedCmpt1(wxCommandEvent& event);
    void OnBnClickedResct1(wxCommandEvent& event);
    void OnBnClickedDbcc1(wxCommandEvent& event);
    //void OnBnClickedCmpt2(wxCommandEvent& event);
    void OnBnClickedResct2(wxCommandEvent& event);
    void OnBnClickedDbcc2(wxCommandEvent& event);

    void OnIdle(wxIdleEvent& event);

    void WriteDigital();
    void WriteAnalog();
    void WriteDebounce1();
    void WriteDebounce2();
    void WriteAll();

    //main panel
    wxPanel * panel;

    //Card Address
    wxCheckBox *SK5,*SK6;
    wxButton *Connect;

    //Connection status
    wxTextCtrl *ConnectionStatus;

    //Test buttons
    wxButton *SetAllDigit,*ClearAllDigit,*SetAllAnalg,*ClearAllAnalg;
    wxToggleButton *OutputTest;

    //Sliders
    wxSlider *AnalgOutput1,*AnalgOutput2;

    //Gauges
    wxGauge *AnalgInput1,*AnalgInput2;
    wxTextCtrl *AnalgInput1Text,*AnalgInput2Text;

    //Inputs
    wxCheckBox *I1,*I2,*I3,*I4,*I5;
    
    //Outputs
    wxCheckBox *O1,*O2,*O3,*O4,*O5,*O6,*O7,*O8;

    //Counters
    wxTextCtrl *Counter1Text,*Counter2Text;
    wxButton *Counter1ResetButton,*Counter2ResetButton;
    wxRadioBox *Counter1Debounce,*Counter2Debounce;

    DECLARE_EVENT_TABLE()
};

#endif
