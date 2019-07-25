# Kalah
Kalah is a popular board game in Africa and Asia played between two players, North and South. There are a number of pits (this can change) and a pot for each player. I also implemented a "SmartPlayer" AI in this game, who will select the best move based on the minimax algorithm.
The rules of the game are as follows: 
1) On player's turn, remove all stones from one of the player's small pits and sow them into other pits counterclockwise around the board.
2) Sowing begins to the right of the source pOt. It include the player's own Kalah (pot), it also includes the opponents's small pits. However, it does not include the opponent's Kalah.
3) Once the player's beans end up in either of the Kalahs, the beans are no longer considered to be in play.
4) The player gets to make another turn if the beans he just sowed end up in his own Kalah.
5) A capture move is made when the player's last bean end up in a pit that isi intially empty. The capture will allow the player to transfer that particular bean and all the beans in the opponent's pit opposite to this particular pit to the player's Kalah
6) The game ends when either side has no beans in their pits. The player with the most beans in his/her Kalah wins the game.

