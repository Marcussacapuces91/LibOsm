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

#ifndef PARSERRELATION_H
#define PARSERRELATION_H

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
#include "../osm/relation.h"
#include "baseinterface.h"
#include "parserconteneurelement.h"

using namespace std;

/**
 * Déclaration simple.
 */
class ParserTag;

/**
 * Classe permettant de parser un fluw XML contenant la description d'une
 * Relation, de ses Tags et de ses membres.
 * Cette classe est instancié par une structure XML de niveau supérieure pendant
 * sa propre analyse.
 */
class ParserRelation : public Parser, public IsTaggableInterface
{
	private:
/// Instance de la Relation dont les attributs seront définis pendant l'analyse.
		Relation fRelation;

	protected:
/**
 * Appelée par le parseur XML, pour chaque sous-élément XML de Relation,
 * cette méthode lance l'appel au parser correspondant.
 * \param aName Une référence sur le libellé du tag XML.
 * \param aAtts Liste d'attributs de l'élément.
 */
		virtual void startElement(const string& aName,
				                  const vector< pair<string, string> >& aAtts);

/**
 * Appelée lors de la cloture de l'élément XML courant (relation), cette méthode
 * transmet fRelation au parser de niveau supérieur.
 * \param aName Une référence sur le libellé de l'élément XML courant.
 * \pre aName == 'relation'.
 */
        virtual void endElement(const string& aName);

	public:
/**
 * Constructeur de l'instance.
 * \param apXML_Parser Un pointeur sur le parseur XML.
 * \param aParent Une référence sur l'instance de niveau supérieure.
 * \param aName Une référence sur le libellé de l'élément XML courant.
 * \param aAtts Liste des attributs de l'élément courant.
 * \pre aName == 'relation'.
 */
		ParserRelation(XML_Parser *const apXML_Parser,
                       ParserConteneurElement& aParent,
		               const string& aName,
				       const vector< pair<string, string> >& aAtts);

/**
 * Destructeur virtuel de l'instance, sans action.
 */
		virtual ~ParserRelation() {};

/**
 * \brief Ajoute un Tag à l'instance de Relation (fRelation).
 *
 * Méthode héritée de l'interface IsTaggableInterface.
 * \param aKey Une référence sur la clé de l'attribut.
 * \param aValue Une référence sur la valeur de l'attribut.
 */
		virtual void addTag(const string& aKey,
		                    const string& aValue) {
            fRelation.setTag(aKey, aValue);
        }

/**
 * Ajoute un Member à l'instance de Relation (fRelation).
 * \param aMember Une référence sur un Member.
 */
        void addMember(const Member& aMember) {
            fRelation.addMember(aMember);
        }

};

#endif
