#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ROWS 7
#define COLUMNS 7   

void initialize_board(char board[ROWS][COLUMNS]){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            board[i][j] = '.';
        }
    }
}

void print_board(char board[ROWS][COLUMNS]) {
    printf("   1   2   3   4   5   6   7\n");
    printf(" +---+---+---+---+---+---+---+\n");
    for (int i = 0; i < ROWS; i++){
        printf("%c", 'a' + i);
        for (int j = 0; j < COLUMNS; j++){
            printf("| %c ", board[i][j] == '.' ? ' ' : board[i][j]);
        }
        printf("|\n");
        printf(" +---+---+---+---+---+---+---+\n");
    }
}

int get_pieces_per_player(){
    int pieces, max_pieces = ROWS * COLUMNS / 2;
    printf("Enter the number of pieces per player (Max %d): \n", max_pieces);
    scanf("%d", &pieces);
    while (pieces < 1 || pieces > max_pieces){
        printf("Invalid number of pieces. Please try again.\n");
        printf("Enter the number of pieces per player (Max %d): \n", max_pieces);
        scanf("%d", &pieces);
    }
    return pieces;
    
}

void place_piece_randomly(char board[ROWS][COLUMNS],int pieces, char piece){
    int placed = 0;
    while (placed < pieces){
        int row = rand() % ROWS;
        int column = rand() % COLUMNS;

        if(board[row][column] == '.'){
            board[row][column] = piece;
            placed++;
        }
    }
}

void place_piece_manually(char board[ROWS][COLUMNS], char piece){
    char row;
    int column;

    printf("Enter the coordinate for %c piece: ", piece);
    scanf(" %c %d",&row, &column);

    while (row < 'a' || row > 'g' || column < 1 || column > 7 || board[row - 'a'][column - 1] != '.'){
        printf("Invalid coordinates. Please try again.\n");
        printf("Enter new coordinates %c: ", piece);
        scanf(" %c %d", &row, &column);
    }
    board[row - 'a'][column - 1] = piece;
}

void place_pieces(char board[ROWS][COLUMNS], int pieces, char piece, int choice){
    if (choice == 1){
        place_piece_randomly(board, pieces, piece);
    }
    else if (choice == 2){
        for (int i = 0; i < pieces; i++){
            place_piece_manually(board, piece);
        }
    }
    else{
        printf("Invalid choice. Please try again.");
    }   
}

void move_piece(char board[ROWS][COLUMNS], char player_piece){
    char from_row, to_row;
    int from_column, to_column;
    int valid_move = 0;
    
    while(!valid_move){
        printf("Choose piece to move: ");
        scanf(" %c%d", &from_row, &from_column);

        while(board[from_row - 'a'][from_column - 1] != player_piece){
            printf("Invalid piece. Please try again.\n");
            printf("Choose piece to move: ");
            scanf(" %c%d", &from_row, &from_column);
        }

        printf("Choose the new position for %c%d: ", from_row, from_column);
        scanf(" %c%d", &to_row, &to_column);

        if(from_row >= 'a' && from_row <= 'g' && to_row >= 'a' && to_row <= 'g' &&
           from_column >= 1 && from_column <= 7 && to_column >= 1 && to_column <= 7 &&
           abs(from_row - to_row) + abs(from_column - to_column) == 1 &&
           board[to_row - 'a'][to_column - 1] == '.'){
                board[from_row - 'a'][from_column - 1] = '.';
                board[to_row - 'a'][to_column - 1] = player_piece;
                valid_move = 1;
                printf("Player moves the piece at %c%d to %c%d\n", from_row, from_column, to_row, to_column);
           }
           else{
               printf("Invalid move. Please try again.\n");
           }
    }
}

int count_moves(char board[ROWS][COLUMNS], char piece){
    int move_count = 0;
    for(int row = 0; row <ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            if(board[row][col] == piece){
                if(row > 0 && board[row - 1][col] == '.'){
                    move_count++;
                }
                if(row < ROWS - 1 && board[row + 1][col] == '.'){
                    move_count++;
                }
                if(col > 0 && board[row][col - 1] == '.'){
                    move_count++;
                }
                if(col < COLUMNS - 1 && board[row][col + 1] == '.'){
                    move_count++;
                }  
            }
        }
    }
    return move_count;
}

void computer_move(char board[ROWS][COLUMNS], char computer_piece, char opponent_piece){
    int best_score = -1;
    int best_row = -1, best_col = -1, best_new_row = -1, best_new_col = -1;
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            if(board[row][col] == computer_piece){
                for(int dir = 0; dir < 4; dir++){
                    int new_row = row + directions[dir][0];
                    int new_col = col + directions[dir][1];

                    if(new_row >= 0 && new_row < ROWS && new_col >= 0 && new_col < COLUMNS && board[new_row][new_col] == '.'){
                        board[row][col] = '.';
                        board[new_row][new_col] = computer_piece;

                        int computer_moves = count_moves(board, computer_piece);
                        int opponent_moves = count_moves(board, opponent_piece);
                        int score = computer_moves - opponent_moves;

                        if(score > best_score){
                            best_score = score;
                            best_row = row;
                            best_col = col;
                            best_new_row = new_row;
                            best_new_col = new_col;
                        }

                        board[row][col] = computer_piece;
                        board[new_row][new_col] = '.';
                    }
                }
            }
        }
    }

    if(best_row != -1){
        board[best_row][best_col] = '.';
        board[best_new_row][best_new_col] = computer_piece;
        printf("Computer moves the piece at %c%d to %c%d\n", best_row + 'a', best_col + 1, best_new_row + 'a', best_new_col + 1);
    }
}

