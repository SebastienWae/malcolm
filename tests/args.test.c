#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>
#include <string.h>

#include "../src/args.h"

struct args_params {
  int argc;
  char **argv;
  bool success;
  mm_args_t args;
};

void cleanup_params(struct criterion_test_params *ctp) {
  struct args_params *param = ctp->params;
  for (int i = 0; i < param->argc; i++) {
    cr_free(param->argv[i]);
  }
}

ParameterizedTestParameters(Args, parse) {
  static struct args_params params[10];

  /* ----------------------------- no arguments ----------------------------- */
  params[0].argc = 1;
  params[0].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[0].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[0].argv[0], str);
  }
  params[0].success = false;

  /* --------------------------- missing source ip -------------------------- */
  params[1].argc = 4;
  params[1].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[1].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[1].argv[0], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[1].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[1].argv[1], str);
  }
  {
    char *str = "1.1.1.1";
    params[1].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[1].argv[2], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[1].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[1].argv[3], str);
  }
  params[1].success = false;

  /* -------------------------- missing source mac -------------------------- */
  params[2].argc = 4;
  params[2].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[2].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[2].argv[0], str);
  }
  {
    char *str = "1.1.1.1";
    params[2].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[2].argv[1], str);
  }
  {
    char *str = "1.1.1.1";
    params[2].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[2].argv[2], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[2].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[2].argv[3], str);
  }
  params[2].success = false;

  /* --------------------------- missing target ip -------------------------- */
  params[3].argc = 4;
  params[3].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[3].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[3].argv[0], str);
  }
  {
    char *str = "1.1.1.1";
    params[3].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[3].argv[1], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[3].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[3].argv[2], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[3].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[3].argv[3], str);
  }
  params[3].success = false;

  /* -------------------------- missing target mac -------------------------- */
  params[4].argc = 4;
  params[4].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[4].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[4].argv[0], str);
  }
  {
    char *str = "1.1.1.1";
    params[4].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[4].argv[1], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[4].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[4].argv[2], str);
  }
  {
    char *str = "1.1.1.1";
    params[4].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[4].argv[3], str);
  }
  params[4].success = false;

  /* --------------------------------- valid -------------------------------- */
  params[5].argc = 5;
  params[5].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[5].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[4].argv[0], str);
  }
  {
    char *str = "1.1.1.1";
    params[5].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[5].argv[1], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[5].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[5].argv[2], str);
  }
  {
    char *str = "1.1.1.1";
    params[5].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[5].argv[3], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[5].argv[4] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[5].argv[4], str);
  }
  params[5].success = true;
  params[5].args = (mm_args_t){
      .source_ipv4 = 0x1010101,
      .source_mac_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
      .target_ipv4 = 0x1010101,
      .target_mac_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
      .verbose = false,
  };

  /* -------------------------------- options ------------------------------- */
  params[6].argc = 6;
  params[6].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[6].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[6].argv[0], str);
  }
  {
    char *str = "-v";
    params[6].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[6].argv[1], str);
  }
  {
    char *str = "1.1.1.1";
    params[6].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[6].argv[2], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[6].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[6].argv[3], str);
  }
  {
    char *str = "1.1.1.1";
    params[6].argv[4] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[6].argv[4], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[6].argv[5] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[6].argv[5], str);
  }
  params[6].success = true;
  params[6].args = (mm_args_t){
      .source_ipv4 = 0x1010101,
      .source_mac_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
      .target_ipv4 = 0x1010101,
      .target_mac_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
      .verbose = true,
  };

  /* ---------------------------- unknown option ---------------------------- */
  params[7].argc = 6;
  params[7].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[7].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[7].argv[0], str);
  }
  {
    char *str = "-test";
    params[7].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[7].argv[1], str);
  }
  {
    char *str = "1.1.1.1";
    params[7].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[7].argv[2], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[7].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[7].argv[3], str);
  }
  {
    char *str = "1.1.1.1";
    params[7].argv[4] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[7].argv[4], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[7].argv[5] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[7].argv[5], str);
  }
  params[7].success = false;

  /* ------------------------------ extra args ------------------------------ */
  params[8].argc = 6;
  params[8].argv = cr_calloc(params[0].argc, sizeof(char *));
  {
    char *str = "test";
    params[8].argv[0] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[8].argv[0], str);
  }
  {
    char *str = "1.1.1.1";
    params[8].argv[1] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[8].argv[1], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[8].argv[2] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[8].argv[2], str);
  }
  {
    char *str = "1.1.1.1";
    params[8].argv[3] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[8].argv[3], str);
  }
  {
    char *str = "ff:ff:ff:ff:ff:ff";
    params[8].argv[4] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[8].argv[4], str);
  }
  {
    char *str = "test";
    params[8].argv[5] = cr_calloc(strlen(str), sizeof(char));
    strcpy(params[8].argv[5], str);
  }
  params[8].success = false;

  size_t nb_params = sizeof(params) / sizeof(struct args_params);
  return cr_make_param_array(struct args_params, params, nb_params,
                             cleanup_params);
}

ParameterizedTest(struct args_params *param, Args, parse) {
  mm_args_t args;

  bool res = parse_args(&args, param->argc, param->argv);

  cr_assert_eq(res, param->success);
  if (param->success) {
    cr_assert_eq(args.source_ipv4, param->args.source_ipv4);
    cr_assert_arr_eq(args.source_mac_address, param->args.source_mac_address,
                     6);
    cr_assert_eq(args.target_ipv4, param->args.target_ipv4);
    cr_assert_arr_eq(args.target_mac_address, param->args.target_mac_address,
                     6);
    cr_assert_eq(args.verbose, param->args.verbose);
  }
}