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

#ifndef PARSERMODIFY_H
#define PARSERMODIFY_H

#include "parserconteneurelement.h"    // header de l'interface héritée.

#include "baseinterface.h"
#include "../Exception.h"

/**
 * \brief Classe analysant un flux XML commençant à l'élément 'modify'.
 *
 * Classe analysant un flux XML commençant à l'élément 'modify' et contenant des
 * éléments 'node', 'way' et 'relation'.
 * Pour utiliser cette classe, il suffit de l'instancier. Des appels seront fait
 * automatiquement aux bonnes classes suivant les éléments rencontrés.
 * Les méthodes traiter() (callback) insèreront les éléments dans la base de données.
 * Ces  méthodes sont les implémentations de l'interface ParserConteneurElement.
 */
class ParserModify : public ParserConteneurElement
{
public:
    /**
     * \brief Constructeur de l'instance qui lance l'analyse du flux.
     * \param apXML_Parser Un pointeur sur le parseur XML (eXpat).
     * \param aBase Une référence sur la base de données où se feront les insertions.
     * \param apParent Un pointeur sur la classe similaire qui est à l'origine de
     *                 cette instance.
     * \param aName Nom de l'élément ayant déclenché l'appel pour vérification.
     * \param aAtts Liste des attributs liés à l'élément 'modify'.
     * \pre aName == 'modify'.
     */
    ParserModify(XML_Parser *const apXML_Parser,
                 BaseInterface& aBase,
                 Parser *const apParent,
                 const string& aName,
                 const vector< pair<string, string> >& aAtts);

    /**
     * Destructeur virtuel de l'instance sans action.
     */
    virtual ~ParserModify() {}

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
     * Méthode callback n'assurant aucune action. Elle déclenche systématiquement une exception.
     * \param aChangeset Une référence sur le Changeset à insérer dans la base.
     */
    void traiter(const Changeset& aChangeset)
    {
        throw Exception("Pas de traitement prévu pour le Changeset",
                        __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

protected:
    virtual void startElement(const string& aName,
                              const vector< pair<string, string> >& aAtts);

    virtual void endElement(const string& aName);

private:
    BaseInterface& fBase;

    Element* fpElement;

    /** Copy constructor
     *  \param other Object to copy from
     */
    ParserModify(const ParserModify& other);

    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    ParserModify& operator=(const ParserModify& other);

};

#endif // PARSERMODIFY_H
