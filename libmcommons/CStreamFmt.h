/*
 * CStreamFmt.h
 *
 *  Created on: 20.12.2015
 *      Author: martin
 */

#ifndef LIBMCOMMONS_CSTREAMFMT_H_
#define LIBMCOMMONS_CSTREAMFMT_H_

namespace muroa {

class CStreamFmt {
public:
	CStreamFmt() {
		numChannels = 0;
		sampleRate = 0;
		sampleSize = 0;
	};

	CStreamFmt(int num_channels, int sample_rate, int sample_size) {
		numChannels = num_channels;
		sampleRate = sample_rate;
		sampleSize = sample_size;
	};

	bool isValid() {
		return (numChannels != 0 && sampleRate != 0 && sampleSize != 0);
	}

	bool operator==(const CStreamFmt& rhs) {
		return numChannels == rhs.numChannels && sampleRate == rhs.sampleRate && sampleSize == rhs.sampleSize;
	}

	bool operator!=(const CStreamFmt& rhs) {
		return ! operator==(rhs);
	}

	int numChannels;
	int sampleRate;
	int sampleSize;
};

} /* namespace muroa */

#endif /* LIBMCOMMONS_CSTREAMFMT_H_ */
