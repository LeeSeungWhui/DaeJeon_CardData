/*
 * BIS.cpp
 *
 *  Created on: 2018. 3. 14.
 *      Author: lsh
 */

#include "Bis.h"
#include "../Raw/RawData.h"
using namespace std;
BIS::BIS() {
	// TODO Auto-generated constructor stub

}

BIS::BIS(sql::ResultSet* res)
{
	cout << "기반 정보 가져오는 중...";
	while(res->next() == true)
	{
		BISDATA* bisdata = new BISDATA(res);
		data.insert(pair<string, BISDATA*>(bisdata->ROUTE_ID + bisdata->TKCAR_STTN_ID + bisdata->APPLY_DATE, bisdata));
	}
	cout << "완료!" << endl;
}

BIS::~BIS() {
	// TODO Auto-generated destructor stub
}

void BIS::apply_date(sql::ResultSet* res)
{
	while(res->next() == true)
		{
			data_apply_date.insert(pair<string, string>(res->getString("노선ID"), res->getString("적용일시")));
		}
}

