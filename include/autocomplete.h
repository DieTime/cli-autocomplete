#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

char **tokenize(char *str, char delimiter);

void free_tokens(char **tokens);

#endif //AUTOCOMPLETE_H
