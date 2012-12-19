/*  
 * Copyright (C) 2008 Martin Pischky (mailto:martin@pischky.de)  
 *
 * This file (JK8055Test.java) is part of libk8055/jk8055.
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
 * $Id: JK8055Test.java,v 1.4 2008/07/14 07:47:55 pischky Exp $
 * 
 */

package net.sf.libk8055.jk8055;

import junit.framework.TestCase;

/**
 * JUnit tests for class JK8055.
 * 
 * If you want to run this from eclipse add
 *   -Djava.library.path=build/c
 * to VM arguments in run configuration.
 * 
 * @author Martin Pischky
 */
public class JK8055Test extends TestCase {

	public static final int DEVICE = 0;

	/*
	 * @see TestCase#setUp()
	 */
	protected void setUp() throws Exception {
		super.setUp();
	}

	/*
	 * @see TestCase#tearDown()
	 */
	protected void tearDown() throws Exception {
		super.tearDown();
	}

	static void sleep( int ms ) {
		try {
			Thread.sleep( ms );
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public void testGetInstance() {
		JK8055.getInstance();
	}

	public void testOpenCloseDevice() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.CloseDevice();
	}

	public void testReadAnalogChannel() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		int a1 = jk8055.ReadAnalogChannel(1);
		int a2 = jk8055.ReadAnalogChannel(2);
		assertTrue("a1>=0",  a1>=0  );
		assertTrue("a1<=255",a1<=255);
		assertTrue("a2>=0",  a2>=0  );
		assertTrue("a2<=255",a2<=255);
		jk8055.CloseDevice();
	}

