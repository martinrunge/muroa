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

using namespace std;

IResamplerBase::IResamplerBase(std::string name) : m_name(name) {
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
    int scale = m_infile->command(SFC_SET_SCALE_FLOAT_INT_READ, NULL, SF_FALSE);
    cout << "SFC_SET_SCALE_FLOAT_INT_READ: " << scale << endl;
}

int IResamplerBase::openOutfile(std::string outfile, int sampleRate) {
	int rc = -1;
	if(m_outfile != 0) {
		delete m_outfile;
	}
    const int format=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
//  const int format=SF_FORMAT_WAV | SF_FORMAT_FLOAT;
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
