/*
 Copyright (c) 2002-2014 "Martin Runge"

 IResamplerBase.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef IRESAMPLERBASE_H_
#define IRESAMPLERBASE_H_

#include <string>
#include <sndfile.hh>
#include <map>


class IResamplerBase {
public:

	typedef enum {
		E_SPEEX,
		E_SOX,
		E_MUROAFP
	} resampler_type_t;

	static IResamplerBase* factory(std::string name);
	static IResamplerBase* factory(resampler_type_t res_type);

	virtual ~IResamplerBase();

	int openInfile(std::string infile);
	int openOutfile(std::string outfile, int sampleRate);

	int readInfile();
	int writeOutfile();

	void closeInfile();
	void closeOutfile();

	virtual void resample() = 0;
	int createSweep();

protected:
	IResamplerBase();

	std::string m_infile_name;
	std::string m_outfile_name;

	int m_in_sample_rate;
	int m_in_num_channels;
	int m_num_in_frames;
	int m_num_out_frames;

	int m_out_sample_rate;
	int m_out_buffer_size;

	bool m_use_float;

	float *m_inbuffer_f, *m_outbuffer_f;
	short *m_inbuffer_i16, *m_outbuffer_i16;

	SndfileHandle *m_infile;
	SndfileHandle *m_outfile;

private:

};


#endif /* IRESAMPLERBASE_H_ */
