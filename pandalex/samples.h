void pandalex_sample_specversion(int, va_list);
void pandalex_sample_entireheader(int, va_list);
void pandalex_sample_objstart(int, va_list);

void pandalex_sample_dictitem_string(int, va_list);
void pandalex_sample_dictitem_name(int, va_list);
void pandalex_sample_dictitem_array(int, va_list);
void pandalex_sample_dictitem_object(int, va_list);
void pandalex_sample_dictitem_dict(int, va_list);
void pandalex_sample_dictitem_int(int, va_list);

void pandalex_sample_stream(int, va_list);
void pandalex_sample_dictint(int, va_list);
void pandalex_sample_procstream(char *, int, char *);

// This data type is needed for pandalex_sample_stream and 
// pandalex_sample_dictint
typedef struct ipandalex_sample_dictint_list{
  char *value;
  int waiting;
  int number;

  char *stream;
  char *filter;

  struct ipandalex_sample_dictint_list  *next; 
} pandalex_sample_dictint_list;
