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

#include "parserosmchange.h"
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
#include "parsermodify.h"
#include "parserdelete.h"
#include "parsercreate.h"

ParserOsmChange::ParserOsmChange(XML_Parser *const apXML_Parser,
                     BaseInterface& aBase,
                     Parser *const apParent,
                     const string& aName,
                     const vector< pair<string, string> >& aAtts) :
	Parser(apXML_Parser, apParent),
	fBase(aBase)
{
	assert(aName == "osmChange");
	if (find(aAtts.begin(), aAtts.end(), pair<string, string>("version", "0.6")) == aAtts.end())
		throw Exception("osmChange Version 0.6 attendu",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserOsmChange::startElement(const string& aName,
                             const vector< pair<string, string> >& aAtts)
{
    if (aName == "modify") {
// cerr << "*";
        fpEnfant = new ParserModify(fpXML_Parser, fBase, this, aName, aAtts);
    }
    else if (aName == "delete") {
// cerr << "-";
        fpEnfant = new ParserDelete(fpXML_Parser, fBase, this, aName, aAtts);
    }
    else if (aName == "create") {
// cerr << "+";
        fpEnfant = new ParserCreate(fpXML_Parser, fBase, this, aName, aAtts);
    }
    else  throw Exception("Element inattendu (" + aName + ")",
                          __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserOsmChange::endElement(const string& aName)
{
	assert(aName == "osmChange");
}

