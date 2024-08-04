#ifndef _DATASTORE_MYSQL_CONNECTION_H_
#define _DATASTORE_MYSQL_CONNECTION_H_

#include <mysql/mysql.h>
#include <string>
#include <mysql/errmsg.h>
#include <assert.h>
#include "glo_def.h"

// 结果集
class SqlRecordSet
{
public:
	SqlRecordSet() : m_pRes(NULL)
	{

	}

	~SqlRecordSet()
	{
		if (m_pRes)
		{
			mysql_free_result(m_pRes);
		}
	}

	explicit SqlRecordSet(MYSQL_RES* pRes)   // 不能隐式赋值
	{
		m_pRes = pRes;
	}

	inline void SetResult(MYSQL_RES* pRes)
	{
		assert(m_pRes == NULL);  // 如果此时已经保存了结果集了  那么应该让程序报错 防止内存泄露
		if (m_pRes)
		{
			LOG_WARN("the MYSQL_RES has already stored result, maybe will cause memory leak.");
		}
		m_pRes = pRes;
	}

	inline MYSQL_RES* GetResult()
	{
		return m_pRes;
	}

	void FetchRow(MYSQL_ROW& row)
	{
		row = mysql_fetch_row(m_pRes);
	}

	inline i32 GetRowcount()
	{
		return m_pRes->row_count;
	}

private:
	MYSQL_RES* m_pRes;
};

class MysqlConnection
{
public:
	MysqlConnection();
	~MysqlConnection();

	MYSQL* Mysql()
	{
		return mysql_;
	}

	bool Init(const char* szHost, int nPort, const char* szUser, const char* szPasswd, const char* szDb);

	bool Execute(const char* szSql);
	bool Execute(const char* szSql, SqlRecordSet& recordSet); //  MYSQL_RES *

	int EscapeString(const char* pSrc, int nSrcLen, char* pDest);  // 特殊字符转义 使其适合查询

	void Close();

	const char* GetErrInfo();  // 获得错误信息

	void Reconnect();  // 出错重连

private:
	MYSQL* mysql_;
};


#endif // !_DATASTORE_MYSQL_CONNECTION_H_
