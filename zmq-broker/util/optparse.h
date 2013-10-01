
#ifndef _HAVE_OPTPARSE_H
#define _HAVE_OPTPARSE_H

/******************************************************************************
 *  Datatypes:
 *****************************************************************************/
struct optparse_option;
typedef struct opt_parser * optparse_t;

/*
 *  prototype for output function used by optparser
 */
typedef int (*opt_log_f) (const char *fmt, ...);

/*
 *  prototype for option callback hook
 */
typedef int (*optparse_cb_f) (struct optparse_option *,
			      const char *optarg, void *arg);
/*
 *  Error codes:
 */
typedef enum {
   OPTPARSE_SUCCESS,       /* Success                                      */
   OPTPARSE_FAILURE,       /* Generic error.                               */
   OPTPARSE_NOMEM,         /* Memory allocation failure                    */
   OPTPARSE_BAD_ARG,       /* Invalid argument                             */
   OPTPARSE_EEXIST,        /* Option exists                                */
   OPTPARSE_NOT_IMPL,      /* Not implemented                              */
} optparse_err_t;

/*
 *  Set of item types for optparse_get and optparse_set:
 */
typedef enum {
    OPTPARSE_USAGE,        /* Set usage message in --help output (char *)   */
    OPTPARSE_LOG_FN,       /* Set log function (default fprintf(stderr,..)) */
    OPTPARSE_OPTION_WIDTH, /* Width allotted to options in --help output    */
    OPTPARSE_LEFT_MARGIN,  /* Left pad for option output (default = 2)      */
} optparse_item_t;

/*
 *  Description of an option:
 */
struct optparse_option {
    const char *  name;    /*  Option name (e.g. "help" for --help)         */
    int           key;     /*  Option key  (e.g. 'h', or other number).
				If !isalnum(key), then this option is
                                assumed to be a long option only.           */

    int           has_arg; /*  0: no arg, 1: req'd arg, 2: optional arg     */
    int           group;   /*  Grouping in --help output                    */
    const char *  arginfo; /*  arg info displayed after = in help output    */
    const char *  usage;   /*  String for usage/help output                 */
    optparse_cb_f cb;      /*  Callback function                            */
    void *        arg;     /*  Optional argument passed back to callback    */
};

#define OPTPARSE_TABLE_END { NULL, 0, 0, 0, NULL, NULL, NULL, NULL }

/******************************************************************************
 *  Prototypes:
 *****************************************************************************/

/*
 *   Create an optparse object for program named [program_name]
 */
optparse_t optparse_create (const char *program_name);

/*
 *   Destroy program options handle [p].
 */
void optparse_destroy (optparse_t p);

/*
 *   Register the option [o] with the program options object [p].
 *
 *   Returns OPTPARSE_SUCCESS if the option was successfully registered
 *    with the parser [p]. Otherwise, one of the following error codes
 *    is returned:
 *
 *    OPTPARSE_NOMEM:  Failed to allocate memory for some options.
 *    OPTPARSE_EEXIST: An attempt to register a duplicate option was detected.
 *    OPTPARSE_EINVAL: The optparse_t object is currupt or invalid.
 */
optparse_err_t optparse_add_option (optparse_t p,
                                    const struct optparse_option *o);

/*
 *   Remove option [name] from parser [p].
 */
optparse_err_t optparse_remove_option (optparse_t p, const char *name);

/*
 *   Register all program options in table [opts] to the program options
 *    object [p]. [opts] should be an array of struct optparse_option
 *    and the last entry should be PROG_OPTS_TABLE_END.
 *
 *   Returns OPTPARSE_SUCCESS if all options are successfully registered
 *    with the parser [p]. Otherwise returns an error as in
 *    optparse_add_option().
 */
optparse_err_t optparse_add_option_table (optparse_t p,
	                                  struct optparse_option const opts[]);

/*
 *   Register a doc string [doc] for display in autogenerated --help output
 *    for program options object [p]. The doc string will preceed the
 *    option output for group [group].
 */
optparse_err_t optparse_add_doc (optparse_t p, const char *doc, int group);

optparse_err_t optparse_set (optparse_t p, optparse_item_t item, ...);

optparse_err_t optparse_get (optparse_t p, optparse_item_t item, ...);

/*
 *   Print the usage output for program options object [p] using the
 *    registered output function.
 */
int optparse_print_help (optparse_t p);

/*
 *   Process command line args in [argc] and [argv] using the options
 *    defined in program options object [p]. This will result in callbacks
 *    being called as options are parsed, or option and argument usage
 *    can be checked after successful completion with
 *    optparse_options_getopt() as defined below.
 *
 *   Returns -1 on failure, first non-option index in argv on success.
 */
int optparse_parse_args (optparse_t p, int argc, char *argv[]);

/*
 *   After a call to optparse_parse_args (), return the number of times the
 *     option 'name' was used, or 0 if not. If the option was used and it takes
 *    an argument, then that argument is passed back in [optargp].
 */
int optparse_getopt (optparse_t p, const char *name, const char **optargp);

#endif /* _HAVE_OPTPARSE_H */
