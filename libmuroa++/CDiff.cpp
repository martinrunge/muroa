/*
 * CDiff.cpp
 *
 *  Created on: 24 Oct 2009
 *      Author: martin
 */

#include "CDiff.h"
#include <malloc.h>

#include <iostream>

using namespace std;


CDiff::CDiff() {

	m_malt.priv = NULL;
	m_malt.malloc = CDiff::wrap_malloc;
	m_malt.free = CDiff::wrap_free;
	m_malt.realloc = CDiff::wrap_realloc;
	xdl_set_allocator(&m_malt);

	m_xecfg.ctxlen = 0;
}

CDiff::~CDiff() {

}


string CDiff::diff(std::string orig, std::string mod)
{
	xpparam_t xpp;
	xpp.flags = 0;

	string delta;

	m_ecb.priv = &delta;
	m_ecb.outf = CDiff::output;

	if( xdl_init_mmfile(&m_mf1, 100 * 1024, XDL_MMF_ATOMIC ))
	{

	}

	xdl_mmfile_ptradd(&m_mf1, const_cast<char*>(orig.data()), orig.size(), XDL_MMB_READONLY);

	if( xdl_init_mmfile(&m_mf2, 100 * 1024, XDL_MMF_ATOMIC ))
	{
		xdl_free_mmfile(&m_mf1);
		return string();
	}

	xdl_mmfile_ptradd(&m_mf2, const_cast<char*>(mod.data()), mod.size(), XDL_MMB_READONLY);

	if(xdl_diff(&m_mf1, &m_mf2, &xpp, &m_xecfg, &m_ecb) < 0) {

		xdl_free_mmfile(&m_mf2);
		xdl_free_mmfile(&m_mf1);
		return string();
	}

	return delta;
}

string CDiff::patch(std::string orig, std::string delta)
{

	string newdata;

	xdemitcb_t ecb;
	ecb.priv = &newdata;
	ecb.outf = CDiff::output;

	xdemitcb_t rjecb;
	rjecb.priv = 0;
	rjecb.outf = CDiff::rejected;

	mmfile_t origFile, deltaFile;


	if( xdl_init_mmfile(&origFile, 100 * 1024, XDL_MMF_ATOMIC ))
	{

	}

	xdl_mmfile_ptradd(&origFile, const_cast<char*>(orig.data()), orig.size(), XDL_MMB_READONLY);

	if( xdl_init_mmfile(&deltaFile, 100 * 1024, XDL_MMF_ATOMIC ))
	{
		xdl_free_mmfile(&origFile);
		return string();
	}

	xdl_mmfile_ptradd(&deltaFile, const_cast<char*>(delta.data()), delta.size(), XDL_MMB_READONLY);


	int retval = xdl_patch(&origFile, &deltaFile, XDL_PATCH_NORMAL, &ecb, &rjecb);

	return newdata;
}


int CDiff::output(void *priv, mmbuffer_t *mb, int nbuf)
{
	int i;

	string* deltaPtr = reinterpret_cast<string*>(priv);

	for (i = 0; i < nbuf; i++)
	{
		deltaPtr->append(mb[i].ptr, mb[i].size );
		// string dbg(mb[i].ptr, mb[i].size);
		// cout << "Diff Block: " <<  dbg << endl;
	}
	return 0;

}

int CDiff::rejected(void *priv, mmbuffer_t *mb, int nbuf)
{
	int i;

	string* deltaPtr = reinterpret_cast<string*>(priv);

	for (i = 0; i < nbuf; i++)
	{
		//deltaPtr->append(mb[i].ptr, mb[i].size );
	}
	return 0;

}



void* CDiff::wrap_malloc(void *priv, unsigned int size) {

	return malloc(size);
}


void CDiff::wrap_free(void *priv, void *ptr) {

	free(ptr);
}


void* CDiff::wrap_realloc(void *priv, void *ptr, unsigned int size) {

	return realloc(ptr, size);
}

//
//int CDiff::xdlt_outf(void *priv, mmbuffer_t *mb, int nbuf) {
//	int i;
//
//	for (i = 0; i < nbuf; i++)
//		if (!fwrite(mb[i].ptr, mb[i].size, 1, (FILE *) priv))
//			return -1;
//
//	return 0;
//}
