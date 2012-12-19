/*  
 * Copyright (C) 2008 Martin Pischky (mailto:martin@pischky.de)  
 *
 * This file (JK8055.java) is part of libk8055/jk8055.
 * jk8055 - a java wrapper for libk8055
 *
 * libk8055/jk8055 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * $Id: JK8055.java,v 1.4 2008/07/28 09:58:30 pischky Exp $
 * 
 */

package net.sf.libk8055.jk8055;

/**
 * JK8055 - A java wrapper for libk8055.
 * 
 * @see <a href="http://libk8055.sourceforge.net/">http://libk8055.sourceforge.net/</a>
 * @author <a href="mailto:martin@pischky.de">Martin Pischky</a>
 *
 */
public class JK8055 {
	
	public static final String CVS_ID = "$Id: JK8055.java,v 1.4 2008/07/28 09:58:30 pischky Exp $";
	
	/**
	 * This class is singleton.
	 */
	private JK8055() {
	}
	
	private static JK8055 instance = null;
	
	/**
	 * Create an instance if needed and return it.
	 * @return the instance
	 */
	public static JK8055 getInstance() {
		if( instance != null ) {
			return instance;
		}
		synchronized (JK8055.class) {
			if( instance == null ) {
				instance = new JK8055();
			}            
		}
		return instance;
	}
	
	public static final int K8055_ERROR = -1;
	
	static {
		System.loadLibrary("jk8055");
	}
	
	/* (non-Javadoc)
	  * @see java.lang.Object#finalize()
	 */
	protected void finalize() throws Throwable {
		try {
			CloseAllOpenDevices();
		} finally {
			super.finalize();
		}
	}
	
	/**
	 * Open device.
	 * Scan through usb busses looking 
	 * for the right device, claim it and then open the device.
	 * @param cardAddress
	 * @return BoardAddress or K8055_ERROR
	 */
	private native int COpenDevice( int cardAddress );
	
