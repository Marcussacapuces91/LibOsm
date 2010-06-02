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
#include <istream>
#include <expat.h>
#include "parserplanet.h"
#include "parser.h"
#include "parserosm.h"
#include "parserosmchange.h"
#include <ctime>

ParserPlanet::ParserPlanet(BaseInterface& aBase,
                           istream& aStream,
                           const unsigned long aTailleBloc) :
    Parser(makeXML_Parser()),
    fBase(aBase),
    fStream(aStream)
{
    assert(aTailleBloc);
    assert(fpXML_Parser);
    unsigned long long total = 0;
    if (fStream.fail()) {
        throw Exception("Erreur à la lecture du fichier",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    XML_SetElementHandler(*fpXML_Parser,
                          Parser::startElementHandler,
                          Parser::endElementHandler);

    const time_t debut = time(NULL);
    while (!fStream.eof()) {
      	void *const pBuffer = XML_GetBuffer(*fpXML_Parser, aTailleBloc);
      	if (pBuffer) {	// si succès
            fStream.read(static_cast<char*const>(pBuffer), aTailleBloc);
            const unsigned lus = fStream.gcount();
            total += lus;
            XML_ParseBuffer(*fpXML_Parser, lus, lus < aTailleBloc);

            cout << "\r";
            const long unsigned temps = time(NULL) - debut;
            if (temps >= 3600) cout << temps / 3600 << ":";
            cout.width(2);
            cout.fill('0');
            cout << (temps / 60) % 60 << ":";
            cout.width(2);
            cout << temps % 60;
            cout << " - Lu : " << fixed << setprecision(2);
            cout << double(total) / 1024.0f / 1024.0f << " Mo";
            cout << " - Changesets : " << fBase.nbChangesets();
            cout << " - Nodes : " << fBase.nbNodes();
            cout << " - Ways : " << fBase.nbWays();
            cout << " - Relations : " << fBase.nbRelations();
      	} else {				// sinon (pBuffer = 0), on utilise un bloc statique.
            throw Exception("Réservation de mémoire par XML_GetBuffer impossible",
                            __FILE__, __LINE__, __PRETTY_FUNCTION__);
      	}
    }
}

ParserPlanet::~ParserPlanet()
{
// Comme c'est moi qui l'ai créé, je me permets de la supprimer.
    assert(fpXML_Parser);
    XML_ParserFree(*fpXML_Parser);
    delete fpXML_Parser;
}

XML_Parser* ParserPlanet::makeXML_Parser()
{
    XML_Parser *const p = new XML_Parser;
    assert(p);
    *p = XML_ParserCreate(0);
    return p;
}

void ParserPlanet::startElement(const string& aName,
                                const vector< pair<string, string> >& aAtts)
{
    if (aName == "osm") {
        fpEnfant = new ParserOsm(fpXML_Parser, fBase, this, aName, aAtts);
    } else if (aName == "osmChange") {
        fpEnfant = new ParserOsmChange(fpXML_Parser, fBase, this, aName, aAtts);
    } else throw Exception("Element inattendu (" + aName + ")",
                           __FILE__, __LINE__, __PRETTY_FUNCTION__);
    assert(fpEnfant);
}

void ParserPlanet::endElement(const string& aName)
{
    throw Exception("Pas de fin attendue (" + aName + ")",
                    __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

