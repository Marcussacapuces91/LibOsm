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

#ifndef BASEINTERFACE_H
#define BASEINTERFACE_H

//class Changeset;
#include "../osm/changeset.h"
#include "../osm/node.h"

class Way;
class Relation;

/**
 * \interface BaseInterface
 * Classe virtuelle pure (interface) assurant la disponibilité de 4 méthodes
 * permettant stocker des instances d'OSM dans une base.
 */
class BaseInterface
{
  public:

/**
 * Destructeur virtuel de l'instance.
 */
    virtual ~BaseInterface(void) {}

/**
 * Ajouter un Changeset dans la base.
 * \param aChangeset Une référence sur une instance de Changeset à inserer.
 */
    virtual void add(const Changeset& aChangeset) = 0;

/**
 * Ajouter un Node dans la base.
 * \param aNode Une référence sur une instance de Node à inserer.
 */
    virtual void add(const Node& aNode) = 0;

/**
 * Ajouter un Way dans la base.
 * \param aWay Une référence sur une instance de Way à inserer.
 */
    virtual void add(const Way& aWay) = 0;

/**
 * Ajouter une Relation dans la base.
 * \param aRelation Une référence sur une instance de Relation à inserer.
 */
    virtual void add(const Relation& aRelation) = 0;

/**
 * Supprimer un Node dans la base.
 * \param aNode Une référence sur une instance de Node à supprimer.
 */
    virtual void suppress(const Node& aNode) = 0;

/**
 * Supprimer un Way dans la base.
 * \param aWay Une référence sur une instance de Way à supprimer.
 */
    virtual void suppress(const Way& aWay) = 0;

/**
 * Supprimer une Relation dans la base.
 * \param aRelation Une référence sur une instance de Relation à supprimer.
 */
    virtual void suppress(const Relation& aRelation) = 0;

/**
 * Modifier un Node dans la base.
 * \param aNode Une référence sur une instance de Node à modifier.
 */
    virtual void modify(const Node& aNode) = 0;

/**
 * Modifier un Way dans la base.
 * \param aWay Une référence sur une instance de Way à modifier.
 */
    virtual void modify(const Way& aWay) = 0;

/**
 * Modifier une Relation dans la base.
 * \param aRelation Une référence sur une instance de Relation à modifier.
 */
    virtual void modify(const Relation& aRelation) = 0;

    virtual unsigned nbChangesets() const  = 0;

/**
 * Retourne de nombre de Nodes dans la base.
 */
    virtual unsigned nbNodes() const  = 0;
    virtual unsigned nbWays() const  = 0;
    virtual unsigned nbRelations() const  = 0;

};

#endif
