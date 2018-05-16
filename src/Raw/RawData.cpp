/*
 * RAWDATA.cpp
 *
 *  Created on: 2018. 3. 13.
 *      Author: lsh
 */

#include "RawData.h"

RAWDATA::RAWDATA() {
	// TODO Auto-generated constructor stub
	CAR_OPRAT_CNT = 0;
}
RAWDATA::RAWDATA(sql::ResultSet* res)
{
	VIRTL_CARD_NO = res->getString("카드번호(암호화)");
	EXCCLC_AREA_CD = res->getString("정산지역코드");
	setDate(&TKCAR_DT, res->getString("승차일시"));
	setDate(&GFF_DT_BF, res->getString("승차전하차일시"));
	setDate(&GFF_DT, res->getString("하차일시"));
	TRNSIT_CNTC = res->getString("환승횟수");
	TFCMN_CD = res->getString("교통수단");
	TFCMN_CD_BF = res->getString("승차전교통수단");
	TFCBIZER_ID = res->getString("교통사업자ID");
	TFCBIZER_NAME = res->getString("교통사업자");
	CAR_ID = res->getString("차량ID");
	CAR_REG_NO = res->getString("차량등록번호");
	setDate(&OPRAT_STG_DT, res->getString("운행출발일시"));
	setDate(&OPRAT_END_DT, res->getString("운행종료일시"));
	TKCAR_STTN_ID = res->getString("승차정류장ID");
	TKCAR_STTN_NAME = res->getString("승차정류장명");
	TKCAR_STTN_AREA = res->getString("승차정류장행정동");
	GFF_STTN_ID = res->getString("하차정류장ID");
	GFF_STTN_NAME = res->getString("하차정류장명");
	GFF_STTN_AREA = res->getString("하차정류장행정동");
	USER_GB_CARD = res->getString("카드권종교통사용자구분");
	USER_GB_MONEY = res->getString("금액권종교통사용자구분");
	NTSS_NAME = res->getString("발행사명");
	USEA_PRSN_CNTC = res->getString("승객수");
	TKCAR_AMT = res->getString("승차금액");
	GFF_AMT = res->getString("하차금액");
	TRNSIT_AMT = res->getString("환승할인금액");
	USEA_DIST = res->getString("통행거리");
	BRDNG_TIME = res->getString("통행시간");
	if(BRDNG_TIME == "") BRDNG_TIME = "~";
	TRNSC_ID = res->getString("트랜젝션ID");
	VIOLATION_AMT = res->getString("승하차위반금액");
	CAR_OPRAT_CNT = 1;
	if (TFCBIZER_NAME == "대전지하철")
	{
		ROUTE_NAME = "대전도시철도 1호선";
		ROUTE_ID = "S3001";
	}
	else
	{
		ROUTE_NAME = res->getString("노선명");
		ROUTE_ID = res->getString("노선ID");
	}
}
RAWDATA::~RAWDATA() {
	// TODO Auto-generated destructor stub
}

void RAWDATA::setDate(tm* tm, string date)
{
	if(date != "~" && date != "\"~\"")
	{
		tm->tm_year = atoi(date.substr(0,4).c_str()) - 1900;
		tm->tm_mon = atoi(date.substr(4,2).c_str()) - 1;
		tm->tm_mday = atoi(date.substr(6,2).c_str());
		tm->tm_hour = atoi(date.substr(8,2).c_str());
		tm->tm_min = atoi(date.substr(10,2).c_str());
		tm->tm_sec = atoi(date.substr(12,2).c_str());
		mktime(tm);

	}
	else
	{
		tm->tm_year = 0;
	}
}

string RAWDATA::dateTostr(tm tm)
{
	string output = to_string(tm.tm_year + 1900);
	if(tm.tm_year != 0)
	{
		if(tm.tm_mon < 9)
		{
			output += "0";
		}
		output += to_string(tm.tm_mon + 1);
		if(tm.tm_mday < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_mday);
		if(tm.tm_hour < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_hour);
		if(tm.tm_min < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_min);
		if(tm.tm_sec < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_sec);
	}
	if(output == "1900")
	{
		output = "~";
	}
	return output;
}

