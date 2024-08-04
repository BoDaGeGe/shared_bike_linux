#ifndef _BRKS_BUS_MAIN_H_
#define _BRKS_BUS_MAIN_H_

#include <memory>
#include "user_event_handler.h"
#include "sqlconnection.h"

class BusinessProcessor
{
public:
	BusinessProcessor(std::shared_ptr<MysqlConnection> conn);
	bool init();

	~BusinessProcessor();

private:
	std::shared_ptr<MysqlConnection>   mysqlconn_;
	std::shared_ptr<UserEventHandler>  ueh_;         // 账户管理系统
};









#endif // !_BRKS_BUS_MAIN_H_
