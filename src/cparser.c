#include "cparser.h"
#include <map.h>
map_str_t items;

char** config_mapize(char* str){
	char** arr = (char**)calloc(2, sizeof(char*));
	char* spl;
	arr[1] = (char*)calloc(1, 128);

	// Just return null for comment lines
	if(str[0] == '/' && str[1] == '/') {
		free(arr[1]);
		free(arr);
		return NULL;
	}

	spl = strtok(str, " ");

	if(spl == NULL) {
		free(arr[1]);
		free(arr);
		return NULL;
	}

	arr[0] = spl;

	spl = strtok(NULL, " ");

	// Join the rest of the data back
	while(spl != NULL) {
		strcat(arr[1], spl);
		strcat(arr[1], " ");
		spl = strtok(NULL, " ");
	}
	return arr;
}


char* config_readline(FILE *fp) {
	int ch;
	int i = 0;
	size_t buff_len = 0;

	char* buffer = malloc(buff_len + 1);
	if (!buffer) return NULL;

	while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
		buff_len++;
		void *tmp = realloc(buffer, buff_len + 1);
		if (tmp == NULL) {
			free(buffer);
			return NULL;
		}
		buffer = tmp;

		buffer[i] = (char) ch;
		i++;
	}
	buffer[i] = '\0';

	if (ch == EOF && (i == 0 || ferror(fp))) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

int config_parse_file(FILE* file){
	map_init(&items);
	while(1) {
		char* line = config_readline(file);
        if(line != NULL){
			char** arr = config_mapize(line);
			if(arr != NULL){
				map_set(&items, arr[0], arr[1]);
				free(arr);
			}
		} else {
			break;
		}
	}
	return 0;
}

char* config_get_item(const char* value_name){
	char* pt = map_get(&items, value_name)[0];
	if(pt == NULL) return "";
	return pt;
}

