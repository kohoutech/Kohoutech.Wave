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

#if !defined(PROJECT_H)
#define PROJECT_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

class Waverly;
class Transport;

class AudioFile
{
public:
	AudioFile(Waverly* AWaverly, char* filename);
	~AudioFile();

	void close();

	Waverly* AWaverly;
	Transport* transport;

	int sampleRate;
	int duration;			//in seconds
	int dataSize;
	float leftPan, rightPan;
	float leftLevel;
	float rightLevel;
		
	inline float getLeftPan() { return leftPan; }
	inline float getRightPan() { return rightPan; }
	void setPan(float _pan) { rightPan = _pan; leftPan = 1.0f - rightPan; }

	float getLeftLevel();
	float getRightLevel();

	float** tracks;
	float* getTrack(int trackNum) { return tracks[trackNum]; };

protected:

	int importTracksFromWavFile(char* filename);

};

#endif // PROJECT_H
