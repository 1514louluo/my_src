/****************************************************************************
 *    (C)2010
 *
 *   :
 *   : cbinmsg.cpp
 *   :
 *   : 1.4
 *       :
 *   : 20101111
 *   :
 *
 *1
 *
 *
 *
 ****************************************************************************/

#include <netinet/in.h>

#include "cbinmsg.h"
#include "bufferoperation.h"

/****************************************************************************
                         field
*****************************************************************************/
CBinMsgField::CBinMsgField()
{
}

CBinMsgField::~CBinMsgField()
{
}

int  CBinMsgField::Encode(char* szBuffer, int& iBufferLen)
{
    if (szBuffer == NULL || iBufferLen < 0)
        return Err_BinMsg_Param;

    CBufferManage clsBuffer;
    int iRet = clsBuffer.AttachBuffer(szBuffer, iBufferLen);
    if (iRet != 0)
        return Err_BinMsg_Param;

    int iTemp = m_strName.length();
    iRet = clsBuffer.PutString(m_strName.c_str(), iTemp);
    if (iRet != 0)
        goto err;

    iTemp = m_strValue.length();
    iRet = clsBuffer.PutString(m_strValue.c_str(), iTemp);
    if (iRet != 0)
        goto err;

err:
    if (iRet != 0)
    {
        iBufferLen = 0;
        return Err_BinMsg_NoEnough;
    }
    else
    {
        iBufferLen = m_strValue.length() + sizeof(int)
            + m_strName.length() + sizeof(int);
        return 0;
    }
}

int  CBinMsgField::Decode(const char* szBuffer, const int iBufferLen)
{
    if (szBuffer == NULL || iBufferLen <= 0)
        return Err_BinMsg_Param;

    CBufferManage clsBuffer;
    int iRet = clsBuffer.AttachBuffer((char*)szBuffer, iBufferLen);
    if (iRet != 0)
        return Err_BinMsg_Param;

    char* szBufferRow = NULL;
    int iFieldLength = 0;

    iRet = clsBuffer.GetString(&szBufferRow, iFieldLength);
    if (iRet != 0)
        return Err_BinMsg_FieldDecode;
    m_strName = std::string(szBufferRow, iFieldLength);

    iRet = clsBuffer.GetString(&szBufferRow, iFieldLength);
    if (iRet != 0)
        return Err_BinMsg_FieldDecode;
    m_strValue = std::string(szBufferRow, iFieldLength);

    return 0;
}

void  CBinMsgField::GetSize(int& iFieldSize)
{
    iFieldSize = m_strName.length() + sizeof(int)
        + m_strValue.length() + sizeof(int);
    return;
}

/****************************************************************************
                         row
*****************************************************************************/

CBinMsgRow::CBinMsgRow()
{
    m_iFieldNum = 0;
    m_iReadField = 0;
}

CBinMsgRow::~CBinMsgRow()
{
}

void  CBinMsgRow::AddField(const std::string &strName, const std::string &strValue)
{
    CBinMsgField clsField;
    clsField.m_strName = strName;
    clsField.m_strValue = strValue;

    AddField(clsField);
}

void  CBinMsgRow::AddField(const std::string &strName, int64_t llValue)
{
    char szUllvalue[64];
    memset((void *)szUllvalue, 0, sizeof(szUllvalue));

    snprintf(szUllvalue, sizeof(szUllvalue) - 1, "%"PRId64"", llValue);

    CBinMsgField clsField;
    clsField.m_strName = strName;
    clsField.m_strValue = szUllvalue;

    AddField(clsField);
}

void  CBinMsgRow::AddField(const std::string &strName, uint64_t ullValue)
{
    char szUllvalue[64];
    memset((void *)szUllvalue, 0, sizeof(szUllvalue));

    snprintf(szUllvalue, sizeof(szUllvalue) - 1, "%"PRIu64"", ullValue);

    CBinMsgField clsField;
    clsField.m_strName = strName;
    clsField.m_strValue = szUllvalue;

    AddField(clsField);
}

void  CBinMsgRow::AddField(const std::string &strName, int32_t iValue)
{
    char szIntvalue[64];
    memset((void *)szIntvalue, 0, sizeof(szIntvalue));

    snprintf(szIntvalue, sizeof(szIntvalue) - 1, "%d", iValue);

    CBinMsgField clsField;
    clsField.m_strName = strName;
    clsField.m_strValue = szIntvalue;

    AddField(clsField);
}

void  CBinMsgRow::AddField(const std::string &strName, char cValue)
{
    char szCharvalue[2] = {cValue, 0};

    CBinMsgField clsField;
    clsField.m_strName = strName;
    clsField.m_strValue = szCharvalue;

    AddField(clsField);
}

