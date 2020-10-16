# COM S-327-2020-BP

This repository was created by Ben Purdy, for COM S 327 Spring 2020.

Re did the entire project 

Takes in arge hanndles switchs. Then parses the file. 

then calls the DFS. Inturn DFS calls DFSUtil and this method check if the game is winnable. 
DFSUtil calls DFS_table and DFS_stock which handles all states and calls DFSUtil  
when they find a valid move. 

DFS_table check for a foundation move then checks for a table move. when found they call DFSUtil on the new game state
DFS_stock checks card flips, stock flips, stock to foundation and stock to table flips. when found they call DFSUtil on the new game state


there is a hash map implemented for the table top 
this is importent becuase if