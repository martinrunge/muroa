//
// Created by martin on 10.02.17.
//

#ifndef MUROA_CALSASRCDESC_H
#define MUROA_CALSASRCDESC_H


#include "IMediaSrcDesc.h"
#include "../libmcommons/CStreamFmt.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace muroa {


    class CAlsaSrcDesc : public IMediaSrcDesc {
    public:
        CAlsaSrcDesc(std::string name, std::string device, muroa::CStreamFmt stream_fmt );

        Json::Value to_json() {};

    public:
        const std::string &getDeviceName() const {
            return m_device_name;
        }

        void setDeviceName(const std::string &device_name) {
            m_device_name = device_name;
        }

        const CStreamFmt &getStreamFmt() const;
        void setStreamFmt(const CStreamFmt &m_stream_fmt);

    private:
        std::string m_device_name;

        muroa::CStreamFmt m_stream_fmt;
    };

}
#endif //MUROA_CALSASRCDESC_H
