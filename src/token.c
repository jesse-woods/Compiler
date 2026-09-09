#include "token.h"

#include <stdlib.h>
//
// Created by Jesse Woods on 9/7/26.
//
void free_token(Token* token) {
    if (token != nullptr) {
        free_slice(token->token);
        free(token);
    }
}