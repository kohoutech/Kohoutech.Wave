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

#include "AudioFile.h"
#include "..\TransWave.h"
#include "..\Engine\Transport.h"

#include <math.h>

#define TRACKLIMIT 10
#define TRACKDELTA 5

//new AudioFile
AudioFile::AudioFile(Megaphone* _megaphone, char* filename) {

	megaphone = _megaphone;
	transport = megaphone->transport;
	transport->setAudioFile(this);

	leftLevel = 0.0f;
	rightLevel = 0.0f;
	leftPan = 0.5f;
	rightPan = 0.5f;

	tracks = new float* [2];
	tracks[0] = NULL;
	tracks[1] = NULL;

	importTracksFromWavFile(filename);
}

//destruct
AudioFile::~AudioFile() {

	transport->stop();

	for (int i = 0; i < 2; i++) {
		if (tracks[i] != NULL)
			delete tracks[i];
	}
	delete[] tracks;
}

//- AudioFile i/o methods -------------------------------------------------------

void AudioFile::close() {
}

//- track management ----------------------------------------------------------

float AudioFile::getLeftLevel() { 
	return (transport->isCurPlaying() ? leftLevel : 0.0f); 
}

float AudioFile::getRightLevel() { 
	return (transport->isCurPlaying() ? rightLevel : 0.0f); 
}

//- data import/export methods ------------------------------------------------

typedef struct wavHdr {
	char sig[4];
	int filesize;
	char format[4];
} WavHdr;

typedef struct fmtChunk {
	char sig[4];
	int size;
	short format;
	short channels;
	int samplerate;
	int byterate;
	short blockallign;
	short bitdepth;
} FmtChunk;

typedef struct dataChunk {
	char sig[4];
	int size;
} DataChunk;

int AudioFile::importTracksFromWavFile(char* filename) {

	FILE* wavFile = fopen (filename, "rb"); 

	//file hdr
	WavHdr wavHdr;
	int bread = fread (&wavHdr, sizeof(WavHdr), 1, wavFile);

	//fmt chunk
	FmtChunk fmtChunk;
	bread = fread (&fmtChunk, sizeof(FmtChunk), 1, wavFile);

	//data chunk
	DataChunk dataChunk;
	bread = fread (&dataChunk, sizeof(DataChunk), 1, wavFile);
	byte* wavData = new byte[dataChunk.size];
	bread = fread (wavData, dataChunk.size, 1, wavFile);

	dataSize = dataChunk.size / fmtChunk.blockallign;
	int sampleSize = fmtChunk.bitdepth / 8;

	int trackNum = 0;
	for (int channel = 0; channel < fmtChunk.channels; channel++) {

		float* track = new float[dataSize];
		int bpos = channel * sampleSize;
		for (int i = 0; i < dataSize; i++ ) {

			float sampleVal;
			switch (sampleSize) {
			case 1 : 
				sampleVal = (wavData[bpos] / 255.0f) - 0.5f;
				break;
			case 2 : {
				short samp = *(short*)&wavData[bpos];
				sampleVal = (samp / 32767.0f);
				break;
					 }
			default: 
				sampleVal = 0.0f;
				break;
			}
			track[i] = sampleVal;
			bpos += fmtChunk.blockallign;			
		}
		tracks[channel] = track;
	}
	delete [] wavData;

	sampleRate = fmtChunk.samplerate;
	duration = (dataSize + sampleRate - 1) / sampleRate;

	return fmtChunk.channels;
}

//printf("there's no sun in the shadow of the wizard.\n");
