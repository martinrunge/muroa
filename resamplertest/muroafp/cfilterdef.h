#ifndef CFILTERDEF_H
#define CFILTERDEF_H

/**
holds definitions nedded for the filter

	@author Martin Runge <martin.runge@web.de>
*/

#include <stdint.h>

class CFilterDef
{
	public:
		CFilterDef ( int nmult, int scale, int nwing, const int16_t* imp, const int16_t* impd );

		~CFilterDef();

		inline int nMult() const {return m_nmult; };
		inline int scale() const {return m_scale; };
		inline int nWing() const {return m_nwing; };

	public:
		int m_nmult;
		int m_scale;
		int m_nwing;

		const int16_t* m_imp;
		const int16_t* m_imp_d;
};

#endif
