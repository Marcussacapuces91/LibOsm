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

#include "parserosm.h"
#include "../exception.h"
#include <cassert>
#include <iostream>
#include <expat.h>
#include "parserbounds.h"
#include "parserchangeset.h"
#include "parsernode.h"
#include "parserway.h"
#include "parserrelation.h"
#include <algorithm>

using namespace std;

ParserOsm::ParserOsm(XML_Parser *const apXML_Parser,
                     BaseInterface& aBase,
                     Parser *const apParent,
                     const string& aName,
                     const vector< pair<string, string> >& aAtts) :
//    Parser(apXML_Parser, apParent),
    ParserConteneurElement(apXML_Parser, *apParent),
    fBase(aBase)
{
    assert(aName == "osm");
    if (find(aAtts.begin(), aAtts.end(), pair<string, string>("version", "0.6")) == aAtts.end())
        throw Exception("osm Version 0.6 attendu",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserOsm::startElement(const string& aName,
                             const vector< pair<string, string> >& aAtts)
{
// Dans l'ordre de probabilité d'apparition.
    if (aName == "node") {
//        fpEnfant = new ParserNode(fpXML_Parser, fBase, this, aName, aAtts);
        fpEnfant = new ParserNode(fpXML_Parser, *this, aName, aAtts);
    }
    else if (aName == "way") {
        fpEnfant = new ParserWay(fpXML_Parser, *this, aName, aAtts);
    }
    else if (aName == "changeset") {
        fpEnfant = new ParserChangeset(fpXML_Parser, *this, aName, aAtts);
    }
    else if (aName == "relation") {
        fpEnfant = new ParserRelation(fpXML_Parser, *this, aName, aAtts);
    }
    else if (aName == "bound" || aName == "bounds") {
        fpEnfant = new ParserBounds(fpXML_Parser, *this, aName, aAtts);
    }
    else throw Exception("Element inattendu (" + aName + ")",
                         __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserOsm::endElement(const string& aName)
{
    assert(aName == "osm");
}


void ParserOsm::traiter(const Node& aNode)
{ fBase.add(aNode); }

void ParserOsm::traiter(const Way& aWay)
{ fBase.add(aWay); }

void ParserOsm::traiter(const Relation& aRelation)
{ fBase.add(aRelation); }

void ParserOsm::traiter(const Changeset& aChangeset)
{ fBase.add(aChangeset); }

