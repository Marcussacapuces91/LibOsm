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

#include "../exception.h"
#include <cassert>
#include <iostream>
#include <expat.h>
#include "parsertag.h"
#include "parsernode.h"

ParserTag::ParserTag(XML_Parser *const apXML_Parser,
                     Parser& aParent,
                     const string& aName,
                     const vector< pair<string, string> >& aAtts) :
    Parser(apXML_Parser, &aParent)
{
    assert(aName == "tag");
    if (aAtts.size() != 2) throw Exception("Nombre d'attributs incorrect",
                                               __FILE__, __LINE__, __PRETTY_FUNCTION__);
    if (aAtts[0].first != "k") throw Exception("Nom du 1er attribut incorrect",
                __FILE__, __LINE__, __PRETTY_FUNCTION__);
    if (aAtts[1].first != "v") throw Exception("Nom du 2ème attribut incorrect",
                __FILE__, __LINE__, __PRETTY_FUNCTION__);
    (dynamic_cast<IsTaggableInterface *const>(fpParent))->addTag(aAtts[0].second,
            aAtts[1].second);
}

void ParserTag::startElement(const string& aName,
                             const vector< pair<string, string> >& aAtts)
{
    throw Exception("Pas de sous-element attendu (" + aName + ")",
                    __FILE__, __LINE__, __PRETTY_FUNCTION__);
    assert(aAtts.empty());
}

void ParserTag::endElement(const string& aName)
{
    assert(aName == "tag");
}
