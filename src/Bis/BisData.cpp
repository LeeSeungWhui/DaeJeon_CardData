/*
 * BISDATA.cpp
 *
 *  Created on: 2018. 3. 14.
 *      Author: lsh
 */

#include "BisData.h"

BISDATA::BISDATA() {
	// TODO Auto-generated constructor stub
	this->APPLY_DATE = "";
	this->ROUTE_ID = "";
	this->STTN_SEQ = 0;
	this->TKCAR_STTN_ID = "";
}

BISDATA::~BISDATA() {
	// TODO Auto-generated destructor stub
}

BISDATA::BISDATA(sql::ResultSet* res)
{
	this->APPLY_DATE = res->getString("적용일시");
	this->ROUTE_ID = res->getString("노선ID");
	this->STTN_SEQ = atoi(res->getString("정류장순서").c_str());
	this->TKCAR_STTN_ID = res->getString("정류장ID");
}
