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
#include "parsermember.h"
#include "parserrelation.h"
#include "parsertag.h"

ParserMember::ParserMember(XML_Parser *const apXML_Parser,
            			   Parser& aParent,
            			   const string& aName,
            			   const vector< pair<string, string> >& aAtts) :
	Parser(apXML_Parser, &aParent),
	fMember()
{
	assert(aName == "member");
	for (vector< pair<string, string> >::const_iterator it = aAtts.begin(); it != aAtts.end(); ++it) {
		fMember.setAttribut(it->first, it->second);
	}
}

void ParserMember::startElement(const string& aName,
                                const vector< pair<string, string> >& /* aAtts */)
{
	throw Exception("Element inattendu (" + aName + ")", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserMember::endElement(const string& aName)
{
	assert(aName == "member");
    dynamic_cast<ParserRelation *const>(fpParent)->addMember(fMember);
}
