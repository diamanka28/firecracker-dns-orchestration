
# permet de demander au serveur de lancer 3 micro-vm
dig @127.0.0.1 -p 2025 psar_firecracker.com/lunch_n_microvm/3 +tries=1 +retry=0


# demander au serveur de lancer 1 micro vm
#dig @127.0.0.1 -p 2025 psar_firecracker.com/lunch_microvm/ +tries=1 +retry=0


# tester les fonctions existantes et non existantes
#dig @127.0.0.1 -p 2025 psar_firecracker.com/func_A/ +tries=1 +retry=0
#dig @127.0.0.1 -p 2025 psar_firecracker.com/func_B/ +tries=1 +retry=0
# func_C fonction qui n'existe pas 
#dig @127.0.0.1 -p 2025 psar_firecracker.com/func_C/ +tries=1 +retry=0

