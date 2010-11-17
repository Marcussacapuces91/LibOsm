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

#include "basesqlite3.h"
#include <cassert>
#include "exception.h"
#include "osm/changeset.h"

BaseSQLite3::BaseSQLite3(const string& aNom,
                         const int aFlags) :
     fpSqlite3(open(aNom, aFlags))
{
     assert(fpSqlite3);
     if (string(sqlite3_version) != SQLITE_VERSION) {
          const string mess = string("Version attendue de la librairie SQLite3 : ") + SQLITE_VERSION;
          throw Exception(mess, __FILE__, __LINE__, __PRETTY_FUNCTION__);
     }
     check(sqlite3_extended_result_codes(fpSqlite3, 1),
           __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

BaseSQLite3::~BaseSQLite3(void)
{
     cout << sqlite3_total_changes(fpSqlite3) << " modifications " << endl;
/// \todo C'est quoi ce merdier ? On ne libère pas la ressource SQLite3 ?
///       Non, sinon ça fait tout planter !
#if 0
     if (sqlite3_close(fpSqlite3) != SQLITE_OK) {
/// \see http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.3
          cerr << sqlite3_errmsg(fpSqlite3) << " a " << __FILE__ << ":";
          cerr << __LINE__ << " dans " << __PRETTY_FUNCTION__ << endl;
     }
#endif
}

sqlite3* BaseSQLite3::open(const string& aNom,
                           const int aFlags)
{
     if (aNom.empty()) throw Exception("Nom du de la base vide !",
                                            __FILE__, __LINE__, __PRETTY_FUNCTION__);
     sqlite3* pSqlite3 = 0;
     const int err = sqlite3_open_v2(aNom.c_str(), &pSqlite3, aFlags, NULL);
     if (err != SQLITE_OK) throw Exception(sqlite3_errmsg(pSqlite3),
                                                __FILE__, __LINE__, __PRETTY_FUNCTION__);
     return pSqlite3;
}

void BaseSQLite3::exec(const string& aSql)
{
     if (aSql.empty()) throw Exception("Requete SQL vide !",
                                            __FILE__, __LINE__, __PRETTY_FUNCTION__);
     const int err = sqlite3_exec(fpSqlite3, aSql.c_str(), 0, 0, 0);
     if (err != SQLITE_OK)
          throw Exception(string(sqlite3_errmsg(fpSqlite3)) + " dans la requête " + aSql,
                          __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void BaseSQLite3::execFile(const string& aPath)
{
     ifstream f(aPath.c_str());
     string s;
     while (f.good()) {
          const char c = f.get();
          if (f.good()) s += c;
     }
     f.close();
     exec(s);
}
