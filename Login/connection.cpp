#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("ma");//inserer nom de l'utilisateur
db.setPassword("malek");//inserer mot de passe de cet utilisateurs

if (db.open())
test=true;





    return  test;
}


