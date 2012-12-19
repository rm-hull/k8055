/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


void K8055QT::ConnectButton_clicked()
{
	if (self.card != None) and (self.k == None):
		try:
			#print "Connect to card #",self.card
	    		self.k = k8055(self.card)
			self.EnableAll()
			self.ClearAllDigitalButton_clicked()
			self.ClearAllAnalogButton_clicked()
			self.comboCounter1.setCurrentItem(0)
			self.comboCounter2.setCurrentItem(0)
			
			self.ConnectButton.setText(self.__tr("Disconnect"))
		except:
    			print "No such Card"
	elif (self.k != None):
		try:
			#print "Disconnect from card #",self.card
			self.k.CloseDevice()
			self.k = None
			self.DisableAll()
			self.OutputBox1.setChecked(0)
			self.OutputBox2.setChecked(0)
			self.OutputBox3.setChecked(0)
			self.OutputBox4.setChecked(0)
			self.OutputBox5.setChecked(0)
			self.OutputBox6.setChecked(0)
			self.OutputBox7.setChecked(0)
			self.OutputBox8.setChecked(0)
			self.BarDA1.setValue(0)
			self.BarDA2.setValue(0)
			self.BarAD1.setValue(0)
			self.BarAD2.setValue(0)
			self.DA1Value.setProperty("intValue",QVariant(0))
			self.DA2Value.setProperty("intValue",QVariant(0))
			self.AD1Value.setProperty("intValue",QVariant(0))
			self.AD2Value.setProperty("intValue",QVariant(0))
			self.comboCounter1.setCurrentItem(0)
			self.comboCounter2.setCurrentItem(0)
			self.Counter1Value.setProperty("intValue",QVariant(0))
			self.Counter2Value.setProperty("intValue",QVariant(0))
			self.ConnectButton.setText(self.__tr("Connect"))
		except:
			pass
		
}


void K8055QT::CardButton1_clicked()
{
	self.card = 0
}

void K8055QT::CardButton2_clicked()
{
	self.card = 1
}

void K8055QT::CardButton3_clicked()
{
	self.card = 2
}

void K8055QT::CardButton4_clicked()
{
	self.card = 3
}


void K8055QT::init()
{
	self.k = None
	self.DisableAll()
}

void K8055QT::ClearCounter1_clicked()
{
	if self.k:
		self.k.ResetCounter(1)
}

void K8055QT::ClearCounter2_clicked()
{
	if self.k:
		self.k.ResetCounter(2)
}


void K8055QT::OutputBox1_clicked()
{
	if self.k:
		if self.OutputBox1.isChecked():
			self.k.SetDigitalChannel(1)
		else:
			self.k.ClearDigitalChannel(1)
}

void K8055QT::OutputBox2_clicked()
{
	if self.k:
		if self.OutputBox2.isChecked():
			self.k.SetDigitalChannel(2)
		else:
			self.k.ClearDigitalChannel(2)

void K8055QT::OutputBox3_clicked()
{
	if self.k:
		if self.OutputBox3.isChecked():
			self.k.SetDigitalChannel(3)
		else:
			self.k.ClearDigitalChannel(3)
}
void K8055QT::OutputBox4_clicked()
{
	if self.k:
		if self.OutputBox4.isChecked():
			self.k.SetDigitalChannel(4)
		else:
			self.k.ClearDigitalChannel(4)
}
void K8055QT::OutputBox5_clicked()
{
	if self.k:
		if self.OutputBox5.isChecked():
			self.k.SetDigitalChannel(5)
		else:
			self.k.ClearDigitalChannel(5)
}
void K8055QT::OutputBox6_clicked()
{
	if self.k:
		if self.OutputBox6.isChecked():
			self.k.SetDigitalChannel(6)
		else:
			self.k.ClearDigitalChannel(6)
}
void K8055QT::OutputBox7_clicked()
{
	if self.k:
		if self.OutputBox7.isChecked():
			self.k.SetDigitalChannel(7)
		else:
			self.k.ClearDigitalChannel(7)
}
void K8055QT::OutputBox8_clicked()
{
	if self.k:
		if self.OutputBox8.isChecked():
			self.k.SetDigitalChannel(8)
		else:
			self.k.ClearDigitalChannel(8)
}


void K8055QT::SetAllAnalogButton_clicked()
{
	if self.k:
		self.k.SetAllAnalog()
		self.BarAD1.setValue(0)
		self.BarAD2.setValue(0)
		self.AD1Value.setProperty("intValue",QVariant(255))
		self.AD2Value.setProperty("intValue",QVariant(255))
}


void K8055QT::ClearAllAnalogButton_clicked()
{
	if self.k:
		self.k.ClearAllAnalog()
		self.BarAD1.setValue(255)
		self.BarAD2.setValue(255)
		self.AD1Value.setProperty("intValue",QVariant(0))
		self.AD2Value.setProperty("intValue",QVariant(0))
}


void K8055QT::SetAllDigitalButton_clicked()
{
	if self.k:
		self.k.SetAllDigital()
		self.OutputBox1.setChecked(1)
		self.OutputBox2.setChecked(1)
		self.OutputBox3.setChecked(1)
		self.OutputBox4.setChecked(1)
		self.OutputBox5.setChecked(1)
		self.OutputBox6.setChecked(1)
		self.OutputBox7.setChecked(1)
		self.OutputBox8.setChecked(1)
}

void K8055QT::ClearAllDigitalButton_clicked()
{
	if self.k:
		self.k.ClearAllDigital()
		self.OutputBox1.setChecked(0)
		self.OutputBox2.setChecked(0)
		self.OutputBox3.setChecked(0)
		self.OutputBox4.setChecked(0)
		self.OutputBox5.setChecked(0)
		self.OutputBox6.setChecked(0)
		self.OutputBox7.setChecked(0)
		self.OutputBox8.setChecked(0)
}


void K8055QT::BarAD1_valueChanged( int )
{
	if self.k:
		self.k.OutputAnalogChannel(1,255-a0)
		self.AD1Value.setProperty("intValue",QVariant(255-a0))
}

void K8055QT::BarAD2_valueChanged( int )
{
	if self.k:
		self.k.OutputAnalogChannel(2,255-a0)
		self.AD2Value.setProperty("intValue",QVariant(255-a0))
}


void K8055QT::comboCounter2_activated( int )
{
	if self.k:
		v = str(self.comboCounter2.currentText()).split()
		self.k.SetCounterDebounceTime(2,int(v[0]))
}


void K8055QT::comboCounter1_activated( int )
{
	if self.k:
		v = str(self.comboCounter1.currentText()).split()
		self.k.SetCounterDebounceTime(1,int(v[0]))
}


void K8055QT::TestDigitalButton_clicked()
{
	self.DigitalTest()
}


void K8055QT::TestAnalogButton_clicked()
{
	self.AnalogTest()
}
