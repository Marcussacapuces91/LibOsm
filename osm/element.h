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

// Class automatically generated by Dev-C++ New Class wizard

#ifndef ELEMENT_H
#define ELEMENT_H

#include "top.h"

#include <string>
#include <vector>
#include <sstream>
#include "datetimeiso8601.h"

using namespace std;

/**
 * Classe abstraite ancètre des types d'éléments (Node, Way & Relation) contenus
 * dans un Osm.
 */
class Element : public Top
{
    public:
/// Status de visibilité.
        bool fVisible;

/// Timestamp de modification dans la base.
        DateTimeISO8601 fTimestamp;

/// Version de l'Element.
		unsigned fVersion;

/// Changset contenant l'Element.
		unsigned long fChangeset;

/**
 * Constructeur protégé par défaut de l'Element.
 */
        Element() :
            Top(0, "", 0),
            fVisible(true),
            fTimestamp(),
            fVersion(0),
            fChangeset(0)
        {}


	public:
/**
 * Destructeur virtuel sans action.
 */
		virtual ~Element() {}

/**
 * Définit un des attributs de l'Element.
 * Les classes héritant de celle-ci doivent définir leur propres méthodes
 * setAttribut pour prendre en compte d'autres attributs.
 * @param aKey Nom de l'attribut.
 * @param aValue Chaîne contenant la valeur à affecter à l'attribut.
 * @throw OsmException si l'attribut n'existe pas pour cet Element.
 */
		virtual void setAttribut(const string& aKey, const string& aValue);

/**
 * Retourne l'état de visibilité de l'Element.
 * @return Un boolean true si l'Element est visible, false sinon.
 */
    	bool visible() const { return fVisible; }

/**
 * Retourne le timestamp de l'Element.
 * @return Une valeur time_t représentant le timestamp.
 */
        const DateTimeISO8601& timestamp() const { return fTimestamp; }

/**
 * Retourne le compte utilisateur qui a modifié l'Element.
 * @return Le nom du compte.
 */
	   const string& user() const { return fUser; }

/**
 * Retourne d'identifiant de l'utilisateur.
 * @return L'identifiant.
 */
        unsigned long uid() const { return fUid; }

        unsigned long changeset() const { return fChangeset; }

        unsigned version() const { return fVersion; }

/**
 * Injecte une description de l'Element au format XML de l'API Osm dans un flux
 * de sortie.
 * @param aStream Un flux de sortie.
 * @return Le flux de sortie après injection de la description de l'Element.
 */
	   ostream& afficher(ostream& aStream) const;

/**
 * Test et retourne l'égalité des différents champs de l'instance.
 * @param aElement Une référence sur l'Element comparé à l'instance.
 * @return true si les 2 Elements sont égaux, false sinon.
 */
	   bool operator==(const Element& aElement) const;

};

#endif // ELEMENT_H
