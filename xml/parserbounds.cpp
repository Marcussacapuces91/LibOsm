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

#include "parserbounds.h"
#include "../exception.h"
#include <cassert>
#include <iostream>
#include <expat.h>

ParserBounds::ParserBounds(XML_Parser *const apXML_Parser,
						   Parser& aParent,
                           const string& aName,
                           const vector< pair<string, string> >&) :
	Parser(apXML_Parser, &aParent)
{
//	cout << "ParserBounds : " << aName << endl;
	assert(aName == "bounds" || aName == "bound");
};

void ParserBounds::startElement(const string& aName,
								const vector< pair<string, string> >&)
{
	throw Exception("Pas de sous-element attendu (" + aName + ")",
                    __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserBounds::endElement(const string& aName)
{
	assert(aName == "bounds" || aName == "bound");
}

