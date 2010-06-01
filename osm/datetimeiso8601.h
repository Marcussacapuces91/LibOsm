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

#ifndef DATETIMEISO8601_H
#define DATETIMEISO8601_H

#include <ctime>
#include <string>
#include <istream>
#include <sstream>
#include "osm_exception.h"
#include <iomanip>

using namespace std;

/**
 * Classe instanciant les méthodes nécessaires à la gestion de la date et de
 * l'heure au format ISO8601. http://www.louisg.net/ISO_8601.htm
 */

class DateTimeISO8601
{
    private:
/// Structure contenant la date et l'heure locale.
        tm fTm;

/// Entier contenant le décallage en minutes entre la locale et l'UTC.
        int fOffset;

        static tm makeTm(const string& aDate)
        {
            tm t;
            istringstream iss(aDate);
            char c;
            iss >> t.tm_year >> c >> t.tm_mon >> c >> t.tm_mday >> c;
            iss >> t.tm_hour >> c >> t.tm_min >> c >> t.tm_sec;
            t.tm_year -= 1900;   // depuis 1900
            t.tm_mon -= 1;       // 0 à 11
            t.tm_wday = -1;
            t.tm_yday = -1;
            t.tm_isdst = -1;
            return t;
        }

        static int makeOffset(const string& aDate)
        {
            tm t;
            istringstream iss(aDate);
            char c;
            iss >> t.tm_year >> c >> t.tm_mon >> c >> t.tm_mday >> c;
            iss >> t.tm_hour >> c >> t.tm_min >> c >> t.tm_sec;
            char s;             // Signe offset
            unsigned hh, mm = 0;    // heures et minutes de l'offset
            iss >> s;
            switch (s) {
                case 'Z' :
                    return 0;
                    break;
                case '+' :
                    iss >> hh >> c >> mm;
                    return 60 * hh + mm;
                    break;
                case '-' :
                    iss >> hh >> c >> mm;
                    return -60 * hh - mm;
                    break;
                default :
                    throw OsmException("Separateur de l'offset incorrect (" + aDate + ")",
                                       __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
        }

    public:
/**
 * Constructeur par défaut de l'instance.
 */
        DateTimeISO8601() :
            fTm(),
            fOffset(0)
        {
            fTm.tm_year = 0;
            fTm.tm_mon = 0;
            fTm.tm_mday = 0;
            fTm.tm_hour = 0;
            fTm.tm_min = 0;
            fTm.tm_sec = 0;
            fTm.tm_wday = -1;
            fTm.tm_yday = -1;
            fTm.tm_isdst = -1;
        }

/**
* Constructeur de l'instance.
* \param aTm Une référence sur une structure contenant l'heure locale.
* \param aOffset Le décallage entre l'heure locale et l'UTC.
*/
        DateTimeISO8601(const tm& aTm,
                        const int& aOffset) :
            fTm(aTm),
            fOffset(aOffset)
        {}

/**
* Constructeur de l'instance à partir d'une chaîne au format ISO 8601.
* \param aDate Une Chaîne contenant l'heure au format ISO 8601.
*/
        DateTimeISO8601(const string& aDate) :
            fTm(makeTm(aDate)),
            fOffset(makeOffset(aDate))
        {}

/**
 * Constructeur de copie.
 */
        DateTimeISO8601(const DateTimeISO8601& aDT) :
            fTm(aDT.fTm),
            fOffset(aDT.fOffset)
        {}

        DateTimeISO8601& operator=(const DateTimeISO8601& aDT)
        {
            fTm = aDT.fTm;
            fOffset = aDT.fOffset;
            return *this;
        }

/**
* Retourne la valeur de l'instance au format ISO 8601.
* \return Une chaîne au format ISO 8601.
*/
        string iso8601() const
        {
            if (fTm.tm_year || fTm.tm_mon || fTm.tm_mday ||
                fTm.tm_hour || fTm.tm_min || fTm.tm_sec) {
                ostringstream result;
                result << setfill('0');
                result << setw(4) << (fTm.tm_year + 1900) << setw(0) << '-';
                result << setw(2) << (fTm.tm_mon + 1) << setw(0) << '-';
                result << setw(2) << fTm.tm_mday << setw(0) << 'T';
                result << setw(2) << fTm.tm_hour << setw(0) << ':';
                result << setw(2) << fTm.tm_min << setw(0) << ':';
                result << setw(2) << fTm.tm_sec << setw(0);

                if (fOffset) {  // <> 0
                    const int sgn = (fOffset < 0 ? -1 : 1);
                    result << (sgn < 0 ? '-' : '+');
                    result << setw(2) << (fOffset * sgn) / 60 << setw(0) << ':';
                    result << setw(2) << (fOffset * sgn) % 60 << setw(0);
                } else {
                    result << 'Z';  // Temps UTC
                }
                return result.str();
            } else {
                return "";
            }
        }

/**
* Retourne l'heure locale dans une structure tm.
* \return Une référence sur une structure tm.
*/
        const tm& getTm() const { return fTm; }

/**
* Retourne le décallage entre l'heure locale et l'UTC.
* \return Un entier indiquant le nombre de minutes du décallage.
*/
        int getOffset() const { return fOffset; }

/**
 * Operateur d'égalité. Indique l'égalité de l'instance et de la référence.
 */
        bool operator==(const DateTimeISO8601& aDT) const
        {
            tm tm1 = fTm;
            tm tm2 = aDT.fTm;
            return (mktime(&tm1) - 60 * fOffset == mktime(&tm2) - 60 * aDT.fOffset);
        }

/**
 * Opérateur de différence. Indique la non-égalité entre l'instance et la référence.
 */
        bool operator!=(const DateTimeISO8601& aDT) const { return !(*this == aDT); }

};

inline istream& operator>>(istream& aStream, DateTimeISO8601& aDT)
{
    string s;
    aStream >> s;
    aDT = DateTimeISO8601(s);
    return aStream;
}

inline ostream& operator<<(DateTimeISO8601& aDT, ostream& aStream)
{
    return (aStream << aDT.iso8601());
}


#endif // DATETIMEISO8601_H


