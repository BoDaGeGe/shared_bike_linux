#include "eventtype.h"

static EErrorReason EER[] =
{
	{ERRC_SUCCESS, "OK."},
	{ERRC_INVALID_MSG, "Invalid message."},
	{ERRC_INVALID_DATA, "Invalid data."},
	{ERRC_METHOD_NOT_ALLOWED, "Method not allowed."},
	{ERRC_PROCCESS_FAILED, "Process failed."},
	{ERRC_BIKE_IS_TOOK, "Bike is took."},
	{ERRC_BIKE_IS_RUNNING, "Bike is running."},
	{ERRC_BIKE_IS_DAMAGED, "Bike is damaged."},
	{ERRC_NULL, "Undefined."}
};

const char* getReasonByErrorCode(i32 code)
{
	i32 i = 0;
	for (i = 0; ERRC_NULL != EER[i].code; i++)
	{
		if (EER[i].code == code)
		{
			return EER[i].reason;
		}
	}

	return EER[i].reason; // "Undefined."
}