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
#include <expat.h>
#include "parsermember.h"
#include "parserrelation.h"
#include "parsertag.h"

ParserMember::ParserMember(XML_Parser *const apXML_Parser,
                           BaseInterface& aBase,
            			   Parser *const apParent,
            			   const string& aName,
            			   const vector< pair<string, string> >& aAtts) :
	Parser(apXML_Parser, apParent),
	fBase(aBase),
	fMember()
{
	assert(aName == "member");
	for (vector< pair<string, string> >::const_iterator it = aAtts.begin(); it != aAtts.end(); ++it) {
		fMember.setAttribut(it->first, it->second);
	}
}

void ParserMember::startElement(const string& aName,
                                const vector< pair<string, string> >& aAtts)
{
	throw Exception("Element inattendu (" + aName + ")", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserMember::endElement(const string& aName)
{
	assert(aName == "member");
    dynamic_cast<ParserRelation *const>(fpParent)->addMember(fMember);
}
