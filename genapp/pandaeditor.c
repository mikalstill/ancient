// A sample genapp module library

/******************************************************************************
DOCBOOK START

FUNCTION isGenAppModule
PURPOSE is this dynamic library a genapp enabled application?

SYNOPSIS START
int isGenAppModule();
SYNOPSIS END

DESCRIPTION This function is called to determine if the dynamic library named in a genapp module (.mdl) file is actually a genapp module.

RETURNS 0 if the dynamic library is a module, otherwise -1.
DOCBOOK END
******************************************************************************/
int isGenAppModule(){
  return 0;
}
