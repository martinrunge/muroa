/*
 Copyright (c) 2002-2014 "Martin Runge"

 IResamplerBase.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "IResamplerBase.h"

#include <cstdint>
#include <iostream>
#include <map>

#include "CSpeexResampler.h"
#include "CSoxResampler.h"
#include "CMuroaFPResampler.h"

#include <math.h>

using namespace std;

IResamplerBase::IResamplerBase() {
	m_infile = 0;
	m_outfile = 0;
	m_use_float = true;
	m_num_out_frames = 0;
	m_inbuffer_f = 0;
	m_inbuffer_i16 = 0;
	m_outbuffer_f = 0;
	m_outbuffer_i16 = 0;

}

IResamplerBase::~IResamplerBase() {
	// TODO Auto-generated destructor stub
}

int IResamplerBase::openInfile(std::string infile) {
	if(m_infile != 0) {
		delete m_infile;
	}
	m_infile = new SndfileHandle(infile, SFM_READ);
	m_in_num_channels = m_infile->channels();
	m_in_sample_rate = m_infile->samplerate();
	m_num_in_frames = m_infile->frames();

    m_out_buffer_size = m_num_in_frames * (float)m_out_sample_rate / m_in_sample_rate;
    m_out_buffer_size += 200; // just to be safe

    double max_peak;
    m_infile->command(SFC_GET_SIGNAL_MAX, &max_peak, sizeof (max_peak));
    cout << "SFC_GET_SIGNAL_MAX: " << max_peak << endl;

    int usescale = 0x7FFF;
    int scale = m_infile->command(SFC_SET_SCALE_FLOAT_INT_READ, &usescale, sizeof(usescale));
    cout << "SFC_SET_SCALE_FLOAT_INT_READ: " << scale << endl;
}

int IResamplerBase::openOutfile(std::string outfile, int sampleRate) {
	int rc = -1;
	if(m_outfile != 0) {
		delete m_outfile;
	}
	int format;
	if(m_use_float) {
		format=SF_FORMAT_WAV | SF_FORMAT_FLOAT;
	} else {
	    format=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	}

    const int channels=1;
    m_out_sample_rate = sampleRate;

    if(m_in_sample_rate != 0) {
    	m_out_buffer_size = m_num_in_frames * (float)m_out_sample_rate / m_in_sample_rate;
    	m_out_buffer_size += 200; // just to be safe
    }

	m_outfile = new SndfileHandle(outfile, SFM_WRITE, format, channels, m_out_sample_rate );
	if( m_outfile ) {
		rc = 0;
	}
	return rc;
}

int IResamplerBase::readInfile() {
	int items_read = 0;
	if(m_use_float == true) {
		m_inbuffer_f = new float[m_num_in_frames];
		m_outbuffer_f = new float[m_out_buffer_size];
		items_read = m_infile->read(m_inbuffer_f, m_num_in_frames);

		float max = 0;
		float min = 0;

		for(int i = 0; i < items_read; i++) {
			max = (m_inbuffer_f[i] > max) ? m_inbuffer_f[i] : max;
			min = (m_inbuffer_f[i] < min) ? m_inbuffer_f[i] : min;
		}
		cout << "maxval:" << max << " minval:" << min << endl;
	}
	else {
		m_inbuffer_i16 = new int16_t[m_num_in_frames];
		m_outbuffer_i16 = new int16_t[m_out_buffer_size];

		// m_infile->command();

		items_read = m_infile->read(m_inbuffer_i16, m_num_in_frames);

		int max = 0;
		int min = 0;

		for(int i = 0; i < items_read; i++) {
			max = (m_inbuffer_i16[i] > max) ? m_inbuffer_i16[i] : max;
			min = (m_inbuffer_i16[i] < min) ? m_inbuffer_i16[i] : min;
		}
		cout << "maxval:" << max << " minval:" << min << endl;
	}
	if( items_read != m_num_in_frames) {
		cout << "Error reading all input samples: read: " << items_read << " but expected: " << m_num_in_frames << endl;
	}

	return items_read;
}

int IResamplerBase::writeOutfile() {
	int items_written = 0;
	if(m_use_float == true) {
    	items_written = m_outfile->write(m_outbuffer_f, m_num_out_frames);
    	delete [] m_outbuffer_f;
	}
	else {
    	items_written = m_outfile->write(m_outbuffer_i16, m_num_out_frames);
    	delete [] m_outbuffer_i16;
	}
    return items_written;
}

void IResamplerBase::closeInfile() {
	delete m_infile;
	m_infile = 0;
}

void IResamplerBase::closeOutfile() {
	delete m_outfile;
	m_outfile = 0;
}

IResamplerBase* IResamplerBase::factory(IResamplerBase::resampler_type_t res_type) {
	IResamplerBase* resamplerPtr;

	switch(res_type) {
	case E_SPEEX:
		resamplerPtr = new CSpeexResampler();
		break;
	case E_SOX:
		resamplerPtr = new CSoxResampler();
		break;
	case E_MUROAFP:
		resamplerPtr = new CMuroaFPResampler();
		break;
	default:
		resamplerPtr = 0;
	}
	return resamplerPtr;
}


int IResamplerBase::createSweep() {
    int sampleRate = 44100;

    const int size = sampleRate * 20;

    double startfreq = 100;
    double endfreq = 20000;
    double maxfactor = endfreq / startfreq;
    double stepfactor = pow(maxfactor, (double)1.0/size);
    double freq = startfreq;

    if(m_use_float) {
		if(m_outbuffer_f == 0) {
			m_outbuffer_f = new float[size];
			m_out_buffer_size = size;
		}
		for (int i=0; i<size; i++) {
			freq *= stepfactor; //1.00001;
			m_outbuffer_f[i] = sin(float(i) * 2 * M_PI * freq / sampleRate);
		}
    }
    else {
		if(m_outbuffer_i16 == 0) {
			m_outbuffer_i16 = new int16_t[size];
			m_out_buffer_size = size;
		}
		for (int i=0; i<size; i++) {
			freq *= stepfactor; //1.00001;
			m_outbuffer_i16[i] = INT16_MAX * sin(float(i) * 2 * M_PI * freq / sampleRate);
		}
    }
    m_num_out_frames = m_out_buffer_size;
    writeOutfile();
    return 0;
}

