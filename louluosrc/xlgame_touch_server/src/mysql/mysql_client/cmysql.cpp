#include "cmysql.h"
#include "SDUtility.h"
#include <sstream>
#include <string.h>


uint32_t CMysql::m_query_num = 0;
uint32_t CMysql::m_read_num = 0;
uint32_t CMysql::m_write_num = 0;
std::map<std::string,uint32_t> CMysql::m_db_read;
std::map<std::string,uint32_t> CMysql::m_db_write;

CMysql:: CMysql()
{
        m_bConnected = false;
        m_iField = 0;
        m_result = NULL;
        bzero(m_szHost, sizeof(m_szHost));
        m_szPass = NULL;
        bzero(m_szUser, sizeof(m_szUser));
        bzero(m_szDataBase, sizeof(m_szDataBase));
        m_szSocket = NULL;
        m_iPort = 0;
        m_iClientFlag = 0;
        m_bFieldIndexInitialized = false;
        m_bIfConfig=false;

        m_isuse = false;

        m_ErrMsg = new char[1024];
}


CMysql:: CMysql(const char* szHost,
                const char* szUser,
                const char* szPass,
                const char* szDataBase,
                unsigned int iPort /* =0 */,
                const char* szSocket /* = NULL */,
                unsigned long iClientFlag /* = 0 */)
{
        m_bConnected = false;
        m_iField = 0;
        m_result = NULL;
        m_bFieldIndexInitialized = false;
        m_bIfConfig=false;
        strncpy(m_szHost, szHost, 64);
        m_szHost[64] = 0;
        strncpy(m_szUser, szUser, 64);
        m_szUser[64] = 0;
        if(szPass != NULL)
        {
                m_szPass = new char[64];
                strncpy(m_szPass, szPass, 64);
                m_szPass[64] = 0;
        }
        else
        {
                m_szPass = NULL;
        }
        strncpy(m_szDataBase, szDataBase, 64);
        m_szDataBase[64] = 0;
        if(szSocket != NULL)
        {
                m_szSocket = new char[1024];
                strncpy(m_szSocket, szSocket, 1023);
                m_szSocket[1023] = 0;
        }
        else
        {
                m_szSocket = NULL;
        }
        m_iPort = iPort;
        m_iClientFlag = iClientFlag;
        m_ErrMsg = new char[1024];

        m_isuse = false;

}

CMysql:: ~CMysql()
{
        if(m_szPass != NULL)
        {
                delete[] m_szPass;
        }
        if(m_szSocket != NULL)
        {
                delete[] m_szSocket;
        }
        delete[] m_ErrMsg;
        Close();
}

int CMysql:: Close()
{
        if(m_bConnected)
        {
                FreeResult();
                mysql_close(&m_connection);
                m_bConnected= false;
        }
        return 0;
}

int CMysql:: EscapeString(string &str)
{
        if(str.size()==0) return 0;

        char *buff= new char[str.size()*2+1];
        mysql_escape_string(buff, str.c_str(), str.size());
        str= buff;
        delete[] buff;
        return 0;
}

void CMysql::Escape(string & str_out, char * buf_in, int buf_in_len)
{
        str_out = "";
        if(buf_in_len <= 0)
        {
                return;
        }

        char *buff= new char[buf_in_len*2+1];
        mysql_escape_string(buff, buf_in, buf_in_len);
        str_out = buff;
        delete[] buff;

        return;
}

