void pdfdump_begindocument(int, va_list);
void pdfdump_specversion(int, va_list);
void pdfdump_entireheader(int, va_list);
void pdfdump_objstart(int, va_list);

void pdfdump_dictitem_string(int, va_list);
void pdfdump_dictitem_name(int, va_list);
void pdfdump_dictitem_array(int, va_list);
void pdfdump_dictitem_object(int, va_list);
void pdfdump_dictitem_dict(int, va_list);
void pdfdump_dictitem_dictend(int, va_list);
void pdfdump_dictitem_int(int, va_list);

void pdfdump_stream(int, va_list);
void pdfdump_dictint(int, va_list);
void pdfdump_procstream(char *, int, char *, int);

// This data type is needed for pdfdump_stream and 
// pdfdump_dictint
typedef struct pdfdump_internal_dictint_list{
  char *value;
  int waiting;
  int number;

  char *stream;
  int streamlen;
  char *filter;

  struct pdfdump_internal_dictint_list  *next; 
} pdfdump_dictint_list;
