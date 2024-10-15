import random

def create_unique_numbers_file(filename, n):
    numbers = list(range(n))
    random.shuffle(numbers) # Перемешиваем последовательность для случайного порядка
    with open(filename, 'w') as f:
        for num in numbers:
            f.write(f"{num}\n")

n = 10**7
create_unique_numbers_file("unique_numbers.txt", n)