/* ----------------------------------------------------------------------------
LibWaverly : a library for playing, editing and storing audio wave data
Copyright (C) 2005-2017  George E Greaney

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
----------------------------------------------------------------------------*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

using Transonic.Wave.System;

namespace Transonic.Wave
{
    public class Waverly
    {
        //communication with wave.dll
        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void WaverlyInit();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void WaverlyShutDown();

        //transport calls -----------------------------------------------------

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportPlay();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportStop();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportPause();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportRewind();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportFastForward(int speed);

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportSetVolume(float volume);

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportSetBalance(float balance);

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern int TransportGetCurrentPos();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern int TransportSetCurrentPos(int pos);

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransportSetWaveOut(int deviceIdx);

        //project calls -------------------------------------------------------

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AudioOpen(string filename);

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AudioClose();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern int AudioGetSampleRate();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern int AudioGetDuration();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern float AudioGetLeftLevel();

        [DllImport("Waverly.DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern float AudioGetRightLevel();


//- signals methods -----------------------------------------------------------

        IWaveView waveWindow;        

        public WaveDevices waveDevices;

        public Waverly(IWaveView _mw)
        {
            waveWindow = _mw;
            waveDevices = new WaveDevices(this);
            WaverlyInit();
        }

        public void shutDown() 
        {
            WaverlyShutDown();
        }

//- transport methods ---------------------------------------------------------

        public void playTransport()
        {
            TransportPlay();
        }

        public void pauseTransport()
        {
            TransportPause();
        }

        public void stopTransport()
        {
            TransportStop();
        }

        public void rewindTransport()
        {
            TransportRewind();
        }

        public void fastForwardTransport(int speed)
        {
            //TransportFastForward(speed);      //doesn't work!
        }

        public void setVolume(float volume)
        {
            TransportSetVolume(volume);
        }

        public void setBalance(float balance)
        {
            TransportSetBalance(balance);
        }

        public int getCurrentPos()
        {
            return TransportGetCurrentPos();    //in samples
        }

        public void setCurrentPos(int pos)
        {
            TransportSetCurrentPos(pos);		//in samples
        }

        public void setWaveOutDevice(int devIdx)
        {
            TransportSetWaveOut(devIdx);
        }
        
//- audio data methods --------------------------------------------------------

        public void openAudioFile(String filename)
        {
            AudioOpen(filename);
        }

        public void closeAudioFile()
        {
            AudioClose();
        }

        public int getAudioSampleRate()
        {
            return AudioGetSampleRate();
        }

        public int getAudioDuration()
        {
            return AudioGetDuration();
        }

        public List<String> getInputDeviceList()
        {
            return waveDevices.getInDevNameList();
        }

        public List<String> getOutputDeviceList()
        {
            return waveDevices.getOutDevNameList();
        }

        public float getAudioLeftLevel()
        {
            return AudioGetLeftLevel();
        }

        public float getAudioRightLevel()
        {
            return AudioGetRightLevel();
        }
    }
}

//  Console.WriteLine(" there's no sun in the shadow of the wizard");