void  CBinMsgRow::AddField(const std::string &strName, uint32_t ulValue)
{
    char szIntvalue[64];
    memset((void *)szIntvalue, 0, sizeof(szIntvalue));

    snprintf(szIntvalue, sizeof(szIntvalue) - 1, "%u", ulValue);

    CBinMsgField clsField;
    clsField.m_strName = strName;
    clsField.m_strValue = szIntvalue;

    AddField(clsField);
}

void  CBinMsgRow::AddField(CBinMsgField clsField)
{
    m_vecField.push_back(clsField);
    m_iFieldNum++;
}

void CBinMsgRow::SetField(const std::string &strName, const std::string &strValue)
{
    int iFieldNum = m_vecField.size();

    int i = 0;
    for (;i < iFieldNum; i++)
        if (m_vecField[i].m_strName == strName)
            break;

    if (i != iFieldNum)
        m_vecField[i].m_strValue = strValue;

    return;
}

void CBinMsgRow::SetField(const std::string &strName, char cValue)
{
    char szCharvalue[2] = {cValue, 0};

    SetField(strName, szCharvalue);
    return;
}

void CBinMsgRow::SetField(const std::string &strName, int32_t iValue)
{
    char szIntvalue[64];
    memset((void *)szIntvalue, 0, sizeof(szIntvalue));

    snprintf(szIntvalue, sizeof(szIntvalue) - 1, "%d", iValue);

    SetField(strName, szIntvalue);
    return;
}

void CBinMsgRow::SetField(const std::string &strName, int64_t llValue)
{
    char szUllvalue[64];
    memset((void *)szUllvalue, 0, sizeof(szUllvalue));

    snprintf(szUllvalue, sizeof(szUllvalue) - 1, "%"PRId64"", llValue);

    SetField(strName, szUllvalue);
    return;
}

void CBinMsgRow::SetField(const std::string &strName, uint64_t ullValue)
{
    char szUllvalue[64];
    memset((void *)szUllvalue, 0, sizeof(szUllvalue));

    snprintf(szUllvalue, sizeof(szUllvalue) - 1, "%"PRIu64"", ullValue);

    SetField(strName, szUllvalue);
    return;
}

void CBinMsgRow::SetField(const std::string &strName, uint32_t ulValue)
{
    char szIntvalue[64];
    memset((void *)szIntvalue, 0, sizeof(szIntvalue));

    snprintf(szIntvalue, sizeof(szIntvalue) - 1, "%u", ulValue);

    SetField(strName, szIntvalue);
    return;
}

int CBinMsgRow::FindField(const std::string &strName)
{
    for (unsigned int i = 0; i < m_vecField.size(); i++)
        if (m_vecField[i].m_strName == strName)
            return 1;

    return 0;
}

CBinMsgField*  CBinMsgRow::GetNextField()
{
    CBinMsgField* pField = GetField(m_iReadField);
    if (pField == NULL)
        return NULL;

    m_iReadField++;
    return pField;
}

std::string  CBinMsgRow::GetField(const std::string &strName)
{
    int iFieldNum = m_vecField.size();

    int i = 0;
    for (;i < iFieldNum; i++)
        if (m_vecField[i].m_strName == strName)
            break;

    if (i == iFieldNum)
        return "";

    return m_vecField[i].m_strValue;
}

CBinMsgField*  CBinMsgRow::GetField(int iIndex)
{
    int iFieldNum = m_vecField.size();

    if (iIndex < 0 || iIndex >= iFieldNum)
        return NULL;

    return &m_vecField[iIndex];
}

int CBinMsgRow::GetFieldNum()
{
    return m_iFieldNum;
}

int CBinMsgRow::Encode(char* szBuffer, int& iBufferLen)
{
    if (szBuffer == NULL || iBufferLen < 0)
        return Err_BinMsg_Param;

    int iRowLen = 0;
    for (int i = 0; i < GetFieldNum(); i++)
    {
        int iTemp = iBufferLen - iRowLen - sizeof(int);
        if (iTemp <= 0)
        {
            iBufferLen = 0;
            return Err_BinMsg_NoEnough;
        }

        int iRet = m_vecField[i].Encode(szBuffer + sizeof(int) + iRowLen, iTemp);
        if (iRet != 0)
        {
            iBufferLen = 0;
            return Err_BinMsg_NoEnough;
        }

        *(int*)(szBuffer + iRowLen) = htonl(iTemp);
        iRowLen += iTemp + sizeof(int);
    }

    iBufferLen = iRowLen;
    return 0;
}

