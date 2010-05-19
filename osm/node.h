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

#ifndef NODE_H
#define NODE_H

#include "element.h" // inheriting class's header file
#include "point.h"

/**
 * \brief Classe définissant un Node tel que le décrit l'Api OSM
 * (http://wiki.openstreetmap.org/wiki/Node).
 *
 * Elle hérite aussi de Point pour permettre la manipulation de point de
 * coordonnées géographiques.
 */
class Node : public Element, public Point
{
    public:
/**
 * Constructeur de l'instance faisant aussi office de constructeur par défaut.
 * @param aLatitude Latitude du Node en degrés géographiques.
 * @param aLongitude Longitude du Node en degrés géographiques.
 */
        Node(const double& aLatitude = 0,
             const double& aLongitude = 0) :
            Point(aLatitude, aLongitude)
        {}

/**
 * Ajoute un attribut à l'instance Node.
 * L'attribut doit faire partie de la liste acceptée par les Nodes de l'Api osm.
 * @param aKey Le nom de l'attribut.
 * @param aValue La valeur de l'attribut sous la forme d'une chaîne de caractères.
 */
		virtual void setAttribut(const string& aKey, const string& aValue);

/// Nom de la classe utilisée par certaines méthodes templates de la classe ApiOsm.
		static const string NOM;

};

/**
 * Permet l'injection de la description d'une instance de Node dans un flux de
 * sortie. La présentation correspond à un flux XML.
 * @param aStream Un flux de sortie recevant la description.
 * @param aNode L'instance dont la description est produite.
 * @return Le flux de sortie après injection de la description.
 */
ostream& operator<<(ostream& aStream, const Node& aNode);

#endif // NODE_H
