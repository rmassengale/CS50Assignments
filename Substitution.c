#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char upper;
    char lower;
} cipher_values;

bool only_letters(string _cipherkey);
bool no_repeat_characters(string _cipher_key_);
bool validate_key(int argcount, string cipherkey);
void encipher_text(string plaintext_, string cipherkey_);

int main(int argc, string argv[])
{
    if (validate_key(argc, argv[1]) == true)
    {
        string plaintext = get_string("plaintext: ");
        string cipherkey = argv[1];

        encipher_text(plaintext, cipherkey);
        return 0;
    }
    else
    {
        return 1;
    }
}


bool only_letters(string _cipherkey)
{
    int len = strlen(_cipherkey);
    for (int i = 0; i < len; i++)
    {
        if (isalpha(_cipherkey[i]))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool no_repeat_characters(string _cipher_key_)
{
    // Initialize an array to track letters
    bool track_letters[26] = {false};
    int _cipher_key_len = strlen(_cipher_key_);
    int index;

    for (int i = 0; i < _cipher_key_len; i++)
    {
        // verify proposed_key is only alphabetical
        if (!isalpha(_cipher_key_[i]))
        {
            // if character not alpha; return false
            return false;
        }

        // verify there are no repeating characters
        if (isupper(_cipher_key_[i]))
        {
            index = _cipher_key_[i] - 'A';
        }
        else if (islower(_cipher_key_[i]))
        {
            index = _cipher_key_[i] - 'a';
        }

        if (track_letters[index])
        {
            // if character already seen
            return false;
        }
        // If not seen, mark as seen
        track_letters[index] = true;
    }
    // if proposed key is only alphabetical and has no repeating characters, the key is valid;
    return true;
}

bool validate_key(int argcount, string cipher_key)
{
    if (argcount != 2)
    {
        printf("Usage: ./substitution key\n");
        return false;
    }
    else if (strlen(cipher_key) != 26)
    {
        printf("key must contain 26 characters...\n");
        return false;
    }
    else if (!only_letters(cipher_key))
    {
        printf("key must contain only letters...\n");
        return false;
    }
    else if (!no_repeat_characters(cipher_key))
    {
        printf("key must not contain repeat letters...\n");
        return false;
    }
    return true;
}

void encipher_text(string plaintext_, string cipherkey_)
{
    int plaintext_len = strlen(plaintext_);
    int cipherkey_len = strlen(cipherkey_);
    cipher_values values[cipherkey_len];

    for (int i = 0; i < cipherkey_len; i++)
    {
        if (isupper(cipherkey_[i]))
        {
            values[i].upper = cipherkey_[i];
            values[i].lower = tolower(cipherkey_[i]);
        }
        else if (islower(cipherkey_[i]))
        {
            values[i].upper = toupper(cipherkey_[i]);
            values[i].lower = cipherkey_[i];
        }
    }

    printf("ciphertext: ");
    for (int j = 0; j < plaintext_len; j++)
    {
        if (isupper(plaintext_[j]))
        {
            plaintext_[j] = values[(plaintext_[j] - 'A')].upper;
            printf("%c", plaintext_[j]);
        }
        else if (islower(plaintext_[j]))
        {
            plaintext_[j] = values[(plaintext_[j] - 'a')].lower;
            printf("%c", plaintext_[j]);
        }
        else
        {
            plaintext_[j] = plaintext_[j];
            printf("%c", plaintext_[j]);
        }
    }
    printf("\n");
}
