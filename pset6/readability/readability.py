import cs50
import math
import string
import re


def main():
    # get text
    new_text = cs50.get_string("Text: ")
    # count total letters
    new_letters = count_letters(new_text)
    # count total words
    new_words = count_words(new_text)
    # count total sentences based on period(.) exclamation mark(!) and question mark(?)
    new_sentences = count_sentences(new_text)
    # Print index
    new_index = coleman_liau_index(new_letters, new_words, new_sentences)
    if new_index < 1:
        print("Before Grade 1")
    elif new_index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {new_index}")


def count_letters(text):
    # count only letters
    count_lett = 0
    for i in range(len(text)):
        if re.search("[a-zA-Z]", text[i]):
            count_lett += 1
    return count_lett


def count_words(text):
    # count only words
    count_word = 0
    # find a space, which means before the space there was a word.
    for i in range(len(text)):
        if re.search("\s", text[i]):
            count_word += 1
    # there is not a space after he last word so lets add 1 to include that word in the count.
    return (count_word + 1)


def count_sentences(text):
    # count only sentences
    count_sent = 0
    for i in range(len(text)):
        if re.search("[.!?]", text[i]):
            count_sent += 1
    return count_sent


def coleman_liau_index(l, w, s):
    L = (100 * l) / w
    S = (100 * s) / w
    index = round((0.0588 * L) - (0.296 * S) - 15.8)
    return index


main()