import socket
import sys
import select

SERVER = ("localhost", 2434)
MAX_TRIALS = 10

client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

p_input = [client_sock, sys.stdin]

# statuses
status = "unregistered"
player = "unregistered"
opponent = ""
board = []
piece = ""
piece2 = ""


def server_response():
    client_sock.sendto(b"OK", SERVER)


def c2c_response(msg):
    response = "OK$" + \
        msg.split('$')[1].split(';')[1] + ";" + msg.split('$')[1].split(';')[0]
    client_sock.sendto(response.encode(), SERVER)


def update_status(st):
    global status
    status = st


def update_opponent(opp):
    global opponent
    opponent = opp


def p_register(msg):
    if (len(msg) != 2):
        msg.append(socket.gethostname())
    if (len(msg[1]) < 1):
        msg[1] = socket.gethostname()
    result = p_outbound(msg[0] + '$' + msg[1])

    if result == "OK":
        update_status("available")
        global player
        player = msg[1]
        print("Player registered: " + player)
    else:
        print("Player registration failed: " + result)


def unregister(msg):
    result = p_outbound(msg[0])

    if result == "OK":
        update_status("unregistered")
        global player
        player = ""
        print("Player unregistered")
    else:
        print("Player unregistration failed: " + result)


def req_list():
    t = 1
    client_sock.settimeout(3)
    reply = ""

    while t < MAX_TRIALS:
        try:
            client_sock.sendto(b"list", SERVER)
            (reply, _) = client_sock.recvfrom(1024)
            reply = reply.decode()
            server_response()
            break
        except socket.timeout:
            t += 1

    client_sock.settimeout(None)

    if t == MAX_TRIALS:
        print("Error: server not responding")
    else:

        print("PLAYER LIST: \n")
        print("Name \t\t\tStatus")
        reply = reply.split('$')[1].split(', ')
        i = 0
        if len(reply[i].split(':')) == 1:
            print("Empty")
            return
        while i < len(reply):
            print(str(reply[i].split(':')[0]).replace('\'', '') +
                  "\t\t\t" + str(reply[i].split(':')[1]).replace('\'', ''))
            i += 1
    print("\n")


def invite(msg):
    inv_msg = msg[0] + '$' + player + ';' + msg[1]

    result = p_outbound(inv_msg)

    if result == "OK":
        print("Waiting for opponent's response...")

        (reply, _) = client_sock.recvfrom(1024)
        reply = reply.decode()
        if reply.split('$')[0] == "invR" and reply.split('$')[1].split(';')[0] == 'Y':
            p_outbound("busy")
            c2c_response("OK$" + msg[1] + ';' + player)

            update_status("busy")
            update_opponent(msg[1])
            print("Invitation accepted")

            global piece
            piece = "O"
            global piece2
            piece2 = "X"

            start_game()
            turn()
        elif reply.split('$')[0] == "invR" and reply.split('$')[1].split(';')[0] == 'N':
            c2c_response("OK$" + msg[1] + ";" + player)
            print("Invitation not accepted")
            return
        else:
            print(result)
    else:
        print(result)


def invite_reply(msg):
    inv_msg = msg[1].split(';')

    print(inv_msg[0] +
          " is inviting you to play.\nAccept the invitation? ([Y]/N)")

    choice = sys.stdin.readline()
    if choice == "Y\n" or choice == "y\n" or choice == "\n":
        result = p_outbound("invR$Y;" + inv_msg[1] + ";" + inv_msg[0])
        if result == "OK":
            p_outbound("busy")
            update_status("busy")
            update_opponent(inv_msg[0])
            print("Invitation accepted")

            global piece
            piece = "X"
            global piece2
            piece2 = "O"
            start_game()
            print_board()
            turn_wait()
        else:
            print(result)
            return
    elif choice == "N\n" or choice == "n\n":
        p_outbound("invR$N;" + inv_msg[1] + ";" + inv_msg[0])
        return
    return


def start_game():
    global board
    board = []
    dummy = 0
    while dummy < 9:
        board.append(dummy)
        dummy += 1


def print_board():
    print("\n")
    print(" " + str(board[0]) + " | " + str(board[1]) + " | " + str(board[2]))
    print("---+---+---")
    print(" " + str(board[3]) + " | " + str(board[4]) + " | " + str(board[5]))
    print("---+---+---")
    print(" " + str(board[6]) + " | " + str(board[7]) + " | " + str(board[8]))
    print("\n")


def turn():
    while True:
        print("It's your turn.\n")
        print_board()
        print("Enter your move (0-8)\nEnter 9 to give up: ")
        move = sys.stdin.readline()
        try:
            place = int(move)
            play(place)
            return
        except:
            print("Invalid move!")


