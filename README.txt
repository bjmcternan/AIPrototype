Code Written by Brennan McTernan

Description:
A multi-threaded prototype


Requirements:
None

Notes:
This was a prototype done to explore the possibility of using threads to control data flow
One thread, timer, counts down and every 5 seconds posts a message, or "data", to another thread, receive, which
simulates a server sending a message to the program