/* ----------------------------------------------------------------------------
LibTransWave : a library for playing, editing and storing audio wave data
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

#if !defined(Megaphone_H)
#define Megaphone_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

class AudioFile;
class Transport;
class WaveOutDevice;

class Megaphone
{
public:
	Megaphone();
	~Megaphone();

	static Megaphone* MegaphoneB;		//for MegaphoneA communication

	Transport* transport;
	AudioFile* currentAudioFile;

	void openAudioFile(char* filename);
	void closeAudioFile();

	WaveOutDevice* waveOut;

protected:

	BOOL loadWaveOutDevice(int devID);
};

#endif // Megaphone_H