void RAWDATA::print(ostream* fout)
{
	if(STTN_SEQ.empty() == false)
	{
		*fout <<
			"\"" << VIRTL_CARD_NO << "\"," <<// 가상카드번호
			"\"" << EXCCLC_AREA_CD << "\"," <<// 정산지역코드
			"\"" << dateTostr(TKCAR_DT) << "\"," <<// 승차일시
			"\"" << dateTostr(GFF_DT_BF) << "\"," <<// 승차전하차일시
			"\"" << dateTostr(GFF_DT) << "\"," <<// 하차일시
			"\"" << TRNSIT_CNTC << "\"," <<// 환승횟수
			"\"" << TFCMN_CD << "\"," <<// 교통수단코드
			"\"" << TFCMN_CD_BF << "\"," <<// 승차전교통수단
			"\"" << TFCBIZER_ID << "\"," <<// 교통사업자ID
			"\"" << TFCBIZER_NAME << "\"," <<// 교통사업자명
			"\"" << ROUTE_ID << "\"," <<// 노선ID
			"\"" << ROUTE_NAME << "\"," <<// 노선명
			"\"" << CAR_ID << "\"," <<// 차량ID
			"\"" << CAR_REG_NO << "\"," <<// 차량등록번호
			"\"" << dateTostr(OPRAT_STG_DT) << "\"," <<// 운행출발일시
			"\"" << dateTostr(OPRAT_END_DT) << "\"," <<// 운행종료일시
			"\"" << TKCAR_STTN_ID << "\"," <<// 승차정류장ID
			"\"" << TKCAR_STTN_NAME << "\"," <<// 승차정류장이름
			"\"" << TKCAR_STTN_AREA << "\"," <<// 승차정류장 행정동
			"\"" << GFF_STTN_ID << "\"," <<// 하차정류장ID
			"\"" << GFF_STTN_NAME << "\"," <<// 하차정류장이름
			"\"" << GFF_STTN_AREA << "\"," <<// 하차정류장행정동
			"\"" << USER_GB_CARD << "\"," <<// 카드권종사용자구분
			"\"" << USER_GB_MONEY << "\"," <<// 금액권종사용자구분
			"\"" << NTSS_NAME << "\"," <<// 발행사
			"\"" << USEA_PRSN_CNTC << "\"," <<// 이용객수
			"\"" << TKCAR_AMT << "\"," <<// 승차금액
			"\"" << GFF_AMT << "\"," <<// 하차금액
			"\"" << TRNSIT_AMT << "\"," <<//환승할인금액
			"\"" << USEA_DIST << "\"," <<// 통행거리
			"\"" << BRDNG_TIME << "\"," <<// 통행시간
			"\"" << TRNSC_ID << "\"," <<// 트랜젝션ID
			"\"" << VIOLATION_AMT << "\",";// 승하차위반금액
		*fout << "\"";
		for(auto n : STTN_SEQ)
		{
			*fout << n << " " ;
		}
		*fout << "\",";
		*fout << "\"" <<  CAR_OPRAT_CNT << "\",";//차량별운행차수
		string wday;
		if(TKCAR_DT.tm_hour < 4)
		{
			TKCAR_DT.tm_wday--;
		}
		switch(TKCAR_DT.tm_wday)
		{
		case 0:
			wday = "일";
			break;
		case 1:
			wday = "월";
			break;
		case 2:
			wday = "화";
			break;
		case 3:
			wday = "수";
			break;
		case 4:
			wday = "목";
			break;
		case 5:
			wday = "금";
			break;
		case 6:
			wday = "토";
			break;
		default:
			wday = "토";
			break;
		}
		*fout <<	"\"" << wday << "\"" << endl;
	}
}

void RAWDATA::print_normalize(ostream* fout, int seq)
{
	*fout <<
		"\"" << seq << "\"," <<// 일련번호
		"\"" << VIRTL_CARD_NO << "\"," <<// 가상카드번호
		"\"" << EXCCLC_AREA_CD << "\"," <<// 정산지역코드
		"\"" << "~" << "\"," <<// 카드구분코드
		"\"" << CAR_ID << "\"," <<// 차량ID
		"\"" << CAR_REG_NO << "\"," <<// 차량등록번호
		"\"" << dateTostr(OPRAT_STG_DT) << "\"," <<// 운행출발일시
		"\"" << dateTostr(OPRAT_END_DT) << "\"," <<// 운행종료일시
		"\"" << TFCMN_CD << "\"," <<// 교통수단코드
		"\"" << ROUTE_ID << "\"," <<// 노선ID
		"\"" << TFCBIZER_ID << "\"," <<// 교통사업자ID
		"\"" << dateTostr(TKCAR_DT) << "\"," <<// 승차일시
		"\"" << "~" << "\"," <<// 발권시간
		"\"" << TKCAR_STTN_ID << "\"," <<// 승차정류장ID
		"\"" << dateTostr(GFF_DT) << "\"," <<// 하차일시
		"\"" << GFF_STTN_ID << "\"," <<// 하차정류장ID
		"\"" << TRNSC_ID << "\"," <<// 트랜젝션ID
		"\"" << TRNSIT_CNTC << "\"," <<// 환승횟수
		"\"" << USER_GB_CARD << "\"," <<// 카드권종사용자구분
		"\"" << USEA_PRSN_CNTC << "\"," <<// 이용객수
		"\"" << TKCAR_AMT << "\"," <<// 승차금액
		"\"" << GFF_AMT << "\"," <<// 하차금액
		"\"" << USEA_DIST << "\"," <<// 통행거리
		"\"" << BRDNG_TIME << "\"" << endl; // 통행시간
}
