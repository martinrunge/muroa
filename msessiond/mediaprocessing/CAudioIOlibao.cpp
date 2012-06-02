/*
 * CAudioIOlibao.cpp
 *
 *  Created on: 19 Jun 2010
 *      Author: martin
 */

#include "CAudioIOlibao.h"

CAudioIOlibao::CAudioIOlibao() {
	ao_initialize();

	/* -- Setup for default driver -- */
	m_default_driver = ao_default_driver_id();

	m_format.bits = 16;
	m_format.channels = 2;
	m_format.rate = 44100;
	m_format.byte_format = AO_FMT_LITTLE;
}

CAudioIOlibao::~CAudioIOlibao() {
	ao_shutdown();
}

int CAudioIOlibao::open(std::string device, int samplerate, int channels)
{
	m_format.rate = samplerate;
	m_format.channels = channels;

	/* -- Open driver -- */
	m_device = ao_open_live(m_default_driver, &m_format, NULL /* no options */);
	if (m_device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return -1;
	}
	return 0;
}

int CAudioIOlibao::close()
{
	/* -- Close and shutdown -- */
	return ao_close(m_device);
}

int CAudioIOlibao::read(char* data, int buffersize) {
	return 0;
}

int CAudioIOlibao::write(char* data, int length) {
	ao_play(m_device, data, length);
	return length;
}


int CAudioIOlibao::getOutOverflows(void)
{
	return 0;
}

int CAudioIOlibao::getBytesPlayed(void)
{
	return 0;
}

int CAudioIOlibao::getDelay()
{
	return 0;
}

int CAudioIOlibao::getWriteGranularity(void)
{
	return 4096;
}

int CAudioIOlibao::getActualSampleRate(void)
{
	return m_format.rate;
}

