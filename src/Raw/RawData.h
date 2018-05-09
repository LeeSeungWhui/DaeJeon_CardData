/*
 * RAWDATA.h
 *
 *  Created on: 2018. 3. 13.
 *      Author: lsh
 */

#ifndef SRC_RAW_RAWDATA_H_
#define SRC_RAW_RAWDATA_H_
#include <string>
#include <vector>
#include <ctime>
#include "../DbConnection/DbConnection.h"

using namespace std;

class RAWDATA {
public:
	RAWDATA();
	RAWDATA(sql::ResultSet* res);
	virtual ~RAWDATA();

	//function
	static void setDate(tm* tm, string date);
	static string dateTostr(tm tm); // sort함수를 사용하여 카드번호순으로 정렬
	void print(ostream* fout); // 출력함수
	void print_normalize(ostream* fout, int seq); // 표준화 출력함수

	// 칼럼
	string VIRTL_CARD_NO;					// 가상카드번호
	string EXCCLC_AREA_CD;					// 정산지역코드
	tm TKCAR_DT;								// 승차일시
	tm GFF_DT_BF;							// 승차전하차일시
	tm GFF_DT;								// 하차일시
	string TRNSIT_CNTC;						// 환승횟수
	string TFCMN_CD;						// 교통수단코드
	string TFCMN_CD_BF;						// 승차전교통수단
	string TFCBIZER_ID;						// 교통사업자ID
	string TFCBIZER_NAME;					// 교통사업자명
	string ROUTE_ID;						// 노선ID
	string ROUTE_NAME;						// 노선명
	string CAR_ID;							// 차량ID
	string CAR_REG_NO;						// 차량등록번호
	tm OPRAT_STG_DT;						// 운행출발일시
	tm OPRAT_END_DT;						// 운행종료일시
	string TKCAR_STTN_ID;					// 승차정류장ID
	string TKCAR_STTN_NAME;				// 승차정류장이름
	string TKCAR_STTN_AREA;				// 승차정류장 행정동
	string GFF_STTN_ID;						// 하차정류장ID
	string GFF_STTN_NAME;					// 하차정류장이름
	string GFF_STTN_AREA;					// 하차정류장행정동
	string USER_GB_CARD;					// 카드권종사용자구분
	string USER_GB_MONEY;					// 금액권종사용자구분
	string NTSS_NAME;						// 발행사
	string USEA_PRSN_CNTC;					// 이용객수
	string TKCAR_AMT;						// 승차금액
	string GFF_AMT;							// 하차금액
	string TRNSIT_AMT;						//	환승할인금액
	string USEA_DIST;						// 통행거리
	string BRDNG_TIME;						// 통행시간
	string TRNSC_ID;						// 트랜젝션ID
	string VIOLATION_AMT;					// 승하차위반금액
	vector<int> STTN_SEQ;					// 승차정류장순서
	int CAR_OPRAT_CNT;						// 차량별운행차수
};

#endif /* SRC_RAW_RAWDATA_H_ */
