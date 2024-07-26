#include "logic.hpp"

int main() {
	const char* conninfo = "dbname=mydb user=admin password=SQLRashedAljahdali12@# hostaddr=192.168.1.15 port=6666";
	PGconn* conn = PSQLSERVERCONN(conninfo);
	
	int which;


	std::cout << "Login [1] create an account [2] : \n";
	std::cin >> which;

	if (which == 1) {
		Loginfun(conn);

	}
	if (which == 2) {
		
		Createanaccount(conn);

	}

	if (which == 020507055) {

		Pulldata(conn);
	}




	
	PQfinish(conn);


}

