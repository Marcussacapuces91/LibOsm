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

#ifndef PARSERPLANET_H
#define PARSERPLANET_H

#include "parser.h"
#include <istream>
#include "baseinterface.h"

using namespace std;

class ParserPlanet : public Parser
{
	private:
        BaseInterface& fBase;

		istream& fStream;

//		long unsigned fNbChangeset;
//		long unsigned fNbNode;
//		long unsigned fNbWay;
//		long unsigned fNbRelation;

		static XML_Parser* makeXML_Parser();

//		void addChangeset() { ++fNbChangeset; }

	protected:
		virtual void startElement(const string& aName,
                                  const vector< pair<string, string> >& aAtts);

        virtual void endElement(const string& aName);

	public:
/**
 * Constructeur de classe.
 * Assure le parsing du flux bloc par bloc.
 * @param aBase Une référence sur une base de données instanciant l'interface
 *              BaseInterface.
 * @param aStream Une référence sur le flux entrant qui sera consommé.
 * @param aTailleBloc Le nombre d'octets lus dans le flux d'un coup avant de
 *        lancer une phase de parsing XML.
 *        Par défaut, le bloc aura une taille de 1 Mo.
 * @throw Exception En cas d'erreur sur la lecture du flux ou l'utilisation des
 *                  fonctions d'expat.
 */
		ParserPlanet(BaseInterface& aBase,
					 istream& aStream,
					 const unsigned long aTailleBloc = (1L << 18));

/**
 * Destructeur de l'instance.
 * N'a ausune action sur les ressources transmises Ã  l'instanciation
 * (flux et XML_Parser).
 */
		virtual ~ParserPlanet();

};


#endif
