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

#include "parser.h"
#include "../exception.h"
#include <cassert>
#include <iostream>
#include <expat.h>


Parser::Parser(XML_Parser *const apXML_Parser,
               Parser *const apParent) :
	fpXML_Parser(apXML_Parser),
	fpParent(apParent),
	fpEnfant(0)
{
	assert(fpXML_Parser);
	XML_SetUserData(*fpXML_Parser, static_cast<void*>(this));
}

Parser::~Parser()
{
    if (fpParent)
        XML_SetUserData(*fpXML_Parser, static_cast<void*>(fpParent));
    else
        XML_SetUserData(*fpXML_Parser, 0);
}

void Parser::startElementHandler(void *userData,
                                 const XML_Char *name,
                                 const XML_Char **atts)
{
	Parser *const p = static_cast<Parser*>(userData);
	assert(p);
	assert(name);
	vector< pair<string,string> > attributs;
	while (*atts) {
		const string k = *(atts++);
		assert(*atts);
		const string v = *(atts++);
		attributs.push_back(make_pair(k,v));
	}
	p->startElement(name, attributs);
}

void Parser::endElementHandler(void *userData,
                               const XML_Char *name)
{
	Parser *const p = static_cast<Parser*>(userData);
	assert(p);
	assert(name);
	p->endElement(name);
	delete p;
}

