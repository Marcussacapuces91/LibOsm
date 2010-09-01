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


#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <expat.h>
#include <expat_external.h>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <utility>

using namespace std;

/**
 * Classe abstraite ancètre des différents parseurs.
 */
class Parser
{
	private:

/**
 * Constructeur de copie rendu inaccessible.
 */
        Parser(const Parser&);

/**
 * Operateur d'affectation rendu inaccessible.
 */
        Parser& operator=(const Parser&);


	protected:
/// Pointeur sur une instance de parser Expat.
		XML_Parser *const fpXML_Parser;

/// Pointeur sur l'instance parente ayant créée l'instance courante
/// ou 0 si on se trouve à la racine de l'arborescence.
		Parser *const fpParent;

/// Pointeur sur une instance enfant de l'instance courante
/// ou 0 si elle n'existe pas.
		Parser* fpEnfant;

/**
 * Constructeur protégé de l'instance virtuelle pure.
 * \param apXML_Parser un pointeur sur une instance de XML_Parser existante ou 0
 *                     pour que ce constructeur instancie lui-même un XML_Parser.
 * \param apParent Un pointeur sur l'instance parente de celle-ci ou 0 s'il
 *                 s'agit de la racine.
 */
		Parser(XML_Parser *const apXML_Parser,
		       Parser *const apParent = 0);

/**
 * \brief Méthode virtuelle pure appelée lors du début d'un élément XML,
 * un sous-élément de l'élément géré par cette classe.
 * Cette méthode doit être implémenté afin d'assurer l'appel des autres classes
 * parserXXX suivant l'élément XML détecté. Cette arborescence est liée à la
 * structure XML des fichiers a analyser.
 * \param aName Le nom de l'élément XML.
 * \param aAtts La liste des attributs liés à cet élément.
 */
		virtual void startElement(const string& aName,
		                          const vector< pair<string,string> >& aAtts) = 0;

/**
 * \brief Méthode virtuelle pure appelée lors de la détection de la fin de
 * l'élément XML courant.
 * L'implémentation de cette méthode doit s'assurer que le libellé de cet
 * élément XML est  cohérant avec le libellé attentu.
 * Une vérification avec un assert() doit être prévue.
 */
        virtual void endElement(const string& aName) = 0;

	public:

/**
 * \brief Méthode classe appelée par le parseur XML (eXpat).
 * Cette méthode appelle startElement pour l'instance parserXXX courante.
 * \param userData Un pointeur sur l'instance parserXXX courante.
 * \param name Un pointeur sur le nom de l'élément XML.
 * \param atts Un tableau de paires clé / valeur représentant chaque attribut.
 * \pre userData != 0.
 * \pre name != 0.
 */
		static void startElementHandler(void *userData,
										const XML_Char *name,
				                        const XML_Char **atts);

/**
 * \brief Méthode classe appelée par le parseur XML (eXpat).
 * Cette méthode appelle endElement pour l'instance parserXXX suivante.
 * \param userData Un pointeur sur l'instance parserXXX précédente (?).
 * \param name Un pointeur sur le nom du sous-élément XML.
 * \pre userData != 0.
 * \pre name != 0.
 */
        static void endElementHandler(void *userData,
                                      const XML_Char *name);

/**
 * \brief Destructeur virtuel de l'instance.
 * Cette méthode rétablit l'instance père de la classe parserXXX courante.
 */
        virtual ~Parser();
};

#endif
