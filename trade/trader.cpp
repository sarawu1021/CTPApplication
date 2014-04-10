#include "traderspi.h"
#include <Windows.h>
#include <fstream>

extern char brokerId[];
extern char userId[];
extern char passwd[];
extern char mdFront[];
extern char tradeFront[];
extern char tradefile[];
extern char orderfile[];
extern int requestId;
extern HANDLE g_hEvent;
CThostFtdcReqUserLoginField reqUserLogin;
	void CTraderSpi::OnFrontConnected()
	{
		cerr << "Connected to the Server!" << endl;
		
		strcpy_s(reqUserLogin.BrokerID, brokerId);
		strcpy_s(reqUserLogin.Password, passwd);
		strcpy_s(reqUserLogin.UserID, userId);
		
		m_pTraderApi->ReqUserLogin(&reqUserLogin, requestId++);
	}


	void CTraderSpi::OnFrontDisconnected(int nReason)
	{
		cerr << "Failed connected to the Server! Retry..." << endl;
		cerr << "Error: " << nReason << endl;
	}
	

	void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast)
	{
		if (!pRspInfo->ErrorID)
		{
			cerr << "Successfully Login!" << endl;

			CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm;
			strcpy_s(pSettlementInfoConfirm.BrokerID, brokerId);
			strcpy_s(pSettlementInfoConfirm.InvestorID, userId);
			m_pTraderApi->ReqSettlementInfoConfirm(&pSettlementInfoConfirm, requestId++);
			
		}
		else
		{
			cerr << "Failed Login! ERROR ID: " << pRspInfo->ErrorID << endl;
			cerr << "error msg: " << pRspInfo->ErrorMsg << endl;
			system("pause");
			exit(-1);
		}
	}

	void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cerr << "ERROR ID: " << pRspInfo->ErrorID;
		cerr << "nRequestID: " << nRequestID;
	}

	void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast)
	{
		if (!pRspInfo->ErrorID && pSettlementInfoConfirm)
		{
			cerr << "Settlement Confirmed!" << endl;
			cerr << "Confirm Date: " << pSettlementInfoConfirm->ConfirmDate;
			cerr << "Confirm Time: " << pSettlementInfoConfirm->ConfirmTime;
			cerr << "Confirm Broker ID: " << pSettlementInfoConfirm->BrokerID;
			cerr << "Confirm Invester ID: " << pSettlementInfoConfirm->InvestorID;
		}
		else
		{
			cerr << "ERROR ID:" << pRspInfo->ErrorID;
			exit(-1);
		}
		if (bIsLast)
		{
			SetEvent(g_hEvent);
		}
	}
	
	void CTraderSpi::ReqQryOrder()
	{
		CThostFtdcQryOrderField pQryOrder;
		strcpy_s(pQryOrder.BrokerID, brokerId);
		strcpy_s(pQryOrder.InvestorID,userId);
		m_pTraderApi->ReqQryOrder(&pQryOrder, requestId++);
	}

	void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast)
	{
		if (!pRspInfo->ErrorID && pOrder)
		{
			ofstream outf(orderfile,ios::app);
			streambuf * buf;
			buf = cout.rdbuf();
			cout.rdbuf(outf.rdbuf());
			
			cout << pOrder->Direction << "\t"
				<< pOrder->InsertDate << "\t"
				<< pOrder->InsertTime << "\t"
				<< pOrder->InstrumentID << "\t"
				<< pOrder->LimitPrice << "\t"
				<< pOrder->OrderPriceType << "\t"
				<< pOrder->OrderType << "\t"
				<< pOrder->OrderStatus << "\t"
				<< pOrder->VolumeTraded << "\t"
				<< endl;
			cout.rdbuf(buf);
		}
		else
		{
			cerr << "ERROR ID: " << pRspInfo->ErrorID << endl;
			exit(-1);
		}
		if (bIsLast)
		{
			SetEvent(g_hEvent);
		}
	}

	void CTraderSpi::ReqQryTrade()
	{
		CThostFtdcQryTradeField pQryTrade;
		strcpy_s(pQryTrade.BrokerID, brokerId);
		strcpy_s(pQryTrade.InvestorID, userId);
		m_pTraderApi->ReqQryTrade(&pQryTrade, requestId++);
	}

	void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast)
	{
		if (!pRspInfo->ErrorID && pTrade)
		{
			ofstream outf(tradefile,ios::app);
			streambuf * buf;
			buf = cout.rdbuf();
			cout.rdbuf(outf.rdbuf());
			
			cout << pTrade->BrokerID << "\t"
				<< pTrade->InstrumentID << "\t"
				<< pTrade->Direction << "\t"
				<< pTrade->TradeID << "\t"
				<< pTrade->Price << "\t"
				<< pTrade->PriceSource << "\t"
				<< pTrade->TradeDate << "\t"
				<< pTrade->TradeTime << "\t"
				<< pTrade->UserID << "\t"
				<< pTrade->Volume << endl;
			cout.rdbuf(buf);
		}
		else
		{
			cerr << "ERROR ID: " << endl;
			exit(-1);
		}

		if (bIsLast)
		{
			SetEvent(g_hEvent);
		}
	}