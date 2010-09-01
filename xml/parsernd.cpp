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
#include "parsernd.h"
#include "parsernode.h"
#include "parserway.h"

ParserNd::ParserNd(XML_Parser *const apXML_Parser,
				   Parser& aParent,
				   const string& /* aName */,
				   const vector< pair<string, string> >& aAtts) :
	Parser(apXML_Parser, &aParent)
{
	if (aAtts.size() != 1)
        throw Exception("Nombre d'attributs incorrect",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
	if (aAtts[0].first != "ref")
        throw Exception("Nom de l'attribut incorrect",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
    istringstream iss(aAtts[0].second);
    unsigned long val;
    iss >> val;
	dynamic_cast<ParserWay *const>(fpParent)->addNode(val);
}

void ParserNd::startElement(const string& aName,
                            const vector< pair<string, string> >& /* aAtts */)
{
	throw Exception("Pas de sous-element attendu (" + aName + ")",
                    __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserNd::endElement(const string& aName)
{
	assert(aName == "nd");
}
