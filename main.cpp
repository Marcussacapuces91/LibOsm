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

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>
#include "wrapper_spatialite.h"
#include "exception.h"
#include "baseosm.h"
#include "xml/parserplanet.h"
#include <tclap/CmdLine.h>
#include "libspatialite/headers/spatialite.h"

using namespace std;
using namespace TCLAP;

static const string VERSION = "0.1";
static const string DESCRIPTION_COURTE = "\
Parseur de fichiers OSM (planet, OsmChange)\
\n\
osm2sqlite Copyright (C) 2010 par Marc Sibert\
\n\
This program comes with ABSOLUTELY NO WARRANTY; for details type option \
'--copyright'. This is free software, and you are welcome to redistribute it \
under certain conditions; type option '--copyright' for details.";

static const string COPYRIGHT = "\
Parseur de fichiers OSM (planet, OsmChange), les données lues sont insérées \
dans une base Sqlite/Spatialite\n\
\n\
Copyright (C) 2010 par Marc Sibert\n\
\n\
This program is free software: you can redistribute it and/or modify \n\
it under the terms of the GNU Lesser General Public License as published by \n\
the Free Software Foundation, either version 3 of the License, or \n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful, \n\
but WITHOUT ANY WARRANTY; without even the implied warranty of \n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the \n\
GNU Lesser General Public License for more details.\n\
\n\
You should have received a copy of the GNU Lesser General Public License \n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\
\n\
Attention ce programme fait appel ou inclue les librairies suivantes qui \n\
peuvent utiliser des licences différentes :\n\
\n\
  - eXpat sous licence \"MIT/X Consortium license\"\n\
  - TCLAP sous licence \"MIT License\" \
<http://opensource.org/licenses/mit-license.php>\n\
  - SQLite3 versé au domaine public <http://www.sqlite.org/copyright.html> \n\
  - Spatialite sous licence MPL v1.1 \
<http://www.mozilla.org/MPL/MPL-1.1.html>\n\
  - IConv sous licence GNU Lesser General Public Licence (LGPL) \
<http://www.gnu.org/copyleft/lesser.html>\n\
  - GEOS sous licence GNU Lesser General Public Licence (LGPL)\n\
  - Proj4 sous licence \"MIT license\"";

/**
 * Classe hérité de TCLAP::Visitor pour assurer l'affichage du message de
 * copyright.
 */
class CopyrightVisitor : public Visitor
{
    public:
        CopyrightVisitor() : Visitor() {} ;
        void visit() { cout << COPYRIGHT << endl;  exit(EXIT_SUCCESS); };
};

/**
 * Fonction principale qui lance l'application.
 * \param argc Nombre d'arguments de la ligne de commande.
 * \param argv Tableau des arguments.
 * \return Un code indiquant le succès ou une erreur dans l'exécution.
 */
int main(int argc, char **argv)
{
    try {
        CmdLine cmd(DESCRIPTION_COURTE, ' ', VERSION);
//        SwitchArg tests("t", "test", "Lancement des tests unitaires", cmd, false);
        ValueArg<string> input("i", "input", "Nom du fichier OSM en entrée", false, "", "string", cmd);
        UnlabeledValueArg<string> pathBase("database", "Chemin vers le fichier de la base SQLite3", true, "", "string", cmd);
        SwitchArg verbose("v", "verbose", "Génération des messages de progression", cmd, false);
        SwitchArg initSpatialite("", "initspatialite", "", cmd, false);
        SwitchArg indexes("", "indexes", "", cmd, false);
        ValueArg<unsigned> cache("c", "cache", "Taille du cache de la BD en pages", false, 0, "entier", cmd);
        CopyrightVisitor copyrightVisitor;
        SwitchArg copyright("", "copyright", "Affiche les licences applicables à ce programme", cmd, false, &copyrightVisitor);
        cmd.parse( argc, argv );

        spatialite_init(0);
//		Spatialite& __attribute__((unused)) spatialite = Spatialite::returnSpatialite();    // Initialise la librairie Spatialite
        const string path = pathBase.getValue();
        const bool init = initSpatialite.getValue();

        BaseOsm base(path, init);
        if (cache.isSet()) base.cacheSize(cache.getValue());

        if (verbose.getValue()) cout << "Debut du parsing" << endl;
        const clock_t debut = clock();
        base.exec("BEGIN");
        if (input.isSet()) {
            ifstream f(input.getValue().c_str(), fstream::in | fstream::binary);
            ParserPlanet parserPlanet(base, f);
            f.close();
        } else {
            ParserPlanet parserPlanet(base, cin);
        }
        base.exec("COMMIT");
        if (verbose.getValue()) {
            const clock_t fin = clock();
            cout << endl << "Fin du parsing - Duree : " << (double(fin) - double(debut)) / CLOCKS_PER_SEC << "s" << endl;
            cout << "Creation des indexes & analyse" << endl;
        }

        if (indexes.getValue()) {
            base.createIndexes();
            if (verbose.getValue()) {
            		const clock_t fin = clock();
                cout << endl << "Fin de l'analyse - Duree : " << (double(fin) - double(debut)) / CLOCKS_PER_SEC << "s" << endl;
            }
        }
    } catch (TCLAP::ArgException &e) { // catch any exceptions
        cerr << "erreur : " << e.error() << " pour l'argument " << e.argId() << endl;
        return EXIT_FAILURE;
    } catch (Exception &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    } catch (exception &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    } catch (...) {
        cerr << "Exception inconnue" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
