void pandaedit_begindocument(int, va_list);
void pandaedit_specversion(int, va_list);
void pandaedit_objstart(int, va_list);
void pandaedit_objend(int, va_list);

void pandaedit_dictitem_string(int, va_list);
void pandaedit_dictitem_name(int, va_list);
void pandaedit_dictitem_arraystart(int, va_list);
void pandaedit_dictitem_arrayitem(int, va_list);
void pandaedit_dictitem_arrayend(int, va_list);
void pandaedit_dictitem_object(int, va_list);
void pandaedit_dictitem_dict(int, va_list);
void pandaedit_dictitem_dictend(int, va_list);
void pandaedit_dictitem_int(int, va_list);

void pandaedit_stream(int, va_list);
void pandaedit_procstream(char *, int, char *, int);

// This data type is needed for pandaedit_stream and 
// pandaedit_dictint
typedef struct pandaedit_internal_dictint_list{
  char *value;
  int waiting;
  int number;

  char *stream;
  int streamlen;
  char *filter;

  struct pandaedit_internal_dictint_list  *next; 
} pandaedit_dictint_list;

int pandaedit_atoi(char *);
