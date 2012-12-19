/* $Id: k8055.h,v 1.4 2008/05/21 20:25:51 mr_brain Exp $

   This file is part of the libk8055 Library.

   The libk8055 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The libk8055 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the
   Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   http://opensource.org/licenses/
*/
#ifdef __cplusplus
extern "C" {
#endif

/* prototypes */
int OpenDevice(long board_address);
int CloseDevice();
long ReadAnalogChannel(long Channelno);
int ReadAllAnalog(long* data1, long* data2);
int OutputAnalogChannel(long channel, long data);
int OutputAllAnalog(long data1,long data2);
int ClearAllAnalog();
int ClearAnalogChannel(long channel);
int SetAnalogChannel(long channel);
int SetAllAnalog();
int WriteAllDigital(long data);
int ClearDigitalChannel(long channel);
int ClearAllDigital();
int SetDigitalChannel(long channel);
int SetAllDigital();
int ReadDigitalChannel(long channel);
long ReadAllDigital();
int ResetCounter(long counternr);
long ReadCounter(long counterno);
int SetCounterDebounceTime(long counterno, long debouncetime);
int ReadAllValues (long int  *data1, long int *data2, long int *data3, long int *data4, long int *data5);
int SetAllValues(int digitaldata, int addata1, int addata2);
long SetCurrentDevice(long deviceno);
long SearchDevices(void);
char *Version(void);
#ifdef __cplusplus
}
#endif

