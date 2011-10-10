#include "cfilterdef.h"

CFilterDef::CFilterDef(int nmult, int scale, int nwing, const int16_t* imp, const int16_t* impd)
{
  m_nmult = nmult;
  m_scale = scale;
  m_nwing = nwing;

  m_imp = imp;
  m_imp_d = impd;
}


CFilterDef::~CFilterDef()
{
}


