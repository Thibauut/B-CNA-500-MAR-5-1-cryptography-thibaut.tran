##
## EPITECH PROJECT, 2023
## B-CNA-500-MAR-5-1-cryptography-thibaut.tran
## File description:
## Makefile
##

# SRC	=

NAME	=	mypgp

all:	$(NAME)

$(NAME):
	g++ src/main.cpp -o $(NAME) -g3 -std=c++17 -lgmpxx -lgmp


clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re:	fclean all