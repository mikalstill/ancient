// Sample callback functions -- note that multiple varargian arguments
// are presented in reverse order...

#include <stdarg.h>

void pandalex_sample_specversion(int event, va_list argptr){
  printf("Spec version is: %s\n", (char *) va_arg(argptr, char *));
}

void pandalex_sample_entireheader(int event, va_list argptr){
  int    i;
  char   *textMatch = (char *) va_arg(argptr, char *);

  printf("Entire document header is: ");

  for(i = 0; i < strlen(textMatch); i++){
    if(isprint(textMatch[i])) printf("%c ", textMatch[i]);
    else printf("\\%d ", textMatch[i]);
  }

  printf("\n");
}

void pandalex_sample_objstart(int event, va_list argptr){
  printf("Object started - generation %d of number %d\n",
	 va_arg(argptr, int), va_arg(argptr, int));
}

void pandalex_sample_dictitem_string(int event, va_list argptr){
  printf("Dictionary string\n");
}

void pandalex_sample_dictitem_name(int event, va_list argptr){
  printf("Dictionary name\n");
}

void pandalex_sample_dictitem_array(int event, va_list argptr){
  printf("Dictionary array\n");
}

void pandalex_sample_dictitem_object(int event, va_list argptr){
  printf("Dictionary object\n");
}

void pandalex_sample_dictitem_dict(int event, va_list argptr){
  printf("Dictionary dict\n");
}

void pandalex_sample_dictitem_int(int event, va_list argptr){
  printf("Dictionary int\n");
}

void pandalex_sample_stream(int event, va_list argptr){

}

void pandalex_sample_dictint(int event, va_list argptr){

}




