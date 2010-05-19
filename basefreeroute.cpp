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

#include "basefreeroute.h"

#include <iostream>
using namespace std;

BaseFreeroute::BaseFreeroute(const string& aPath,
                             const int aFlags) :
	BaseSQLite3(aPath, aFlags)
{
// spatial_ref_sys table en important le script init_spatialite.sql
	execFile("init_spatialite-2.3.sql");
	createAmenity();
	fInsertAmenity.setStatment(fpSqlite3,
                               "INSERT INTO amenity \
                                       (id, type, user, date, amenity, version, changeset, objet, name, source, description, note, opening_hours, takeaway, website, operator, wifi) \
                                     VALUES \
                                       (?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14,?15,?16,?17)");
}

BaseFreeroute::~BaseFreeroute()
{
}

void BaseFreeroute::createAmenity(void)
{
  exec("BEGIN");
	exec("CREATE TABLE amenity (                                     \n\
					id INTEGER(10) PRIMARY KEY NOT NULL,		                 \n\
					type VARCHAR(8) NOT NULL,										  					 -- 'node','way','relation' \n\
					user VARCHAR(255) NOT NULL,									             \n\
					date TEXT NOT NULL,			  															 -- ISO8601 strings ('YYYY-MM-DD HH:MM:SS.SSS') \n\
					amenity VARCHAR(15) NOT NULL,  													 -- 'bar','biergarten','cafe','fast_food','pub','restaurant' \n\
					version INTEGER(10) NOT NULL,						    						 \n\
					changeset INTEGER(10) NOT NULL,                          \n\
					timestamp TEXT NOT NULL DEFAULT (datetime('now')),       \n\
					name VARCHAR(255) default NULL,													 \n\
					source VARCHAR(255) default NULL,												 \n\
					description TEXT default NULL,													 \n\
					note TEXT default NULL,																	 \n\
					opening_hours TEXT default NULL, 												 -- Suivant http://wiki.openstreetmap.org/wiki/Key:opening_hours  \n\
					takeaway INTEGER(1) default NULL,									 			 \n\
					website TEXT default NULL,															 \n\
					operator INTEGER(10) default NULL,											 -- ID de l'operator			 \n\
					wifi INTEGER(1) default NULL)");
	exec("SELECT AddGeometryColumn('amenity', 'objet', 4326, 'GEOMETRY', 2, 0)");
	exec("SELECT CreateSpatialIndex('amenity','objet')");
  exec("COMMIT");
}

void BaseFreeroute::insertAmenity(const long aId,
                                  const type_t aType,
                                  const string& aUser,
                                  const string& aDate,
                                  const string& aAmenity,
                                  const long aVersion,
  		                          const long aChangeset,
	  	                          const string& aName,
		                          const string& aSource,
		                          const string& aDescription,
		                          const string& aNote,
		                          const string& aOpening_hours,
		                          const bool* apTakeaway,
		                          const string& aWebsite,
		                          const string& aOperator,
		                          const bool* apWifi)
{
//	check(sqlite3_reset(fPreparedInsertAmenity), __FILE__, __LINE__, __PRETTY_FUNCTION__);
	check(sqlite3_bind_int64(*fInsertAmenity, 1, sqlite3_int64(aId)), __FILE__, __LINE__, __PRETTY_FUNCTION__);
	string type;
  switch (aType) {
    case TYPE_NODE :
          type = "node";
          break;
    case TYPE_WAY :
          type = "way";
          break;
    case TYPE_RELATION :
          type = "relation";
          break;
  }
  check(sqlite3_bind_text(*fInsertAmenity, 2, type.c_str(), type.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  check(sqlite3_bind_text(*fInsertAmenity, 3, aUser.c_str(), aUser.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  check(sqlite3_bind_text(*fInsertAmenity, 4, aDate.c_str(), aDate.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  check(sqlite3_bind_text(*fInsertAmenity, 5, aAmenity.c_str(), aAmenity.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  check(sqlite3_bind_int64(*fInsertAmenity, 6, sqlite3_int64(aVersion)), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  check(sqlite3_bind_int64(*fInsertAmenity, 7, sqlite3_int64(aChangeset)), __FILE__, __LINE__, __PRETTY_FUNCTION__);
// 8 objet
  if (aName.empty()) check(sqlite3_bind_null(*fInsertAmenity, 9));
  else check(sqlite3_bind_text(*fInsertAmenity, 9, aName.c_str(), aName.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  if (aSource.empty()) check(sqlite3_bind_null(*fInsertAmenity, 10));
  else check(sqlite3_bind_text(*fInsertAmenity, 10, aSource.c_str(), aSource.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  if (aDescription.empty()) check(sqlite3_bind_null(*fInsertAmenity, 11));
  else check(sqlite3_bind_text(*fInsertAmenity, 11, aDescription.c_str(), aDescription.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  if (aNote.empty()) check(sqlite3_bind_null(*fInsertAmenity, 12));
  else check(sqlite3_bind_text(*fInsertAmenity, 12, aNote.c_str(), aNote.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  if (aOpening_hours.empty()) check(sqlite3_bind_null(*fInsertAmenity, 13));
  else check(sqlite3_bind_text(*fInsertAmenity, 13, aOpening_hours.c_str(), aOpening_hours.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  if (!apTakeaway) check(sqlite3_bind_null(*fInsertAmenity, 14));
  else check(sqlite3_bind_int(*fInsertAmenity, 14, (*apTakeaway ? 1 : 0)), __FILE__, __LINE__, __PRETTY_FUNCTION__);
  if (aWebsite.empty()) check(sqlite3_bind_null(*fInsertAmenity, 15));
  else check(sqlite3_bind_text(*fInsertAmenity, 15, aWebsite.c_str(), aWebsite.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
/*
  if (aOperator.empty()) check(sqlite3_bind_null(fPreparedInsertAmenity, 16));
  else check(sqlite3_bind_text(fPreparedInsertAmenity, 16, aOperator.c_str(), aOperator.size(), SQLITE_STATIC), __FILE__, __LINE__, __PRETTY_FUNCTION__);
*/
  if (!apWifi) check(sqlite3_bind_null(*fInsertAmenity, 17));
  else check(sqlite3_bind_int(*fInsertAmenity, 17, (*apWifi ? 1 : 0)), __FILE__, __LINE__, __PRETTY_FUNCTION__);
	const int e = sqlite3_step(*fInsertAmenity);
  check(sqlite3_reset(*fInsertAmenity), __FILE__, __LINE__, __PRETTY_FUNCTION__);
	if (e != SQLITE_DONE) {
		check(e, __FILE__, __LINE__, __PRETTY_FUNCTION__);
	}
}

