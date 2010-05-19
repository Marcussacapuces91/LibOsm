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

#ifndef PARSERMEMBER_H
#define PARSERMEMBER_H

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

using namespace std;


/**
 * Classe implémentant un parser XML pour les Membres composant les Relations.
 */
class ParserMember : public Parser
{
	private:
/// Référence sur une base de données acceptant les insertions des différents
/// Elements.
        BaseInterface& fBase;

/// La Member renseigné par le parser.
		Member fMember;

	protected:
/**
 * Méthode virtuelle appelée en début d'élément XML inclus dans une Relation.
 * @param aName Le nom de l'élément XML.
 * @param aAtts Un tableau d'attributs XML liés à cet élément.
 */
		virtual void startElement(const string& aName,
				                  const vector< pair<string, string> >& aAtts);

/**
 * Méthode virtuelle appelée en fin d'élément XML Relation.
 * @param aName Le nom de l'élément XML Relation.
 * @pre Le nom transmis doit être "relation".
 */
        virtual void endElement(const string& aName);

	public:
/**
 * Constructeur de l'instance.
 * @param apXML_Parser Un pointeur sur un parseur XML Expat.
 * @param aBase Une référence sur une base de données recevant les données parsées.
 * @param apParent Pointeur sur l'instance ayant appelé ce constructeur.
 * @param aName Nom de l'élément XML ayant déclenché l'appel de ce constructeur.
 * @param aAtts Tableau d'attributs XML liés à l'élément aName.
 * @pre apXML_Parser est non-nul.
 * @pre aName est à "relation".
 */
		ParserMember(XML_Parser *const apXML_Parser,
                     BaseInterface& aBase,
                     Parser *const apParent,
		             const string& aName,
				     const vector< pair<string, string> >& aAtts);

/**
 * Destructeur virtuel de l'instance.
 * Sans action.
 */
		virtual ~ParserMember() {}

};

#endif
