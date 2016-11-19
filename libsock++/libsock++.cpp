/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdlib>
#include <signal.h>


bool global_termination_flag;
struct sigaction old_action;

void emptySigHandler(int /* signr */) {
	return;
}

__attribute__((constructor))
void installSigHandler()
{
	  struct sigaction new_action;

	  /* Set up the structure to specify the new action. */
	  new_action.sa_handler = emptySigHandler;
	  sigemptyset (&new_action.sa_mask);
	  new_action.sa_flags = 0;

	  sigaction (SIGUSR1, &new_action, &old_action);
}

__attribute__((destructor))
void removeSigHandler()
{
	  sigaction (SIGUSR1, &old_action, NULL);
}
