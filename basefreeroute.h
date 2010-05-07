/*
    Copyright © 2010 par Marc Sibert

    This file is part of LIBOSM

    LIBOSM is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LIBOSM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LIBOSM.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file
 * \author Marc Sibert
 */

#ifndef BASEFREEROUTE_H
#define BASEFREEROUTE_H

#include "basesqlite3.h"
#include "exception.h"
#include "xml/baseinterface.h"

class Changeset;
class Node;
class Way;
class Relation;

/**
 *
 */
class BaseFreeroute : public BaseSQLite3, public BaseInterface
{
	private:
		Commande fInsertAmenity;

	public:
    enum type_t { TYPE_NODE, TYPE_WAY, TYPE_RELATION };

		BaseFreeroute(const string& aPath,
		              const int aFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

		~BaseFreeroute(void);

		void createAmenity(void);

		void insertAmenity(const long aId,
                       const type_t aType,
                       const string& aUser,
                       const string& aDate,
                       const string& aAmenity,
                       const long aVersion,
                       const long aChangeset,
                       const string& aName = "",
                       const string& aSource = "",
                       const string& aDescription= "",
                       const string& aNote = "",
                       const string& aOpening_hours = "",
                       const bool *const apTakeaway = 0,
                       const string& aWebsite = "",
                       const string& aOperator = "",
                       const bool *const apWifi = 0);

};

#endif
