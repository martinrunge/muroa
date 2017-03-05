//
// Created by martin on 10.02.17.
//

#include "CAlsaSrcDesc.h"
#include "../libmcommons/CStreamFmt.h"

namespace muroa {

    CAlsaSrcDesc::CAlsaSrcDesc(std::string name, std::string device, muroa::CStreamFmt stream_fmt) {
        setSrcName(name);
        setDeviceName(device);
        setStreamFmt(stream_fmt);
    }

    const CStreamFmt &CAlsaSrcDesc::getStreamFmt() const {
        return m_stream_fmt;
    }

    void CAlsaSrcDesc::setStreamFmt(const CStreamFmt &stream_fmt) {
        m_stream_fmt = stream_fmt;
    };

}