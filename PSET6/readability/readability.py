from cs50 import get_string


def main():
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    #print(f"letters: {letters}, words: {words}, sentences: {sentences}")
    L = (letters / (words / 100))
    S = (sentences / (words / 100))
    index = round(0.0588 * L - 0.296 * S - 15.8)
    #print(f"L: {L}, S: {S}, index: {index}")
    if index < 1:
        print("Before Grade 1")
    elif 1 <= index and index <= 15:
        print(f"Grade {index}")
    else:
        print("Grade 16+")


# Counts the amount of letters in the text
def count_letters(text):
    letters = 0
    for letter in text:
        if letter.isalpha():
            letters += 1
    return letters


# Counts the amount of words
def count_words(text):
    words = 1
    for letter in text:
        if letter == " ":
            words += 1
    return words


# Counts the amount of sentences
def count_sentences(text):
    sentences = 0
    for letter in text:
        if letter == "!" or letter == "?" or letter == ".":
            sentences += 1
    return sentences


main()