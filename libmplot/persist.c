#include <stdio.h>
#include "libmplot.h"

// Persist information to a text string, and then get it back
char *plot_persiststate(plot_state *state)
{
  char *pstr;

  if((pstr = malloc(4096 * sizeof(char))) == NULL)
    return NULL;

  snprintf(pstr, 4096, "linewidthx=%d;linewidthy=%d;linecap=%d;linejoin=%d;fillcolor=[%d,%d,%d];linecolor=[%d,%d,%d];fontcolor=[%d,%d,%d];textx=%d;texty=%d;penx=%d;peny=%d;fontpath=%s;fontsize=%d",
	   state->linewidthx, state->linewidthy,
	   state->linecap, state->linejoin, 
	   state->fillcolor.r, state->fillcolor.g, state->fillcolor.b,
	   state->linecolor.r, state->linecolor.g, state->linecolor.b,
	   state->fontcolor.r, state->fontcolor.g, state->fontcolor.b,
	   state->textx, state->texty, state->penx, state->peny,
	   state->fontpath, state->fontsize);
  return pstr;
}

void plot_applystate(plot_state *state, char *pstr)
{
  char *lstr, *token, *cmd, *val, *r, *g, *b;
  
  // Don't clobber the passed string
  lstr = strdup(pstr);
  token = strtok(lstr, ";");

  while(token){
    cmd = strtok(token, "=");
    val = strtok(NULL, "=");

    if(strcmp(cmd, "linewidthx") == 0) state->linewidthx = atoi(val);
    else if(strcmp(cmd, "linewidthy") == 0) state->linewidthy = atoi(val);
    else if(strcmp(cmd, "linecap") == 0) state->linecap = atoi(val);
    else if(strcmp(cmd, "linejoin") == 0) state->linejoin = atoi(val);
    else if(strcmp(cmd, "fillcolor") == 0){
      strtok(val, "[,]");
      r = strtok(NULL, "[,]");
      g = strtok(NULL, "[,]");
      b = strtok(NULL, "[,]");
      state->fillcolor.r = atoi(r);
      state->fillcolor.g = atoi(g);
      state->fillcolor.b = atoi(b);
    }
    else if(strcmp(cmd, "linecolor") == 0){
      strtok(val, "[,]");
      r = strtok(NULL, "[,]");
      g = strtok(NULL, "[,]");
      b = strtok(NULL, "[,]");
      state->fillcolor.r = atoi(r);
      state->fillcolor.g = atoi(g);
      state->fillcolor.b = atoi(b);
    }
    else if(strcmp(cmd, "fontcolor") == 0){
      strtok(val, "[,]");
      r = strtok(NULL, "[,]");
      g = strtok(NULL, "[,]");
      b = strtok(NULL, "[,]");
      state->fillcolor.r = atoi(r);
      state->fillcolor.g = atoi(g);
      state->fillcolor.b = atoi(b);
    }
    else if(strcmp(cmd, "textx") == 0) state->textx = atoi(val);
    else if(strcmp(cmd, "texty") == 0) state->texty = atoi(val);
    else if(strcmp(cmd, "penx") == 0) state->penx = atoi(val);
    else if(strcmp(cmd, "peny") == 0) state->peny = atoi(val);
    else if(strcmp(cmd, "fontpath") == 0){
      if(state->fontpath != NULL)
	free(state->fontpath);
      state->fontpath = strdup(val);
    }
    else if(strcmp(cmd, "fontsize") == 0) state->fontsize = atoi(val);
    else{
      fprintf(stderr, "Unknown persistance value \"%s\"\n", cmd);
    }

    token = strtok(NULL, ";");
  }
}
