#include <iostream>
#include <unistd.h>
#include "bike.pb.h"
#include "ievent.h"
#include "events_def.h"
#include "user_event_handler.h"
#include "DispatchMsgService.h"
#include "NetworkInterface.h"
#include "ini/iniconfig.h"
#include "log4cpp/Logger.h"
#include "sqlconnection.h"
#include "SqlTables.h"
#include "BusProcessor.h"

using namespace std;

int main(int argc, char** argv)
{
	if (3 != argc)
	{
		LOG_ERROR("Please input shbk < config file path > !\n");
		return -1;
	}

	if (!Logger::instance()->init(std::string(argv[2])))
	{
		fprintf(stderr, "init log module failed.\n");
		return -2;
	}

	// 读取数据库配置
	Iniconfig* config = Iniconfig::getInstance();
	if (!config->loadfile(std::string(argv[1])))
	{
		LOG_ERROR("load %s failed.\n", argv[1]);
		return -3;
	}

	st_env_config conf_args = config->getconfig();
	LOG_INFO("database ip: [%s]  port: [%d]  user: [%s]  pwd: [%s]  db: [%s]   srever port: [%d]", 
		conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(), 
		conf_args.db_pwd.c_str(), conf_args.db_name.c_str(), conf_args.svr_port);

	// 初始化数据库
	std::shared_ptr<MysqlConnection> mysqlConn(new MysqlConnection);
	if (!mysqlConn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),
		conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("Database init failed. exit!\n");
		return -4;
	}

	BusinessProcessor busPro(mysqlConn);
	busPro.init();

	DispatchMsgService* iDms = DispatchMsgService::getInstance();
	iDms->open();

	NetworkInterface* NTIF = new NetworkInterface();
	NTIF->start(conf_args.svr_port);

	while (1 == 1)
	{
		NTIF->network_event_dispatch();
		sleep(1);
		LOG_DEBUG("network_event_dispatch ...");
	}

	/*MobileCodeReqEv* pmcre = new MobileCodeReqEv("123456789");
	iDms->enqueue(pmcre);
	sleep(5);*/

	iDms->close();
	sleep(5);

	return 0;
}