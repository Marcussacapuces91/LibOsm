/*
 * Le droit d'auteur français s'applique à ce fichier, toutefois cette création
 * est mise à disposition selon le Contrat Paternité 3.0 Unported disponible en
 * ligne http://creativecommons.org/licenses/by/3.0/ ou par courrier postal à
 * Creative Commons, 171 Second Street, Suite 300, San Francisco,
 * California 94105, USA.
 *
 * En aucun cas, l'auteur ne peut être tenu responsable de tous dommages directs
 * ou indirects ou encore d'un défaut de fonctionnement de ce code source ou
 * d'un périphérique résultant de l'utilisation de ce code source original ou
 * modifié.
 *
 * Ce code source est fourni tel quel. Les demandes d'aide, de correction de bug
 * ou d'ajout de fonctionnalité peuvent être demandées, par contre, l'auteur ne
 * garantit pas la correction de ce bug ou l'ajout de fonctionnalité.
 *
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
    assert(fpXML_Parser);
	unsigned long long total = 0;
	if (fStream.fail()) {
		throw Exception("Erreur à la lecture du fichier",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
	}
	XML_SetElementHandler(*fpXML_Parser, Parser::startElementHandler, Parser::endElementHandler);

	const time_t debut = time(0);
	while (!fStream.eof()) {
		void *const pBuffer = XML_GetBuffer(*fpXML_Parser, aTailleBloc);
		if (pBuffer) {	// si succès
			fStream.read(static_cast<char*const>(pBuffer), aTailleBloc);
			const unsigned lus = fStream.gcount();
			total += lus;
//			cout << string(static_cast<char*const>(pBuffer), lus);
			XML_ParseBuffer(*fpXML_Parser, lus, lus < aTailleBloc);

			cout << "\r";
			const long unsigned temps = time(0) - debut;
			if (temps >= 3600) cout << temps / 3600 << ":";
			cout.width(2);
			cout.fill('0');
			cout << (temps / 60) % 60 << ":";
			cout.width(2);
			cout << temps % 60;
            cout << " - Lu : " << fixed << setprecision(2) << double(total) / 1024.0f / 1024.0f << " Mo";
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
	XML_ParserFree(*fpXML_Parser);
    delete fpXML_Parser;
}

XML_Parser* ParserPlanet::makeXML_Parser()
{
    XML_Parser *const p = new XML_Parser;
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
	} else throw Exception("Element inattendu (" + aName + ")", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserPlanet::endElement(const string& aName)
{
	throw Exception("Pas de fin attendue (" + aName + ")", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

