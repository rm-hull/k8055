#ifndef _MYAPP_
#define _MYAPP_

#include <wx/wx.h>
#include "MyFrame.h"

class MyApp: public wxApp
{
    virtual bool OnInit();
};

DECLARE_APP(MyApp)

#endif
