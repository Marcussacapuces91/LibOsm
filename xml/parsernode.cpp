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
#include "parsernode.h"
#include "parsertag.h"
#include "../osm/node.h"

ParserNode::ParserNode(XML_Parser *const apXML_Parser,
//                       BaseInterface& aBase,
                       ParserConteneurElement& aParent,
					   const string& aName,
					   const vector< pair<string, string> >& aAtts) :
	Parser(apXML_Parser, &aParent),
//	fBase(aBase),
	fNode()
//	fParent(*apParent)  // et si apParent == 0 ??
{
//    assert(apParent);
	assert(aName == "node");
	for (vector< pair<string, string> >::const_iterator it = aAtts.begin(); it != aAtts.end(); ++it) {
		fNode.setAttribut(it->first, it->second);
	}
}

void ParserNode::startElement(const string& aName,
                              const vector< pair<string, string> >& aAtts)
{
	if (aName == "tag") {
		fpEnfant = new ParserTag(fpXML_Parser, *this, aName, aAtts);
	} else throw Exception("Element inattendu (" + aName + ")",
                           __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserNode::endElement(const string& aName)
{
	assert(aName == "node");
    dynamic_cast<ParserConteneurElement&>(*fpParent).traiter(fNode);
}
