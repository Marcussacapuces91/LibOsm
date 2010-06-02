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
/// Chaîne contenant le libellé final de l'exception.
        const string fLibelle;

/**
 * Méthode de classe construisant une chaîne à partir des différents éléments
 * passés en paramètre.
 * \param aLibelle Une chaîne indiquant le libellé de l'erreur.
 * \param aFichier Une chaîne indiquant le fichier source où s'est produit
 *        l'erreur.
 * \param aLine Un entier indiquant la ligne dans le fichier source où s'est
 *        produit l'erreur.
 * \param aFonction Une chaîne indiquant la fonction ou la méthode où s'est
 *        produit l'erreur.
 */
        static string makeLibelle(const string& aLibelle,
                                  const string& aFichier,
                                  const unsigned aLigne,
                                  const string& aFonction)
        {
            ostringstream oss;
            oss << '"' << aLibelle << '"' << endl;
            oss << aFichier << ':' << aLigne << ", " << aFonction;
            return oss.str();
        }

    public:
/**
 * \brief Constructeur par défaut de l'instance.
 *
 * Contruit le libellé final de l'exception à partir des paramètres indiqués.
 * \param aLibelle Une chaîne indiquant le libellé de l'erreur.
 * \param aFichier Une chaîne indiquant le fichier source où s'est produit
 *        l'erreur, normalement on utilise la macro __FILE__.
 * \param aLine Un entier indiquant la ligne dans le fichier source où s'est
 *        produit l'erreur, normalement on utilise la macro __LINE__.
 * \param aFonction Une chaîne indiquant la fonction ou la méthode où s'est
 *        produit l'erreur, normalement on utilise la macro __PRETTY_FUNCTION__.
 */
        Exception(const string& aLibelle = "",
                  const string& aFichier = "",
                  const unsigned aLigne = 0,
                  const string& aFonction = "") :
            fLibelle(makeLibelle(aLibelle, aFichier, aLigne, aFonction))
        {}

/**
 * \brief Destructeur virtuel de l'instance.
 *
 * Sans action.
 */
        virtual ~Exception() throw() {};

/**
 * \brief Retourne le message d'erreur lié à l'exception.
 *
 * Le message retourné est composé des différents éléments collectés à
 * l'initilisation de l'instance, en particulier (un message, le fichier source,
 * la ligne de code dans ce fichier et le méthode ou la fonction où s'est
 * produite l'erreur).
 * \return Un pointeur sur une chaîne de caractères.
 */
        virtual const char* what() const throw()
        { return fLibelle.c_str(); }
};

#endif