	public void testReadAllAnalog() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		JK8055.AllAnalog aa = jk8055.ReadAllAnalog();
		assertTrue( "aa.data1>=0",  aa.data1>=0   );
		assertTrue( "aa.data1<=255",aa.data1<=255 );
		assertTrue( "aa.data2>=0",  aa.data2>=0   );
		assertTrue( "aa.data2<=255",aa.data2<=255 );
		int a1 = jk8055.ReadAnalogChannel(1);
		int a2 = jk8055.ReadAnalogChannel(2);
		assertTrue( "a1 ~= aa.data1", Math.abs(a1-aa.data1)<2  );
		assertTrue( "a2 ~= aa.data2", Math.abs(a2-aa.data2)<2  );
		jk8055.CloseDevice();
	}

	public void testOutputAnalogChannel() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.OutputAnalogChannel( 1, 0 );
		jk8055.OutputAnalogChannel( 2, 255 );
		sleep( 500 );
		for (int i = 0; i <= 255; i++) {
			jk8055.OutputAnalogChannel( 1, i );
			jk8055.OutputAnalogChannel( 2, 255-i );
		}
		for (int i = 255; i >= 0; i--) {
			jk8055.OutputAnalogChannel( 1, i );
			jk8055.OutputAnalogChannel( 2, 255-i );
		}
		sleep( 500 );
		jk8055.OutputAnalogChannel( 1, 0 );
		jk8055.OutputAnalogChannel( 2, 0 );
		jk8055.CloseDevice();
	}

	public void testOutputAllAnalog() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.OutputAllAnalog( 0, 255 );
		sleep( 500 );
		for (int i = 0; i <= 255; i++) {
			jk8055.OutputAllAnalog( i, 255-i );
		}
		for (int i = 255; i >= 0; i--) {
			jk8055.OutputAllAnalog( i, 255-i );
		}
		sleep( 500 );
		jk8055.OutputAllAnalog( 0, 0 );
		jk8055.CloseDevice();
	}

	public void testClearAllAnalog() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.ClearAllAnalog();
		jk8055.CloseDevice();
	}

	public void testClearAnalogChannel() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.ClearAnalogChannel( 1 );
		jk8055.ClearAnalogChannel( 2 );
		jk8055.CloseDevice();
	}

	public void testSetAnalogChannel() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.SetAnalogChannel( 1 );
		jk8055.SetAnalogChannel( 2 );
		jk8055.CloseDevice();
	}

	public void testSetAllAnalog() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.SetAllAnalog();
		jk8055.CloseDevice();
	}

	public void testWriteAllDigital() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.WriteAllDigital( 0 );
		for (int i = 0; i <= 7; i++) {
			jk8055.WriteAllDigital( 1<<i );
			sleep( 50 );
		}
		for (int i = 7; i >= 0; i--) {
			jk8055.WriteAllDigital( 1<<i );
			sleep( 50 );
		}
		jk8055.WriteAllDigital( 0 );
		jk8055.CloseDevice();
	}

	public void testClearDigitalChannel() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		for (int i = 1; i <= 8; i++) {
			jk8055.ClearDigitalChannel( i );
		}
		jk8055.CloseDevice();
	}

	public void testClearAllDigital() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.ClearAllDigital();
		jk8055.CloseDevice();
	}

	public void testSetDigitalChannel() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		for (int i = 1; i <= 8; i++) {
			jk8055.SetDigitalChannel( i );
		}
		jk8055.CloseDevice();
	}

	public void testSetAllDigital() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.SetAllDigital();
		jk8055.CloseDevice();
	}

	public void testReadDigitalChannel() throws JK8055Exception {
		System.out.println( "---- testReadDigitalChannel()" );
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		for (int i = 1; i <= 5; i++) {
			boolean b = jk8055.ReadDigitalChannel( i );
			System.out.println( "Inp"+i+"="+b );
		}
		jk8055.CloseDevice();
	}

	public void testReadAllDigital() throws JK8055Exception {
		System.out.println( "---- testReadAllDigital()" );
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		int data = jk8055.ReadAllDigital();
		assertTrue("data>=0",  data>=0  );
		assertTrue("data<=255",data<=255);
		for (int i = 1; i <= 5; i++) {
			boolean b = (data & (1<<(i-1))) != 0;
			System.out.println( "Inp"+i+"="+b );
		}
		jk8055.CloseDevice();
	}

	public void testResetCount() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.ResetCount( 1 );
		jk8055.ResetCount( 2 );
		jk8055.CloseDevice();
	}

	public void testReadCounter() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		int c1 = jk8055.ReadCounter( 1 );
		int c2 = jk8055.ReadCounter( 2 );
		assertTrue( "c1", 0<=c1 && c1<=65535 );
		assertTrue( "c2", 0<=c2 && c2<=65535 );
		jk8055.CloseDevice();
	}

	public void testSetCounterDebounceTime() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.SetCounterDebounceTime( 1, 1214 );
		jk8055.SetCounterDebounceTime( 2, 1214 );
		jk8055.CloseDevice();
	}

	public void testReadAllValues() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		JK8055.AllValues av = jk8055.ReadAllValues();
		assertTrue( "av.input>=0",        av.input>=0     );
		assertTrue( "av.input<=32",       av.input<=32    );
		assertTrue( "av.analog1>=0",      av.analog1>=0   );
		assertTrue( "av.analog1<=255",    av.analog2<=255 );
		assertTrue( "av.analog2>=0",      av.analog2>=0   );
		assertTrue( "av.analog2<=255",    av.analog2<=255 );
		assertTrue( "av.counter1>=0",     av.counter1>=0   );
		assertTrue( "av.counter1<=65535", av.counter1<=65535 );
		assertTrue( "av.counter2>=0",     av.counter2>=0   );
		assertTrue( "av.counter2<=65535", av.counter2<=65535 );
		int a1 = jk8055.ReadAnalogChannel(1);
		int a2 = jk8055.ReadAnalogChannel(2);
		assertTrue( "a1 ~= aa.data1", Math.abs(a1-av.analog1)<2  );
		assertTrue( "a2 ~= aa.data2", Math.abs(a2-av.analog2)<2  );
		jk8055.CloseDevice();
	}

	public void testSetAllValues() throws JK8055Exception {
		JK8055 jk8055 = JK8055.getInstance();
		jk8055.OpenDevice( DEVICE );
		jk8055.SetAllValues( 123, 232, 87 );
		jk8055.CloseDevice();
	}

	public void testSetCurrentDevice() throws JK8055Exception {
		System.out.println( "---- testSetCurrentDevice()" );
		JK8055 jk8055 = JK8055.getInstance();
		int d = jk8055.SearchDevices();
		if( d == 0 ) {
			System.out.println( "No devices found." );
		} else {
			for (int i = 0; i <= 3; i++) {
				if( (d & (1<<i)) != 0 ) {
					System.out.println( "Device with card address "+i+" was found." );
				}
			}
		}
		jk8055.CloseDevice(); // required for windows version ?!?
		for (int i = 0; i <= 3; i++) {
			if( (d & (1<<i)) != 0 ) {
				System.out.println( "OpenDevice("+i+")" );
				jk8055.OpenDevice( i );
			}
		}
		for (int i = 0; i <= 3; i++) {
			if( (d & (1<<i)) != 0 ) {
				System.out.println( "SetCurrentDevice("+i+")" );
				jk8055.SetCurrentDevice( i );
				System.out.println( "WriteAllDigital("+(1<<i)+")" );
				jk8055.WriteAllDigital( 1<<i );
			}
		}
		for (int i = 0; i <= 3; i++) {
			if( (d & (1<<i)) == 0 ) {
				boolean exThrown = false;
				try {
					jk8055.SetCurrentDevice( i );
				} catch( JK8055Exception ex ) {
					exThrown = true;
				}
				assertTrue( "exThrown", exThrown );
			}
		}
		for (int i = 0; i <= 3; i++) {
			if( (d & (1<<i)) != 0 ) {
				System.out.println( "SetCurrentDevice("+i+")" );
				jk8055.SetCurrentDevice( i );
				System.out.println( "CloseDevice()" );
				jk8055.CloseDevice();       
			}
		}
	}

	public void testCloseAllOpenDevices() throws JK8055Exception {
		System.out.println( "---- testCloseAllOpenDevices()" );
		JK8055 jk8055 = JK8055.getInstance();
		int d = jk8055.SearchDevices();
		if( d == 0 ) {
			System.out.println( "No devices found." );
		} else {
			for (int i = 0; i <= 3; i++) {
				if( (d & (1<<i)) != 0 ) {
					System.out.println( "Device with card address "+i+" was found." );
				}
			}
		}
		jk8055.CloseDevice(); // required for windows version ?!?
		for (int i = 0; i <= 3; i++) {
			if( (d & (1<<i)) != 0 ) {
				System.out.println( "OpenDevice("+i+")" );
				jk8055.OpenDevice( i );
			}
		}
		for (int i = 0; i <= 3; i++) {
			if( (d & (1<<i)) != 0 ) {
				System.out.println( "SetCurrentDevice("+i+")" );
				jk8055.SetCurrentDevice( i );
				System.out.println( "WriteAllDigital("+(1<<i)+")" );
				jk8055.WriteAllDigital( 1<<i );
			}
		}
		jk8055.CloseAllOpenDevices();
	}

	public void testSearchDevices() throws JK8055Exception {
		System.out.println( "---- testSearchDevices()" );
		JK8055 jk8055 = JK8055.getInstance();
		//jk8055.OpenDevice( DEVICE );
		// Windows Version (Velleman DDL v2)
		// does not return devices already opened
		int d = jk8055.SearchDevices();
		if( d == 0 ) {
			System.out.println( "No devices found." );
		} else {
			for (int i = 0; i <= 3; i++) {
				if( (d & (1<<i)) != 0 ) {
					System.out.println( "Device with card address "+i+" was found." );
				}
			}
		}
		jk8055.CloseDevice();
	}

	public void testVersion() {
		System.out.println( "---- testVersion()" );
		assertNotNull( JK8055.getInstance().Version() );
		assertTrue( JK8055.getInstance().Version().length() > 0 );
		System.out.println( "version=\""+JK8055.getInstance().Version()+"\"" );
	}

}
