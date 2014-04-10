#include "config.h"
#include "stdafx.h"
#include "trade\traderspi.h"
#include "md\md.cpp"
#include <Windows.h>

HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);

void getMd()
{
	CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcMdSpi *pUserSpi = new CMdSpi(pUserApi);

	pUserApi->RegisterSpi(pUserSpi);

	pUserApi->RegisterFront(mdFront);
	pUserApi->RegisterFront(mdFront);
	pUserApi->RegisterFront(mdFront);
	
	pUserApi->Init();
	
	pUserApi->Join();
	
	pUserApi->Release();
}

void testTrade()
{
	CThostFtdcTraderApi *pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CTraderSpi *pTraderSpi = new CTraderSpi(pTraderApi);

	pTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderSpi);

	
	pTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);
	
	pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	
	pTraderApi->RegisterFront(tradeFront);
	pTraderApi->RegisterFront(tradeFront);
	pTraderApi->RegisterFront(tradeFront);
	
	pTraderApi->Init();
	
	pTraderApi->Join();
	
	pTraderApi->Release();
	system("pause");
}
int main(int argc, const char * argv[])
{
	getMd();
	//testTrade();
	system("pause");
	return 0;
}