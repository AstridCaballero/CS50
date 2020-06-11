#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_letter(string arr[]);

int main(int argc, string argv[])
{
    // argv[] is the array that holds the command line parameters, argv[1] is the one that has the key.
    //get key
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]); //argv[0] holds the name of the program
        return 1;
    }

    //check key only has letters
    int let = is_letter(argv);
    if (let == 1)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    //check length of key
    int len = strlen(argv[1]);
    if (len != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    //convert to lower case the values in argv[1]
    char lower_argv[len + 1];
    for (int m = 0; m < len; m++)
    {
        lower_argv[m] = tolower(argv[1][m]);
    }

    //check key's items are unique
    for (int p = 0; p < len; p++)
    {
        for (int z = p + 1; z < len; z++)
        {
            if (lower_argv[p] == lower_argv[z])
            {
                printf("Usage: %s key\n", argv[0]);
                return 1;
            }
        }
    }

    //get plaintext
    string new_plaintext = get_string("plaintext: ");
    int len_plain = strlen(new_plaintext);


    //scale the ascci values of the plaintext letters to 1 - 26
    int len_text = len_plain + 1;
    char scaled_plain[len_text];
    for (int i = 0; i < len_text; i++)
    {
        if (new_plaintext[i] >= 65 && new_plaintext[i] <= 90)
        {
            scaled_plain[i] = new_plaintext[i] - 64; //subtract 64 to scale from 1. if subtraction is 65 the scale starts at zero.
        }
        else if (new_plaintext[i] >= 97 && new_plaintext[i] <= 122)
        {
            scaled_plain[i] = new_plaintext[i] - 96;
        }
        else
        {
            scaled_plain[i] = new_plaintext[i];
        }
    }
    //take the scaled ascii value in plain text ie: H which is 7 and find the 8th position in key
    char ciphertext[len_text];
    for (int j = 0; j < len_text; j++)
    {
        int k = (scaled_plain[j]); //get value in position j and make it integer
        if (k >= 1 && k <= 26)
        {
            k = k - 1; //get the correct index
            ciphertext[j] = lower_argv[k]; //go to index K and save the value in chyphertext
        }
        else
        {
            //if plaintext value is not a letter then copy original value in chypertext
            ciphertext[j] = scaled_plain[j];
        }


    }

    //make chypertext match the case of plaintext
    for (int n = 0; n < len_text; n++)
    {
        if (isupper(new_plaintext[n]))
        {
            //if plaintext was uppercase then take the lowercase value and substract 32 to make it uppercase.
            ciphertext[n] = ciphertext[n] - 32;
        }
        else
        {
            ciphertext[n] = ciphertext[n];
        }
    }

    //print chyphertext
    //int x = strlen(ciphertext);
    printf("ciphertext: %s\n", ciphertext);
    //printf("%i\n", x);
    return 0;
}


//Checks if input only has letters
int is_letter(string arr[])
{
    int l = strlen(arr[1]);
    for (int i = 0; i < l; i++)
    {
        if (isalpha(arr[1][i]) == false)
        {
            return 1;
            break;
        }
    }
    return 0;
}



