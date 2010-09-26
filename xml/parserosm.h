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

#ifndef PARSEROSM_H
#define PARSEROSM_H

#include <expat.h>
#include <expat_external.h>
#include <string>
#include <vector>
#include "parser.h"
#include "parserplanet.h"
#include "baseinterface.h"
#include "parserconteneurelement.h"

using namespace std;

// class ParserPlanet;

/**
 * Classe analysant un flux XML commençant à l'élément 'osm' et contenant des
 * éléments 'bound', 'node', 'way', 'relation' et 'changeset'.
 * Pour utiliser cette classe, il suffit de l'instancier. Des appels seront fait
 * automatiquement aux bonnes classes suivant les éléments rencontrés.
 * Les méthodes add (callback) insèreront les éléments dans la base de données.
 * Ces  méthodes sont les implémentations de l'interface ParserConteneurElement.
 */
class ParserOsm : public ParserConteneurElement
{
private:
/// Une référence sur la base de données où sont stockés les différents éléments.
    BaseInterface& fBase;

protected:
    /**
     * Méthode appelée par le parseur à chauque détection d'un sous-élément de 'osm'.
     * \param aName Nom de l'élément détecté.
     * \param aAtts Liste des attributs liés à l'élément détecté.
     */
    virtual void startElement(const string& aName,
                              const vector< pair<string, string> >& aAtts);

    /**
     * Méthode appelée par le parseur à la fermeture de l'élément 'osm'.
     * \param aName Nom de l'élément.
     * \pre aName == 'osm'.
     */
    virtual void endElement(const string& aName);

public:
    /**
     * \brief Constructeur de l'instance qui lance l'analyse du flux.
     * \param apXML_Parser Un pointeur sur le parseur XML (eXpat).
     * \param aBase Une référence sur la base de données où se feront les insertions.
     * \param apParent Un pointeur sur la classe similaire qui est à l'origine de
     *                 cette instance.
     * \param aName Nom de l'élément ayant déclenché l'appel pour vérification.
     * \param aAtts Liste des attributs liés à l'élément 'osm'.
     * \pre aName == 'osm'.
     * \pre aAtts(version = 0.6).
     */
    ParserOsm(XML_Parser *const apXML_Parser,
              BaseInterface& aBase,
              Parser *const apParent,
              const string& aName,
              const vector< pair<string, string> >& aAtts);

    /**
     * Destructeur virtuel de l'instance sans action.
     */
    virtual ~ParserOsm() {}

    /**
     * Méthode callback assurant l'insertion d'un Node dans la base de données.
     * \param aNode Une référence sur le Node à insérer dans la base.
     */
    void traiter(const Node& aNode);

    /**
     * Méthode callback assurant l'insertion d'un Way dans la base de données.
     * \param aWay Une référence sur le Way à insérer dans la base.
     */
    void traiter(const Way& aWay);

    /**
     * Méthode callback assurant l'insertion d'une Relation dans la base de données.
     * \param aRelation Une référence sur la Relation à insérer dans la base.
     */
    void traiter(const Relation& aRelation);

    /**
     * Méthode callback assurant l'insertion d'un Changeset dans la base de données.
     * \param aChangeset Une référence sur le Changeset à insérer dans la base.
     */
    void traiter(const Changeset& aChangeset);

};

#endif
