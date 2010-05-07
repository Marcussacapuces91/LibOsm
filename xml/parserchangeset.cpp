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

#include "parserchangeset.h"

#include "../exception.h"
#include <cassert>
#include <iostream>
#include <expat.h>
#include <sstream>
#include "parserosm.h"
#include "parsertag.h"

using namespace std;

struct KeyIs
{
	const string& fValue;
	KeyIs(const string& aValue) : fValue(aValue) {}
	bool operator()(const pair<string, string>& aAttribut) { return aAttribut.first == fValue; }
};

ParserChangeset::ParserChangeset(XML_Parser *const apXML_Parser,
                                 BaseInterface& aBase,
                                 Parser *const apParent,
								 const string& aName,
								 const vector< pair<string, string> >& aAtts) : 
	Parser(apXML_Parser, apParent),
	fBase(aBase)
{
	assert(aName == "changeset");
    for (vector< pair<string, string> >::const_iterator it = aAtts.begin();
         it != aAtts.end(); ++it)
        fChangeset.setAttribut(it->first, it->second);
}

void ParserChangeset::startElement(const string& aName,
                                   const vector< pair<string, string> >& aAtts)
{
  if (aName == "tag") {
     fpEnfant = new ParserTag(fpXML_Parser, this, aName, aAtts);
  }
  else throw Exception("Element inattendu (" + aName + ")",
                       __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void ParserChangeset::endElement(const string& aName)
{
	assert(aName == "changeset");
  fBase.add(fChangeset);
}

void ParserChangeset::addTag(const string& aKey,
                             const string& aValue)
{
  fChangeset.setTag(aKey, aValue);
}


