#pragma once

#include <iostream>
#include <libpq/libpq-fs.h>
#include <libpq-fe.h>
#include <string>
#include <cstdio>


PGconn* PSQLSERVERCONN(const char* conninfo) {



	PGconn* conn = PQconnectdb(conninfo);

	if (PQstatus(conn) != CONNECTION_OK) {
		std::cerr << "failed" << PQerrorMessage(conn) << std::endl;
		PQfinish(conn);
	}
	std::cout << "Connection to database succeeded!" << std::endl;
	return conn;

}


void Createanaccount(PGconn* conn) {

	std::string name, accountid, money, password;
	int User_age;
	std::cout << "Enter name: ";
	std::cin >> name;
	std::cout << "Enter age: ";
	std::cin >> User_age;
	std::cout << "Enter account ID: ";
	std::cin >> accountid;
	std::cout << "Enter money: ";
	std::cin >> money;
	std::cout << "Enter password: ";
	std::cin >> password;
	char q[512];
	snprintf(q, sizeof(q),
    "INSERT INTO users (name , user_age , accountid , money , password) VALUES ('%s' , %d , '%s' , '%s' , '%s')",
		name.c_str(), User_age, accountid.c_str(), money.c_str(), password.c_str());

	PGresult* res = PQexec(conn, q);

	if (PQresultStatus(res) != PGRES_COMMAND_OK) {

		std::cerr << "Failed The Insert command" << PQerrorMessage(conn) << std::endl;
		PQclear(res);
		PQfinish(conn);
		exit(1);
	}
	std::cout << "done The Insert command" << std::endl;
	PQclear(res);

}





void Pulldata(PGconn* conn) {
	const char *q = "SELECT * FROM users";

	PGresult* res = PQexec(conn, q);

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {

		std::cerr << "Failed The PULL command" << PQerrorMessage(conn) << std::endl;
		PQclear(res);
		PQfinish(conn);
		exit(1);
	}
	int nrows = PQntuples(res);
	int ncols = PQnfields(res);

	// Print column names
	for (int i = 0; i < ncols; i++) {
		std::cout << PQfname(res, i) << "\t";
	}
	std::cout << std::endl;

	// Print rows
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			std::cout << PQgetvalue(res, i, j) << "\t";
		}
		std::cout << std::endl;
	}

	PQclear(res);


}



bool LogIn(PGconn* conn , const std::string& accountid , const std::string& password) {


	const char* paramValues[1] = { accountid.c_str() };
	
	PGresult* res = PQexecParams(conn, "SELECT password FROM users WHERE accountid = $1", 1, nullptr, paramValues, nullptr, nullptr, 0);

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cerr << "SELECT command failed: " << PQerrorMessage(conn) << std::endl;
		PQclear(res);
		return false;
	}

	
	if (PQntuples(res) == 0) {
		std::cerr << "No such account ID found." << std::endl;
		PQclear(res);
		return false;
	}

	
	std::string db_password = PQgetvalue(res, 0, 0);
	PQclear(res);

	
	if (password == db_password) {
		return true;
	}
	else {
		std::cerr << "Incorrect password." << std::endl;
		return false;
	}


}


void Loginfun(PGconn* conn) {

	std::string accountid, password;

	// Prompt user for input
	std::cout << "Enter account ID: ";
	std::cin >> accountid;
	std::cout << "Enter password: ";
	std::cin >> password;

	// Check login credentials
	if (LogIn(conn, accountid, password)) {
		std::cout << "Login successful!" << std::endl;
	}
	else {
		std::cout << "Login failed." << std::endl;
	}

	PQfinish(conn);

}
