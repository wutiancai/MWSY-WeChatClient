#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

/*
* 提供简单的基于websocket的通信能力
*/

#include <QWebSocket>

#include <functional>
#include <QTimer>
#include "json/CJsonObject.hpp"

#define msg_type_req 1
#define msg_type_resp 2
#define msg_type_push 3
#define msg_type_broadcast 4
#define msg_type_push_s 5

class QWSClientMgr : public QObject
{
	Q_OBJECT
private:
	QWSClientMgr();
public:

	static QWSClientMgr* getInstance();

	void sendMsg(const QString& message);

	typedef std::function<void(neb::CJsonObject&)> NetEventCall;
	struct NetEventCallData {
		NetEventCall callFun = nullptr;
		time_t dt = 0;
	};

	typedef std::map<QString, NetEventCall> Msg2CallbackMap;
	typedef std::map<int, NetEventCallData> RequestMsg2CallbackMap;

	void regMsgCall(QString cmd,NetEventCall netEventCall);

	void transfer(neb::CJsonObject& msg);

	void request(const std::string& cmd, neb::CJsonObject& json);
	void request(const std::string& cmd, neb::CJsonObject& json, NetEventCall call);

	void onNetMsgDo(int64 msgId, neb::CJsonObject& msgJson);
	void onNetMsgDo(std::string cmd, neb::CJsonObject& msgJson);
	
public slots:
	void slot_connected();
	void slot_disconnected();
	void slot_recvMsg(const QString& message);
	void slot_timer();
public:
	static QWSClientMgr *m_WsClientMgr;

	QWebSocket *m_webSock;
	//接收远端消息
	Msg2CallbackMap m_Msg2CallbackMap;
	//请求后回调此函数
	RequestMsg2CallbackMap m_RequestMsg2CallbackMap;
	int64 m_MsgId = 0;
	QTimer* m_timer;
	int m_time = 0;
};
