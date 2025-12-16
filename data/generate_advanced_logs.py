import random
import time
import sys
import os

# --- CONFIGURATION ---
OUTPUT_FILE = "heavy_traffic.log"
TARGET_SIZE_GB = 1  # On génère 1 Go de "Vrai" aléatoire (tu pourras le multiplier après)
CHUNK_SIZE = 100000 # Nombre de lignes par écriture disque (pour la perf)

# --- SCÉNARIO ---
# Les 10 IPs du Botnet (ce sont elles que ton C++ devra trouver dans le top 10)
BOTNET_IPS = [f"10.66.6.{i}" for i in range(1, 11)]

HTTP_CODES = ["200", "404", "500", "301", "403"]
USER_AGENTS = ["Mozilla/5.0", "Curl/7.64", "BotGoogle", "iPhone/Safari"]
URLS = ["/login", "/home", "/api/v1/data", "/image.png", "/dashboard"]

def generate_random_ip():
    """Génère une IP totalement aléatoire (Ex: 212.45.12.99)"""
    return f"{random.randint(1, 255)}.{random.randint(0, 255)}.{random.randint(0, 255)}.{random.randint(0, 255)}"

def generate_chunk():
    """Génère un gros bloc de texte en mémoire"""
    lines = []
    for _ in range(CHUNK_SIZE):
        # 10% de chance que ce soit le Botnet (l'attaque), 90% random
        if random.random() < 0.10:
            ip = random.choice(BOTNET_IPS)
            msg = "DDOS_ATTACK_SIGNATURE"
        else:
            ip = generate_random_ip()
            msg = "Normal_User_Activity"

        # Format standard Nginx/Apache simplifié
        # Exemple: 2023-10-27T10:00:00 [INFO] GET /login 200 from IP 192.168.1.1
        timestamp = time.strftime("%Y-%m-%dT%H:%M:%S")
        code = random.choice(HTTP_CODES)
        url = random.choice(URLS)
        
        # NOTE IMPORTANTE : J'ajoute "from IP" pour faciliter ton parsing C++ plus tard
        line = f"{timestamp} [INFO] GET {url} {code} {msg} from IP {ip}\n"
        lines.append(line)
    return "".join(lines)

def main():
    print(f"Generating {TARGET_SIZE_GB} GB of high-entropy log data...")
    print(f"Target File: {OUTPUT_FILE}")
    
    start_time = time.time()
    current_bytes = 0
    target_bytes = TARGET_SIZE_GB * 1024 * 1024 * 1024
    
    # Création du dossier data si inexistant
    os.makedirs(os.path.dirname(OUTPUT_FILE), exist_ok=True)

    with open(OUTPUT_FILE, "w") as f:
        while current_bytes < target_bytes:
            chunk = generate_chunk()
            f.write(chunk)
            current_bytes += len(chunk)
            
            # Barre de progression simple
            percent = (current_bytes / target_bytes) * 100
            sys.stdout.write(f"\rProgress: {percent:.2f}%  ({current_bytes / (1024**2):.0f} MB)")
            sys.stdout.flush()

    elapsed = time.time() - start_time
    print(f"\nDone! Generated in {elapsed:.2f} seconds.")
    print(f"Botnet IPs to find: {', '.join(BOTNET_IPS)}")

if __name__ == "__main__":
    main()
