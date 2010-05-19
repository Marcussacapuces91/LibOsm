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

#include "spatialite.h"

#include "libspatialite/headers/spatialite.h"      // pour être plus explicite

Spatialite::Spatialite(void)
{
	spatialite_init(0);
}

Spatialite::Spatialite* Spatialite::fpSpatialite = 0;

string Spatialite::versions(void) const
{
	return string("SQLite3 version ") + sqlite3_libversion() + "\n" +
	       "Spatialite version " + spatialite_version() + "\n";
//			       "Proj4 version " + proj4_version() + "\n" +
//			       "Geos version " + geos_version() + "\n";
}

Spatialite& Spatialite::makeSpatialite(void)
{
	if (fpSpatialite) return *fpSpatialite;
	fpSpatialite = new Spatialite;
	return *fpSpatialite;
}
