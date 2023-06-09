from typing import List, Dict, Optional
from string import ascii_lowercase
from functools import reduce


n = 0
MAX_VOCABULARY_LIST_SIZE = 100000
MAX_LARGE_COVERAGE_STAGE_RATIO = 0.5


class Constraints():
    def __init__(self) -> None:
        self.in_answer_letters = set()
        self.not_in_answer_letters = set()
        self.in_pos_letters = dict()
        self.not_in_pos_letters = set()

    def is_valid_word(self, word: str) -> bool:
        word_set = set(word)
        return len(word) == n and all(
            letter in word_set for letter in self.in_answer_letters) and all(
            letter not in word_set for letter in self.not_in_answer_letters) and all(
            word[pos] == letter for pos, letter in self.in_pos_letters.items()) and all(
            word[pos] != letter for pos, letter in self.not_in_pos_letters)


def get_letter_count(words: List[str]) -> List[Dict[str, int]]:
    letter_count = [{letter: 0 for letter in ascii_lowercase} for _ in range(n)]
    for word in words:
        for i, letter in enumerate(word):
            letter_count[i][letter] += 1
    return letter_count


def get_best_word(words: List[str]) -> str:
    letter_count = get_letter_count(words)
    words_size = len(words)

    def get_word_score(word: str):
        score = 1.0
        for i, letter in enumerate(word):
            score *= letter_count[i][letter] / words_size
        return score

    return max([(word, get_word_score(word)) for word in words], key=lambda x: x[1])[0]


def get_large_coverage_word(words: List[str], constraints: Constraints) -> Optional[str]:
    letter_count = get_letter_count(words)
    words_size = len(words)

    def get_word_score(word: str):
        if not constraints.is_valid_word(word):
            return -114514
        score = 1.0
        for i, letter in enumerate(word):
            score *= letter_count[i][letter] / words_size
        return score - len(word) + len(set(word))

    word, score = max([(word, get_word_score(word)) for word in words], key=lambda x: x[1])
    if score < -len(word):
        return None
    return word


def main():
    global n
    n = int(input())

    words = []
    with open('count_1w.txt') as file:
        for l in file.readlines():
            word = l.split()[0]
            if len(word) == n:
                words.append(word)
                if len(words) > MAX_VOCABULARY_LIST_SIZE:
                    break
    words = list(set(words))

    tot_round = n + 1
    constraints = Constraints()
    round = 0
    large_coverage_constraints = Constraints()
    large_coverage_searching = True
    while True:
        round += 1
        print(f'Round {round}: ')

        if large_coverage_searching is True:
            if round > tot_round * MAX_LARGE_COVERAGE_STAGE_RATIO:
                large_coverage_searching = False
            else:
                large_coverage_word = get_large_coverage_word(words, large_coverage_constraints)
                if large_coverage_word is None:
                    large_coverage_searching = False
                else:
                    large_coverage_constraints.not_in_answer_letters.update(set(large_coverage_word))
                    word = large_coverage_word

        if large_coverage_searching is False:
            words = list(filter(constraints.is_valid_word, words))
            if len(words):
                print(f'{len(words)} valid words')
            else:
                print('No valid words. Exit.')
                exit()

            word = get_best_word(words)

        print(f"My answer: {word}")

        def get_result() -> bool:
            result = input("Result(b or y or g): ")
            if result == "unknown":
                round -= 1
                words.remove(word)
                return True
            if len(result) != n:
                return False
            if any(i not in ['b', 'y', 'g'] for i in result):
                return False
            for i, res in enumerate(result):
                if res == 'b':
                    constraints.not_in_answer_letters.add(word[i])
                elif res == 'y':
                    constraints.in_answer_letters.add(word[i])
                    constraints.not_in_pos_letters.add((i, word[i]))
                elif res == 'g':
                    constraints.in_pos_letters[i] = word[i]
            return True

        while get_result() is False:
            print("result is error. Retry")


if __name__ == '__main__':
    main()