int CMysql:: Connect(const char* szHost,
                     const char* szUser,
                     const char* szPass,
                     const char* szDataBase,
                     unsigned int iPort /* =0 */,
                     const char* szSocket /* = NULL */,
                     unsigned long iClientFlag /* = 0 */)
{
        strncpy(m_szHost, szHost, 64);
        m_szHost[64] = 0;
        strncpy(m_szUser, szUser, 64);
        m_szUser[64] = 0;
        if(szPass != NULL)
        {
                if(m_szPass == NULL)
                {
                        m_szPass = new char[65];
                }
                strncpy(m_szPass, szPass, 64);
                m_szPass[64] = 0;
        }
        else
        {
                if(m_szPass != NULL)
                {
                        delete[] m_szPass;
                        m_szPass = NULL;
                }
        }
        strncpy(m_szDataBase, szDataBase, 64);
        m_szDataBase[64] = 0;
        if(szSocket != NULL)
        {
                if(m_szSocket == NULL)
                {
                        m_szSocket = new char[65];
                }
                strncpy(m_szSocket, szSocket, 64);
                m_szSocket[64] = 0;
        }
        else
        {
                if(m_szSocket != NULL)
                {
                        delete[] m_szSocket;
                        m_szSocket = NULL;
                }
        }

        m_iPort = iPort;
        m_iClientFlag = iClientFlag;
        Close();
        return Connect();
}

int CMysql:: Connect()
{
        if(!m_bConnected)
        {
                mysql_init(&m_connection);
                if(mysql_real_connect(&m_connection, m_szHost, m_szUser, m_szPass, m_szDataBase, m_iPort, m_szSocket, m_iClientFlag) == NULL)
                {
                        sprintf(m_ErrMsg, "connect[-h%s -u%s -p%s %s] fail.\nError %u (%s)\n",
                                m_szHost, m_szUser, m_szPass, m_szDataBase,
                                mysql_errno(&m_connection), mysql_error(&m_connection));
                        return -1; //throw CMysqlException(m_ErrMsg);
                }
                //printf("Connect SET NAMES \n");
                mysql_real_query(&m_connection,"SET NAMES 'UTF8'",strlen("SET NAMES 'UTF8'"));
                mysql_query(&m_connection,"SET CHARACTER SET UTF8");
                mysql_query(&m_connection,"SET CHARACTER_SET_RESULTS=UTF8");

                m_bConnected = true;
        }
        return 0;
}

bool CMysql:: IfConnected(const char* szHost)
{
        if(m_bConnected)
                if(!strcmp(szHost, m_szHost))
                        return true;
        return false;
}

int CMysql:: Query(char* szSqlString)
{
        Connect();
        if(m_bConnected == true)
        {
                //printf("111 SET NAMES %s\n", szSqlString);
                int result = mysql_real_query(&m_connection,"SET NAMES 'UTF8'",strlen("SET NAMES 'UTF8'"));
                if(result != 0)
                {
                        //printf("1111  SET NAMES 'UTF8 failed result =%d mysql=%s\n", result, szSqlString);
                        Close();
                        Connect();
                }
                else
                {
                        //printf("1111  SET NAMES 'UTF8 success sql=%s\n", szSqlString);
                }
        }
        if(mysql_real_query(&m_connection, szSqlString, strlen(szSqlString)) != 0)
        {
                if(mysql_errno(&m_connection) == 2013)
                {
                        //printf("mysql_errno(&m_connection) == 2013 sql=%s\n", szSqlString);
                        Close();
                        Connect();
                        if(m_bConnected == true)
                        {
                                //printf("22222 SET NAMES %s\n",szSqlString);
                                mysql_real_query(&m_connection,"SET NAMES 'UTF8'",strlen("SET NAMES 'UTF8'"));
                        }
                        if(mysql_real_query(&m_connection, szSqlString, strlen(szSqlString)) != 0)
                        {
                                char *pdata=new char[strlen(szSqlString)+1];
                                memset(pdata, 0, strlen(szSqlString)+1);
                                memcpy(pdata, szSqlString, strlen(szSqlString));
                                if(strlen(szSqlString) > 500)
                                {
                                        pdata[499] = '\0';
                                }
                                sprintf(m_ErrMsg, "mysql query fail [%s].\nError=[%s]\nSQL=%s", m_szHost,  mysql_error(&m_connection), pdata);
                                delete []pdata;
                                Close();
                                //return -1;
                                throw CMysqlException(m_ErrMsg);
                        }
                        return 0;
                }
                else
                {
                        char *pdata=new char[strlen(szSqlString)+1];
                        memset(pdata, 0, strlen(szSqlString)+1);
                        memcpy(pdata, szSqlString, strlen(szSqlString));
                        if(strlen(szSqlString) > 500)
                        {
                                pdata[499] = '\0';
                        }
                        sprintf(m_ErrMsg, "mysql query fail [%s].\nError=[%s]\nSQL=%s", m_szHost,  mysql_error(&m_connection), pdata);
                        Close();
                        delete []pdata;
                        throw CMysqlException(m_ErrMsg);
                        //return -1;
                }


        }

        record_query(szSqlString);

        return 0;
}

