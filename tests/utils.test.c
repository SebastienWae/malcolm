#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "../src/utils.h"

struct hextodec_params {
  char hex[32];
  long dec;
};

ParameterizedTestParameters(Utils, hextodec) {
  static struct hextodec_params params[] = {
      {
          .hex = "a",
          .dec = 10,
      },
      {
          .hex = "A",
          .dec = 10,
      },
      {
          .hex = "b",
          .dec = 11,
      },
      {
          .hex = "B",
          .dec = 11,
      },
      {
          .hex = "c",
          .dec = 12,
      },
      {
          .hex = "C",
          .dec = 12,
      },
      {
          .hex = "d",
          .dec = 13,
      },
      {
          .hex = "D",
          .dec = 13,
      },
      {
          .hex = "e",
          .dec = 14,
      },
      {
          .hex = "E",
          .dec = 14,
      },
      {
          .hex = "f",
          .dec = 15,
      },
      {
          .hex = "F",
          .dec = 15,
      },
      {
          .hex = "10",
          .dec = 16,
      },
      {
          .hex = "0",
          .dec = 0,
      },
      {
          .hex = "1",
          .dec = 1,
      },
      {
          .hex = "14A31d8C",
          .dec = 346234252,
      },
      {
          .hex = "D60f1",
          .dec = 876785,
      },
      {
          .hex = "7FFfFfFF",
          .dec = 2147483647,
      },
  };

  size_t nb_params = sizeof(params) / sizeof(struct hextodec_params);
  return cr_make_param_array(struct hextodec_params, params, nb_params);
}

#include <stdio.h>
ParameterizedTest(struct hextodec_params *param, Utils, hextodec) {
  long dec = 0;
  hextodec(param->hex, &dec, strlen(param->hex));
  cr_assert_eq(dec, param->dec);
}