/*
 * CSimpleCmds.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CSIMPLECMDS_H_
#define CSIMPLECMDS_H_

namespace muroa {

class CCmdPlay {
public:
	CCmdPlay();
	virtual ~CCmdPlay();
};



class CCmdStop {
public:
	CCmdStop();
	virtual ~CCmdStop();
};



class CCmdNext {
public:
	CCmdNext();
	virtual ~CCmdNext();
};



class CCmdPrev {
public:
	CCmdPrev();
	virtual ~CCmdPrev();
};

} /* namespace muroa */
#endif /* CSIMPLECMDS_H_ */
