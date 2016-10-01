#include <stdio.h>
#include "read_config.h"
#include "../GenericHashMap/HashMap.h"

int PrintPair(char* _key, char* _value, void* _context)
{
	printf("Key: %s, Value: %s\n", _key, _value);
}

int main()
{
	Config* config;
	char* path;
	HashMap* map = NULL;
	
	config = ReadConfig("log_config");
	GetPath(config, &path);
	printf("%s\n", path);

	for(map = GetNextConfig(config); map != NULL ; map = GetNextConfig(config))
	{
		HashMap_ForEach(map, (KeyValueActionFunction) PrintPair, NULL);
		putchar('\n');
	}
}
