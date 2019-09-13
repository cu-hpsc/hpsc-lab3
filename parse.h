#include <argp.h>

/* This structure is used by main to communicate with parse_opt. */
struct args
{
  long density;
  long max_iter;
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
{
  {"density",'d',"count",0,"the frequency of candidate points on a unit interval"},
  {"max_iter",'m',"count",0,"Number of blocks (default 100000)"},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct args *args = state->input;
  switch (key)
    {
    case ARGP_KEY_INIT:
      args->density = 100;
      args->max_iter = 1000;
      break;
    case 'd':
      args->density = strtol(arg,NULL,10);
      break;
    case 'm':
      args->max_iter = strtol(arg,NULL,10);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

void parse_args(struct args *args, int argc, char **argv)
{
  static struct argp argp = {options, parse_opt, NULL, NULL};
  argp_parse(&argp, argc, argv, 0, 0, args);
  return;
}
