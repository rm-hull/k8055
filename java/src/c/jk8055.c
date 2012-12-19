/*  
 * Copyright (C) 2008 Martin Pischky (mailto:martin@pischky.de)  
 *
 * This file (jk8055.c) is part of libk8055/jk8055.
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
 * $Id: jk8055.c,v 1.2 2008/07/12 11:32:01 pischky Exp $
 * 
 */

#include <jni.h>
#ifdef USE_VELLEMAN_DLL
    // assumes that Velleman DLL Version 2 has been installed
    #define bool int
    #include <K8055D_C.h> 
#else
    // assumes that the c library as been installed with header file
    #include <k8055.h> 
#endif 
#include <stdio.h>
#include "jk8055.h"

#define K8055_ERROR -1

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    COpenDevice
 * Signature: (I)I
 */
JNIEXPORT jint 
JNICALL Java_net_sf_libk8055_jk8055_JK8055_COpenDevice
(JNIEnv* env, jobject obj, jint cardAddress)
{
	return OpenDevice(cardAddress);
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CCloseDevice
 * Signature: ()I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CCloseDevice
(JNIEnv* env, jobject obj)
{
	return CloseDevice();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CReadAnalogChannel
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CReadAnalogChannel
(JNIEnv* env, jobject obj, jint channel)
{
	return ReadAnalogChannel( channel );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CReadAllAnalog
 * Signature: (Lnet/sf/libk8055/jk8055/JK8055$AllAnalog;)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CReadAllAnalog
(JNIEnv* env, jobject obj, jobject data)
{
	long a1, a2;
	jfieldID fid1, fid2;
	int retval;
	jclass cls;

	retval = ReadAllAnalog( &a1, &a2 );
	if( retval == K8055_ERROR ) return retval;
	cls = (*env)->GetObjectClass( env, data );
	fid1 = (*env)->GetFieldID( env, cls, "data1", "I" );
	if( fid1 == NULL ) return K8055_ERROR;
	fid2 = (*env)->GetFieldID( env, cls, "data2", "I" );
	if( fid2 == NULL ) return K8055_ERROR;
	(*env)->SetIntField( env, data, fid1, (jint) a1 );
	(*env)->SetIntField( env, data, fid2, (jint) a2 );
	return 0;
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    COutputAnalogChannel
 * Signature: (II)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_COutputAnalogChannel
(JNIEnv* env, jobject obj, jint channel, jint data)
{
	return OutputAnalogChannel( channel, data );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    COutputAllAnalog
 * Signature: (II)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_COutputAllAnalog
(JNIEnv* env, jobject obj, jint data1, jint data2)
{
	return OutputAllAnalog( data1, data2 );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CClearAllAnalog
 * Signature: ()I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CClearAllAnalog
(JNIEnv* env, jobject obj)
{
	return ClearAllAnalog();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CClearAnalogChannel
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CClearAnalogChannel
(JNIEnv* env, jobject obj, jint channel)
{
	return ClearAnalogChannel( channel );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetAnalogChannel
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetAnalogChannel
(JNIEnv* env, jobject obj, jint channel)
{
	return SetAnalogChannel( channel );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetAllAnalog
 * Signature: ()I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetAllAnalog
(JNIEnv* env, jobject obj)
{
	return SetAllAnalog();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CWriteAllDigital
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CWriteAllDigital
(JNIEnv* env, jobject obj, jint data)
{
	return WriteAllDigital( data );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CClearDigitalChannel
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CClearDigitalChannel
(JNIEnv* env, jobject obj, jint channel)
{
	return ClearDigitalChannel( channel );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CClearAllDigital
 * Signature: ()I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CClearAllDigital
(JNIEnv* env, jobject obj)
{
	return ClearAllDigital();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetDigitalChannel
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetDigitalChannel
(JNIEnv* env, jobject obj, jint channel)
{
	return SetDigitalChannel( channel );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetAllDigital
 * Signature: ()I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetAllDigital
(JNIEnv* env, jobject obj)
{
	return SetAllDigital();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CReadDigitalChannel
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CReadDigitalChannel
(JNIEnv* env, jobject obj, jint channel)
{
	return ReadDigitalChannel( channel );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CReadAllDigital
 * Signature: ()I
 */
JNIEXPORT jint 
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CReadAllDigital
(JNIEnv* env, jobject obj)
{
	return ReadAllDigital();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CResetCounter
 * Signature: (I)I
 */
JNIEXPORT jint 
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CResetCounter
(JNIEnv* env, jobject obj, jint counterno)
{
	return ResetCounter( counterno );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CReadCounter
 * Signature: (I)I
 */
JNIEXPORT jint 
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CReadCounter
(JNIEnv* env, jobject obj, jint counterno)
{
	return ReadCounter( counterno ); 
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetCounterDebounceTime
 * Signature: (II)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetCounterDebounceTime
(JNIEnv* env, jobject obj, jint counterno, jint debouncetime)
{
	return SetCounterDebounceTime( counterno, debouncetime );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CReadAllValues
 * Signature: (Lnet/sf/libk8055/jk8055/JK8055$AllValues;)I
 */
JNIEXPORT jint 
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CReadAllValues
(JNIEnv* env, jobject obj, jobject av)
{
	long input, analog1, analog2, counter1, counter2;
	int retval;
	jfieldID fid_input, fid_analog1, fid_analog2, fid_counter1, fid_counter2;
	jclass cls;

    #ifdef USE_VELLEMAN_DLL
        input = ReadAllDigital();
        if( input == K8055_ERROR ) return input;
        retval = ReadAllAnalog( &analog1, &analog2 );
        if( retval == K8055_ERROR ) return retval;
        counter1 = ReadCounter( 1 );
        if( counter1 == K8055_ERROR ) return counter1;
        counter2 = ReadCounter( 2 );
        if( counter2 == K8055_ERROR ) return counter2;
    #else
        retval = ReadAllValues( &input, &analog1, &analog2, &counter1, &counter2 );
        if( retval == K8055_ERROR ) return retval;
    #endif
	cls = (*env)->GetObjectClass( env, av );
	fid_input = (*env)->GetFieldID( env, cls, "input", "I" );
	if( fid_input == NULL ) return K8055_ERROR;	
	fid_analog1 = (*env)->GetFieldID( env, cls, "analog1", "I" );
	if( fid_analog1 == NULL ) return K8055_ERROR;
	fid_analog2 = (*env)->GetFieldID( env, cls, "analog2", "I" );
	if( fid_analog2 == NULL ) return K8055_ERROR;
	fid_counter1 = (*env)->GetFieldID( env, cls, "counter1", "I" );
	if( fid_counter1 == NULL ) return K8055_ERROR;
	fid_counter2 = (*env)->GetFieldID( env, cls, "counter2", "I" );
	if( fid_counter2 == NULL ) return K8055_ERROR;
	(*env)->SetIntField( env, av, fid_input, (jint) input );
	(*env)->SetIntField( env, av, fid_analog1, (jint) analog1 );
	(*env)->SetIntField( env, av, fid_analog2, (jint) analog2 );
	(*env)->SetIntField( env, av, fid_counter1, (jint) counter1 );
	(*env)->SetIntField( env, av, fid_counter2, (jint) counter2 );
	return 0;
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetAllValues
 * Signature: (III)I
 */
JNIEXPORT jint 
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetAllValues
(JNIEnv* env, jobject obj, jint digitaldata, jint analogdata1, jint analogdata2)
{
    int retval;
    #ifdef USE_VELLEMAN_DLL
        retval = WriteAllDigital( digitaldata );
        if( retval == K8055_ERROR ) return retval;
        retval = OutputAllAnalog( analogdata1, analogdata2 );
        if( retval == K8055_ERROR ) return retval;
    #else    
	    retval = SetAllValues( digitaldata, analogdata1, analogdata2 );
	#endif
	return retval;
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSetCurrentDevice
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSetCurrentDevice
(JNIEnv* env, jobject obj, jint deviceno)
{
	return SetCurrentDevice( deviceno );
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CSearchDevices
 * Signature: ()I
 */
JNIEXPORT jint
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CSearchDevices
(JNIEnv* env, jobject obj)
{
	return SearchDevices();
}

/*
 * Class:     net_sf_libk8055_jk8055_JK8055
 * Method:    CVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring
JNICALL Java_net_sf_libk8055_jk8055_JK8055_CVersion
(JNIEnv* env, jobject obj)
{
    #ifdef USE_VELLEMAN_DLL
        return (*env)->NewStringUTF( env, "unknown" );
    #else
	    return (*env)->NewStringUTF( env, Version() );
	#endif 
}
