# Firecracker over DNS Proxy : Orchestration de micro-vm via un resolver DNS

## Firecracker

### I. Présentation

Firecracker est un logiciel de virtualisation open-source développé par Amazon Web Services (AWS) qui permet de créer et de gérer des **microVM** (micro machines virtuelles). Il est conçu pour offrir une solution légère, sécurisée et performante pour exécuter des charges de travail isolées, notamment dans des environnements cloud.

<u>Principales caractéristiques de Firecracker</u> :

1. *Légèreté :*
Firecracker est conçu pour être extrêmement léger, avec une empreinte mémoire minimale. Chaque microVM peut démarrer en quelques millisecondes, ce qui le rend idéal pour des scénarios où la rapidité de déploiement est cruciale.

2. *Sécurité :*
Firecracker utilise des technologies de **virtualisation matérielle** (comme KVM) pour isoler les microVM les unes des autres. Il est conçu avec une surface d'attaque minimale, ce qui réduit les risques de vulnérabilités de sécurité.

3. *Performances :*
Grâce à son architecture légère, Firecracker offre des performances proches du **bare-metal**, ce qui en fait un choix populaire pour les applications nécessitant une faible latence et un haut débit.

4. *Intégration avec AWS :*
Firecracker est utilisé en interne par AWS pour des services comme AWS Lambda et AWS Fargate, où il permet de gérer des milliers de fonctions serverless ou de conteneurs de manière efficace.

5. *Open-source :*
Firecracker est disponible sous licence **open-source**, ce qui permet à la communauté de contribuer à son développement et de l'utiliser dans divers projets.

<u>Cas d'utilisation courants</u> :

1. *Serverless Computing :*
Firecracker est souvent utilisé pour exécuter des **fonctions serverless**, où des milliers de microVM peuvent être démarrées et arrêtées rapidement.

2. *Conteneurs :*
Bien que Firecracker ne soit pas un outil de conteneurisation comme Docker, il peut être utilisé pour exécuter des **conteneurs** dans des microVM, offrant ainsi un niveau supplémentaire d'isolation et de sécurité.


3. *Environnements de test :*
Sa capacité à démarrer rapidement des VM légères en fait un outil idéal pour les environnements de test et de développement.

<u>Comment ça marche</u> ?

Firecracker utilise un **hyperviseur** basé sur **KVM** (Kernel-based Virtual Machine) pour créer des microVM. Chaque microVM est isolée des autres et peut exécuter des fonctions sur un système d'exploitation invité. De plus, Firecracker fournit une **API REST** pour la gestion des microVM, permettant de les démarrer, arrêter, et configurer de manière programmatique.

### II. Ordonnancement

Voici une explication du fonctionnement de l'ordonnancement dans Firecracker  :

1. <u>vCPU dans des threads</u>
- Chaque **vCPU** dans une microVM est associé à un thread distinct dans le système d'exploitation hôte.
- Ces threads sont ordonnancés par l'ordonnanceur du système d'exploitation hôte, qui détermine comment le temps CPU est alloué entre tous les threads en cours d'exécution (y compris ceux des microVM et d'autres processus).

2. <u>Ordonnanceur du système d’exploitation hôte</u>
- L'ordonnanceur du système d'exploitation hôte (par exemple, le **Completely Fair Scheduler** (CFS) de Linux) est responsable de la gestion de l'exécution de ces threads.
- L'ordonnanceur garantit une allocation équitable et efficace des ressources CPU entre tous les threads, y compris ceux appartenant aux microVM.

3. <u>Épinglage des vCPU (Optionnel)</u>
- Firecracker permet d'épingler les threads à des cœurs de CPU physiques spécifiques sur l'hôte. Cela se fait en utilisant la configuration `cpu_template` ou `cpu-affinity`.
- **L'épinglage** peut améliorer les performances en réduisant les changements de contexte et les miss de cache, mais il nécessite une planification minutieuse pour éviter de surcharger des cœurs spécifiques.

4. <u>Gestion des E/S et Événements</u>
- Firecracker utilise une architecture basée sur les **évènements** pour les opérations d'E/S, ce qui réduit le besoin de changements de contexte fréquents et améliore l'efficacité.
- Le VMM (**Virtual Machine Monitor**) gère les événements et interruptions d'E/S, en veillant à ce que les vCPU ne soient réveillés que lorsque c'est nécessaire.

5. <u>Ordonnancement en temps réel (Optionnel)</u>
- Pour les charges de travail nécessitant une faible latence, vous pouvez configurer les threads pour utiliser des politiques d'ordonnancement en temps réel (par exemple, `SCHED_FIFO` ou `SCHED_RR` sous Linux).
- Cela nécessite des autorisations et une configuration appropriées sur le système d'exploitation hôte.

### III. Utilisation

**1. Cloner le répertoire Github**

```
git clone https://github.com/nhaykhaud/M1-PSAR.git
```

**2. Lancer le script `installation.sh`**

```
./installation.sh
```

- le script télécharge l'**image du kernel**, le **rootfs** et les **binaires de Firecracker**.
    - image du kernel : le binaire du noyau Linux et agit comme le coeur du système d’exploitation.
    - rootfs : le système de fichiers de la microVM.
    - binaires de Firecracker : les fichiers permettant l'exécution de Firecracker.
- puis crée une **clé ssh** pour le rootfs
- et enfin transforme le rootfs en un fichier **ext4** car Firecracker ne peut pas travailler directement sur le rootfs

**3. Ajouter les sources dans le dossier `src`**

**4. Lancer le script `compile.sh`**

```
./compile.sh
```
- le script compile les fichiers sources de `src` dans le dossier `bin`
- puis monte le fichier **ext4** dans le dossier `/mnt`, copie l'exécutable de `bin` dans `/mtn/root` et démonte le dossier `/mnt`
    - `/mnt/root` est le répertoire par défaut de la microVM
- et enfin nettoie le dossier `bin`

**5. Lancer le script `start.sh`**

```
./start.sh
```

- Cela lance la commande `./setup/firecracker --no-api --config-file setup/vm-config.json`
    - `vm-config.json` est le fichier de configuration de la microVM, incluant les chemins vers le **kernel** et le **rootfs**, les paramètres de mémoire et des vCPU.

**6. Fermeture de la microVM**

```
reboot
```

- Il faut lancer cette commande dans la micrcoVM pour l'arrêter.

### IV. Bibliographie 

1. Présentation

    :link: [Firecracker - site officiel](https://firecracker-microvm.github.io/)
    
    :link: [Firecracker - Github](https://github.com/firecracker-microvm/firecracker)

2. Ordonnancement 

    :link: [cgroup.rs](https://github.com/firecracker-microvm/firecracker/blob/main/src/jailer/src/cgroup.rs)
    
    :link: [Jailer](https://github.com/firecracker-microvm/firecracker/blob/main/docs/jailer.md)
    
    :link: [cpu.cfs_period_us](https://github.com/firecracker-microvm/firecracker/blob/main/docs/prod-host-setup.md)
    
    :link: [CPU Accounting Controller](https://www.kernel.org/doc/Documentation/cgroup-v1/cpuacct.txt)

3. Utilisation

    :link: [Getting Started with Firecracker](https://github.com/firecracker-microvm/firecracker/blob/main/docs/getting-started.md)

## DNS
