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

#include "AudioData.h"
#include "..\Waverly.h"
#include "..\Engine\Transport.h"

AudioData::AudioData(Waverly* _AWaverly) {

	AWaverly = _AWaverly;
	transport = AWaverly->transport;
	transport->setAudioData(this);

	leftLevel = 0.0f;
	rightLevel = 0.0f;
	leftPan = 0.5f;
	rightPan = 0.5f;

	tracks = new float* [2];
	tracks[0] = NULL;
	tracks[1] = NULL;
}

//destruct
AudioData::~AudioData() {

	transport->stop();

	for (int i = 0; i < 2; i++) {
		if (tracks[i] != NULL)
			delete tracks[i];
	}
	delete[] tracks;
}


//- AudioFile i/o methods -------------------------------------------------------

void AudioData::close() {
}

//- track management ----------------------------------------------------------

float AudioData::getLeftLevel() { 
	return (transport->isCurPlaying() ? leftLevel : 0.0f); 
}

float AudioData::getRightLevel() { 
	return (transport->isCurPlaying() ? rightLevel : 0.0f); 
}

