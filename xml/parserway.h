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

#ifndef PARSERWAY_H
#define PARSERWAY_H

#include <fstream>
#include <expat.h>
#include <expat_external.h>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include "parser.h"
#include "istaggableinterface.h"
#include "../osm/way.h"
#include "baseinterface.h"
#include "parserconteneurelement.h"

using namespace std;


class ParserTag;

/**
 * Classe permettant de parser un fluw XML contenant la description d'un Way,
 * de ses Tags et de la liste des Nodes le composant.
 * Cette classe est instancié par une structure XML de niveau supérieure pendant
 * sa propre analyse.
 */
class ParserWay : public Parser, public IsTaggableInterface
{
	private:
/// Instance du Way dont les attributs seront définis pendant l'analyse.
		Way fWay;

	protected:
/**
 * Appelée par le parseur XML, pour chaque sous-élément XML de Way,
 * cette méthode lance l'appel au parser correspondant.
 * \param aName Une référence sur le libellé du tag XML.
 * \param aAtts Liste d'attributs de l'élément.
 */
		virtual void startElement(const string& aName,
				                  const vector< pair<string, string> >& aAtts);

/**
 * Appelée lors de la cloture de l'élément XML courant (way), cette méthode
 * transmet fWay au parser de niveau supérieur.
 * \param aName Une référence sur le libellé de l'élément XML courant.
 * \pre aName == 'way'.
 */
        virtual void endElement(const string& aName);

	public:
/**
 * Constructeur de l'instance.
 * \param apXML_Parser Un pointeur sur le parseur XML.
 * \param aParent Une référence sur l'instance de niveau supérieure.
 * \param aName Une référence sur le libellé de l'élément XML courant.
 * \param aAtts Liste des attributs de l'élément courant.
 * \pre aName == 'way'.
 */
		ParserWay(XML_Parser *const apXML_Parser,
		          ParserConteneurElement& aParent,
		          const string& aName,
				  const vector< pair<string, string> >& aAtts);

/**
 * Destructeur virtuel de l'instance, sans action.
 */
		virtual ~ParserWay() {};

/**
 * \brief Ajoute un Tag à l'instance de Way (fWay).
 *
 * Méthode héritée de l'interface IsTaggableInterface.
 * \param aKey Une référence sur la clé de l'attribut.
 * \param aValue Une référence sur la valeur de l'attribut.
 */
		virtual void addTag(const string& aKey,
		                    const string& aValue) { fWay.setTag(aKey, aValue); }

/**
 * Ajoute un Node à l'instance de Way (fWay).
 * \param aNode L'identifiant entier représentant le Node ajouté dans le Way.
 */
        void addNode(const unsigned long& aNode) { fWay.addNode(aNode); }

};

#endif
