#include "../inc/zlog.h"
#include "../inc/HashMap.h"
#include <string.h>
#include <stdlib.h>
#include "../inc/read_config.h"

#define MAX_FILENAME_LENGTH 128
#define MAX_MODULENAME_LENGTH 32

struct Zlog
{
	char* m_fileName;
	Level m_level;
	char* m_module;
	FILE* m_f;
};

static int ReturnZeroOnBracket(char* _key, char* _value, char** _moduleName)
{
	if(_key[0] == '[')
	{
		*_moduleName = _value;
		return 0;
	}
	return 1;
}

static void GetModuleNameFromConfig(HashMap* _config, char** _moduleName)
{
	HashMap_ForEach(_config, (KeyValueActionFunction) ReturnZeroOnBracket, (void**) _moduleName);
}


static Level GetLevel(char* _level)
{
	if(strcmp(_level,"LOG_TRACE") == 0)
	{
		return LOG_TRACE;
	}
	
	if(strcmp(_level,"LOG_DEBUG") == 0)
	{
		return LOG_DEBUG;
	}
	
	if(strcmp(_level,"LOG_INFO") == 0)
	{
		return LOG_INFO;
	}
	
	if(strcmp(_level,"LOG_WARNING") == 0)
	{
		return LOG_WARNING;
	}
	
	if(strcmp(_level,"LOG_ERROR") == 0)
	{
		return LOG_ERROR;
	}
	
	if(strcmp(_level,"LOG_CRITICAL") == 0)
	{
		return LOG_CRITICAL;
	}
	
	if(strcmp(_level,"LOG_SEVERE") == 0)
	{
		return LOG_SEVERE;
	}
	
	if(strcmp(_level,"LOG_FATAL") == 0)
	{
		return LOG_FATAL;
	}
	
	if(strcmp(_level,"LOG_NONE") == 0)
	{
		return LOG_NONE;
	}
	
	return LOG_ERROR;/*default option*/
}

static int FillZlog(char* _key, char* _value, Zlog* _zlog)
{
	
	if(strcmp(_key, "Level") == 0)
	{
		_zlog->m_level = GetLevel(_value);
	}
	else if(strcmp(_key, "File") == 0)
	{
		strcpy(_zlog->m_fileName, _value); 
	}
	else if(_key[0] == '[')
	{
		strcpy(_zlog->m_module, _value);
	}

	return 1;
}

Level GetZlogLevel(Zlog* _zlog)
{
	return _zlog->m_level;
}

Zlog* ZlogCreate(HashMap* _config)
{
	Zlog* zlog = NULL;
	
	zlog = (Zlog*) malloc(sizeof(Zlog));
	if(NULL == zlog)
	{
		return NULL;
	}	

	zlog->m_fileName = (char*) malloc(MAX_FILENAME_LENGTH * sizeof(char));
	zlog->m_module = (char*) malloc(MAX_MODULENAME_LENGTH * sizeof(char));
	
	if(NULL == zlog->m_fileName || NULL == zlog->m_module)
	{
		return NULL;
	}

	HashMap_ForEach(_config, (KeyValueActionFunction) FillZlog, zlog);

	char* m_fileName;
	Level m_level;
	char* m_module;
	FILE* m_f;		
	zlog->m_f = NULL;
	return zlog;
}

void ZlogDestroy(Zlog* _zlog)
{
	if(NULL != _zlog->m_f)
	{
		fclose(_zlog->m_f);
	}	

	free(_zlog->m_module);
	free(_zlog->m_fileName);	
	free(_zlog);
}

char* GetModuleName(Zlog* _zlog)
{
	return _zlog->m_module;
}

FILE* GetFilePtr(Zlog* _zlog)
{
	if(NULL == _zlog->m_f)
	{
		_zlog->m_f = fopen(_zlog->m_fileName, "ab+");	
	}

	return _zlog->m_f;
}


/*
int PrintPair(char* _key, char* _value, void* _context)
{
	printf("Key: %s, Value: %s\n", _key, _value);
}*/

/*
int main()
{
	Config* config;
	char* path;
	char* moduleName = NULL;
	HashMap* map = NULL;
	Zlog* zlog = NULL;
	
	config = ReadConfig("log_config");
	GetPath(config, &path);

	for(map = GetNextConfig(config); map != NULL ; map = GetNextConfig(config))
	{
		GetModuleNameFromConfig(map, (void**)&moduleName);
		HashMap_ForEach(map, (KeyValueActionFunction) PrintPair, NULL);
		zlog = ZlogCreate(map);
		
		printf("Module name: <%s>, Zlog leve: %u, Filename: %s", GetModuleName(zlog), GetZlogLevel(zlog), zlog->m_fileName);
		putchar('\n');
	}
}


*/














































