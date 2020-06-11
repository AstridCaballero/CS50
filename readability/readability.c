#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int coleman_liau_Index(int letter, int word, int sentence);

int main(void)
{
    //get text
    string new_text = get_string("Text: ");

    //count total letters
    int new_letter = count_letters(new_text);

    //count total words
    int new_word = count_words(new_text);

    //count total sentences based on period(.) exclamation mark(!) and question mark(?)
    int new_sentence = count_sentences(new_text);

    //print index
    int new_index = coleman_liau_Index(new_letter, new_word, new_sentence);
    if (new_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (new_index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", new_index);
    }

}

//count only letters
int count_letters(string text)
{
    float count_lett = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            count_lett ++;
        }
    }
    return count_lett;
}

//count words
int count_words(string text)
{
    float count_word = 0;
    //find a space, which means before the space there was a word.
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == ' ')
        {
            count_word ++;
        }
    }
    //there is not a space after he last word so lets add 1 to include that word in the count.
    return (count_word + 1);
}

//count sentences
int count_sentences(string text)
{
    float count_sent = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i]  == '?')
        {
            count_sent ++;
        }
    }
    return count_sent;
}

// calculate the Coleman-Liau index

int coleman_liau_Index(int letter, int word, int sentence)
{
    //to get a float L and a float S the variables have to be float
    float f_letter = letter;
    float f_word = word;
    float f_sentence = sentence;

    float L = (100 * f_letter) / f_word;
    float S = (100 * f_sentence) / f_word;
    float index = round((0.0588 * L) - (0.296 * S) - 15.8);
    return index;
}
