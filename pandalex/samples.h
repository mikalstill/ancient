void pdfinfo_begindocument(int, va_list);
void pdfinfo_specversion(int, va_list);
void pdfinfo_entireheader(int, va_list);
void pdfinfo_objstart(int, va_list);

void pdfinfo_dictitem_string(int, va_list);
void pdfinfo_dictitem_name(int, va_list);
void pdfinfo_dictitem_array(int, va_list);
void pdfinfo_dictitem_object(int, va_list);
void pdfinfo_dictitem_dict(int, va_list);
void pdfinfo_dictitem_int(int, va_list);

void pdfinfo_stream(int, va_list);
void pdfinfo_dictint(int, va_list);
void pdfinfo_procstream(char *, int, char *, int);

// This data type is needed for pdfinfo_stream and 
// pdfinfo_dictint
typedef struct pdfinfo_internal_dictint_list{
  char *value;
  int waiting;
  int number;

  char *stream;
  int streamlen;
  char *filter;

  struct pdfinfo_internal_dictint_list  *next; 
} pdfinfo_dictint_list;