int is_game_over(char board[ROWS][COLUMNS], char player1_piece, char player2_piece){
    int player1_moves = count_moves(board, player1_piece);
    int player2_moves = count_moves(board, player2_piece);

    if(player1_moves == 0 || player2_moves == 0){
        if (player1_moves == 0 && player2_moves > 0){
            printf("Player 1 has no moves left. Player 2 wins!\n");
        }
        else if(player1_moves > 0 && player2_moves == 0){
            printf("Player 2 has no moves left. Player 1 wins!\n");
        }
        else{
            printf("Both players have no moves left. It's a tie!\n");
        }
        return 1;
    }
    return 0;
}

void determine_winner(char board[ROWS][COLUMNS]){
    int x_moves = count_moves(board, 'X');
    int o_moves = count_moves(board, 'O');

    if(x_moves == 0 && o_moves > 0){
        printf("Player 2 wins\n");
    }
    else if(x_moves > 0 && o_moves == 0){
        printf("Player 1 wins\n");
    }
    else if(x_moves > o_moves){
        printf("Player 1 wins with more available moves!\n");
    }
    else if(x_moves < o_moves){
        printf("Player 2 wins with more available moves!\n");
    }
    else{
        printf("It's a tie!\n");
    }
}

void quit_game_animation(){
    printf("Quitting game");
    fflush(stdout);

    for(int i = 0; i < 3; i++){
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
    printf("Goodbye!\n");
  
}

void print_slowly(const char *message, unsigned int delay){
    for(int i = 0; message[i] != '\0'; i++){
        printf("%c", message[i]);
        fflush(stdout);
        usleep(delay * 1000);
    }
    printf("\n");
}

void show_instructions(){
    const char *instructions = "Instructions:\n"
                               "The game is played on a 7x7 board.\n"
                               "Each player's pieces will be determined by the player at the beginning of the game.\n"
                               "The player can place the pieces randomly or manually.\n"
                               "The game has n turns, where n is the total number of moves determined by the player.\n"
                               "The players take turns moving their pieces.\n"
                               "A piece can be moved to an adjacent empty space.\n"
                               "The player who has no moves left or has less available moves at the end loses.\n";
    print_slowly(instructions, 50);
}

int main(){
    int choice;
    char board[ROWS][COLUMNS];
    int pieces_per_player = 0;
    char player_piece, computer_piece;
    int total_moves, current_move = 0;
    int moves_per_player;
    
    while (1){
        printf("\nMain Menu:\n");
        printf("1. Play\n");
        printf("2. Instructions\n");
        printf("3. Quit\n");
        printf("Choose option 1-3 then press enter");
        scanf("%d", &choice);

        switch (choice){
        case 1:
            srand(time(NULL));

            initialize_board(board);
            pieces_per_player = get_pieces_per_player();

            printf("Enter the total number of moves per player in the game: ");
            scanf("%d", &moves_per_player);
            int total_moves = moves_per_player * 2;

            current_move = 0;

            int player_choice;
            printf("Do you want to be Player 1 (X) or Player 2 (O)? Enter 1 for X, 2 for O: ");
            scanf("%d", &player_choice);
            player_piece = (player_choice == 1) ? 'X' : 'O';
            computer_piece = (player_piece == 'X') ? 'O' : 'X';

            int placement_choice;
            printf("How would you like to place your pieces?(1 for random, 2 for manual\n");
            scanf("%d", &placement_choice);

            place_pieces(board, pieces_per_player, 'X', placement_choice);
            place_pieces(board, pieces_per_player, 'O', placement_choice);

            print_board(board);

            while(current_move < total_moves){
                if (current_move % 2 == (player_choice - 1)){
                    printf("Your turn\n");
                    move_piece(board, player_piece);
                }
                else{
                    printf("Computer's turn\n");
                    computer_move(board, computer_piece, player_piece);

                }
                print_board(board);
                current_move++;

                if(is_game_over(board, player_piece, computer_piece)){
                    break;
                }
            }

            if(current_move == total_moves){
                determine_winner(board);
                printf("Moves available for X: %d\n", count_moves(board, 'X'));
                printf("Moves available for O: %d\n", count_moves(board, 'O'));
            } 

            break;
        case 2:
            show_instructions();
            break;
        case 3:
            quit_game_animation();
            exit(0);
        default:
            printf("Invalid option. Choose again");
        }

    }
    return 0;
}