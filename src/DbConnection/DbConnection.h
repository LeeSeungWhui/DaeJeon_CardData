/*
 * DBCONNECTION.h
 *
 *  Created on: 2018. 3. 12.
 *      Author: lsh
 */

#ifndef DBCONNECTION_DBCONNECTION_H_
#define DBCONNECTION_DBCONNECTION_H_
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string>

class DBCONNECTION {
public:
	//fuction
	int connect();
	int connect(std::string db_host, std::string db_user, std::string db_pass, std::string db_name);
	int runQuery();
	int runQuery(std::string query);
	void useDatabase(std::string database);
	// contructor & destructor
	DBCONNECTION();
	DBCONNECTION(std::string db_host, std::string db_user, std::string db_pass, std::string db_name);
	virtual ~DBCONNECTION();

	// getter & setter
	const std::string& getDbHost() const {
		return db_host;
	}

	void setDbHost(const std::string& dbHost) {
		db_host = dbHost;
	}

	const std::string& getDbName() const {
		return db_name;
	}

	void setDbName(const std::string& dbName) {
		db_name = dbName;
	}

	const std::string& getDbPass() const {
		return db_pass;
	}

	void setDbPass(const std::string& dbPass) {
		db_pass = dbPass;
	}

	const std::string& getDbUser() const {
		return db_user;
	}

	void setDbUser(const std::string& dbUser) {
		db_user = dbUser;
	}

	const std::string& getErrorLog() const {
		return error_log;
	}

	const std::string& getQuery() const {
		return query;
	}

	void setQuery(const std::string& query) {
		this->query = query;
	}

	const sql::Connection* getConn() const {
		return conn;
	}

	const sql::Driver* getDriver() const {
		return driver;
	}

	sql::ResultSet* getRes() const {
		return res;
	}

	const sql::Statement* getStmt() const {
		return stmt;
	}

private:
	std::string db_host; // ip주소
	std::string db_user; // DB접속 ID
	std::string db_pass; // ID의 비밀번호
	std::string db_name; // 접속할 DB 스키마 이름
	std::string query; // 실행할쿼리문
	std::string error_log; // 에러로그

	sql::Driver *driver;
	sql::ConnectOptionsMap connection_properties;
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *res;
};

#endif /* DBCONNECTION_DBCONNECTION_H_ */
