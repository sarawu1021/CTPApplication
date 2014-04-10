#include "../stdafx.h"
#include "../Header/ThostFtdcMdApi.h"
#include <Windows.h>
#include <fstream>
#pragma comment(lib,"thostmduserapi.lib")

extern char brokerId[];
extern char userId[];
extern char passwd[];
extern char mdFront[];
extern char tradeFront[];
extern HANDLE g_hEvent;
extern char mdfile[];

class CMdSpi : public CThostFtdcMdSpi
{
private:
	CThostFtdcMdApi *m_pUserApi;
public:
	CMdSpi(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi){}
	~CMdSpi(){}

	void CMdSpi::OnFrontConnected()
	{
		printf("Connected to the server!\n");

		CThostFtdcReqUserLoginField reqUserLogin;
		
		strcpy_s(reqUserLogin.BrokerID, brokerId);
		strcpy_s(reqUserLogin.UserID, userId);
		strcpy_s(reqUserLogin.Password, passwd);
		
		m_pUserApi->ReqUserLogin(&reqUserLogin, 0);

		printf("Logining...\n");
	}

	virtual void CMdSpi::OnFrontDisconnected(int nReason)
	{
		printf("OnFrontDisconnected! Error ID: %d\nRetry...\n\n", nReason);
	}

	virtual void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast)
	{
		if (pRspInfo->ErrorID != 0)
		{
			printf("Login Failed.%d,%s\n",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
			system("pause");
			exit(-1);
		}
		
		printf("Successfully login.\n");

		char *Instrument[] = { "IF1409" };
		
		printf("Subscribe %s Market Data...\n", Instrument[0]);

		m_pUserApi->SubscribeMarketData(Instrument, 1);
		//m_pUserApi->UnSubscribeMarketData(Instrument, 1);
	}

	virtual void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast)
	{
		if (pRspInfo->ErrorID != 0)
		{
			printf("Failed at Subscribing %s Market Data!\nErrorID: %s,Error Message: %s\n",
				pSpecificInstrument->InstrumentID, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			exit(-1);
		}
		printf("Subscribe Instrument %s Market Data Successfully!\n", pSpecificInstrument->InstrumentID);
		cerr << "The maket data is writting to md.txt" << endl;
	}

	virtual void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
	{
		printf("%d", nTimeLapse);
	exit(-1);
	}

	virtual void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketDataField)
	{
		ofstream outf(mdfile,ios::app);
		streambuf * buf;
		buf = cout.rdbuf();
		cout.rdbuf(outf.rdbuf());

		cout << pDepthMarketDataField->TradingDay << "\t"
			<< pDepthMarketDataField->UpdateTime << "\t"
			<< pDepthMarketDataField->OpenPrice << "\t"
			<< pDepthMarketDataField->HighestPrice << "\t"
			<< pDepthMarketDataField->LowestPrice << "\t"
			<< pDepthMarketDataField->InstrumentID << "\t"
			<< pDepthMarketDataField->Volume << "\t"
			<< pDepthMarketDataField->AskPrice1 << "\t"
			<< pDepthMarketDataField->BidPrice1 << "\t"
			<< pDepthMarketDataField->AskVolume1 << "\t"
			<< pDepthMarketDataField->BidVolume1 << "\t"
			<< endl;

		cout.rdbuf(buf);
		SetEvent(g_hEvent);
	}
};