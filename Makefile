# Project Structure
NAME	=	minitalk
SERVER	=	server
CLIENT	=	client
SRCDIR	=	srcs
OBJDIR	=	objs
INCDIR	=	incs
LIBDIR	=	libs
LIBFT	=	libft

# Files
SRCS_SERVER	:=	server.c
OBJS_SERVER	:=	$(SRCS_SERVER:.c=.o)
SRCS_SERVER	:=	$(addprefix $(SRCDIR)/, $(SRCS_SERVER))
OBJS_SERVER	:=	$(addprefix $(OBJDIR)/, $(OBJS_SERVER))
SRCS_CLIENT	:=	client.c
OBJS_CLIENT	:=	$(SRCS_CLIENT:.c=.o)
SRCS_CLIENT	:=	$(addprefix $(SRCDIR)/, $(SRCS_CLIENT))
OBJS_CLIENT	:=	$(addprefix $(OBJDIR)/, $(OBJS_CLIENT))

# Utils
RM	=	rm -rf

# Compiler Options
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
CLIBS	=	-L./$(LIBDIR) -lft
CINCS	=	-I./$(INCDIR) -I./$(LIBDIR)/$(LIBFT)
DEBUG	=	-g3 -gdwarf-4

all: $(NAME)

bonus: all

$(NAME): makelibs $(SERVER) $(CLIENT)

$(SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(SERVER) $(CLIBS)

$(CLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(CLIENT) $(CLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CINCS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

makelibs: $(LIBDIR)/libft.a

$(LIBDIR)/libft.a:
	make -C $(LIBDIR)/$(LIBFT)
	cp $(LIBDIR)/$(LIBFT)/libft.a $(LIBDIR)/libft.a

clean:
	$(RM) $(OBJS_SERVER)
	$(RM) $(OBJS_CLIENT)
	make -C $(LIBDIR)/$(LIBFT) clean

fclean: clean
	$(RM) $(SERVER)
	$(RM) $(CLIENT)

re: fclean all

.PHONY: all $(NAME) makelibs clean fclean re bonus
# clean fclean re bonus rules and phony
# makelibs rule
