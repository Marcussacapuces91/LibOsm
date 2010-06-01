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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>

using namespace std;


/**
 * Classe d'exception permettant de transmettre la localisation du
 * déclenchement de l'exception.
 */
class Exception : public exception
{
    private:
        const string fLibelle;
        const string fFichier;
        const string fFonction;
        const unsigned fLigne;

    public:
        Exception(const string& aLibelle = "",
                  const string& aFichier = "",
                  const unsigned aLigne = 0,
                  const string& aFonction = "") :
            fLibelle(aLibelle),
            fFichier(aFichier),
            fFonction(aFonction),
            fLigne(aLigne)
        {};

        virtual ~Exception() throw() {};

        virtual const char* what() const throw()
        {
            ostringstream oss;
            oss << "\"" << fLibelle << "\"" << endl;
            oss << fFichier << ":" << fLigne << ", " << fFonction;
            return oss.str().c_str();
        }
};

#endif