int CBinMsgRow::Decode(const char* szBuffer, int iBufferLen)
{
    if (szBuffer == NULL || iBufferLen <= 0)
        return Err_BinMsg_Param;

    CBufferManage clsBuffer;
    int iRet = clsBuffer.AttachBuffer((char*)szBuffer, iBufferLen);
    if (iRet != 0)
        return Err_BinMsg_Param;

    char* szBufferRow = NULL;
    int iRowLength = 0;
    int flag = 0;
    for (; (iRet = clsBuffer.GetString(&szBufferRow, iRowLength)) == 0; )
    {
        if (flag == 0)
            flag = 1;
        CBinMsgField clsField;
        iRet = clsField.Decode(szBufferRow, iRowLength);
        if (iRet != 0)
            return Err_BinMsg_FieldDecode;

        AddField(clsField);
    }

    //Modified by Garry, 2010-03-18
    //return 0;
    return flag ? 0 : Err_BinMsg_FieldDecode;
}

void CBinMsgRow::GetSize(int& iRowSize)
{
    iRowSize = 0;

    for (int i = 0; i < GetFieldNum(); i++)
    {
        int iTemp = 0;
        m_vecField[i].GetSize(iTemp);

        iRowSize += iTemp + sizeof(int);
    }

    return;
}

/****************************************************************************
                          record
*****************************************************************************/

CBinMsgRecord::CBinMsgRecord()
{
    m_iRowNum = 0;
    m_iReadRow = 0;
}

CBinMsgRecord::~CBinMsgRecord()
{
}

CBinMsgRow*  CBinMsgRecord::GetNextRow()
{
    CBinMsgRow* pRow = 0;

    pRow = GetRow(m_iReadRow);
    if (pRow == NULL)
        return NULL;

    m_iReadRow++;
    return pRow;
}

CBinMsgRow*  CBinMsgRecord::GetRow(int iIndex)
{
    int iRowNum = m_vecRow.size();

    if (iIndex < 0 || iIndex >= iRowNum)
        return NULL;

    return &m_vecRow[iIndex];
}

int CBinMsgRecord::GetRowNum()
{
    return m_iRowNum;
}

void CBinMsgRecord::AddRow(CBinMsgRow clsRow)
{
    m_vecRow.push_back(clsRow);
    m_iRowNum++;
}

int CBinMsgRecord::Decode(const char* szBuffer, const int iBufferLen)
{
    if (szBuffer == NULL || iBufferLen < (int)sizeof(int))
        return Err_BinMsg_Param;

    CBufferManage clsBuffer;
    int iRet = clsBuffer.AttachBuffer((char*)(szBuffer + sizeof(int)), iBufferLen - sizeof(int));
    if (iRet != 0)
        return Err_BinMsg_Param;

    char* szBufferRow = NULL;
    int iRowLength = 0;
    int flag = 0;
    for (; (iRet = clsBuffer.GetString(&szBufferRow, iRowLength)) == 0; )
    {
        if (flag == 0)
            flag = 1;
        CBinMsgRow clsRow;
        iRet = clsRow.Decode(szBufferRow, iRowLength);
        if (iRet != 0)
            return Err_BinMsg_RowDecode;

        AddRow(clsRow);
    }

    //Modified by Garry, 2010-03-18
    //return 0;
    return flag ? 0 : Err_BinMsg_RowDecode;
}

int CBinMsgRecord::Encode(char* szBuffer, int& iBufferLen)
{
    if (szBuffer == NULL || iBufferLen < 0)
        return Err_BinMsg_Param;

    int iRecLen = sizeof(int);

    for (int i = 0; i < GetRowNum(); i++)
    {
        int iTemp = iBufferLen - iRecLen - sizeof(int);
        if (iTemp <= 0)
        {
            iBufferLen = 0;
            return Err_BinMsg_NoEnough;
        }

        int iRet = m_vecRow[i].Encode(szBuffer + sizeof(int) + iRecLen, iTemp);
        if (iRet != 0)
        {
            iBufferLen = 0;
            return Err_BinMsg_NoEnough;
        }

        *(int*)(szBuffer + iRecLen) = htonl(iTemp);
        iRecLen += iTemp + sizeof(int);
    }

    *(int*)szBuffer = htonl(iRecLen);
    iBufferLen = iRecLen;

    return 0;
}

void CBinMsgRecord::GetSize(int& iRecSize)
{
    iRecSize = sizeof(int);

    for (int i = 0; i < GetRowNum(); i++)
    {
        int iTemp = 0;
        m_vecRow[i].GetSize(iTemp);

        iRecSize += iTemp + sizeof(int);
    }

    return;
}
