/*
 * BISDATA.h
 *
 *  Created on: 2018. 3. 14.
 *      Author: lsh
 */

#ifndef SRC_BIS_BISDATA_H_
#define SRC_BIS_BISDATA_H_
#include "../DbConnection/DbConnection.h"
#include <string>

class BISDATA {
public:
	BISDATA();
	BISDATA(sql::ResultSet* res);
	virtual ~BISDATA();

	std::string APPLY_DATE;		// 적용일시
	std::string ROUTE_ID;			// 노선ID
	std::string TKCAR_STTN_ID;		// 승차정류장ID
	int STTN_SEQ;			// 정류장순서
};

#endif /* SRC_BIS_BISDATA_H_ */
