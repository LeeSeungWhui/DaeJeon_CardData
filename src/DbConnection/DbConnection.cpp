/*
 * DBCONNECTION.cpp
 *
 *  Created on: 2018. 3. 12.
 *      Author: lsh
 */

#include "DbConnection.h"
#include <iostream>
using namespace std;

DBCONNECTION::DBCONNECTION(): conn(nullptr), stmt(nullptr), res(nullptr)
{
	driver = get_driver_instance();
}

DBCONNECTION::DBCONNECTION(std::string db_host, std::string db_user, std::string db_pass, std::string db_name)
{
	driver = get_driver_instance();
	stmt = nullptr;
	res = nullptr;
	connect(db_host, db_user, db_pass, db_name);
}

DBCONNECTION::~DBCONNECTION()
{

}

void DBCONNECTION::useDatabase(string database)
{
    try
    {
        conn->setSchema(database);
        stmt=conn->createStatement();
    }
    catch (sql::SQLException &e)
    {
    	string file = __FILE__;
    	string func = __FUNCTION__;
    	string what = e.what();
    	error_log = "# ERR: SQLException in " + file;
  	   error_log += "(" + func + ") on line ";
  	   error_log += __LINE__ + "\r\n";
  	   error_log += "# ERR: " + what;
  	   error_log += " (MySQL error code: " + e.getErrorCode();
  	   error_log += ", SQLState: " + e.getSQLState() + " )" + "\r\n";
  	  // cout << error_log;
  	}
}

int DBCONNECTION::connect()
{
	if(conn != nullptr)
	{
		//conn->close();
	}
	try
	{
		connection_properties["hostName"] = (sql::SQLString)db_host;
		connection_properties["userName"] = (sql::SQLString)db_user;
		connection_properties["password"] = (sql::SQLString)db_pass;
		connection_properties["OPT_LOCAL_INFILE"] = 1;
		cout << db_host << "에 연결중...";
		conn = driver->connect(connection_properties);

		stmt = nullptr;
		res = nullptr;
		useDatabase(db_name);
		cout << "완료!" << endl;
	}
	catch (sql::SQLException &e)
	{
		string file = __FILE__;
		string func = __FUNCTION__;
		string what = e.what();
		error_log = "# ERR: SQLException in " + file;
	   error_log += "(" + func + ") on line ";
	   error_log += __LINE__ + "\r\n";
	   error_log += "# ERR: " + what;
	   error_log += " (MySQL error code: " + e.getErrorCode();
	   error_log += ", SQLState: " + e.getSQLState() + " )" + "\r\n";
	  // cout << error_log;
	  	  return 1;
	}
	return 0;
}

int DBCONNECTION::connect(std::string db_host, std::string db_user, std::string db_pass, std::string db_name)
{
	this->db_host = db_host;
	this->db_user = db_user;
	this->db_pass = db_pass;
	this->db_name = db_name;
	return connect();
}

int DBCONNECTION::runQuery()
{
	try
	{
		res = stmt->executeQuery(this->query);
	}
	catch (sql::SQLException &e)
	{
		string file = __FILE__;
		string func = __FUNCTION__;
		string what = e.what();
		error_log = "# ERR: SQLException in " + file;
	   error_log += "(" + func + ") on line ";
	   error_log += __LINE__ + "\r\n";
	   error_log += "# ERR: " + what;
	   error_log += " (MySQL error code: " + e.getErrorCode();
	   error_log += ", SQLState: " + e.getSQLState() + " )" + "\r\n";
	   //cout << error_log;
		  return 1;
	}
	return 0;
}

int DBCONNECTION::runQuery(string query)
{
	this->query = query;
	return runQuery();
}
