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

#include "parsermodify.h"

#include "parsernode.h"
#include "parserway.h"
#include "parserrelation.h"
#include "../exception.h"
#include <algorithm>

using namespace std;

ParserModify::ParserModify(XML_Parser *const apXML_Parser,
                           BaseInterface& aBase,
                           Parser *const apParent,
                           const string& aName,
                           const vector< pair<string, string> >& /* aAtts */) :
    ParserConteneurElement(apXML_Parser, *apParent),
	fBase(aBase),
	fpElement(0)
{
    assert(aName == "modify");
/*    if (find(aAtts.begin(), aAtts.end(), pair<string, string>("version", "0.6")) == aAtts.end())
        throw Exception("osm Version 0.6 attendu",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__); */
}

void ParserModify::startElement(const string& aName,
                                const vector< pair<string, string> >& aAtts)
{
    if (aName == "node") {
// cerr << 'n';
        fpEnfant = new ParserNode(fpXML_Parser, *this, aName, aAtts);
//        fpEnfant = new ParserNode(fpXML_Parser, fBase, this, aName, aAtts);
    }
    else if (aName == "way") {
// cerr << 'w';
        fpEnfant = new ParserWay(fpXML_Parser, *this, aName, aAtts);
    }
//    else if (aName == "changeset") {
//        fpEnfant = new ParserChangeset(fpXML_Parser, fBase, this, aName, aAtts);
//    }
    else if (aName == "relation") {
// cerr << 'r';
        fpEnfant = new ParserRelation(fpXML_Parser, *this, aName, aAtts);
    }
    else throw Exception("Element inattendu (" + aName + ")",
                         __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserModify::endElement(const string& aName)
{
    assert(aName == "modify");
}

void ParserModify::traiter(const Node& aNode)
{
    try {
        fBase.modify(aNode);
    } catch (Exception& e) {}
}

void ParserModify::traiter(const Way& aWay)
{
    try {
        fBase.modify(aWay);
    } catch (Exception& e) {}
}

void ParserModify::traiter(const Relation& aRelation)
{
    try {
        fBase.modify(aRelation);
    } catch (Exception& e) {}
}

