import socket

SERVER_PORT = 2434
MAX_TRIALS = 10

tttserver = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
tttserver.bind(('', SERVER_PORT))

# lists
addr_to_p = {}
p_to_addr = {}
p_status = {}

# commands and handler


def invalid_command(addr):
    tttserver.sendto(b"Invalid command", addr)


def register(addr, player):
    if player in p_to_addr or addr in addr_to_p:
        tttserver.sendto(b"NOK$Name is already exists", addr)
    else:
        p_to_addr[player] = addr
        addr_to_p[addr] = player
        p_status[player] = "available"
        tttserver.sendto(b"OK", addr)


def unregister(addr):
    player = addr_to_p[addr]
    if addr not in addr_to_p:
        tttserver.sendto(b"NOK$Name doesn't exist", addr)
    elif p_status[player] == "busy":
        tttserver.sendto(b"NOK$Player is busy", addr)
    else:
        del p_to_addr[player]
        del addr_to_p[addr]
        del p_status[player]
        tttserver.sendto(b"OK", addr)


def client_list(addr):
    temp = ""
    plist = []
    for k, v in p_status.items():
        temp = k + ':' + v
        plist.append(temp)
    outbound(addr, "LIST$" + str(plist)[1:len(str(plist))-1])

def outbound(addr, msg):
    t = 1
    reply = ""

    tttserver.settimeout(1)
    while t < MAX_TRIALS:
        try:
            tttserver.sendto(msg.encode('UTF-8'), addr)
            reply = tttserver.recv(1024)
            break
        except socket.timeout:
            t += 1
    tttserver.settimeout(None)
    if t == MAX_TRIALS:
        print("Error: didn't get ACK from client")
    else:
        print(reply.decode())


def to_other_client(name, msg):
    if name in p_to_addr:
        tttserver.sendto(msg, p_to_addr[name])


def set_busy(addr):
    if addr in addr_to_p:
        p_status[addr_to_p[addr]] = "busy"
        tttserver.sendto(b"OK", addr)
    else:
        tttserver.sendto(b"NOK$Player is not connected", addr)


def set_available(addr):
    if addr in addr_to_p:
        p_status[addr_to_p[addr]] = "available"
        tttserver.sendto(b"OK", addr)
    else:
        tttserver.sendto(b"NOK$Player is not connected", addr)


# main
while True:
    print("Server is listening...")
    msg, addr = tttserver.recvfrom(1024)
    cmd = msg.decode().split("$")
    if cmd[0] == "register":
        print("Registering player...")
        register(addr, cmd[1])
    elif cmd[0] == "unregister":
        print("Unregistering player...")
        unregister(addr)
    elif cmd[0] == "list":
        print("Listing players...")
        client_list(addr)
    elif cmd[0] == "invite":
        print("Inviting player...")
        n = cmd[1].split(';')[1]
        if n in p_to_addr:
            if p_status[n] == "busy":
                tttserver.sendto(b"NOK$Player is busy", addr)
            else:
                to_other_client(n, msg)
        else:
            tttserver.sendto(b"NOK$Player is not connected", addr)
    elif cmd[0] == "invR":
        print("Invitation response...")
        to_other_client(cmd[1].split(';')[2], msg)
    elif cmd[0] == "OK" or cmd[0] == "NOK" or cmd[0] == "play" or cmd[0] == "end" or cmd[0] == "quit":
        print("Responding...")
        to_other_client(cmd[1].split(';')[1], msg)
    elif cmd[0] == "busy":
        print("Setting player busy...")
        set_busy(addr)
    elif cmd[0] == "available":
        print("Setting player available...")
        set_available(addr)
        break
    else:
        invalid_command(addr)

tttserver.close()
