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

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <expat.h>
#include <expat_external.h>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <utility>

using namespace std;

class Parser
{
	private:

	protected:
		XML_Parser *const fpXML_Parser;
		Parser *const fpParent;
		Parser* fpEnfant;

/**
 * Constructeur protégé de l'instance virtuelle pure.
 * @param apXML_Parser un pointeur sur une instance de XML_Parser existante ou 0
 *                     pour que ce constructeur instancie lui-même un XML_Parser.
 * @param apParent Un pointeur sur l'instance parente de celle-ci ou 0 s'il
 *                 s'agit de la racine.
 */
		Parser(XML_Parser *const apXML_Parser,
		       Parser *const apParent = 0);

		virtual void startElement(const string& aName,
		                          const vector< pair<string,string> >& aAtts) = 0;

        virtual void endElement(const string& aName) = 0;

	public:

		static void startElementHandler(void *userData,
										const XML_Char *name,
				                        const XML_Char **atts);

        static void endElementHandler(void *userData,
                                      const XML_Char *name);

        virtual ~Parser();
};

#endif
