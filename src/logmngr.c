#include "../inc/logmngr.h"
#include "../inc/zlog.h"
#include <stdlib.h>
#include <string.h>
#include "../inc/read_config.h"
#include "../inc/ADTDefs.h"

#define DEFAULT_MODULE "#"
#define HASHMAP_CAPACITY 100
#define MAX_PATH_LENGTH 256
#define MAX_MODULE_NAME_LENGTH 256

struct LogManager
{
	Zlog* m_defaultZlog;
	char* m_path; 
	HashMap* m_logs;
};

/*-------------------------------Globals-----------------------------------------*/

static LogManager* g_logManager; /*can only be used in this file because of static*/

/*----------------------Static functions declarations----------------------------*/

static size_t MyHash(char* _string);
static int MyEquality(char* _str1, char* _str2);
static LogManager* LogManagerCreate();
static void AddNewZlog(HashMap* _config);

/*-------------------------------------------------------------------------------*/

void ZlogInit(const char* _configFileName)
{
	HashMap* currConfig = NULL;
	Config* configs = ReadConfig(_configFileName);

	g_logManager = LogManagerCreate();

	GetPath(configs, &(g_logManager->m_path));
	AddNewZlog(currConfig);
	
	for(currConfig = GetNextConfig(configs); NULL != currConfig; currConfig = GetNextConfig(configs))
	{
		AddNewZlog(currConfig);
		HashMap_Destroy(&currConfig, free, NULL);
	}
	DestroyConfigs(configs);
}

Zlog* ZlogGet(const char* _module)
{
	Map_Result result;
	Zlog* zlog = NULL;
	result = HashMap_Find(g_logManager->m_logs, _module, (void**)&zlog);
	return (result == MAP_SUCCESS ? zlog : g_logManager->m_defaultZlog);
}

void LogManagerDestroy()
{
	HashMap_Destroy(&(g_logManager->m_logs), NULL, (void(*)(void*))ZlogDestroy);
	ZlogDestroy(g_logManager->m_defaultZlog);
	free(g_logManager->m_path);
	free(g_logManager);
}

/*-------------------------------------------------------------------------------*/

static size_t MyHash(char* _string)
{
	return (size_t)_string[0];
}

static int MyEquality(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0 ? 1 : 0);
}

static LogManager* LogManagerCreate()
{
	LogManager* logManager = NULL;
	logManager = (LogManager*) malloc(sizeof(LogManager));
	logManager->m_path = (char*) malloc(MAX_PATH_LENGTH * sizeof(char));
	logManager->m_logs = HashMap_Create(HASHMAP_CAPACITY, (HashFunction) MyHash, (EqualityFunction) MyEquality);
	
	return logManager;
}

static void AddNewZlog(HashMap* _config)
{
	char* moduleName = NULL;
	Zlog* zlog = NULL;
	
	zlog = ZlogCreate(_config);
	moduleName = GetModuleName(zlog);
	
	if(strcmp(moduleName, DEFAULT_MODULE) != 0)
	{
		HashMap_Insert(g_logManager->m_logs, moduleName, zlog);
	}
	else
	{
		g_logManager->m_defaultZlog = zlog;
	}
}












































