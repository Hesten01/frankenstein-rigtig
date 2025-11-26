//
//  main.c
//  Frankentext
//
//  Created by noah helweg on 29/10/2025.
//
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_WORD_COUNT 50000
#define MAX_SUCCESSOR_COUNT (MAX_WORD_COUNT / 2)
#define BOOK_SIZE 50000

// i copied the code that makes the book readable by the file
// since the embed functionality dinte work because i am not using a c23
// compiler.

#include "pg84.h" /* generated from pg84.txt using xxd -i */

// char that holds the books size
char book[BOOK_SIZE];

/// Array of tokens registered so far
/// No duplicates are allowed.
char *tokens[MAX_WORD_COUNT];
/// `tokens`'s current size
size_t tokens_size = 0;

/// Array of successor tokens
/// One token can have many successor tokens. `succs[x]` corresponds to
/// `token[x]`'s successors.
/// We store directly tokens instead of token_ids, because we will directly
/// print them. If we wanted to delete the book, then it would make more sense
/// to store `token_id`s
char *succs[MAX_WORD_COUNT][MAX_SUCCESSOR_COUNT];
/// `succs`'s size
size_t succs_sizes[MAX_WORD_COUNT];

/// Overwrites non-printable characters in `book` with a space.
/// Non-printable characters may lead to duplicates like
/// `"\xefthe" != "the"` even both print `the`.
    /// your code here
void replace_non_printable_chars_with_space(void) {
    // replaces all of the non printable characters with spaces
    // it does so by checking though the the entire book array and looks for
    // non printable characters, it then replaces them with a space
    for (size_t i = 0; i < BOOK_SIZE; i++) {
        if (!isprint((char)book[i]))
            book[i] = ' ';
    }
}

/// Returns token id if token is known, otherwise creates a new entry and
/// returns the new token id
size_t token_id(char *token) {
  // YOUR CODE HERE
  // this function looks though all the exsisting tokens and compares them
  // to the input token if they are the same it returns the input token id
  // thereby making sure that is dosent create duplicate tokens
    size_t id;
    for (id = 0; id < tokens_size; id++) {
        if (strcmp(tokens[id], token) == 0) {
            return id;
        }
    }
    tokens[id] = token;
    tokens_size++;
    return id;
}

/// Appends a token to the successors list of a `token`
void append_to_succs(char *token, char *succ) {
    // YOUR CODE HERE 
    // the int id is used as a temporary variable that stores the token
    // that temporary token id get the index of the token in the tokens array
    // if the token exsists it returns its index value if not it creates a new token
    // with a new index value
    size_t id = token_id(token);
    // if the index was empty it will store the token is the successors array
    // this makes sure that each token has it own successor array
    // the & operator makes sure we get the address since we
    // wanna modify and update the value of the succ_size array
    size_t *empty_index = &succs_sizes[id];

    // takes the successor token and adds it to the original token sucessor array
    // it increments the sucessor array size counter so that each sucessor has its own
    // value
    succs[id][(*empty_index)++] = succ;
}

/// Creates tokens from the `book` and fills `tokens` and `succs` arrays using
/// the `token`s.
void tokenize_and_fill_succs(char *delimiters, char *str) {
  // YOUR CODE HERE
  // the char prev starts at 0, meaning it dosent point anywhere untill, it is given
  // a token
    char *prev = 0;
    // the char tok points to the "tokenizer" strtok. it splits the book into tokens
    // it uses the delimiters that are defined later in the main function
    char *tok = strtok(str, delimiters);
    // this while loop runs through the tokens created by strtok, if there are no tokens
    // left 
    while (tok != 0) {
        // gets the token index of the curren token
        (void)token_id(tok);
        // if there wasent any previous tokens it dosent link the token to anything
        // so for the first token it dosent create a sucessor link
        if (prev != 0) {
            append_to_succs(prev, tok);
        }
        // goes to the next token by taking the current token and storing it 
        // as the prev token
        prev = tok;
        // creates the next token by using strtok agian
        tok = strtok(NULL, delimiters);
    }
    
}

/// Returns last character of a string
char last_char(char *str) {
  // YOUR CODE HERE
  // get the length of the token string, if the length of the string is 0 it return a
  // null character
    size_t len = strlen(str);
    if (len == 0) {
        return '\0';
    }
    // it returns the last character of the string but removes the null character from
    // the string
    return str[len - 1];
}

/// Returns whether the token ends with `!`, `?` or `.`.
bool token_ends_a_sentence(char *token) {
  // YOUR CODE HERE
  // the char c takes the value of the last character in the token
    char c = last_char(token);
    // if that value is a . or ! or ? it returns the token itself as true. if not
    // it returns false
    if (c == '.' || c == '!' || c == '?') {
        return true;
    } else {
        return false;
    }
}

