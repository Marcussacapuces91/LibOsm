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

#ifndef PARSERBOUNDS_H
#define PARSERBOUNDS_H

#include <expat.h>
#include <expat_external.h>
#include <string>
#include <vector>
#include "parser.h"
using namespace std;

class ParserBounds : public Parser
{
	protected:
		virtual void startElement(const string& aName,
                                  const vector< pair<string, string> >&);

        virtual void endElement(const string& aName);

	public:
		ParserBounds(XML_Parser *const apXML_Parser,
                     Parser& aParent,
					 const string& aName,
					 const vector< pair<string, string> >&);

		virtual ~ParserBounds() {};
};

#endif
