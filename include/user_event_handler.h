#ifndef _BRKS_BUS_USERM_HANDLER_H_
#define _BRKS_BUS_USERM_HANDLER_H_

#include <string>
#include <map>
#include <memory>
#include "glo_def.h"
#include "iEventHandler.h"
#include "events_def.h"

class UserEventHandler : public iEventHandler
{
public:
	UserEventHandler();
	virtual ~UserEventHandler();
	virtual iEvent* handle(const iEvent* ev);

private:
	MobileCodeRspEv* handle_mobile_code_req(MobileCodeReqEv* ev);
	LoginRspEv* handle_login_req(LoginReqEv* ev);
	i32 code_gen();

private:
	std::map<std::string, i32> m2c_;  // <mobile, icode>
	pthread_mutex_t pm_;
};

















#endif // !_BRKS_BUS_USERM_HANDLER_H_
