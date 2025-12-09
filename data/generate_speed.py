import random
import datetime

# Un fichier de 100 Mo environ
TARGET_SIZE = 100 * 1024 * 1024 
LEVELS = ["INFO", "WARN", "ERROR", "DEBUG", "CRITICAL"]
MSGS = [
    "User logged in from IP 192.168.1.1",
    "Database connection failed timeout",
    "Cache miss for key user_12345",
    "Payment processed successfully amount=500",
    "Index out of bounds exception in module X"
]

with open("seed.log", "w") as f:
    current_size = 0
    while current_size < TARGET_SIZE:
        timestamp = datetime.datetime.now().isoformat()
        level = random.choice(LEVELS)
        msg = random.choice(MSGS)
        line = f"{timestamp} [{level}] {msg} {random.randint(0, 9999)}\n"
        f.write(line)
        current_size += len(line)

print("Fichier seed.log généré !")