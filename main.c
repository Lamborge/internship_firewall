#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Package {
	char src[20];
	char dst[20];
	uint16_t src_port;
	uint16_t dst_port;
	char proto[6];
} Package;

typedef struct Rule {
	// [field] // default value
	char src[20]; // ""
	char dst[20]; // ""
	uint16_t src_port; // 0
	uint16_t dst_port; // 0
	char proto[5]; // ""
	bool result; // false (DROP)
} Rule;

// covert package-valid string to struct Package
Package strpkg(const char* str) { 
	Package result;
	char* tok;
	const char delim[] = " ";

	// copying str to local _str for parse
	char* _str = (char*)malloc(strlen(str)); 
	strcpy(_str, str);

	// start of parsing
	//src
	tok = strtok(_str, delim);
	strcpy(result.src, tok);

	//dst
	tok = strtok(NULL, delim);
	strcpy(result.dst, tok);

	//src_port
	tok = strtok(NULL, delim);
	result.src_port = atoi(tok);

	//dst_port
	tok = strtok(NULL, delim);
	result.dst_port = atoi(tok);

	//proto
	tok = strtok(NULL, delim);
	switch (atoi(tok)) {
	// 6 - tcp 17 - udp 27 - rdp 28 irtp
		case 6:
			strcpy(result.proto, "tcp");
			break;
		case 17:
			strcpy(result.proto, "udp");
			break;
		case 27:
			strcpy(result.proto, "rdp");
			break;
		case 28:
			strcpy(result.proto, "irtp");
			break;
		default:
			strcpy(result.proto, "unk");
			break;
	}

	free(_str);
	return result;
}

// pkg: package to check; rule: the rules array with which the packet will be checked; n: size of rules array;
bool rule_check(const Package pkg, const Rule* rules, const size_t n) { // true - ACCEPT; false - DROP
	bool result = false;

	for(size_t i = 0; i < n; i++){
		if(
				(strcmp(pkg.src, rules[i].src) == 0 || strcmp(rules[i].src, "") == 0) &&
				(strcmp(pkg.dst, rules[i].dst) == 0 || strcmp(rules[i].dst, "") == 0) &&
				(pkg.src_port == rules[i].src_port || rules[i].src_port == 0) &&
				(pkg.dst_port == rules[i].dst_port || rules[i].dst_port == 0) &&
				(strcmp(pkg.proto, rules[i].proto) == 0 || strcmp(rules[i].proto, "") == 0)
			)
		{
			return rules[i].result;
		}
	}

	return result;
}

// file: filestream with readable permission; rules: a pre-exsist allocated array of struct Rule where the rules will be written; size: size of rules array
void fimport_rules(FILE* file, Rule* rules, const size_t size) {
	char buf[110]; // calculated max rule string length

	char* tok;
	const char delims[] = " 	"; // space and tab

	char* words[12]; // array for separeted words from text db rules file lines

	size_t i_rules = 0; // iterator for while
	while(fscanf(file, "%[^\n]%*c\n", buf) != EOF) { //parse file string by string by use scanset

	// creating empty rule for futher filling
		strcpy(rules[i_rules].src, "");
		strcpy(rules[i_rules].dst, "");
		rules[i_rules].src_port = 0;
		rules[i_rules].dst_port = 0;
		strcpy(rules[i_rules].proto, "");
		

		// separating string taken from file to single words
		tok = strtok(buf, delims);
		{ // for local use i variable
			uint8_t i = 0;
			while(tok != NULL) { // filling words array
				words[i] = tok;
				tok = strtok(NULL, delims);

				i++;
			}
		}

		// src: [src ip] dst: [dst ip] src_port: [src port] dst_port: [dst port] proto: [protocol] => [ACCEP/DROP]
		for(uint8_t i = 0; i < 12; i++) {
			if(strcmp(words[i], "src:") == 0) {
				i++;
				strcpy(rules[i_rules].src, words[i]);
			}
			else if(strcmp(words[i], "dst:") == 0) {
				i++;
				strcpy(rules[i_rules].dst, words[i]);
			}
			else if(strcmp(words[i], "src_port:") == 0) {
				i++;
				rules[i_rules].src_port = atoi(words[i]);
			}
			else if(strcmp(words[i], "dst_port:") == 0) {
				i++;
				rules[i_rules].dst_port = atoi(words[i]);
			}
			else if(strcmp(words[i], "proto:") == 0) {
				i++;
				strcpy(rules[i_rules].proto, words[i]);
			}
			else if(strcmp(words[i], "=>") == 0) {
				i++;
				rules[i_rules].result = strcmp(words[i], "ACCEPT") == 0 ? true : false;
				break;
			}
		}
		

		i_rules++;
	}
}

int main(int argc, char* argv[]) {
	if(argc == 1) {
		printf("No db filename given");
		return 1;
	}

	Package in_pkg;
	char in_str[53]; // 53 - calculated max length of input string

	FILE* db; // filestream for db file
	size_t rules_size;
	Rule* rules;
	
	if(argc >= 2) {
		printf("Using: %s\n", argv[1]);
		db = fopen(argv[1], "r");
		if(db == NULL) { 
			printf("Error while opening file\n");
			return 1;
		}

		// calculating size of rules array
		char c;
		rules_size = 0;
    for (c = getc(db); c != EOF; c = getc(db))
			if (c == '\n') // Increment count if this character is newline
				rules_size++;
		rules_size++; // rules db fule must not be ending at \n symbol

		// importing rules from file to struct Rule* rules via func
		freopen(argv[1], "r", db);
		rules = (Rule*)malloc(sizeof(Rule) * rules_size);
		fimport_rules(db, rules, rules_size);

		fclose(db);
	}

	char in_str_prev[53]; // for solving bug with scanf in cycle
	if (argc == 2) { // [prgramm name] [db file]
		printf("Enter \"stop\" for stop\n");
		do {
			printf("Enter package:\n");
			scanf("%[^\n]", in_str);
			char c = getchar();
			if(c == -1) return 0;

			if(strcmp(in_str, "stop") == 0) return 0;

			in_pkg = strpkg(in_str);

			bool res = rule_check(in_pkg, rules, rules_size);
			printf("%s\n", res ? "ACCEPT" : "DROP");

		} while(true);
	}
	else if (argc == 3) { // [program name] [db file] [input string file]
		printf("Using: %s tests\n", argv[2]);

		FILE* tests = fopen(argv[2], "r");
		if(tests == NULL) {
			printf("Error while opening file\n");
			return 1;
		}

		while(fscanf(tests, "%[^\n]%*c\n", in_str) != EOF) {
			printf("Using package: %s\n", in_str);
			in_pkg = strpkg(in_str);

			bool res = rule_check(in_pkg, rules, rules_size);
			printf("%s\n", res ? "ACCEPT" : "DROP");
		}
	}

	return 0;
}
