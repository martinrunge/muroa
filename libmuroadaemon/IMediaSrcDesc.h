//
// Created by martin on 10.02.17.
//

#ifndef MUROA_IMEDIASRCDESC_H
#define MUROA_IMEDIASRCDESC_H

#include <string>
#include <jsoncpp/json/json.h>

namespace muroa {

    class IMediaSrcDesc {
    protected:
        IMediaSrcDesc() {};
        virtual Json::Value to_json() = 0;

    public:
        const std::string &getSrcName() const {
            return m_name;
        }

        void setSrcName(const std::string &name) {
            m_name = name;
        }

        const std::string &getType() const {
            return m_type;
        }

        void setType(const std::string &type) {
            m_type = type;
        }

    private:
        std::string m_name;
        std::string m_type;
    };

}

#endif //MUROA_IMEDIASRCDESC_H
