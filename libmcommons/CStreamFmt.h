/*
 * CStreamFmt.h
 *
 *  Created on: 20.12.2015
 *      Author: martin
 */

#ifndef LIBMCOMMONS_CSTREAMFMT_H_
#define LIBMCOMMONS_CSTREAMFMT_H_

extern "C" {
#include <libavutil/samplefmt.h>
}

namespace muroa {

    class CStreamFmt {
    public:
        CStreamFmt() {
            m_numChannels = 0;
            m_sampleRate = 0;
            m_sampleFormat = AV_SAMPLE_FMT_NONE;
        };

        CStreamFmt(int sample_rate, int num_channels, enum AVSampleFormat sf) {
            m_sampleRate = sample_rate;
            m_numChannels = num_channels;
            m_sampleFormat = sf;
        };

        int getNumChannels() const {
            return m_numChannels;
        }

        void setNumChannels(int numChannels) {
            m_numChannels = numChannels;
        }

        int getSampleRate() const {
            return m_sampleRate;
        }

        void setSampleRate(int sampleRate) {
            m_sampleRate = sampleRate;
        }

        int getSampleSize() const {
            switch (m_sampleFormat) {

                case AV_SAMPLE_FMT_U8:
                case AV_SAMPLE_FMT_U8P:
                    return 1;

                case AV_SAMPLE_FMT_S16:
                case AV_SAMPLE_FMT_S16P:
                    return 2;

                case AV_SAMPLE_FMT_S32:
                case AV_SAMPLE_FMT_S32P:
                    return 4;

                case AV_SAMPLE_FMT_FLT:
                case AV_SAMPLE_FMT_FLTP:
                    return sizeof(float);

                case AV_SAMPLE_FMT_DBL:
                case AV_SAMPLE_FMT_DBLP:
                    return sizeof(double);

                default:
                    return 0;
            }
        }

        enum AVSampleFormat getSampleFormat() const {
            return m_sampleFormat;
        }

        void setSampleFormat(AVSampleFormat sampleFormat) {
            m_sampleFormat = sampleFormat;
        }

        void setSampleFormat(std::string sampleFormat) {
            setSampleFormat(str2sampleFormat(sampleFormat));
        }

        static enum AVSampleFormat str2sampleFormat(std::string sampleFormat) {
            if (sampleFormat.compare("U8") == 0) {
                return AV_SAMPLE_FMT_U8;
            } else if (sampleFormat.compare("U8P") == 0) {
                return AV_SAMPLE_FMT_U8P;
            } else if (sampleFormat.compare("S16") == 0) {
                return AV_SAMPLE_FMT_S16;
            } else if (sampleFormat.compare("S16P") == 0) {
                return AV_SAMPLE_FMT_S16P;
            } else if (sampleFormat.compare("S32") == 0) {
                return AV_SAMPLE_FMT_S32;
            } else if (sampleFormat.compare("S32P") == 0) {
                return AV_SAMPLE_FMT_S32P;
            } else if (sampleFormat.compare("FLOAT") == 0) {
                return AV_SAMPLE_FMT_FLT;
            } else if (sampleFormat.compare("FLOATP") == 0) {
                return AV_SAMPLE_FMT_FLTP;
            } else if (sampleFormat.compare("DOUBLE") == 0) {
                return AV_SAMPLE_FMT_DBL;
            } else if (sampleFormat.compare("DOUBLEP") == 0) {
                return AV_SAMPLE_FMT_DBLP;
            } else {
                return AV_SAMPLE_FMT_NONE;
            }
        }

        static std::string sampleFormat2str(enum AVSampleFormat sampleFormat) {
            switch(sampleFormat) {
                case AV_SAMPLE_FMT_U8:
                    return "U8";
                case AV_SAMPLE_FMT_U8P:
                    return "U8P";
                case AV_SAMPLE_FMT_S16:
                    return "S16";
                case AV_SAMPLE_FMT_S16P:
                    return "S16P";
                case AV_SAMPLE_FMT_S32:
                    return "S32";
                case AV_SAMPLE_FMT_S32P:
                    return "S32P";
                case AV_SAMPLE_FMT_FLT:
                    return "FLOAT";
                case AV_SAMPLE_FMT_FLTP:
                    return "FLOATP";
                case AV_SAMPLE_FMT_DBL:
                    return "DOUBLE";
                case AV_SAMPLE_FMT_DBLP:
                    return "DOUBLEP";
                default:
                    "NONE";
            }
        }

        bool isValid() {
            return (m_numChannels != 0 && m_sampleRate != 0 && m_sampleFormat != AV_SAMPLE_FMT_NONE);
        }

        bool operator==(const CStreamFmt &rhs) {
            return m_numChannels == rhs.m_numChannels && m_sampleRate == rhs.m_sampleRate && m_sampleFormat == rhs.m_sampleFormat;
        }

        bool operator!=(const CStreamFmt &rhs) {
            return !operator==(rhs);
        }

    private:
        int m_numChannels;
        int m_sampleRate;
        int m_sampleSize;
        enum AVSampleFormat m_sampleFormat;

    };

} /* namespace muroa */

#endif /* LIBMCOMMONS_CSTREAMFMT_H_ */
