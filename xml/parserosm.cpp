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

#include "parserosm.h"
#include "../exception.h"
#include <cassert>
#include <iostream>
#include <expat.h>
#include "parserbounds.h"
#include "parserchangeset.h"
#include "parsernode.h"
#include "parserway.h"
#include "parserrelation.h"

ParserOsm::ParserOsm(XML_Parser *const apXML_Parser,
                     BaseInterface& aBase,
                     Parser *const apParent,
                     const string& aName,
                     const vector< pair<string, string> >& aAtts) :
	Parser(apXML_Parser, apParent),
	fBase(aBase)
{
	assert(aName == "osm");
	if (find(aAtts.begin(), aAtts.end(), pair<string, string>("version", "0.6")) == aAtts.end())
		throw Exception("osm Version 0.6 attendu", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserOsm::startElement(const string& aName,
                             const vector< pair<string, string> >& aAtts)
{
// Dans l'ordre de probabilité d'apparition.
	if (aName == "node") {
		fpEnfant = new ParserNode(fpXML_Parser, fBase, this, aName, aAtts);
	}
	else if (aName == "way") {
		fpEnfant = new ParserWay(fpXML_Parser, fBase, this, aName, aAtts);
	}
	else if (aName == "changeset") {
		fpEnfant = new ParserChangeset(fpXML_Parser, fBase, this, aName, aAtts);
	}
	else if (aName == "relation") {
		fpEnfant = new ParserRelation(fpXML_Parser, fBase, this, aName, aAtts);
	}
	else if (aName == "bound" || aName == "bounds") {
		fpEnfant = new ParserBounds(fpXML_Parser, this, aName, aAtts);
	}
	else throw Exception("Element inattendu (" + aName + ")", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserOsm::endElement(const string& aName)
{
	assert(aName == "osm");
}