def play(p):
    if p == 9:
        result = p_outbound("quit$" + player + ";" + opponent)
        update_status("available")
        update_opponent("")
        p_outbound("available")
        print("You've given up. Quitting...")
        return "quit"

    result = p_outbound("play$" + player + ";" + opponent + ";" + str(p))

    if result == "OK":
        board[p] = piece
        print_board()
        result = turn_wait()

        if result == "quit":
            p_outbound("available")
            update_status("available")
            return
    else:
        print(result)
        turn()
        return


def turn_wait():
    game_state = 0

    print("Waiting for the other player to make a move...")
    while True:
        if game_state == 1 or game_state == 2:
            print_board()
            if game_state == 1:
                print("Lost!")
            if game_state == 2:
                print("Draw!")
            print("Quitting game...")
            update_status("available")
            break

        (result, _) = client_sock.recvfrom(1024)
        result = result.decode()
        if result.split('$')[0] == "quit":
            c2c_response(result)
            print("The other player has given up. \nQuitting game...")
            p_outbound("available$" + result.split('$')[1])
            update_status("available")
            break

        if result.split('$')[0] == "end":
            c2c_response(result)
            print("Game ended. " + result.split('$')[1].split(';')[2])
            print("quitting game...")
            p_outbound("available")
            update_status("available")

            break

        place = int(result.split('$')[1].split(';')[2])

        if result.split('$')[0] == "play":
            if place > 9 or board[place] == "X" or board[place] == "O":
                client_sock.sendto(b"Invalid move", SERVER)
                continue
            else:
                board[place] = piece2
                send = "OK$" + player + ";" + opponent
                client_sock.sendto(send.encode(), SERVER)
                game_state = winning()
                if game_state == 1:
                    p_outbound("end$" + player + ";" + opponent + ";" + "Win!")
                    continue
                elif game_state == 2:
                    p_outbound("end$" + player + ";" +
                               opponent + ";" + "Draw!")
                    continue
                elif game_state == 0:
                    turn()
        else:
            client_sock.sendto(b"available", SERVER)
            start_game()
            print("Abrupting...")
        return "quit"


def winning():
    if board[0] == board[1] == board[2]:
        return 1
    elif board[3] == board[4] == board[5]:
        return 1
    elif board[6] == board[7] == board[8]:
        return 1
    elif board[0] == board[3] == board[6]:
        return 1
    elif board[1] == board[4] == board[7]:
        return 1
    elif board[2] == board[5] == board[8]:
        return 1
    elif board[0] == board[4] == board[8]:
        return 1
    elif board[2] == board[4] == board[6]:
        return 1
    elif board.count("O") == 5:
        return 2
    else:
        return 0


def p_outbound(msg):
    t = 1
    reply = ""

    client_sock.settimeout(1)
    while t < MAX_TRIALS:
        try:
            client_sock.sendto(msg.encode('UTF-8'), SERVER)
            (reply, _) = client_sock.recvfrom(1024)
            reply = reply.decode()
            break
        except socket.timeout:
            t += 1
    client_sock.settimeout(None)
    if t == MAX_TRIALS:
        reply = "Error: server not responding"
    elif reply == "OK":
        return "OK"
    elif reply.split('$')[0] == "NOK":
        return reply.split('$')[1]
    elif reply.split('$')[0] == "OK":
        return "OK"


while True:
    print("[" + player + "] Input message to server below. (Type 'help' for help)")
    inp, out, ex = select.select(p_input, [], [])
    for i in inp:
        if i == sys.stdin:
            msg = sys.stdin.readline().replace('\n', '').split(' ')
            if msg[0] == "register":
                if status != "unregistered":
                    print("ERROR: You're already registered with the server")
                else:
                    p_register(msg)
            elif msg[0] == "unregister":
                if status == "unregistered":
                    print("ERROR: You're not registered with the server")
                elif status == "busy":
                    print("ERROR: You must finish your game first")
                else:
                    unregister(msg)
            elif msg[0] == "list":
                print("Requesting list...")
                req_list()
            elif msg[0] == "invite":
                if status == "unregistered":
                    print("ERROR: You're not registered with the server")
                elif status == "busy":
                    print("ERROR: You must finish your game first")
                else:
                    invite(msg)
            elif msg[0] == "help":
                print("register - register with the server")
                print("unregister - unregister from the server")
                print("list - list all players")
                print("invite <player> - invite a player to play with you")
                print("quit - quit the game")
            elif msg[0] == "quit":
                if status != "unregistered":
                    msg[0] = "unregister"
                    unregister(msg)
                quit()

        elif i == client_sock:
            (msg_temp, _) = client_sock.recvfrom(1024)
            c2c_response(msg_temp.decode())
            msg = msg_temp.decode()
            msg = msg.split('$')
            if msg[0] == "invite" and status == "available":
                invite_reply(msg)
