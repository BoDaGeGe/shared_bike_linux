#include <iostream>
#include <sstream>
#include "events_def.h"

std::ostream& MobileCodeReqEv::dump(std::ostream& out) const
{ 
	out << "MobileCodeReq sn = " << get_sn() << " ,  ";
	out << "mobile = " << msg_.mobile() << std::endl;

	return out; 
}

std::ostream& MobileCodeRspEv::dump(std::ostream& out)const
{
	out << "MobileCodeRspEv sn = " << get_sn() << " ,  ";
	out << "code = " << msg_.code() << " ,  ";
	out << "icode = " << msg_.icode() << " ,  ";
	out << "describle = " << msg_.data() << std::endl;

	return out;
}

std::ostream& LoginReqEv::dump(std::ostream& out)const
{
	out << "LoginReqEv sn = " << get_sn() << " ,  ";
	out << "mobile = " << msg_.mobile() << " ,  ";
	out << "icode = " << msg_.icode() << std::endl;

	return out;
}

std::ostream& LoginRspEv::dump(std::ostream& out)const
{
	out << "LoginRspEv sn = " << get_sn() << " ,  ";
	out << "code = " << msg_.code() << " ,  ";
	out << "describle = " << msg_.desc() << std::endl;

	return out;
}