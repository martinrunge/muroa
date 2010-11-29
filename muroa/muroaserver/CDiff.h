/*
 * CDiff.h
 *
 *  Created on: 24 Oct 2009
 *      Author: martin
 */

#ifndef CDIFF_H_
#define CDIFF_H_

#include <xdiff.h>
#include <QString>
#include <QByteArray>


class CDiff {
public:
	CDiff();
	virtual ~CDiff();

	QString diff(QString orig, QString mod);
	QString patch(QString orig, QString delta);

private:
	memallocator_t m_malt;
	mmfile_t m_mf1, m_mf2;
	xdemitconf_t m_xecfg;
	xdemitcb_t m_ecb;

	QString m_delta;

	static int output(void *priv, mmbuffer_t *mb, int nbuf);
	static int rejected(void *priv, mmbuffer_t *mb, int nbuf);

	static void* wrap_malloc(void *priv, unsigned int size);
	static void wrap_free(void *priv, void *ptr);
	static void* wrap_realloc(void *priv, void *ptr, unsigned int size);

};

#endif /* CDIFF_H_ */
