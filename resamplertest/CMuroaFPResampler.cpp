/*
 Copyright (c) 2002-2014 "Martin Runge"

 CMuroaFPResampler.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CMuroaFPResampler.h"
#include "muroafp/cfixpointresampler.h"

using namespace std;

string CMuroaFPResampler::name() { return string("muroafp"); };

CMuroaFPResampler::CMuroaFPResampler() : IResamplerBase() {
	m_use_float = false;
	m_blocksize = 8196;
}

CMuroaFPResampler::~CMuroaFPResampler() {
}

void CMuroaFPResampler::resample() {
	int err = readInfile();
	m_muroafp_resampler = new CFixPointResampler(m_outbuffer_i16, m_out_buffer_size, best, 1);
	uint32_t inframes_left = m_num_in_frames;
	int inframes;
	int16_t* ptr = m_inbuffer_i16 - m_blocksize;
	int16_t* outbuffer = m_outbuffer_i16;
	m_num_out_frames = 0;

	do  {
		ptr += m_blocksize;
		inframes = (inframes_left >= m_blocksize) ? m_blocksize : inframes_left;
		int num_out_frames = m_muroafp_resampler->resampleFrame( ptr, inframes, outbuffer, (double)48000.0/44100.0);

		inframes_left -= inframes;
		outbuffer += num_out_frames;
		m_num_out_frames += num_out_frames;
	} while(inframes_left > 0);

	err = writeOutfile();
	delete m_muroafp_resampler;

}
