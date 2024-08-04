#ifndef _DATASTORE_MYSQL_CONNECTION_H_
#define _DATASTORE_MYSQL_CONNECTION_H_

#include <mysql/mysql.h>
#include <string>
#include <mysql/errmsg.h>
#include <assert.h>
#include "glo_def.h"

// �����
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

	explicit SqlRecordSet(MYSQL_RES* pRes)   // ������ʽ��ֵ
	{
		m_pRes = pRes;
	}

	inline void SetResult(MYSQL_RES* pRes)
	{
		assert(m_pRes == NULL);  // �����ʱ�Ѿ������˽������  ��ôӦ���ó��򱨴� ��ֹ�ڴ�й¶
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

	int EscapeString(const char* pSrc, int nSrcLen, char* pDest);  // �����ַ�ת�� ʹ���ʺϲ�ѯ

	void Close();

	const char* GetErrInfo();  // ��ô�����Ϣ

	void Reconnect();  // ��������

private:
	MYSQL* mysql_;
};


#endif // !_DATASTORE_MYSQL_CONNECTION_H_