int CMysql:: Query(string& szSqlString)
{
        Connect();
        if(m_bConnected == true)
        {
                //printf("111 SET NAMES %s\n", szSqlString);
                int result = mysql_real_query(&m_connection,"SET NAMES 'UTF8'",strlen("SET NAMES 'UTF8'"));
                if(result != 0)
                {
                        //printf("1111  SET NAMES 'UTF8 failed result =%d mysql=%s\n", result, szSqlString);
                        Close();
                        Connect();
                }
                else
                {
                        //printf("1111  SET NAMES 'UTF8 success sql=%s\n", szSqlString);
                }
        }

        //if (m_bConnected == true)
        //{
        //	mysql_query(&m_connection,"SET NAMES 'UTF8'");
        //}

        if(mysql_real_query(&m_connection, szSqlString.c_str(), szSqlString.size()) != 0)
        {
                if(mysql_errno(&m_connection) == 2013)
                {
                        Close();
                        Connect();
                        if(m_bConnected == true)
                        {
                                mysql_query(&m_connection,"SET NAMES 'UTF8'");
                        }
                        if(mysql_real_query(&m_connection, szSqlString.c_str(), szSqlString.size()) != 0)
                        {
                                char *pdata=new char[szSqlString.size()+1];
                                memset(pdata, 0, szSqlString.size()+1);
                                memcpy(pdata, szSqlString.c_str(), szSqlString.size());
                                if(szSqlString.size() > 500)
                                {
                                        pdata[499] = '\0';
                                }
                                sprintf(m_ErrMsg, "mysql query fail [%s].\nError=[%s]\nSQL=%s", m_szHost,  mysql_error(&m_connection), pdata);
                                delete []pdata;
                                Close();
                                //return -1;
                                throw CMysqlException(m_ErrMsg);
                        }
                        return 0;
                }
                else
                {
                        char *pdata=new char[szSqlString.size()+1];
                        memset(pdata, 0, szSqlString.size()+1);
                        memcpy(pdata, szSqlString.c_str(), szSqlString.size());
                        if(szSqlString.size() > 500)
                        {
                                pdata[499] = '\0';
                        }
                        sprintf(m_ErrMsg, "mysql query fail [%s].\nError=[%s]\nSQL=%s", m_szHost,  mysql_error(&m_connection), pdata);
                        Close();
                        delete []pdata;
                        throw CMysqlException(m_ErrMsg);
                        //return -1;
                }
        }

        record_query((char *)szSqlString.c_str());

        return 0;

}
int CMysql:: FreeResult()
{
        if(m_result != NULL)
                mysql_free_result(m_result);
        m_iField = 0;
        m_result = NULL;
        if(m_bFieldIndexInitialized)
        {
                m_FieldIndex.erase(m_FieldIndex.begin(), m_FieldIndex.end());
                m_bFieldIndexInitialized = false;
        }
        return 0;
}

int CMysql:: StoreResult()
{
        FreeResult();
        m_result = mysql_store_result(&m_connection);
        if(m_result == NULL)
        {
                sprintf(m_ErrMsg, "mysql_store_result :%s!", mysql_error(&m_connection));
                throw CMysqlException(m_ErrMsg);
                //return -1;//throw CMysqlException(m_ErrMsg);
        }
        m_iField = mysql_num_fields(m_result);
        m_iRows = mysql_num_rows(m_result);
        return 0;
}

