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

#ifndef PARSER_CONTENEUR_ELEMENT_H
#define PARSER_CONTENEUR_ELEMENT_H

#include "parser.h"

class Node;
class Way;
class Relation;
class Changeset;

/**
 * Interface indiquant qu'une classe va publier 3 méthodes :
 * - void traiter(const Node&)
 * - void traiter(const Way&)
 * - void traiter(const Relation&)
 * Ces méthodes seront appelées par les méthodes ParserNode::endElement,
 * ParserWay::endElement et ParserRelation::endElement afin de transmettre les
 * éléments correspondants au parseur parent.
 */
class ParserConteneurElement : public Parser
{
    public:
 /**
  * Destructeur virtuel, sans action.
  */
        virtual ~ParserConteneurElement() {}

/**
 * \brief Méthode abstraite de l'interface.
 *
 * Cette méthode devra implémenter le code nécessaire pour traiter l'élément
 * correspondant.
 * \param aNode Une référence sur un Node à traiter.
 * \warning La validité de la référence n'est pas garantie au-delà de l'appel de
 *          la méthode.
 */
        virtual void traiter(const Node& aNode) = 0;

/**
 * \brief Méthode abstraite de l'interface.
 *
 * Cette méthode devra implémenter le code nécessaire pour traiter l'élément
 * correspondant.
 * \param aWay Une référence sur un Way à traiter.
 * \warning La validité de la référence n'est pas garantie au-delà de l'appel de
 *          la méthode.
 */
        virtual void traiter(const Way& aWay) = 0;

/**
 * \brief Méthode abstraite de l'interface.
 *
 * Cette méthode devra implémenter le code nécessaire pour traiter l'élément
 * correspondant.
 * \param aRelation Une référence sur une Relation à traiter.
 * \warning La validité de la référence n'est pas garantie au-delà de l'appel de
 *          la méthode.
 */
        virtual void traiter(const Relation& aRelation) = 0;

        virtual void traiter(const Changeset& aChangeset) = 0;

    protected:
/**
 * \brief Constructeur par défaut.
 *
 * Déclarée protected, cette classe est non instanciable, mais dérivable.
 */
        ParserConteneurElement(XML_Parser *const apXML_Parser,
                Parser& aParent) :
            Parser(apXML_Parser, &aParent)
        {}


    private:
};

#endif // PARSER_CONTENEUR_ELEMENT_H