/// Returns a random `token_id` that corresponds to a `token` that starts with a
/// capital letter.
/// Uses `tokens`, `tokens_size.
size_t random_token_id_that_starts_a_sentence(void) {
    // YOUR CODE HERE
    // here it goes through all the tokens that start with an uppercase letter
    // it incrementes the uppercase_token variable each time i finds one
    size_t Uppercase_token = 0;
    for (size_t i = 0; i < tokens_size; ++i) {
        // creating a char that points to the token
        char *t = tokens[i];
        // if that char points to a token that isent a null character or a new line
        // but instead a uppercase letter it incrementes the variable
        if (t != NULL && t[0] != '\0' && isupper((char)t[0])) {
            ++Uppercase_token;
        }
    }

    // Picks a random token among those stored in Uppercase_token to start
    // the sentence. Rand produces a random number that is then modulated with
    // the uppercase_token value
    size_t target = (size_t)(rand() % Uppercase_token);

    // This for loop goes thourgh all the tokens agian and for each token with a
    // uppercase letter. It then checks if that token has the index value chosen
    // by the taget function
    for (size_t i = 0; i < tokens_size; i++) {
        // a char that points to the tokens
        char *t = tokens[i];
        // goes thourgh the tokens looking for the chosen uppercase token and returns
        // its index value
        if (t != NULL && t[0] != '\0' && isupper((char)t[0])) {
                return i;
            }
        }
        return 0;
    }

    
    /// Generates a random sentence using `tokens`, `succs`, and `succs_sizes`.
    /// The sentence array will be filled up to `sentence_size-1` characters using
    /// random tokens until:
    /// - a token is found where `token_ends_a_sentence()`
    /// - or more tokens cannot be concatenated to the `sentence` anymore.
    ///  Returns the filled sentence array.
    char *generate_sentence(char *sentence, size_t sentence_size) {
        size_t current_token_id = random_token_id_that_starts_a_sentence();
        char *token = tokens[current_token_id];

        sentence[0] = '\0';
        strcat(sentence, token);
        if (token_ends_a_sentence(token))
            return sentence;
        
        // Calculated sentence length for the next iteration.
        // Used to stop the loop if the length exceeds sentence size
        size_t sentence_len_next;
        // Concatenates random successors to the sentence as long as
        // `sentence` can hold them.
        do {
            // YOUR CODE HERE
            // if the token has no succession tokens the loop will break
            if (succs_sizes[current_token_id] == 0) {
                printf("There are no sucessors for this token\n");
                break;
            }
            // picks out a random token succesor
            size_t random_successor = (size_t)(rand() % succs_sizes[current_token_id]);
            char *next = succs[current_token_id][random_successor];
            // calculates the length of the next sentence and adds 1 to make room
            // for the space
            sentence_len_next = strlen(sentence) + 1 + strlen(next);
           
            // add a space and the next sentence
            strcat(sentence, " ");
            strcat(sentence, next);
            // advance to the next sentence
            current_token_id = token_id(next);
            
            // break out of the loop if the appended token ends the sentence
            if (token_ends_a_sentence(next)) {
                break;
            }
            // keeps the loop running while the sentence length is less 
            // than the maximum sentence size
        } while (sentence_len_next < sentence_size - 1);
        return sentence;
    }
    
    int main(void) {
        
        /* Copy embedded book data (from pg84.h) into the `book` buffer and
           ensure it is NUL-terminated. */
        size_t pg84_len = (size_t)pg84_txt_len;
        size_t to_copy = (pg84_len < BOOK_SIZE - 1) ? pg84_len : (BOOK_SIZE - 1);
        memcpy(book, pg84_txt, to_copy);
        book[to_copy] = '\0';

        replace_non_printable_chars_with_space();
        // defining the delimiters used to create the tokens
        char *delimiters = " \n\r";
        // creaates a token from the book and adds it to the token and sucessor array
        tokenize_and_fill_succs(delimiters, book);
        // defining the maximum sentence size
        char sentence[1000];
        // creates a random number based on the current time
        srand(time(NULL));
        
        // Initialize `sentence` and then find a question sentence.
        do {
            generate_sentence(sentence, sizeof sentence);
        } while (last_char(sentence) != '?');
        puts(sentence);
        puts("");
        
        // Initialize `sentence` and then find a sentence ending with exclamation
        // mark.
        do {
            generate_sentence(sentence, sizeof sentence);
        } while (last_char(sentence) != '!');
        puts(sentence);
    }