char** CMysql:: FetchRow()
{
        if(m_result == NULL)
        {
                StoreResult();
        }
        if(m_result == NULL)
        {
                return false;
        }
        m_row = mysql_fetch_row(m_result);
        return m_row;
}

int CMysql:: InitFieldName()
{
        if((!m_bFieldIndexInitialized) && (m_result!=NULL))
        {
                unsigned int i;
                MYSQL_FIELD *fields;

                fields = mysql_fetch_fields(m_result);
                for(i = 0; i < m_iField; i++)
                {
                        m_FieldIndex[fields[i].name] = i;
                }
                m_bFieldIndexInitialized = true;
        }
        return 0;
}

const char* CMysql:: GetFieldName(int iField)
{
        if(m_result==NULL)
        {
                return NULL;
        }
        MYSQL_FIELD *fields;
        fields = mysql_fetch_fields(m_result);
        if((unsigned int)iField> m_iField)
        {
                return NULL;
        }
        return fields[iField].name;
}

/*
	返回结果的行数
*/
int CMysql:: GetAffectedRows()
{
        my_ulonglong iNumRows;

        if(!m_bConnected) return 0;
        iNumRows = mysql_affected_rows(&m_connection);

        return (int)iNumRows;
}

/* 返回最近一次insert的ID
*/
unsigned int CMysql:: GetLastInsertId()
{
        return (unsigned int)mysql_insert_id(&m_connection);
}

/*
	按照字段名取回当前行的结果
*/
char* CMysql:: GetField(char* szFieldName)
{
        InitFieldName();
        return GetField(m_FieldIndex[szFieldName]);
}

/*
	按照字段索引取回当前行的结果
*/
char* CMysql:: GetField(unsigned int iField)
{
        if(iField > m_iField)
                return NULL;
        return m_row[iField];
}

char* CMysql::GetFieldBinary(const char * szFieldName, int & iFieldLen)
{
        InitFieldName();
        return GetFieldBinary(m_FieldIndex[szFieldName], iFieldLen);
}

char* CMysql::GetFieldBinary(unsigned int iField, int & iFieldLen)
{
        if(iField > m_iField)
                return NULL;

        //
        iFieldLen = mysql_fetch_lengths(m_result)[iField];

        return m_row[iField];
}

unsigned int CMysql::GetErrorNo(void)
{
        return mysql_errno(&m_connection);
}


void CMysql::record_query(char* szSqlString)
{
        m_query_num++;

        if(SDUtility::strnicmp(szSqlString,"select",6) == 0)
        {
                m_read_num++;
                m_db_read[string(m_szDataBase)]++;
        }
        else
        {
                m_write_num++;
                m_db_write[string(m_szDataBase)]++;
        }
}


std::string CMysql::get_statistic(int reset)
{
        stringstream output;

        output << "[Mysql]"
               << " m_query_num:" << m_query_num
               << ",m_read_num:" << m_read_num << endl;

        std::map<std::string,uint32_t> db_read_tmp;
        db_read_tmp.insert(m_db_read.begin(),m_db_read.end());

        std::map<std::string,uint32_t>::iterator it;
        for(it = db_read_tmp.begin() ; it != db_read_tmp.end() ; ++it)
        {
                output << it->first << ":" << it->second << std::endl;
        }

        output << ",m_write_num:" << m_write_num << endl;

        std::map<std::string,uint32_t> db_write_tmp;
        db_write_tmp.insert(m_db_write.begin(),m_db_write.end());

        for(it = m_db_write.begin() ; it != m_db_write.end() ; ++it)
        {
                output << it->first << ":" << it->second << std::endl;
        }

        if(reset)
        {
                m_query_num = 0;
                m_read_num  = 0;
                m_write_num = 0;

                m_db_read.erase(m_db_read.begin(),m_db_read.end());
                m_db_write.erase(m_db_write.begin(),m_db_write.end());
        }

        return output.str();

}
