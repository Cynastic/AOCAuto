#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define URL "https://adventofcode.com/"
#define YEAR "2024"

const char* DAY;
const char* SESSION_TOKEN;

const char* get_request_mapping() {
	const char* request_mapping = calloc(1024, sizeof(char));
	sprintf_s(request_mapping, 1024, "%s%s/day/%s/input", URL, YEAR, DAY);
	request_mapping = realloc(request_mapping, strlen(request_mapping) + 1 * sizeof(char));
	return request_mapping;
}

long write_output_to_file(const char* command, FILE* file) {
	FILE* command_output = _popen(command, "r");
	long output_size = 0;
	char current_char = 0;
	while(feof(command_output) == 0) {
		putc(current_char, file);
		output_size++;
		current_char = fgetc(command_output);
	}

	_pclose(command_output);
	return output_size;
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		printf("[ERROR] You have to provide a day.%s\n");
		exit(0);
	}

	DAY = argv[1];
	SESSION_TOKEN = getenv("AOC_TOKEN");
	if(SESSION_TOKEN == NULL) {
		printf("[ERROR] Environment Variable SESSION_TOKEN not set.\n");
	}

	const char* request_mapping = get_request_mapping(); 
	printf("Requesting data at: %s\n", request_mapping);

	//create new dir
	
	const char* relative_dir_path = calloc(255, sizeof(char));
	sprintf_s(relative_dir_path, 255, "day%s", DAY);

	const char* create_dir_command = calloc(255, sizeof(char));
	sprintf_s(create_dir_command, 255, "mkdir %s", relative_dir_path);
	printf("Create dir command: %s\n", create_dir_command);
	system(create_dir_command);


	//read output from curl
	const char* output_file_path = calloc(255, sizeof(char));
	sprintf_s(output_file_path, 255, "%s/input.txt", relative_dir_path);
	FILE* output_file = fopen(output_file_path, "w");
	if(output_file == NULL) {
		printf("[ERROR] Couldn't create file.");
		exit(0);
	}

	const char* curl_command = calloc(255, sizeof(char));
	sprintf_s(curl_command, 255, "curl %s -H \"Cookie: session=%s", request_mapping, SESSION_TOKEN);
	long output_size = write_output_to_file(curl_command, output_file);
	fclose(output_file);

	printf("Input written to: %s\n", output_file_path);
	printf("Input size: %ld\n", output_size);

	exit(0);
}
