#include "../inc/read_config.h"
#include <string.h>
#include "../inc/HashMap.h"
#include <stdlib.h>
#include "../inc/ADTDefs.h"

#define CAPACITY 30
#define STR_LENGTH 128

struct Config
{
	Queue* m_configs;
	char* m_path;
};

static Queue* ReadConfigQueue(const char* _configFileName, char** _path);
static void InsertTokensToHashMap(HashMap* _hashMap, char* _string);
static size_t MyHash(char* _string);
static int MyEquality(char* _str1, char* _str2);

Config* ReadConfig(const char* _configFileName)
{
	Config* config = NULL;
	config = (Config*) malloc(sizeof(Config));
	if(NULL == config)
	{
		return NULL;
	}
	
	config->m_configs = ReadConfigQueue(_configFileName, &(config->m_path));
	
	return config;
}

HashMap* GetNextConfig(Config* _configs)
{
	HashMap* config;
	if(NULL == _configs)
	{
		return NULL;
	}
	if(ERR_OK == QueueRemove(_configs->m_configs, (void**)(&config)))
	{
		return config;
	}
	else
	{
		return NULL;
	}
}

void DestroyConfigs(Config* _configs)
{
	if(NULL != _configs)
	{
		QueueDestroy(_configs->m_configs, NULL);
		free(_configs);
	}
}

void GetPath(Config* _configs, char** _path)
{
	if(NULL != _configs && NULL != _path)
	{
		*_path = _configs->m_path;
	}
}

static void InsertTokensToHashMap(HashMap* _hashMap, char* _string)
{
	char* keyPtr = NULL;
	char* valPtr = NULL;
	keyPtr = strtok(_string, " ]\n");
	
	if(keyPtr[0] == '[')
	{
		HashMap_Insert(_hashMap, keyPtr, keyPtr + 1);
		return;
	}
	
	valPtr = strtok(NULL, " \n");
	valPtr = strtok(NULL, " \n");
	
	HashMap_Insert(_hashMap, keyPtr, valPtr);
}	

static Queue* ReadConfigQueue(const char* _configFileName, char** _path)
{
	FILE* fp;
	Queue* queue;
	char* currString = NULL;
	HashMap* hashMap = NULL; 
	queue = QueueCreate(100);
	hashMap = HashMap_Create(CAPACITY, (HashFunction) MyHash, (EqualityFunction) MyEquality);
	if(NULL == hashMap)
	{
		return NULL;
	}
	
	fp = fopen(_configFileName, "r");
	if(NULL == fp)
	{
		return NULL;
	}

	currString = (char*) malloc(STR_LENGTH * sizeof(char));

	fgets(currString, STR_LENGTH, fp);
	
	InsertTokensToHashMap(hashMap, currString);	

	currString = (char*) malloc(STR_LENGTH * sizeof(char));

	while(NULL != fgets(currString, STR_LENGTH, fp))
	{
		
		if(currString[0] == '[')
		{
			QueueInsert(queue, hashMap);
			hashMap = HashMap_Create(CAPACITY, (HashFunction) MyHash, (EqualityFunction) MyEquality);
		}
		InsertTokensToHashMap(hashMap, currString);	
		
		HashMap_Find(hashMap, "Path", (void**)_path);
		
		currString = (char*) malloc(STR_LENGTH * sizeof(char));	
	}
	QueueInsert(queue, hashMap);
	free(currString);
	fclose(fp);
	return queue;
}

static size_t MyHash(char* _string)
{
	return (size_t)_string[0];
}

static int MyEquality(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0 ? 1 : 0);
}















































