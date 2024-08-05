#include "user_event_handler.h"
#include "thread.h"
#include "DispatchMsgService.h"
#include "sqlconnection.h"
#include "iniconfig.h"
#include "user_service.h"

UserEventHandler::UserEventHandler() : iEventHandler("UserEventHandler")
{
	// 未来需要实现订阅事件的处理
	DispatchMsgService::getInstance()->subscribe(EEVENTID_GET_MOBLIE_CODE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_create(&pm_);
}

UserEventHandler::~UserEventHandler()
{
	// 未来需要实现取消订阅事件的处理
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_GET_MOBLIE_CODE_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_destroy(&pm_);
}

iEvent* UserEventHandler::handle(const iEvent* ev)
{
	if (NULL == ev)
	{
		// LOG_ERROR("input ev is NULL");
		LOG_ERROR("input ev is NULL");
	}

	u32 eid = ev->get_eid();

	if (EEVENTID_GET_MOBLIE_CODE_REQ == eid)
	{
		return handle_mobile_code_req((MobileCodeReqEv*)ev);
	}
	else if (EEVENTID_LOGIN_REQ == eid)
	{
		return handle_login_req((LoginReqEv*)ev);
	}
	else if (EEVENTID_RECHARGE_REQ == eid)
	{
		// return handle_recharge_req((RechargeEv*)ev)
	}
	else if (EEVENTID_GET_ACCOUNT_BALANCE_REQ == eid)
	{
		// return handle_get_account_balance_req((GetAccountBalanceEv*)ev);
	}
	else if (EEVENTID_LIST_ACCOUNT_RECORDS_REQ == eid)
	{
		// return handle_list_account_records_req((ListAccountRecordsReqEv*)ev);
	}

	return NULL;
}

MobileCodeRspEv* UserEventHandler::handle_mobile_code_req(MobileCodeReqEv* ev)
{
	i32 icode = 0;
	std::string mobile_ = ev->get_mobile();
	LOG_DEBUG("try to get mobile phone %s validate code.\n", mobile_.c_str());

	icode = code_gen();
	thread_mutex_lock(&pm_);
	m2c_[mobile_] = icode;
	thread_mutex_unlock(&pm_);
	LOG_DEBUG("mobile: %s, code: %d\n", mobile_.c_str(), icode);

	return new MobileCodeRspEv(ERRC_SUCCESS, icode);
}

LoginRspEv* UserEventHandler::handle_login_req(LoginReqEv* ev)
{
	std::string mobile = ev->get_mobile();
	i32 code = ev->get_icode();

	LOG_DEBUG("try to handle login ev, mobile = %s, code = %d \n", mobile.c_str(), code);

	thread_mutex_lock(&pm_);
	auto iter = m2c_.find(mobile);
	if ((iter != m2c_.end()) && (code != iter->second) ||
		(iter == m2c_.end()))
	{
		thread_mutex_unlock(&pm_);
		LOG_ERROR("UserEventHandler::handle_login_req - ERRC_INVALID_DATA");
		return new LoginRspEv(ERRC_INVALID_DATA);
	}

	thread_mutex_unlock(&pm_);

	// 如果验证成功 则要判断用户在数据库是否存在  如果不存在则插入用户记录
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);

	st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	if (!mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.svr_port, conf_args.db_user.c_str(),
		conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed!");
		return new LoginRspEv(ERRC_PROCCESS_FAILED);
	}

	UserService us(mysqlconn);
	bool result = false;

	if (!us.exist(mobile))
	{
		result = us.insert(mobile);
		if (!result)
		{
			LOG_ERROR("insert user(%s) to db failed.", mobile.c_str());
			return new LoginRspEv(ERRC_PROCCESS_FAILED);
		}
	}

	return new LoginRspEv(ERRC_SUCCESS);
}

i32 UserEventHandler::code_gen()
{
	i32 code = 0;
	srand((unsigned int)time(NULL));

	code = (unsigned int)(rand() % (999999 - 100000) + 100000);

	return code;
}