/*
 * CAudioIOlibao.h
 *
 *  Created on: 19 Jun 2010
 *      Author: martin
 */

#ifndef CAUDIOIOLIBAO_H_
#define CAUDIOIOLIBAO_H_

#include "IAudioIO.h"
#include <ao/ao.h>

class CAudioIOlibao : public IAudioIO
{
public:
	CAudioIOlibao();
	virtual ~CAudioIOlibao();

    virtual int close();
    virtual int open(std::string device, int samplerate = 48000, int channels = 2);
    virtual int read(char* data, int buffersize);
    virtual int write(char* data, int length);

    virtual int getOutOverflows(void);
    virtual int getBytesPlayed(void);
    virtual int getDelay();
    virtual int getWriteGranularity(void);
    virtual int getActualSampleRate(void);

private:
	ao_device *m_device;
	ao_sample_format m_format;
	int m_default_driver;

};

#endif /* CAUDIOIOLIBAO_H_ */
