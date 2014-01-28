/*
 Copyright (c) 2002-2014 "Martin Runge"

 CSpeexResampler.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CSpeexResampler.h"
#include <cstdint>

using namespace std;

CSpeexResampler::CSpeexResampler() : IResamplerBase("speex") {
	int err;
	m_resampler = speex_resampler_init(1, 44100, 48000, 7, &err);
	m_use_float = true;

}

CSpeexResampler::~CSpeexResampler() {
	speex_resampler_destroy(m_resampler);
}


void CSpeexResampler::resample() {
	int err = readInfile();
	spx_uint32_t outbufsize = m_out_buffer_size;
	uint32_t inframes_used = m_num_in_frames;

	if(m_use_float == true) {
		err = speex_resampler_process_float(m_resampler,
                                            0,
                                            m_inbuffer_f,
                                            &inframes_used,
                                            m_outbuffer_f,
                                            &outbufsize);
	}
	else {
		err = speex_resampler_process_int(m_resampler,
                                          0,
                                          m_inbuffer_i16,
                                          &inframes_used,
                                          m_outbuffer_i16,
                                          &outbufsize);

	}
	if(!err) {
		m_num_out_frames = outbufsize;
	}
	err = writeOutfile();
}


