#pragma once

enum eScriptType :unsigned short
{
    SCRIPT_TYPE_DEFAULT = 0,
    SCRIPT_TYPE_CUSTOM = 1
};

enum eOpcodeResult :char
{
    OR_CONTINUE = 0,
    OR_TERMINATE = 1,
    OR_UNDEFINED = -1
};

union tScriptVar
{
    int nVar;
    float fVar;
    char *cVar;
    void *pVar;
};

enum eParamType :unsigned char
{
    PARAM_TYPE_END_OF_PARAMS = 0,
    PARAM_TYPE_INT32 = 1,
    PARAM_TYPE_GVAR = 2,
    PARAM_TYPE_LVAR = 3,
    PARAM_TYPE_INT8 = 4,
    PARAM_TYPE_INT16 = 5,
    PARAM_TYPE_FLOAT = 6,
    PARAM_TYPE_STRING = 14
};

struct tParamType
{
    eParamType type : 7; // eParamType
    bool processed : 1; // did we process long string already
};

struct ScmFunction
{
    ScmFunction *prev;
    tScriptVar vars[16];
    int retAddr;
};

class CCustomScript
{
public:
    /* 0x00 */ CCustomScript *m_pNext;
    /* 0x04 */ CCustomScript *m_pPrev;
    /* 0x08 */ char m_acName[8];
    /* 0x10 */ unsigned int m_dwIp;
    /* 0x14 */ unsigned int m_aGosubAddr[6];
    /* 0x2C */ unsigned short m_nCurrentGosub;
    /* 0x2E */ eScriptType m_nScriptType; // see eScriptType
    /* 0x30 */ tScriptVar m_aLVars[18];
#ifdef CLEO_VC
    /* 0x78 */ bool m_bIsActive;
    /* 0x79 */ bool m_bCondResult;
    /* 0x7A */ bool m_bIsMission;
#elif defined CLEO_III
    /* 0x78 */ bool m_bCondResult;
    /* 0x79 */ bool m_bIsMission;
    /* 0x7A */ bool m_bIsActive;
#endif
    /* 0x7B */ bool m_bAwake;
    /* 0x7C */ unsigned int m_dwWakeTime;
    /* 0x80 */ unsigned short m_wIfOp;
    /* 0x82 */ bool m_bNotFlag;
    /* 0x83 */ bool m_bDeathArrestCheckEnabled;
    /* 0x84 */ bool m_bWastedOrBusted;
    /* 0x85 */ bool m_bMissionFlag;
    /* 0x86 */ short _pad;

    /* Custom data */
    unsigned int m_nLastPedSearchIndex;
    unsigned int m_nLastVehicleSearchIndex;
    unsigned int m_nLastObjectSearchIndex;
    struct {
        unsigned int m_bFileNotFound : 1;
        unsigned int m_bFileSeekError : 1;
        unsigned int m_bInternalError : 1;
        unsigned int m_bEmptyFile : 1;
        unsigned int m_bAllocationFailed : 1;
        unsigned int m_bEofReached : 1;
    } m_Errors;
    char *m_pCodeData;
    unsigned int m_dwBaseIp;
    ScmFunction *m_pScmFunction;
    CCustomScript *m_pNextCustom;
    CCustomScript *m_pPrevCustom;

#ifdef CLEO_VC
    tScriptVar m_aLargeLVars[1024];
#endif
    void Init();

    bool Loaded();

    void AddToCustomList(CCustomScript **list);

    void RemoveFromCustomList(CCustomScript **list);

    void Collect(unsigned int numParams);

    void Collect(unsigned int *pIp, unsigned int numParams);

    tScriptVar CollectNextWithoutIncreasingPC(unsigned int ip);

    eParamType GetNextParamType();

    void Store(unsigned int numParams);

    void ReadShortString(char *out);

    void UpdateCompareFlag(bool result);

    tScriptVar *GetPointerToScriptVariable();

    CCustomScript();

    CCustomScript(const char *filepath);

    ~CCustomScript();

    void JumpTo(int address);

    eOpcodeResult ProcessOneCommand();
};

#ifdef CLEO_VC
static_assert(sizeof(CCustomScript) == 0x10AC, "Error with CCustomScript");
#else
static_assert(sizeof(CCustomScript) == 0xAC, "Error with CCustomScript");
#endif
