#include <QMouseEvent>
#include <QScrollBar>
#include "QDealNewFriendsApplyWnd.h"
#include "QDealNewFriendsApplyItemWnd.h"
#include "QSimpleSplit.h"
#include "QWSClientMgr.h"
#include "QMainWnd.h"
#include "./json/CJsonObject.hpp"

QDealNewFriendsApplyWnd::QDealNewFriendsApplyWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	m_vLayout = new QVBoxLayout(this);
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	m_vLayout->setSpacing(0);

	m_hLayout = new QHBoxLayout();

	m_minBtn = new QPushButton();
	m_minBtn->setIcon(QPixmap("./img/minWnd.png"));
	m_minBtn->setIconSize(QSize(20, 20));
	m_minBtn->setFixedSize(20, 20);

	m_maxBtn = new QPushButton();
	m_maxBtn->setIcon(QPixmap("./img/maxWnd.png"));
	m_maxBtn->setIconSize(QSize(20, 20));
	m_maxBtn->setFixedSize(20, 20);

	m_closeBtn = new QPushButton();
	m_closeBtn->setIcon(QPixmap("./img/closeWnd.png"));
	m_closeBtn->setIconSize(QSize(20, 20));
	m_closeBtn->setFixedSize(20, 20);

	m_hLayout->addSpacing(20);
	m_hLayout->addStretch();
	m_hLayout->addWidget(m_minBtn);
	m_hLayout->addWidget(m_maxBtn);
	m_hLayout->addWidget(m_closeBtn);
	m_vLayout->addLayout(m_hLayout);


	//m_sLayout = new QStackedLayout();
	m_listWnd1 = new QListWidget();
	m_listWnd2 = new QListWidget();


	m_tabWnd = new QTabWidget();
	m_tabWnd->addTab(m_listWnd1, "好友验证");
	m_tabWnd->addTab(m_listWnd2, "群系统消息");

	m_tabWnd->setStyleSheet("QTabBar::tab{height:40;width:100}");

	m_vLayout->addSpacing(15);
	m_vLayout->addWidget(m_tabWnd);

	setMinimumSize(640, 600);
	setWindowFlags(Qt::FramelessWindowHint);

	m_listWnd1->setAttribute(Qt::WA_StyledBackground);
	m_listWnd2->setAttribute(Qt::WA_StyledBackground);
	setAttribute(Qt::WA_StyledBackground);

	m_listWnd1->setStyleSheet("border:0px;");
	m_listWnd2->setStyleSheet("border:0px;");
}

void QDealNewFriendsApplyWnd::addListItem(const char* headurl,const char* name,const char* msg,int state,int id, bool isApplyer, int userid)
{
	QDealNewFriendsApplyItemWnd* pMsgItem = new QDealNewFriendsApplyItemWnd(m_listWnd1,headurl,name,msg,state,id,isApplyer,userid);
	QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd1);

	pMsgItem->setFixedWidth(this->width() - 5);
	pListItem->setSizeHint(QSize(this->width() - 5, 65));
	m_listWnd1->setItemWidget(pListItem, pMsgItem);
}

void QDealNewFriendsApplyWnd::setFriendApplyList()
{
	//
	neb::CJsonObject json;
	json.Add("ownerid", QMainWnd::getInstance()->m_userid);
	QWSClientMgr::getInstance()->request("cs_msg_get_applyadduserlist", json, [this](neb::CJsonObject& msg)
		{
			qDebug() << msg.ToString().c_str();
			for (int i = 0; i < msg["data"].GetArraySize(); i++)
			{
				neb::CJsonObject tempJsonObj;
				if (! msg["data"].Get(i, tempJsonObj))
				{
					continue;
				}

				//
				int64_t ownerid;
				if (!tempJsonObj.Get("ownerid", ownerid))
				{
					continue;
				}

				int64_t friendid;
				if (!tempJsonObj.Get("friendid", friendid))
				{
					continue;
				}

				if (ownerid == QMainWnd::getInstance()->m_userid)
				{
					std::string username;
					if (!tempJsonObj.Get("friendusername", username))
					{
						continue;
					}

					std::string applymsg;
					if (!tempJsonObj.Get("applymsg",applymsg))
					{
						continue;
					}

					int applystate = -1;
					if (!tempJsonObj.Get("applystate", applystate))
					{
						continue;
					}

					int id = -1;
					if (!tempJsonObj.Get("id", id))
					{
						continue;
					}

					if (hasThisFriendApplyById(id))
					{
						continue;
					}

					addListItem("./img/head2.png", username.c_str(),applymsg.c_str(), applystate,id,true,friendid);
				}
				else
				{
					std::string username;
					if (!tempJsonObj.Get("ownerusername", username))
					{
						continue;
					}

					std::string applymsg;
					if (!tempJsonObj.Get("applymsg", applymsg))
					{
						continue;
					}

					int applystate = -1;
					if (!tempJsonObj.Get("applystate", applystate))
					{
						continue;
					}

					int id = -1;
					if (!tempJsonObj.Get("id", id))
					{
						continue;
					}

					if (hasThisFriendApplyById(id))
					{
						continue;
					}

					addListItem("./img/head2.png", username.c_str(), applymsg.c_str(), applystate,id,false,ownerid);
				}
			}
		});
}

bool QDealNewFriendsApplyWnd::hasThisFriendApplyById(int id)
{
	bool bHas = false;
	for (int i = 0; i < m_listWnd1->count(); i++)
	{
		QListWidgetItem* pitem = m_listWnd1->item(i);
		QDealNewFriendsApplyItemWnd* pApplyItemWnd = dynamic_cast<QDealNewFriendsApplyItemWnd*>(m_listWnd1->itemWidget(pitem));
		if(pApplyItemWnd->m_id == id)
		{
			bHas = true;
			break;
		}
	}

	return bHas;
}
