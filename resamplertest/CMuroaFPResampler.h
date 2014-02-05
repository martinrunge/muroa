/*
 Copyright (c) 2002-2014 "Martin Runge"

 CMuroaFPResampler.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef CMUROAFPRESAMPLER_H_
#define CMUROAFPRESAMPLER_H_

#include "IResamplerBase.h"

class CMuroaFPResampler: public IResamplerBase {
public:
	CMuroaFPResampler();
	virtual ~CMuroaFPResampler();

	static std::string name();

	void resample();

};

#endif /* CMUROAFPRESAMPLER_H_ */
