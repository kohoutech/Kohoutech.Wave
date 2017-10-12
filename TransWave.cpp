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

#include "TransWave.h"
#include "Engine\Transport.h"
#include "System\WaveOutDevice.h"
#include "Audio\AudioFile.h"

#include <conio.h>

#define WAVEBUFCOUNT  10
#define WAVEBUFDURATION   100		//buf duration in ms

Megaphone* Megaphone::MegaphoneB;

//- SignalsA iface exports ----------------------------------------------------

extern "C" __declspec(dllexport) void MegaphoneInit() {

	Megaphone::MegaphoneB = new Megaphone();
}

extern "C" __declspec(dllexport) void MegaphoneShutDown() {

	delete Megaphone::MegaphoneB;
}

//- transport exports ---------------------------------------------------------

extern "C" __declspec(dllexport) void TransportPlay() {

	Megaphone::MegaphoneB->transport->play();	
}

extern "C" __declspec(dllexport) void TransportPause() {

	Megaphone::MegaphoneB->transport->pause();
}

extern "C" __declspec(dllexport) void TransportStop() {

	Megaphone::MegaphoneB->transport->stop();
}

extern "C" __declspec(dllexport) void TransportRewind(int speed) {

//	Megaphone::MegaphoneB->transport->rewind(speed);
}

extern "C" __declspec(dllexport) void TransportFastForward(int speed) {

//	Megaphone::MegaphoneB->transport->fastForward(speed);
}

extern "C" __declspec(dllexport) void TransportSetVolume(float volume) {

	Megaphone::MegaphoneB->transport->setLeftOutLevel(volume);
	Megaphone::MegaphoneB->transport->setRightOutLevel(volume);
}

extern "C" __declspec(dllexport) void TransportSetBalance(float balance) {

	return Megaphone::MegaphoneB->currentAudioFile->setPan(balance);
}

extern "C" __declspec(dllexport) float TransportGetLeftLevel() {

	return Megaphone::MegaphoneB->currentAudioFile->getLeftLevel();
}

extern "C" __declspec(dllexport) float TransportGetRightLevel() {

	return Megaphone::MegaphoneB->currentAudioFile->getRightLevel();
}

extern "C" __declspec(dllexport) int TransportGetCurrentPos() {

	return Megaphone::MegaphoneB->transport->getCurrentPos();		//in samples
}

extern "C" __declspec(dllexport) void TransportSetCurrentPos(int curPos) {

	Megaphone::MegaphoneB->transport->setCurrentPos(curPos);		//in samples
}

//not implemented yet
extern "C" __declspec(dllexport) void TransportSetWaveOut(int deviceIdx) {
}

//- project exports -----------------------------------------------------------

extern "C" __declspec(dllexport) void AudioOpen(char* filename) {

	Megaphone::MegaphoneB->openAudioFile(filename);
}

extern "C" __declspec(dllexport) void AudioClose() {

	Megaphone::MegaphoneB->closeAudioFile();
}

extern "C" __declspec(dllexport) int AudioGetSampleRate() {

	return Megaphone::MegaphoneB->currentAudioFile->sampleRate;
}

extern "C" __declspec(dllexport) int AudioGetDuration() {

	return Megaphone::MegaphoneB->currentAudioFile->duration;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//cons
Megaphone::Megaphone()
{
	transport = new Transport();
	
	//use default out for now
	loadWaveOutDevice(WAVE_MAPPER);		// open output device

	currentAudioFile = NULL;
}

//shut down
Megaphone::~Megaphone()
{
	closeAudioFile();
	waveOut->close();	
	delete transport;
}

//- project methods ------------------------------------------------------------

void Megaphone::openAudioFile(char* filename) 
{
	currentAudioFile = new AudioFile(this, filename);
}

void Megaphone::closeAudioFile() {

	if (currentAudioFile != NULL) {
		delete currentAudioFile;
	}
	currentAudioFile = NULL;
}

//- device methods ------------------------------------------------------------

BOOL Megaphone::loadWaveOutDevice	(int devID)
{
	BOOL result = FALSE;

	waveOut = new WaveOutDevice();
	waveOut->setBufferCount(WAVEBUFCOUNT);
	waveOut->setBufferDuration(WAVEBUFDURATION);
	result = waveOut->open(devID, 44100, 16, 2);		//stereo out

	transport->setWaveOut(waveOut);
	transport->setBlockSize(4410);

	return result;
}

	//printf("there's no sun in the shadow of the wizard.\n");
