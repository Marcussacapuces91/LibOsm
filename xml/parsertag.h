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

#ifndef PARSERTAG_H
#define PARSERTAG_H

#include <expat.h>
#include <expat_external.h>
#include <vector>
#include "parser.h"
using namespace std;

class ParserTag : public Parser
{
protected:
    /**
     * Appelée par le parseur XML, pour chaque sous-élément XML de Tag,
     * cette méthode ne devrait jamais être appelée et lance systématiquement une exception.
     * \param aName Une référence sur le libellé du tag XML.
     * \param aAtts Liste d'attributs de l'élément.
     */
    virtual void startElement(const string& aName,
                              const vector< pair<string, string> >& aAtts);

    /**
     * Appelée lors de la cloture de l'élément XML courant (tag), cette méthode
     * est sans action.
     * \param aName Une référence sur le libellé de l'élément XML courant.
     * \pre aName == 'tag'.
     */
    virtual void endElement(const string& aName);

public:
    /**
     * \brief Constructeur de l'instance.
     *
     * Lance l'analyse d'un Tag et transmet le Tag au parser de niveau supérieur.
     * \param apXML_Parser Un pointeur sur le parseur XML.
     * \param aParent Une référence sur l'instance de niveau supérieure.
     * \param aName Une référence sur le libellé de l'élément XML courant.
     * \param aAtts Liste des attributs de l'élément courant.
     * \pre aName == 'tag'.
     */
    ParserTag(XML_Parser *const apXML_Parser,
              Parser& aParent,
              const string& aName,
              const vector< pair<string, string> >& aAtts);

    /**
     * Destructeur virtuel de l'instance sans action.
     */
    virtual ~ParserTag() {};

};

#endif