	/**
	 * Opens the communication link to the K8055 device.
	 * Opens the communication link to the K8055 card. Loads the drivers 
	 * needed to communicate via the USB port. This procedure must be performed
	 * before any attempts to communicate with the K8055 card.
	 * <br>
	 * This function can also be used to selects the active K8055 card to read
	 * and write the data. All the communication routines after this function 
	 * call are addressed to this card until the other card is selected by this 
	 * function call.
	 * 
	 * @param cardAddress Value between 0 and 3 which corresponds to the 
	 *                    jumper (SK5, SK6) setting on the K8055 board.
	 * @return If succeeded the return value will be the card address read 
	 *         from the K8055 hardware.
	 * @throws JK8055Exception indicates that K8055 card was not found.
	 */
	public synchronized int OpenDevice( int cardAddress ) 
	throws JK8055Exception
	{
		if( cardAddress<0 || 3<cardAddress ) {
			throw new IllegalArgumentException( "cardAddress: "+cardAddress );
		}
		int retval = COpenDevice( cardAddress );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "OpenDevice("+cardAddress+") failed" );
		}
		return retval;
	}
	
	/**
	 * Close the Current device.
	 * @return retunvalue from usb_close: 0 on success or < 0 on error.
	 */
	private native int CCloseDevice();
	
	/**
	 * Closes the link to the K8055 device.
	 * Unloads the communication routines for K8055 card and unloads the driver 
	 * needed to communicate via the USB port. This is the last action of the 
	 * application program before termination.
	 * 
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized void CloseDevice() 
	throws JK8055Exception 
	{
		int retval = CCloseDevice();
		if( retval<0 ) {
			throw new JK8055Exception( "CloseDevice failed with rc="+retval );
		}
	}
	
	/**
	 * Close all open devices.
	 * Check what open devices still exist an closes them.
	 *
	 * @throws JK8055Exception 
	 */
	public synchronized void CloseAllOpenDevices()
	throws JK8055Exception
	{
		for( int i = 0; i < 4; i++ ) {
			if( CSetCurrentDevice( i ) != K8055_ERROR ) {
				CloseDevice();
			}
		}
	} 
	
	/**
	 * @param channel 1..2
	 * @return 0..255 or K8055_ERROR
	 */
	private native int CReadAnalogChannel( int channel );
	
	/**
	 * Reads the status of one analogue input-channel.
	 * The input voltage of the selected 8-bit Analogue to Digital converter 
	 * channel is converted to a value which lies between 0 and 255.
	 * 
	 * @param channel Value between 1 and 2 which corresponds to the AD channel 
	 *                whose status is to be read.
	 * @return The corresponding Analogue to Digital Converter data is read.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid channel number.
	 */
	public synchronized int ReadAnalogChannel( int channel ) 
	throws JK8055Exception 
	{
		if( channel<1 || 2<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		int retval = CReadAnalogChannel( channel );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ReadAnalogChannel failed" );
		}
		return retval;
	}
	
	public class AllAnalog {
		public int data1;
		public int data2;
	}
	
	/**
	 * @param data
	 * @return 0 or K8055_ERROR
	 */
	private native int CReadAllAnalog(AllAnalog data);
	
	/**
	 * Reads the status of both analogue input-channels.
	 * The status of both Analogue to Digital Converters are read to a new
	 * object of class AllAnalogarray 
	 * 
	 * @return the values read
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized AllAnalog ReadAllAnalog() 
	throws JK8055Exception 
	{
		AllAnalog aa = new AllAnalog();
		int retval = CReadAllAnalog( aa );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ReadAllAnalog failed" );
		}
		return aa;
	}
	
	/**
	 * @param channel 1..2
	 * @param data 0..255
	 * @return K8055_ERROR or 0
	 */
	private native int COutputAnalogChannel( int channel, int data );
	
	/**
	 * Sets the analogue output channel according to the data.
	 * The indicated 8-bit Digital to Analogue Converter channel is altered 
	 * according to the new data. This means that the data corresponds to a 
	 * specific voltage. The value 0 corresponds to a minimum output voltage 
	 * (0 Volt) and the value 255 corresponds to a maximum output voltage 
	 * (+5V). A value of 'Data' lying in between these extremes can be 
	 * translated by the following formula : Data / 255 x 5V.
	 * 
	 * @param channel Value between 1 and 2 which corresponds to the 8-bit DA 
	 *                channel number whose data is to be set.
	 * @param data    Value between 0 and 255 which is to be sent to the 8-bit 
	 *                Digital to Analogue Converter.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized void OutputAnalogChannel( int channel, int data ) 
	throws JK8055Exception 
	{
		if( channel<1 || 2<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		if( data<0 || 255<data ) {
			throw new IllegalArgumentException( "data: "+data );
		}
		int retval = COutputAnalogChannel( channel, data );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "OutputAnalogChannel failed" );
		}
	}
	
	/**
	 * @param data1 0..255
	 * @param data2 0..255
	 * @return K8055_ERROR or 0
	 */
	private native int COutputAllAnalog( int data1, int data2 );
	
	/**
	 * Sets both analogue output channels according to the data.
	 * Both 8-bit Digital to Analogue Converter channels are altered according 
	 * to the new data. This means that the data corresponds to a specific 
	 * voltage. The value 0 corresponds to a minimum output voltage (0 Volt) 
	 * and the value 255 corresponds to a maximum output voltage (+5V). A 
	 * value of 'Data1' or 'Data2' lying in between these extremes can be 
	 * translated by the following formula : Data / 255 x 5V.
	 * 
	 * @param data1 Value between 0 and 255 which is to be sent to the 8-bit 
	 *              Digital to Analogue Converter.
	 * @param data2 Value between 0 and 255 which is to be sent to the 8-bit 
	 *              Digital to Analogue Converter.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized void OutputAllAnalog( int data1, int data2 ) 
	throws JK8055Exception 
	{
		if( data1<0 || 255<data1 ) {
			throw new IllegalArgumentException( "data1: "+data1 );
		}
		if( data2<0 || 255<data2 ) {
			throw new IllegalArgumentException( "data2: "+data2 );
		}
		int retval = COutputAllAnalog( data1, data2 );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "OutputAllAnalog failed" );
		}
	}
	
	/**
	 * @return K8055_ERROR or 0
	 */
	private native int CClearAllAnalog();
	
	/**
	 * Sets all analogue output channels to minimum.
	 * Both DA-channels are set to minimum output voltage (0 Volt) .
	 * 
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized void ClearAllAnalog() 
	throws JK8055Exception 
	{
		int retval = CClearAllAnalog(); 
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ClearAllAnalog failed" );
		}
	}
	
	/**
	 * @param channel 1..2
	 * @return K8055_ERROR or 0
	 */
	private native int CClearAnalogChannel( int channel );
	
	/**
	 * Sets the analogue output channel to minimum.
	 * The selected DA-channel is set to minimum output voltage (0 Volt).
	 * 
	 * @param channel Value between 1 and 2 which corresponds to the 8-bit DA 
	 *                channel number in which the data is to be erased.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid channel number.
	 */
	public synchronized void ClearAnalogChannel( int channel ) 
	throws JK8055Exception 
	{
		if( channel<1 || 2<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		int retval = CClearAnalogChannel( channel );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ClearAllAnalog failed" );
		}
	}
	
	/**
	 * @param channel 1..2
	 * @return K8055_ERROR or 0
	 */
	private native int CSetAnalogChannel( int channel );
	
	/**
	 * Sets the analogue output channel to maximum.
	 * The selected 8-bit Digital to Analogue Converter channel is set to 
	 * maximum output voltage.
	 * 
	 * @param channel Value between 1 and 2 which corresponds to the 8-bit DA 
	 *                channel number in which the data is to be set to maximum.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid channel number.
	 */
	public synchronized void SetAnalogChannel( int channel ) 
	throws JK8055Exception 
	{
		if( channel<1 || 2<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		int retval = CSetAnalogChannel( channel );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetAnalogChannel failed" );
		}
	}
	
	/**
	 * @return K8055_ERROR or 0
	 */
	private native int CSetAllAnalog();
	
	/**
	 * Sets all analogue output channels to maximum.
	 * All channels of the 8-bit Digital to Analogue Converters are set to 
	 * maximum output voltage.
	 * 
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized void SetAllAnalog() 
	throws JK8055Exception 
	{
		int retval = CSetAllAnalog(); 
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetAllAnalog failed" );
		}
	}
	
	/**
	 * @param data 0..255
	 * @return K8055_ERROR or 0
	 */
	private native int CWriteAllDigital( int data );
	
	/**
	 * Sets the digital outputs according to the data.
	 * The channels of the digital output port are updated with the status of 
	 * the corresponding bits in the data parameter. A high (1) level means 
	 * that the microcontroller IC1 output is set, and a low (0) level means
	 * that the output is cleared.
	 * 
	 * @param data Value between 0 and 255 that is sent to the output 
	 *        port (8 channels).
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized void WriteAllDigital( int data ) 
	throws JK8055Exception
	{
		if( data<0 || 255<data ) {
			throw new IllegalArgumentException( "data: "+data );
		}
		int retval = CWriteAllDigital( data );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "WriteAllDigital failed" );
		}
	}
	
	/**
	 * @param channel 1..8
	 * @return K8055_ERROR or 0
	 */
	private native int CClearDigitalChannel( int channel );
	
	/**
	 * Clears the output channel.
	 * The selected channel is cleared.
	 * 
	 * @param channel Value between 1 and 8 which corresponds to the output 
	 *                channel that is to be cleared.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid channel number.
	 */
	public synchronized void ClearDigitalChannel( int channel )
	throws JK8055Exception
	{
		if( channel<1 || 8<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		int retval = CClearDigitalChannel( channel );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ClearDigitalChannel failed" );
		}
	}
	
	/**
	 * @return K8055_ERROR or 0
	 */
	private native int CClearAllDigital();
	
	/**
	 * Clears all output channels.
	 * All digital outputs are cleared.
	 * 
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized void ClearAllDigital()
	throws JK8055Exception
	{
		int retval = CClearAllDigital();
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ClearAllDigital failed" );
		}
	}
	
	/**
	 * @param channel 1..8
	 * @return K8055_ERROR or 0
	 */
	private native int CSetDigitalChannel( int channel );
	
	/**
	 * Sets the output channel.
	 * The selected digital output channel is set.
	 * 
	 * @param channel Value between 1 and 8 which corresponds to the output 
	 *                channel that is to be set.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid channel number.
	 */
	public synchronized void SetDigitalChannel( int channel )
	throws JK8055Exception
	{
		if( channel<1 || 8<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		int retval = CSetDigitalChannel( channel );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetDigitalChannel failed" );
		}
	}
	
	/**
	 * @return K8055_ERROR or 0
	 */
	private native int CSetAllDigital();
	
	/**
	 * Sets all output channels.
	 * All the digital output channels are set.
	 * 
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized void SetAllDigital()
	throws JK8055Exception
	{
		int retval = CSetAllDigital();
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetAllDigital failed" );
		}
	}
	
	/**
	 * @param channel 1..5
	 * @return K8055_ERROR or 0,1
	 */
	private native int CReadDigitalChannel( int channel );
	
	/**
	 * Reads the status of the input channel.
	 * The status of the selected Input channel is read.
	 * 
	 * @param channel Value between 1 and 5 which corresponds to the input 
	 *                channel whose status is to be read.
	 * @return TRUE means that the channel has been set and FALSE means that 
	 *         it has been cleared.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid channel number.
	 */
	public synchronized boolean ReadDigitalChannel( int channel )
	throws JK8055Exception
	{
		if( channel<1 || 5<channel ) {
			throw new IllegalArgumentException( "channel: "+channel );
		}
		int retval = CReadDigitalChannel( channel );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ReadDigitalChannel failed" );
		}
		return retval != 0;
	}
	
	/**
	 * @return K8055_ERROR or 0..31
	 */
	private native int CReadAllDigital();
	
	/**
	 * Reads the status of all the input channels.
	 * The function returns the status of the digital inputs.
	 * 
	 * @return The 5 LSB correspond to the status of the input channels. A high 
	 *         (1) means that the channel is HIGH, a low (0) means that the 
	 *         channel is LOW.
	 * @throws JK8055Exception indicates an error.
	 */
	public synchronized int ReadAllDigital()
	throws JK8055Exception
	{
		int retval = CReadAllDigital();
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ReadAllDigital failed" );
		}
		return retval;
	}

	/**
	 * @param counterno 1..2
	 * @return K8055_ERROR or 0
	 */
	private native int CResetCounter( int counterno );
	
	/**
	 * Resets the 16 bit pulse counter number 1 or counter number 2.
	 * The selected pulse counter is reset.
	 * 
	 * @param counterno Value 1 or 2, which corresponds to the counter to 
	 *                  be reset.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid counter number.
	 */
	public synchronized void ResetCount( int counterno ) 
	throws JK8055Exception
	{
		if( counterno<1 || 2<counterno ) {
			throw new IllegalArgumentException( "counterno: "+counterno );
		}
		int retval = CResetCounter( counterno );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ResetCount failed" );
		}
	}
	
	/**
	 * @param counterno 1..2
	 * @return K8055_ERROR or 0..65535
	 */
	private native int CReadCounter( int counterno );
	
	/**
	 * Reads the content of the pulse counter number 1 or counter number 2.
	 * The function returns the status of the selected 16 bit pulse counter.
	 * <br>
	 * The counter number 1 counts the pulses fed to the input I1 and the 
	 * counter number 2 counts the pulses fed to the input I2.
	 * 
	 * @param counterno Value 1 or 2, which corresponds to the counter to be 
	 *                  read.
	 * @return The content of the 16 bit pulse counter.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid counter number.
	 */
	public synchronized int ReadCounter( int counterno )
	throws JK8055Exception
	{
		if( counterno<1 || 2<counterno ) {
			throw new IllegalArgumentException( "counterno: "+counterno );
		}
		int retval = CReadCounter( counterno );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ReadCounter failed" );
		}
		return retval;
	}
	
	/**
	 * @param counterno 1..2
	 * @param debouncetime 0..7450
	 * @return K8055_ERROR or 0
	 */
	private native int CSetCounterDebounceTime( int counterno, int debouncetime );
	
	/**
	 * Sets the debounce time to the pulse counter.
	 * The counter inputs are debounced in the software to prevent false 
	 * triggering when mechanical switches or relay inputs are used. The 
	 * debounce time is equal for both falling and rising edges. The
	 * default debounce time is 2ms. This means the counter input must be 
	 * stable for at least 2ms before it is recognised, giving the maximum 
	 * count rate of about 200 counts per second.
	 * <br>
	 * If the debounce time is set to 0, then the maximum counting rate is 
	 * about 2000 counts per second.
	 * 
	 * @param counterno    Value 1 or 2, which corresponds to the counter 
	 *                     to be set.
	 * @param debouncetime (0..7450?) Debounce time for the pulse counter.
	 *                     <br>
	 *                     The DebounceTime value corresponds to the debounce 
	 *                     time in milliseconds (ms) to be set for the
	 *                     pulse counter. Debounce time value may vary between 
	 *                     0 and 5000?.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized void SetCounterDebounceTime( int counterno, 
	                                                 int debouncetime )
	throws JK8055Exception
	{
		if( counterno<1 || 2<counterno ) {
			throw new IllegalArgumentException( "counterno: "+counterno );
		}
		if( debouncetime<0 || 7450<debouncetime ) {
			throw new IllegalArgumentException( "debouncetime: "+debouncetime );
		}
		int retval = CSetCounterDebounceTime( counterno, debouncetime );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetCounterDebounceTime failed" );
		}
	}
	
	public class AllValues {
		public int input;
		public int analog1;
		public int analog2;
		public int counter1;
		public int counter2;
	}
	
	/**
	 * @param av
	 * @return K8055_ERROR or 0
	 */
	private native int CReadAllValues( AllValues av );
	
	/**
	 * Read all values at once.
	 * 
	 * @return an new object of type AllValues
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized AllValues ReadAllValues()
	throws JK8055Exception 
	{
		AllValues av = new AllValues();
		int retval = CReadAllValues( av );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "ReadAllValues failed" );
		}
		return av;
	}
	
	/**
	 * @param digitaldata 0..255
	 * @param analogdata1 0..255
	 * @param analogdata2 0..255
	 * @return K8055_ERROR or 0
	 */
	private native int CSetAllValues( int digitaldata, 
	                                  int analogdata1, int analogdata2 );
	
	/**
	 * Set all values at once.
	 * 
	 * @param digitaldata 0..255
	 * @param analogdata1 0..255
	 * @param analogdata2 0..255
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized void SetAllValues( int digitaldata, 
	                                       int analogdata1, int analogdata2 )
	throws JK8055Exception
	{
		if( digitaldata<1 || 255<digitaldata ) {
			throw new IllegalArgumentException( "digitaldata: "+digitaldata );
		}
		if( analogdata1<1 || 255<analogdata1 ) {
			throw new IllegalArgumentException( "analogdata1: "+analogdata1 );
		}
		if( analogdata2<1 || 255<analogdata2 ) {
			throw new IllegalArgumentException( "analogdata2: "+analogdata2 );
		}
		int retval = CSetAllValues( digitaldata, analogdata1, analogdata2);
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetAllValues failed" );
		}
	}
	
	/**
	 * @param deviceno 0..3
	 * @return K8055_ERROR or 0..3
	 */
	private native int CSetCurrentDevice( int deviceno );
	
	/**
	 * The function set the current controlled device. 
	 * If no device with the address parameter was found.
	 * an JK8055Exception is thrown.
	 * 
	 * @param deviceno Value 0 to 3, which corresponds to the device address.
	 * @throws JK8055Exception indicates an error.
	 * @throws IllegalArgumentException indicates an invalid parameter.
	 */
	public synchronized void SetCurrentDevice( int deviceno ) 
	throws JK8055Exception
	{
		if( deviceno<0 || 3<deviceno ) {
			throw new IllegalArgumentException( "deviceno: "+deviceno );
		}
		int retval = CSetCurrentDevice( deviceno );
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SetCurrentDevice failed" );
		}
	}
	
	/**
	 * @return K8055_ERROR or 0..15
	 */
	private native int CSearchDevices();
	
	/**
	 * The function returns all connected devices on the computer.
	 * The returned value is a bit field.
	 * <br><b>Note</b>
	 * <br> Once a specific device address is connected with a program 
	 * another program can't get access to it.
	 * 
	 * @return 
	 * <pre> 
	 * Bin 0000, Dec 0 : No devices was found
	 * Bin 0001, Dec 1 : Card address 0 was found.
	 * Bin 0010, Dec 2 : Card address 1 was found.
	 * Bin 0100, Dec 4 : Card address 2 was found.
	 * Bin 1000, Dec 8 : Card address 3 was found.
	 * Example : return value 9 = devices with address 0 and 3 are connected.
	 * </pre>
	 * @throws JK8055Exception
	 */
	public synchronized int SearchDevices()
	throws JK8055Exception
	{
		int retval = CSearchDevices();
		if( retval == K8055_ERROR ) {
			throw new JK8055Exception( "SearchDevices failed" );
		}
		return retval;
	}
	
	private native String CVersion();
	
	/**
	 * Return the version number of the libk8055 library used.
	 * 
	 * @return a string like "0.3"
	 */
	public synchronized String Version() {
		return CVersion();
	}
	
}
