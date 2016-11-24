/**
 * Created by meow.
 *
 * zy11043@nottingham.edu.cn
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct Person{
  char *name;
  char *number;
  char *department;
  struct Person *next;
};


typedef struct Person PERSON;
typedef void (*MENU_FUN)(void);

PERSON *TABLE = NULL;

void prompt(const char *content, char *dst); 

void print_person(const PERSON *p);
void menu_print();

PERSON* search(PERSON *head, const char* key);
void menu_search();

PERSON* free_person(PERSON *person);
void menu_delete();


PERSON* create_person(const char *name, const char *number, const char *department);
void append_person(PERSON **head, PERSON *person);
void menu_add();
/**
 * Created by meow.
 *
 * zy11043@nottingham.edu.cn
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct Person{
  char *name;
  char *number;
  char *department;
  struct Person *next;
};

typedef struct Person PERSON;
typedef void (*MENU_FUN)(void);

PERSON *TABLE = NULL;

char* prompt(const char *content); 

void print_person(const PERSON *p);
void menu_print();

PERSON* search(PERSON *head, const char* key);
void menu_search();

PERSON* free_person(PERSON *person);
void menu_delete();


void menu_modify();

PERSON* create_person(const char *name, const char *number, const char *department);
void append_person(PERSON **head, PERSON *person);
void menu_add();

void menu_exit();

int main(){
  
  MENU_FUN menu[] = {
    menu_print,
    menu_add,
    menu_search,
    menu_delete,
    menu_modify,
    menu_exit
  };

  while(1){
    puts("");
    puts("--*--Phone Book--*--");
    puts("1)Print directory");
    puts("2)Add Person");
    puts("3)Search Person");
    puts("4)Delete Person");
    puts("5)Modify Person");
    puts("6)Exit");
    puts("--*--Main Menu--*--");
    char* choice = prompt("Choice:");
    if(choice[1] != '\0'){
      puts("Plz enter single digit");
    } else if (choice[0] - '1' < 0 || choice[0] - '6' > 0){
      puts("Plz enter digit between 1 and 6");
    } else {
      menu[choice[0] - '1']();
    }
    free(choice);
  }
}

char* prompt(const char *content){
  printf("%s", content);
  
  size_t size = 128; 
  char* buf = (char*) malloc(size * sizeof(char));
  if(!buf) exit(EXIT_FAILURE);
  char ch;
  size_t count = 0;
  do{
    scanf("%c", &ch);
    if(count + 1 > size){
      char tmp[size];
      strcpy(buf, tmp);
      size += size;
      buf =(char*) malloc(size * sizeof(char));
      strcpy(tmp, buf);
    }
    if(ch == '\n') buf[count] = '\0';
    else buf[count] = ch;
    count++;
  }while(ch && ch != '\n');

  return buf;
}

void menu_print(){
  for(PERSON *p = TABLE; p; p = p->next){
    print_person(p);
  }
}

void print_person(const PERSON *p){
  if(!p) return;
  if(!p->number){
    printf("ERR invalid person!");
  }
  printf("%s | %s | (%s)\n",
         p->number,
         p->name? p->name : "noname",
         p->department? p->department : "nodepartment");
}


void menu_add(){
  char* name = prompt("Name:");
  char* number = prompt("Number:");
  char* department = prompt("Department:");

  if(!strlen(number)) {
    puts("number not null!");
    return;
  }

  for(PERSON *p = TABLE; p; p = p->next){
    if(!strcmp(p->name, number)){
      puts("Number exist!");
      return;
    }
  }

  append_person(&TABLE, create_person(name, number, department));
  free(name);
  free(number);
  free(department);
}


void append_person(PERSON **head, PERSON *person){
  if(!head){
    printf("ERR null head pointer!\b");
    return;
  }
  if(!*head){
    *head = person;
  }
  else{
    PERSON *p = *head;
    while(p->next){
      p = p->next;
    }
    p->next = person;
  }
}



PERSON* create_person(const char *name, const char *number, const char *department){

  PERSON *p = (PERSON*) malloc(sizeof(PERSON));

  p->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
  p->number = (char*) malloc((strlen(number) + 1) * sizeof(char));
  p->department = (char*) malloc((strlen(department) + 1) * sizeof(char));
  p->next = NULL;

  if(!p->name || !p->number || !p->department){
    puts("ERR No enough space for malloc!");
    exit(EXIT_FAILURE);
  }

  
  strcpy(p->name, name);
  strcpy(p->department, department);
  strcpy(p->number, number);

  return p;
}

void menu_search(){

  char* key = prompt("Enter search key:");
  PERSON *p = search(TABLE, key);
  if(!p){
    printf("No such person!");
  }else{
    print_person(p);
  }
  free(key);
}


PERSON* search(PERSON *head, const char* key){
  for(PERSON *p = head; p; p = p->next){
    if(strstr(p->name, key)) return p;
  }
  
  return NULL;
}

void menu_delete(){
  
  char *key = prompt("Plz choose delete id:");

  PERSON *prev, *p;
  
  prev = TABLE;

  if(!prev){
    return;
  }

  if(!prev->next && !strcmp(prev->number, key)){
    TABLE = free_person(prev);
  }

  p = prev->next;

  while(p){
    if(!strcmp(p->number, key)){
      prev->next = free_person(p);
      return;
    }
    prev = p;
    p = p->next;
  }
  puts("ERR No such number!");
}


PERSON* free_person(PERSON *p){
  if(p){
    PERSON * next = p->next;
    p->next = NULL;
    free(p->number);
    free(p->name);
    free(p->department);
    free(p);
    return next;
  }else{
    puts("ERR free null person!");
    return NULL;
  }
}


void menu_exit(){
  PERSON *p = TABLE;

  puts("delete:");
  while(p){
    print_person(p);
    p = free_person(p);
  }
  puts("finish");
  
  exit(0);
}



void menu_modify(){
  char* number = prompt("Number to modify:");

  PERSON *p = search(TABLE, number);

  if(!p){
    printf("No such person!");
    return;
  }

  free(number);

  char* item = prompt("Choose item: 1) name 2)number 3)department ");
  if(!item || item[1] != '\0' || item[0] - '1' < 0 || item[0] - '3' > 0){
    printf("Plz enter correct number");
    return;
  }
  char *dat;

  switch(item[0]){
  case '1':
    free(p->name);
    p->name = prompt("Name:");
    break;
  case '2':
    dat = prompt("Number:");
    if(!dat){
      printf("Number null!");
      return;
    }
    free(p->number);
    p->number = dat;
    break;
  case '3':
    free(p->department);
    p->department = prompt("Department:");
    break;
  default:
    break;
  }
  free(item);

}

void menu_exit();


int main(){
  
  
  MENU_FUN menu[] = {
    menu_print,
    menu_add,
    menu_search,
    menu_delete,
    menu_exit
  };

  while(1){
    puts("");
    puts("--*--Phone Book--*--");
    puts("1)Print directory");
    puts("2)Add Person");
    puts("3)Search Person");
    puts("4)Delete Person");
    puts("5)Exit");
    puts("--*--Main Menu--*--");
    char choice[128];
    prompt("Choice:", choice);
    if(choice[1] != '\0'){
      puts("Plz enter single digit");
    } else if (choice[0] - '1' < 0 || choice[0] - '5' > 0){
      puts("Plz enter digit between 1 and 5");
    } else {
      menu[choice[0] - '1']();
    }
  }
}

void prompt(const char *content, char *dst){
  printf("%s", content);
  
  scanf("%s", dst);
}

void menu_print(){
  for(PERSON *p = TABLE; p; p = p->next){
    print_person(p);
  }
}

void print_person(const PERSON *p){
  if(!p) return;
  if(!p->number){
    printf("ERR invalid person!");
  }
  printf("%s | %s | (%s)\n",
         p->number,
         p->name? p->name : "noname",
         p->department? p->department : "nodepartment");
}


void menu_add(){
  char name[128];
  char number[128];
  char department[128];
  prompt("Name:", name);
  prompt("Number:", number);
  prompt("Department:", department);

  append_person(&TABLE, create_person(name, number, department));
}


void append_person(PERSON **head, PERSON *person){
  if(!head){
    printf("ERR null head pointer!\b");
    return;
  }
  if(!*head){
    *head = person;
  }
  else{
    PERSON *p = *head;
    while(p->next){
      p = p->next;
    }
    p->next = person;
  }
}



PERSON* create_person(const char *name, const char *number, const char *department){

  PERSON *p = (PERSON*) malloc(sizeof(PERSON));

  p->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
  p->number = (char*) malloc((strlen(number) + 1) * sizeof(char));
  p->department = (char*) malloc((strlen(department) + 1) * sizeof(char));
  p->next = NULL;

  if(!p->name || !p->number || !p->department){
    puts("ERR No enough space for malloc!");
    exit(1);
  }
  
  strcpy(p->name, name);
  strcpy(p->department, department);
  strcpy(p->number, number);

  return p;
}

void menu_search(){

  char key[128];
  prompt("Enter search key:", key);
  PERSON *p = search(TABLE, key);
  if(!p){
    printf("No such person!");
  }else{
    print_person(p);
  }
}


PERSON* search(PERSON *head, const char* key){
  for(PERSON *p = head; p; p = p->next){
    if(strstr(p->name, key)) return p;
  }
  
  return NULL;
}

void menu_delete(){
  
  char key[128];
  prompt("Plz choose delete id:", key);

  PERSON *prev, *p;
  
  prev = TABLE;

  if(!prev){
    return;
  }

  if(!prev->next && !strcmp(prev->number, key)){
    TABLE = free_person(prev);
  }

  p = prev->next;

  while(p){
    if(!strcmp(p->number, key)){
      prev->next = free_person(p);
      return;
    }
    prev = p;
    p = p->next;
  }
  puts("ERR No such number!");
}


PERSON* free_person(PERSON *p){
  if(p){
    PERSON * next = p->next;
    p->next = NULL;
    free(p->number);
    free(p->name);
    free(p->department);
    free(p);
    return next;
  }else{
    puts("ERR free null person!");
    return NULL;
  }
}


void menu_exit(){
  PERSON *p = TABLE;

  puts("delete:");
  while(p){
    print_person(p);
    p = free_person(p);
  }
  puts("finish");

  
  exit(0);
}
