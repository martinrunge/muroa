/*
 Copyright (c) 2002-2014 "Martin Runge"

 StreamCtrlIDs.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CTRLRPCXML_STREAMCTRLIDS_H_
#define LIBMSTREAM_CTRLRPCXML_STREAMCTRLIDS_H_

#include <cstdint>

namespace muroa {
namespace StreamCtrlIDs {

const int EC_MEMBER_OF_OTHER_SESSION = 1;
const int EC_OUT_OF_SERVICE = 2;
uint32_t last_cmd_id = 0;

uint32_t getCmdID();

}  /* namespace StreamCtrlIDs */
} /* namespace muroa */

#endif /* LIBMSTREAM_CTRLRPCXML_STREAMCTRLIDS_H_ */
