/*
 * CDiff.cpp
 *
 *  Created on: 24 Oct 2009
 *      Author: martin
 */

#include "CDiff.h"
#include <malloc.h>

// #include <iostream>

#include <QDebug>

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


QString CDiff::diff(QString orig, QString mod)
{
	xpparam_t xpp;
	xpp.flags = 0;

	QByteArray origData(orig.toUtf8());
	QByteArray modData(mod.toUtf8());


	QByteArray delta;

	m_ecb.priv = &delta;
	m_ecb.outf = CDiff::output;

	if( xdl_init_mmfile(&m_mf1, 100 * 1024, XDL_MMF_ATOMIC ))
	{

	}

	xdl_mmfile_ptradd(&m_mf1, origData.data(), origData.size(), XDL_MMB_READONLY);

	if( xdl_init_mmfile(&m_mf2, 100 * 1024, XDL_MMF_ATOMIC ))
	{
		xdl_free_mmfile(&m_mf1);
		return QString();
	}

	xdl_mmfile_ptradd(&m_mf2, modData.data(), mod.size(), XDL_MMB_READONLY);

	if(xdl_diff(&m_mf1, &m_mf2, &xpp, &m_xecfg, &m_ecb) < 0) {

		xdl_free_mmfile(&m_mf2);
		xdl_free_mmfile(&m_mf1);
		return QString();
	}

	return QString::fromUtf8(delta.constData(), delta.size());
}

QString CDiff::patch(QString orig, QString delta)
{
	QByteArray origData(orig.toUtf8());
	QByteArray deltaData(delta.toUtf8());


	QByteArray newdata;

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

	xdl_mmfile_ptradd(&origFile, origData.data(), origData.size(), XDL_MMB_READONLY);

	if( xdl_init_mmfile(&deltaFile, 100 * 1024, XDL_MMF_ATOMIC ))
	{
		xdl_free_mmfile(&origFile);
		return QString();
	}

	xdl_mmfile_ptradd(&deltaFile, deltaData.data(), deltaData.size(), XDL_MMB_READONLY);

	int retval = xdl_patch(&origFile, &deltaFile, XDL_PATCH_NORMAL, &ecb, &rjecb);

	return QString::fromUtf8(newdata.constData(), newdata.size());
}


int CDiff::output(void *priv, mmbuffer_t *mb, int nbuf)
{
	int i;

	QByteArray* deltaPtr = reinterpret_cast<QByteArray*>(priv);

	// first, calculate additional space in delta QString
	int bytesToAdd(0);
	for (i = 0; i < nbuf; i++)
	{
		bytesToAdd += mb[i].size;
	}

	deltaPtr->reserve(deltaPtr->size() + bytesToAdd );

	for (i = 0; i < nbuf; i++)
	{
		deltaPtr->append(mb[i].ptr, mb[i].size);
		QString dbg(QString::fromUtf8(mb[i].ptr, mb[i].size));
		qDebug() << dbg;
	}
	return 0;

}

int CDiff::rejected(void *priv, mmbuffer_t *mb, int nbuf)
{
	int i;

	QByteArray* deltaPtr = reinterpret_cast<QByteArray*>(priv);

	for (i = 0; i < nbuf; i++)
	{
		deltaPtr->append(mb[i].ptr, mb[i].size );
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
