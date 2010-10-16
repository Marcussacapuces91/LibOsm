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

#include "wrapper_spatialite.h" // class's header file

#include <spatialite.h>

WrapperSpatialite WrapperSpatialite::fWrapperSpatialite;

WrapperSpatialite::WrapperSpatialite(void)
{
    spatialite_init(0);
}

string WrapperSpatialite::versions(void) const
{
    return string("SQLite3 version ") + sqlite3_libversion() + "\n" +
           "Spatialite version " + spatialite_version() + "\n";
//			       "Proj4 version " + proj4_version() + "\n" +
//			       "Geos version " + geos_version() + "\n";
}

WrapperSpatialite& WrapperSpatialite::returnSpatialite(void)
{
    return fWrapperSpatialite;
}
