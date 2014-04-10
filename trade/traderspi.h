#include "../stdafx.h"
#include "../Header/ThostFtdcTraderApi.h"
#pragma comment(lib,"thosttraderapi.lib")

class CTraderSpi : public CThostFtdcTraderSpi
{
private:
	CThostFtdcTraderApi *m_pTraderApi;
public:
	CTraderSpi(CThostFtdcTraderApi *pTraderApi) :m_pTraderApi(pTraderApi){}
	~CTraderSpi(){}


	virtual void CTraderSpi::OnFrontConnected();

	virtual void CTraderSpi::OnFrontDisconnected(int nReason);


	virtual void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	virtual void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/*virtual void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);*/

	//virtual void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	virtual void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	virtual void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, 
		CThostFtdcRspInfoField *pRspInfo, 
		int nRequestID, 
		bool bIsLast);

public:
	void ReqQryOrder();
	void ReqQryTrade();
	
};